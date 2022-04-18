
#include "planning/turn_planner.hh"

#include <iostream>

namespace sts::planning {

int score_state(const sim::CombatState &state) {
  int score = 0;
  {
    // Losing health is really bad
    constexpr int HEALTH_LOSS_FACTOR = 5;
    score -= HEALTH_LOSS_FACTOR * (state.player.max_hp - state.player.current_hp);
  }
  {
    constexpr int MONSTER_DAMAGE_FACTOR = 1;
    for (const auto &monster : state.monsters) {
      // Damaging monsters is good
      score += MONSTER_DAMAGE_FACTOR * (monster.max_hp - monster.current_hp);
    }
  }
  return score;
};

GameTree build_game_tree(const sim::CombatState &initial_state) {
  std::vector<std::pair<sim::Action, GameTree>> successors;
  // Add end turn action
  successors.push_back(std::make_pair(
      sim::Action{.descriptor = "End Turn",
                  .maybe_card = nullptr,
                  .maybe_target = std::nullopt,
                  .apply = {}},
      GameTree{.score = score_state(initial_state), .state = initial_state, .successors = {}}));

  // Add card actions
  for (const auto &card : initial_state.deck.hand) {
    const auto curr_card_actions = card->create_actions(initial_state);
    for (const auto &action : curr_card_actions) {
      successors.push_back(std::make_pair(action, build_game_tree(action.apply(initial_state))));
    }
  }

  // Add potion actions

  return {.score = score_state(initial_state),
          .state = initial_state,
          .successors = std::move(successors)};
}
}  // namespace sts::planning
