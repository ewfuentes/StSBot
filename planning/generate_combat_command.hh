
#pragma once

#include <string>

#include "interface/game_state.hh"

namespace sts::planning {
std::string generate_combat_command(const interface::CombatState &combat_state);
}  // namespace sts::planning
