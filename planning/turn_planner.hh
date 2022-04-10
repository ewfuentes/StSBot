
#pragma once

#include <utility>

#include "sim/combat.hh"

namespace sts::planning {

struct GameTree {
  sim::CombatState state;
  std::vector<std::pair<sim::Action, GameTree>> successors;
};

GameTree build_game_tree(const sim::CombatState &initial_state);

}  // namespace sts::planning
