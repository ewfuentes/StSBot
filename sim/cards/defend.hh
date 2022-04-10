
#pragma once

#include "sim/cards/get_unique_id.hh"
#include "sim/combat.hh"
#include "sim/combat_updater.hh"

namespace sts::sim::cards {
struct Block : public Card {
  int block_amount;

  Block(int cost, int block_amount)
      : Card("Defend", cost, get_unique_id(), Type::SKILL),
        block_amount(block_amount) {}

  std::vector<Action> create_actions(const CombatState &combat_state) override {
    if (combat_state.player.current_energy < cost) {
      return {};
    }

    std::vector<Action> out;
    out.push_back({.descriptor = "Apply " + name + " for " +
                                 std::to_string(block_amount) +
                                 "(Cost: " + std::to_string(cost) + ")",
                   .apply = [this](const CombatState &state) -> CombatState {
                     return CombatUpdater(state)
                         .apply_player_block(block_amount)
                         .adjust_energy(-cost)
                         .move_card_from_to(CardLocation::HAND,
                                            CardLocation::DISCARD, *this);
                   }});
    return out;
  }
};
}  // namespace sts::sim::cards
