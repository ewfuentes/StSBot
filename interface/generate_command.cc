
#include "interface/generate_command.hh"

#include <algorithm>

namespace sts::interface {
namespace {
std::string generate_not_in_game_command(const GameState &game_state) {
  if (!game_state.available_commands.contains("start")) {
    throw std::invalid_argument("`start` command not available when not in game");
  }
  // Start an ironclad game with no ascension
  return "start ironclad";
}

int select_neow_power_up(const std::vector<EventScreenState::Option> &options) {
  std::vector<std::string> priority_order{
      "Enemies in your next three combats have 1 HP",
      "Max HP +8",
  };

  auto sorted_options = options;
  std::sort(sorted_options.begin(), sorted_options.end(), [&](const auto &a, const auto &b) {
    return std::find(priority_order.begin(), priority_order.end(), a.label) <
           std::find(priority_order.begin(), priority_order.end(), b.label);
  });
  return sorted_options.front().choice_idx;
}

std::string generate_event_command(const EventScreenState &screen) {
  if (screen.event_id == "Neow Event") {
    if (screen.options.size() == 1) {
      // This is the initial menu
      return "choose " + std::to_string(screen.options.front().choice_idx);
    } else {
      // Select among initial power ups
      return "choose " + std::to_string(select_neow_power_up(screen.options));
    }
  }
  return "";
}

std::string generate_map_command(const MapScreenState &screen) {
  // Choose the first path
  (void)screen;
  return "choose 0";
}

std::string generate_in_game_command(const GameState &game_state) {
  if (!game_state.dungeon.has_value()) {
    throw std::invalid_argument("Missing dungeon state when in game");
  }
  const auto &dungeon = *game_state.dungeon;
  if (dungeon.event_screen.has_value()) {
    return generate_event_command(*game_state.dungeon->event_screen);
  } else if (dungeon.map_screen.has_value()) {
    return generate_map_command(*game_state.dungeon->map_screen);
  }
  return "";
}
}  // namespace

std::string generate_command(const GameState &game_state) {
  if (!game_state.ready_for_command) {
    return "";
  }

  if (game_state.in_game) {
    return generate_in_game_command(game_state);
  } else {
    return generate_not_in_game_command(game_state);
  }
}
}  // namespace sts::interface
