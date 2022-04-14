
#pragma once

#include <set>
#include <string>
#include <variant>
#include <vector>

namespace sts::interface {
struct EventScreenState {
  struct Option {
    std::optional<int> choice_idx;
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

struct CampfireScreenState {
  bool has_rested;
};

struct ShopScreenState {};

struct GameOverScreenState {};

struct GridScreenState {
  int num_cards;
  bool is_for_purge;
  bool is_for_transform;
  bool is_for_upgrade;
  bool is_confirm_up;
  std::vector<std::string> card_names;
};

struct CombatState {
  struct Card {
    int cost;
    bool ethereal;
    bool exhausts;
    bool has_target;
    bool is_playable;
    std::string id;
    int upgrades;
    std::string uuid;
  };
  struct Power {
    int amount;
    std::string id;
  };
  struct Monster {
    std::string id;
    int block;
    int current_hp;
    bool half_dead;
    std::string intent;
    bool is_gone;
    int max_hp;
    int move_adjusted_damage;
    int move_base_damage;
    int move_hits;
    int move_id;
    std::vector<Power> powers;
  };
  struct Player {
    struct Orb {};
    int block;
    int current_hp;
    int max_hp;
    int energy;

    std::vector<Power> powers;
    std::vector<Orb> orbs;
  };

  std::vector<Card> discard;
  std::vector<Card> draw;
  std::vector<Card> limbo;
  std::vector<Card> exhaust;
  std::vector<Card> hand;

  std::vector<Monster> monsters;
  Player player;
  int turn;
};

struct CombatRewardScreenState {
  struct GoldReward {
    int amount;
  };
  struct CardReward {};
  struct UnknownReward {
    std::string type;
  };
  using RewardTypes = std::variant<GoldReward, CardReward, UnknownReward>;
  std::vector<RewardTypes> rewards;
};

struct ChestScreenState {
  bool is_chest_open;
};

struct DungeonState {
  int act;
  std::string act_boss;
  std::optional<EventScreenState> event_screen;
  std::optional<MapScreenState> map_screen;
  std::optional<CombatRewardScreenState> combat_reward_screen;
  std::optional<CampfireScreenState> campfire_screen;
  std::optional<GameOverScreenState> game_over_screen;
  std::optional<ShopScreenState> shop_screen;
  std::optional<ChestScreenState> chest_screen;
  std::optional<GridScreenState> grid_screen;
  std::optional<CombatState> combat;
};

struct GameState {
  std::set<std::string> available_commands;
  bool in_game;
  bool ready_for_command;
  std::optional<DungeonState> dungeon;
};
}  // namespace sts::interface
