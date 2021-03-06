
#pragma once

#include "sim/cards/get_unique_id.hh"
#include "sim/combat.hh"
#include "sim/combat_updater.hh"

namespace sts::sim::cards {
struct Defend : public Card {
  int block_amount;

  Defend(int cost, int block_amount)
      : Card("Defend", cost, get_unique_id(), Type::SKILL), block_amount(block_amount) {}

  std::vector<Action> create_actions(const CombatState &combat_state) const override {
    if (combat_state.player.current_energy < cost) {
      return {};
    }

    if (std::all_of(combat_state.monsters.begin(), combat_state.monsters.end(),
                    [](const auto &m) { return m.current_hp == 0; })) {
      return {};
    }

    std::vector<Action> out;
    out.push_back({.descriptor = name + "(" + std::to_string(id) + ") for " +
                                 std::to_string(block_amount) + " (Cost: " + std::to_string(cost) +
                                 ")",
                   .maybe_card = shared_from_this(),
                   .maybe_target = std::nullopt,
                   .apply = [this](const CombatState &state) -> CombatState {
                     return CombatUpdater(state)
                         .apply_player_block(block_amount)
                         .adjust_energy(-cost)
                         .move_card_from_to(CardLocation::HAND, CardLocation::DISCARD, *this);
                   }});
    return out;
  }
};
}  // namespace sts::sim::cards
