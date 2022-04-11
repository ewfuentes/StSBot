
#pragma once

#include "interface/game_state.hh"
#include "nlohmann/json.hpp"

namespace sts::interface {
GameState game_state_from_json(const nlohmann::json &json);
}  // namespace sts::interface
