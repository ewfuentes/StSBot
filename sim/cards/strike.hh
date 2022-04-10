
#pragma once

#include "sim/cards/get_unique_id.hh"
#include "sim/combat.hh"
#include "sim/combat_updater.hh"

namespace sts::sim::cards {
struct Strike : public Card {
  int damage_amount;

  Strike(int energy_cost, int damage_amount)
      : Card("Strike", energy_cost, get_unique_id(), Type::ATTACK),
        damage_amount(damage_amount) {}

  std::vector<Action> create_actions(
      const CombatState &combat_state) const override {
    if (combat_state.player.current_energy < cost) {
      return {};
    }

    std::vector<Action> out;
    for (size_t monster_idx = 0; monster_idx < combat_state.monsters.size();
         monster_idx++) {
      const auto &m = combat_state.monsters.at(monster_idx);
      out.push_back({.descriptor = name + "(" + std::to_string(id) + ") for " +
                                   std::to_string(damage_amount) +
                                   " targeting: " + m.name +
                                   " (Cost: " + std::to_string(cost) + ")",
                     .apply = [this, monster_idx](
                                  const CombatState &state) -> CombatState {
                       return CombatUpdater(state)
                           .adjust_energy(-cost)
                           .damage_monster(damage_amount, monster_idx)
                           .move_card_from_to(CardLocation::HAND,
                                              CardLocation::DISCARD, *this);
                     }});
    }
    return out;
  }
};
}  // namespace sts::sim::cards
