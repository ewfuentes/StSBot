
#include "interface/generate_command.hh"

#include <algorithm>
#include <iostream>

#include "planning/generate_combat_command.hh"

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
      "Obtain 100 Gold",
      "Enemies in your next three combats have 1 HP",
      "Max HP +8",
  };

  auto sorted_options = options;
  std::sort(sorted_options.begin(), sorted_options.end(), [&](const auto &a, const auto &b) {
    return std::find(priority_order.begin(), priority_order.end(), a.label) <
           std::find(priority_order.begin(), priority_order.end(), b.label);
  });
  return sorted_options.front().choice_idx.value();
}

std::string generate_event_command(const EventScreenState &screen) {
  if (screen.options.size() == 1) {
    return "choose 0";
  }
  if (screen.event_id == "Accursed Blacksmith") {
    return "choose leave";
  } else if (screen.event_id == "Addict") {
    return "choose leave";
  } else if (screen.event_id == "Back to Basics") {
    return "choose elegance";
  } else if (screen.event_id == "Beggar") {
    return "choose leave";
  } else if (screen.event_id == "Big Fish") {
    return "choose banana";
  } else if (screen.event_id == "Bonfire Elementals") {
    return "choose offer";
  } else if (screen.event_id == "Cursed Tome") {
    return "choose leave";
  } else if (screen.event_id == "Dead Adventurer") {
    return "choose leave";
  } else if (screen.event_id == "Drug Dealer") {
    return "choose ingest mutagens";
  } else if (screen.event_id == "Duplicator") {
    return "choose leave";
  } else if (screen.event_id == "Falling") {
    return "choose land";
  } else if (screen.event_id == "Forgotten Altar") {
    return "choose sacrifice";
  } else if (screen.event_id == "Fountain of Cleansing") {
    return "choose drink";
  } else if (screen.event_id == "Ghosts") {
    return "choose refuse";
  } else if (screen.event_id == "Golden Idol") {
    return "choose leave";
  } else if (screen.event_id == "Golden Shrine") {
    return "choose pray";
  } else if (screen.event_id == "Golden Wing") {
    return "choose leave";
  } else if (screen.event_id == "Knowing Skull") {
    return "choose how do i leave?";
  } else if (screen.event_id == "Lab") {
  } else if (screen.event_id == "Liars Game") {
    return "choose disagree";
  } else if (screen.event_id == "Living Wall") {
    if (screen.options.size() == 3) {
      return "choose forget";
    }
  } else if (screen.event_id == "Masked Bandits") {
    return "choose fight";
  } else if (screen.event_id == "Match and Keep!") {
    return "choose " + std::to_string(screen.options.front().choice_idx.value());
  } else if (screen.event_id == "Mushrooms") {
    return "choose stomp";
  } else if (screen.event_id == "Mysterious Sphere") {
    return "choose leave";
  } else if (screen.event_id == "N'loth") {
    return "choose leave";
  } else if (screen.event_id == "Purifier") {
    return "choose leave";
  } else if (screen.event_id == "Scrap Ooze") {
    return "choose leave";
  } else if (screen.event_id == "SecretPortal") {
    return "choose leave";
  } else if (screen.event_id == "SensoryStone") {
    return "choose 0";
  } else if (screen.event_id == "Shining Light") {
    return "choose leave";
  } else if (screen.event_id == "Spire Heart") {
  } else if (screen.event_id == "The Cleric") {
    return "choose leave";
  } else if (screen.event_id == "The Joust") {
    return "choose murderer";
  } else if (screen.event_id == "The Library") {
    return "choose sleep";
  } else if (screen.event_id == "The Mausoleum") {
    return "choose leave";
  } else if (screen.event_id == "The Moai Head") {
    return "choose leave";
  } else if (screen.event_id == "The Woman in Blue") {
    return "choose leave";
  } else if (screen.event_id == "Thinking with Puddles") {
  } else if (screen.event_id == "Tomb of Lord Red Mask") {
    return "choose leave";
  } else if (screen.event_id == "Transmorgrifier") {
    return "choose leave";
  } else if (screen.event_id == "Upgrade Shrine") {
    return "choose leave";
  } else if (screen.event_id == "Vampires") {
    return "choose refuse";
  } else if (screen.event_id == "Wheel of Change") {
  } else if (screen.event_id == "Winding Halls") {
    return "choose retrace your steps";
  } else if (screen.event_id == "World of Goop") {
    return "choose leave it";
  } else if (screen.event_id == "MindBloom") {
    return "choose i am war";
  } else if (screen.event_id == "Nest") {
    return "choose smash and grab";
  } else if (screen.event_id == "FaceTrader") {
    return "choose leave";
  } else if (screen.event_id == "NoteForYourself") {
    return "choose ignore";
  } else if (screen.event_id == "WeMeetAgain") {
    return "choose attack";
  } else if (screen.event_id == "Designer") {
    return "choose punch";
  } else if (screen.event_id == "Colosseum") {
    return "choose cowardice";
  } else if (screen.event_id == "Neow Event") {
    // Select among initial power ups
    return "choose " + std::to_string(select_neow_power_up(screen.options));
  }
  std::cout << "############################" << std::endl;
  std::cout << "Unknown Event: " << screen.event_id << std::endl;
  return "";
}

std::string generate_map_command(const MapScreenState &screen) {
  // Choose the first path
  (void)screen;
  return "choose 0";
}

std::string generate_combat_command(const CombatState &combat_state) {

  return planning::generate_combat_command(combat_state);

  // // Play the first card, if it requires a target, target the first monster
  //  if (combat_state.hand.empty()) {
  //    return "end";
  //  }
  //  if (std::all_of(combat_state.hand.begin(), combat_state.hand.end(), [&](const auto &card) {
  //        return combat_state.player.energy < card.cost || !card.is_playable;
  //      })) {
  //    // Not enough energy, end the turn;
  //    return "end";
  //  }
  //
  //  const auto first_card_iter =
  //      std::find_if(combat_state.hand.begin(), combat_state.hand.end(), [&](const auto &card) {
  //        return card.cost <= combat_state.player.energy && card.is_playable;
  //      });
  //  const int first_card_idx = std::distance(combat_state.hand.begin(), first_card_iter) + 1;
  //  const auto first_target_iter =
  //      std::find_if(combat_state.monsters.begin(), combat_state.monsters.end(),
  //                   [](const auto &monster) { return !monster.is_gone; });
  //  const int target_idx = std::distance(combat_state.monsters.begin(), first_target_iter);
  //  if (first_card_iter->has_target) {
  //    return "play " + std::to_string(first_card_idx) + " " + std::to_string(target_idx);
  //  } else {
  //    return "play " + std::to_string(first_card_idx);
  //  }
}
template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

std::string generate_combat_reward_command(const CombatRewardScreenState &combat_reward_screen) {
  for (const auto &reward : combat_reward_screen.rewards) {
    const std::string command = std::visit(
        overloaded{[](const CombatRewardScreenState::GoldReward &) { return "choose gold"; },
                   [](const CombatRewardScreenState::CardReward &) { return ""; },
                   [](const CombatRewardScreenState::UnknownReward &) { return ""; }},
        reward);
    if (command.size() > 0) {
      return command;
    }
  }
  return "proceed";
}

std::string generate_campfire_command(const CampfireScreenState &campfire_screen) {
  if (campfire_screen.has_rested) {
    return "proceed";
  }
  return "choose rest";
}

std::string generate_game_over_command(const GameOverScreenState &game_over_screen) {
  (void)game_over_screen;
  return "proceed";
}

std::string generate_shop_command(const ShopScreenState &shop_screen) {
  (void)shop_screen;
  return "proceed";
}

std::string generate_chest_command(const ChestScreenState &chest_screen) {
  (void)chest_screen;
  return "proceed";
}

std::string generate_grid_command(const GridScreenState &grid_screen) {
  if (grid_screen.is_confirm_up) {
    return "confirm";
  } else {
    return "choose " + grid_screen.card_names.front();
  }
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
  } else if (dungeon.combat_reward_screen.has_value()) {
    return generate_combat_reward_command(*game_state.dungeon->combat_reward_screen);
  } else if (dungeon.campfire_screen.has_value()) {
    return generate_campfire_command(*game_state.dungeon->campfire_screen);
  } else if (dungeon.game_over_screen.has_value()) {
    return generate_game_over_command(*game_state.dungeon->game_over_screen);
  } else if (dungeon.shop_screen.has_value()) {
    return generate_shop_command(*game_state.dungeon->shop_screen);
  } else if (dungeon.chest_screen.has_value()) {
    return generate_chest_command(*game_state.dungeon->chest_screen);
  } else if (dungeon.grid_screen.has_value()) {
    return generate_grid_command(*game_state.dungeon->grid_screen);
  } else if (dungeon.combat.has_value()) {
    return generate_combat_command(*game_state.dungeon->combat);
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
