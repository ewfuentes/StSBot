
#include "planning/generate_combat_command.hh"

#include <algorithm>
#include <iostream>

#include "planning/turn_planner.hh"
#include "sim/cards/bash.hh"
#include "sim/cards/defend.hh"
#include "sim/cards/strike.hh"
#include "sim/combat.hh"

namespace sts::planning {
namespace {

struct ActionSequence {
  std::vector<sim::Action> sequence;
  int score;
};

sim::PlayerState convert_player(const interface::CombatState &combat_state) {
  return {.max_hp = combat_state.player.max_hp,
          .current_hp = combat_state.player.current_hp,
          .current_block = combat_state.player.block,
          .current_energy = combat_state.player.energy};
}

std::vector<sim::MonsterState> convert_monsters(const interface::CombatState &combat_state) {
  std::vector<sim::MonsterState> out;
  std::transform(combat_state.monsters.begin(), combat_state.monsters.end(),
                 std::back_inserter(out), [](const auto &monster) -> sim::MonsterState {
                   return {.name = monster.id,
                           .max_hp = monster.max_hp,
                           .current_hp = monster.current_hp,
                           .current_block = monster.block};
                 });
  return out;
}
sim::DeckState convert_deck(const interface::CombatState &combat_state) {
  const auto convert_pile = [](const std::vector<interface::CombatState::Card> &cards)
      -> std::set<std::shared_ptr<const sim::Card>> {
    std::vector<std::shared_ptr<const sim::Card>> out;
    std::transform(
        cards.begin(), cards.end(), std::back_inserter(out),
        [](const interface::CombatState::Card &card) -> std::shared_ptr<const sim::Card> {
          if (card.id == "Defend_R") {
            return std::make_shared<sim::cards::Defend>(1, 5);
          } else if (card.id == "Strike_R") {
            return std::make_shared<sim::cards::Strike>(1, 6);
          } else if (card.id == "Bash") {
            return std::make_shared<sim::cards::Bash>(2, 8);
          }
          return nullptr;
        });
    return std::set(out.begin(), out.end());
  };
  return {
      .hand = convert_pile(combat_state.hand),
      .discard = convert_pile(combat_state.discard),
      .draw = convert_pile(combat_state.draw),
      .exhaust = convert_pile(combat_state.exhaust),
  };
}

sim::CombatState convert_combat_state(const interface::CombatState &combat_state) {
  return {.player = convert_player(combat_state),
          .monsters = convert_monsters(combat_state),
          .deck = convert_deck(combat_state)};
}

std::vector<ActionSequence> find_action_sequences(const GameTree &tree,
                                                  std::vector<sim::Action> &current,
                                                  std::vector<ActionSequence> &out) {
  if (tree.successors.empty()) {
    out.push_back(ActionSequence{.sequence = current, .score = tree.score});
  }
  for (const auto &child : tree.successors) {
    current.push_back(child.first);
    find_action_sequences(child.second, current, out);
    current.pop_back();
  }
  return out;
}

std::vector<ActionSequence> find_action_sequences(const GameTree &tree) {
  std::vector<sim::Action> actions;
  std::vector<ActionSequence> action_sequences;

  return find_action_sequences(tree, actions, action_sequences);
}

std::string emit_matching_action(const ActionSequence &sequence,
                                 const interface::CombatState &combat_state) {
  const auto &action = sequence.sequence.front();
  if (action.maybe_card) {
    // Find a matching card
    const auto result_iter = std::find_if(combat_state.hand.begin(), combat_state.hand.end(),
                                          [&match_card = action.maybe_card](const auto &c) {
                                            return c.id.starts_with(match_card->name);
                                          });
    if (result_iter == combat_state.hand.end()) {
      std::cout << "Unabled to find matching card " << action.maybe_card->name << std::endl;
      return "";
    }
    const int card_idx = std::distance(combat_state.hand.begin(), result_iter) + 1;
    if (action.maybe_target.has_value()) {
      return "play " + std::to_string(card_idx) + " " +
             std::to_string(action.maybe_target.value());
    } else {
      return "play " + std::to_string(card_idx);
    }
  } else if (action.descriptor == "End Turn") {
    return "end";
  }
  return "";
}

}  // namespace

std::string generate_combat_command(const interface::CombatState &combat_state) {
  // Convert the combat state into something the turn planner can reason about
  const sim::CombatState game_state = convert_combat_state(combat_state);
  // Generate the game tree
  const GameTree game_tree = build_game_tree(game_state);

  // Find the action sequence with the highest score
  std::vector<ActionSequence> action_sequences = find_action_sequences(game_tree);
  std::sort(action_sequences.begin(), action_sequences.end(),
            [](const auto &a, const auto &b) { return a.score < b.score; });

  const auto &best_sequence = action_sequences.back();

  return emit_matching_action(best_sequence, combat_state);
}
}  // namespace sts::planning
