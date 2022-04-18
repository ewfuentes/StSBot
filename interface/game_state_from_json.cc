
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
                     std::optional<int> choice_idx;
                     if (option.contains("choice_index")) {
                       choice_idx = option.at("choice_index");
                     }
                     return EventScreenState::Option{.choice_idx = choice_idx,
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

CombatRewardScreenState parse_combat_reward_screen_state(const nlohmann::json &json) {
  CombatRewardScreenState out;
  const auto &rewards = json.at("rewards");
  std::transform(rewards.begin(), rewards.end(), std::back_inserter(out.rewards),
                 [](const auto &reward) -> CombatRewardScreenState::RewardTypes {
                   const auto &reward_type = reward.at("reward_type");
                   if (reward_type == "GOLD") {
                     return CombatRewardScreenState::GoldReward{.amount = reward.at("gold")};
                   } else if (reward_type == "CARD") {
                     return CombatRewardScreenState::CardReward{};
                   } else {
                     return CombatRewardScreenState::UnknownReward{.type = reward_type};
                   }
                 });

  return out;
}

CampfireScreenState parse_campfire_screen_state(const nlohmann::json &json) {
  nlohmann::json campfire_json = json;
  return CampfireScreenState{.has_rested = campfire_json.at("has_rested")};
}

GameOverScreenState parse_game_over_screen_state(const nlohmann::json &json) {
  nlohmann::json game_over_json = json;
  return GameOverScreenState{};
}

ShopScreenState parse_shop_screen_state(const nlohmann::json &json) {
  nlohmann::json shop_json = json;
  return ShopScreenState{};
}

ChestScreenState parse_chest_screen_state(const nlohmann::json &json) {
  return ChestScreenState{.is_chest_open = json.at("chest_open")};
}

GridScreenState parse_grid_screen_state(const nlohmann::json &json) {
  nlohmann::json grid_json = json;
  GridScreenState out;
  {
    out.num_cards = grid_json.at("num_cards");
    grid_json.erase("num_cards");
  }
  {
    out.is_for_purge = grid_json.at("for_purge");
    grid_json.erase("for_purge");
  }
  {
    out.is_for_upgrade = grid_json.at("for_upgrade");
    grid_json.erase("for_upgrade");
  }
  {
    out.is_for_transform = grid_json.at("for_transform");
    grid_json.erase("for_transform");
  }
  {
    out.is_confirm_up = grid_json.at("confirm_up");
    grid_json.erase("confirm_up");
  }
  {
    const auto &cards = grid_json.at("cards");
    std::transform(cards.begin(), cards.end(), std::back_inserter(out.card_names),
                   [](const auto &card) { return card.at("name"); });
    grid_json.erase("cards");
  }
  return out;
}

CombatState parse_combat_state(const nlohmann::json &json) {
  nlohmann::json combat_json = json;
  CombatState out;
  const auto parse_card_list =
      [](const nlohmann::json &card_list) -> std::vector<CombatState::Card> {
    std::vector<CombatState::Card> out;
    std::transform(card_list.begin(), card_list.end(), std::back_inserter(out),
                   [](const auto &item) {
                     return CombatState::Card{.cost = item.at("cost"),
                                              .ethereal = item.at("ethereal"),
                                              .exhausts = item.at("exhausts"),
                                              .has_target = item.at("has_target"),
                                              .is_playable = item.at("is_playable"),
                                              .id = item.at("id"),
                                              .upgrades = item.at("upgrades"),
                                              .uuid = item.at("uuid")};
                   });
    return out;
  };
  const auto parse_powers = [](const auto &powers_json) {
    std::vector<CombatState::Power> out;
    std::transform(powers_json.begin(), powers_json.end(), std::back_inserter(out),
                   [](const auto &item) {
                     return CombatState::Power{.amount = item.at("amount"), .id = item.at("id")};
                   });
    return out;
  };
  const auto parse_monster_list = [&parse_powers](const auto &monsters_json) {
    std::vector<CombatState::Monster> out;
    std::transform(
        monsters_json.begin(), monsters_json.end(), std::back_inserter(out), [&](const auto &item) {
          return CombatState::Monster{.id = item.at("id"),
                                      .block = item.at("block"),
                                      .current_hp = item.at("current_hp"),
                                      .half_dead = item.at("half_dead"),
                                      .intent = item.at("intent"),
                                      .is_gone = item.at("is_gone"),
                                      .max_hp = item.at("max_hp"),
                                      .move_adjusted_damage = item.at("move_adjusted_damage"),
                                      .move_base_damage = item.at("move_base_damage"),
                                      .move_hits = item.at("move_hits"),
                                      .move_id = item.at("move_id"),
                                      .powers = parse_powers(item.at("powers"))};
        });
    return out;
  };
  const auto parse_player = [&parse_powers](const auto &player_json) {
    return CombatState::Player{.block = player_json.at("block"),
                               .current_hp = player_json.at("current_hp"),
                               .max_hp = player_json.at("max_hp"),
                               .energy = player_json.at("energy"),
                               .powers = parse_powers(player_json.at("powers")),
                               .orbs = {}};
  };
  {
    out.discard = parse_card_list(combat_json.at("discard_pile"));
    combat_json.erase("discard_pile");
  }
  {
    out.draw = parse_card_list(combat_json.at("draw_pile"));
    combat_json.erase("draw_pile");
  }
  {
    out.limbo = parse_card_list(combat_json.at("limbo"));
    combat_json.erase("limbo");
  }
  {
    out.exhaust = parse_card_list(combat_json.at("exhaust_pile"));
    combat_json.erase("exhaust_pile");
  }
  {
    out.hand = parse_card_list(combat_json.at("hand"));
    combat_json.erase("hand");
  }
  {
    out.monsters = parse_monster_list(combat_json.at("monsters"));
    combat_json.erase("monsters");
  }
  {
    out.player = parse_player(combat_json.at("player"));
    combat_json.erase("player");
  }
  {
    out.turn = combat_json.at("turn");
    combat_json.erase("turn");
  }

  std::cout << "Unparsed combat: " << combat_json << std::endl;
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
  } else if (dungeon_json.at("screen_type") == "COMBAT_REWARD") {
    out.combat_reward_screen = parse_combat_reward_screen_state(dungeon_json.at("screen_state"));
    dungeon_json.erase("screen_state");
  } else if (dungeon_json.at("screen_type") == "REST") {
    out.campfire_screen = parse_campfire_screen_state(dungeon_json.at("screen_state"));
  } else if (dungeon_json.at("screen_type") == "GAME_OVER") {
    out.game_over_screen = parse_game_over_screen_state(dungeon_json.at("screen_state"));
  } else if (dungeon_json.at("screen_type") == "SHOP_ROOM") {
    out.shop_screen = parse_shop_screen_state(dungeon_json.at("screen_state"));
  } else if (dungeon_json.at("screen_type") == "CHEST") {
    out.chest_screen = parse_chest_screen_state(dungeon_json.at("screen_state"));
  } else if (dungeon_json.at("screen_type") == "GRID") {
    out.grid_screen = parse_grid_screen_state(dungeon_json.at("screen_state"));
  }

  if (dungeon_json.contains("combat_state")) {
    out.combat = parse_combat_state(dungeon_json.at("combat_state"));
    dungeon_json.erase("combat_state");
  }

  std::cout << "Unparsed dungeon: " << dungeon_json << std::endl;
  return out;
}

}  // namespace

GameState game_state_from_json(const nlohmann::json &json) {
  std::cout << "*******************************************************" << std::endl;
  std::cout << "Input json: " << json << std::endl;
  std::cout << "*******************************************************" << std::endl;
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
