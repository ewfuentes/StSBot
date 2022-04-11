
#pragma once

#include <set>
#include <string>
#include <vector>

namespace sts::interface {
struct EventScreenState {
  struct Option {
    int choice_idx;
    std::string label;
    bool disabled;
  };
  std::string event_id;
  std::vector<Option> options;
};

struct MapScreenState {
  struct MapNode {
    int x;
    int y;
    std::string symbol;
  };

  std::vector<MapNode> map_nodes;
};

struct DungeonState {
  int act;
  std::string act_boss;
  std::optional<EventScreenState> event_screen;
  std::optional<MapScreenState> map_screen;
};

struct GameState {
  std::set<std::string> available_commands;
  bool in_game;
  bool ready_for_command;
  std::optional<DungeonState> dungeon;
};
}  // namespace sts::interface
