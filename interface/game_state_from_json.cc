
#include "interface/game_state_from_json.hh"

#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace sts::interface {
namespace {
std::set<std::string> parse_available_commands(const nlohmann::json &json) {
  std::set<std::string> out(json.begin(), json.end());
  return out;
}

EventScreenState parse_event_screen_state(const nlohmann::json &json) {
  nlohmann::json screen_json = json;
  EventScreenState out;
  {
    out.event_id = screen_json.at("event_id");
    screen_json.erase("event_id");
  }
  {
    const auto &options = screen_json.at("options");
    std::transform(options.begin(), options.end(), std::back_inserter(out.options),
                   [](const auto &option) {
                     return EventScreenState::Option{.choice_idx = option.at("choice_index"),
                                                     .label = option.at("label"),
                                                     .disabled = option.at("disabled")};
                   });
    screen_json.erase("options");
  }

  std::cout << "Unparsed screen state: " << screen_json << std::endl;
  return out;
}

MapScreenState parse_map_screen_state(const nlohmann::json &json) {
  nlohmann::json map_json = json;
  MapScreenState out;

  {
    const auto &next_nodes = map_json.at("next_nodes");
    std::transform(next_nodes.begin(), next_nodes.end(), std::back_inserter(out.map_nodes),
                   [](const auto &node) {
                     return MapScreenState::MapNode{
                         .x = node.at("x"), .y = node.at("y"), .symbol = node.at("symbol")};
                   });
    map_json.erase("next_nodes");
  }
  std::cout << "unparsed screen_state: " << map_json << std::endl;
  return out;
}

DungeonState parse_dungeon_state(const nlohmann::json &json) {
  nlohmann::json dungeon_json = json;
  DungeonState out;
  {
    out.act = dungeon_json.at("act");
    dungeon_json.erase("act");
  }
  {
    out.act_boss = dungeon_json.at("act_boss");
    dungeon_json.erase("act_boss");
  }
  {
    // out.map = parse_map(dungeon_json.at("map"));
    dungeon_json.erase("map");
  }
  {
    // out.deck = parse_deck(dungeon_json.at("deck"));
    dungeon_json.erase("deck");
  }
  {
    // out.potions = parse_potions(dungeon_json.at("potions"));
    dungeon_json.erase("potions");
  }
  {
    // out.relics= parse_relics(dungeon_json.at("relics"));
    dungeon_json.erase("relics");
  }
  if (dungeon_json.at("screen_type") == "EVENT") {
    out.event_screen = parse_event_screen_state(dungeon_json.at("screen_state"));
    dungeon_json.erase("screen_state");
  } else if (dungeon_json.at("screen_type") == "MAP") {
    out.map_screen = parse_map_screen_state(dungeon_json.at("screen_state"));
    dungeon_json.erase("screen_state");
  }

  std::cout << "Unparsed dungeon: " << dungeon_json << std::endl;
  return out;
}

}  // namespace

GameState game_state_from_json(const nlohmann::json &json) {
  std::cout << "Input json: " << json << std::endl;
  GameState out;
  nlohmann::json state_json = json;
  {
    out.available_commands = parse_available_commands(state_json.at("available_commands"));
    state_json.erase("available_commands");
  }
  {
    out.in_game = state_json.at("in_game");
    state_json.erase("in_game");
  }
  {
    out.ready_for_command = state_json.at("ready_for_command");
    state_json.erase("ready_for_command");
  }
  if (state_json.contains("game_state")) {
    out.dungeon = parse_dungeon_state(state_json.at("game_state"));
    state_json.erase("game_state");
  }

  std::cout << "Unparsed JSON: " << state_json << std::endl;
  return out;
}
}  // namespace sts::interface
