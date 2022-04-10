
#include "planning/turn_planner.hh"

#include <iostream>

namespace sts::planning {

GameTree build_game_tree(const sim::CombatState &initial_state) {
  std::vector<std::pair<sim::Action, GameTree>> successors;
  for (const auto &card : initial_state.deck.hand) {
    const auto curr_card_actions = card->create_actions(initial_state);
    for (const auto &action : curr_card_actions) {
      successors.push_back(
          std::make_pair(action, build_game_tree(action.apply(initial_state))));
    }
  }

  return {.state = initial_state, .successors = std::move(successors)};
}
}  // namespace sts::planning
