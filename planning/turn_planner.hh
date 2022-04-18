
#pragma once

#include <utility>

#include "sim/combat.hh"

namespace sts::planning {

struct GameTree {
  int score;
  sim::CombatState state;
  std::vector<std::pair<sim::Action, GameTree>> successors;
};

GameTree build_game_tree(const sim::CombatState &initial_state);

int score_state(const sim::CombatState &state);
}  // namespace sts::planning
