
#pragma once

#include "sim/cards/strike.hh"

namespace sts::sim::cards {
struct Bash : public Card {
  int damage_amount;
  Bash(int energy_cost, int damage_amount)
      : Card("Bash", energy_cost, get_unique_id(), Type::ATTACK), damage_amount(damage_amount) {}

  std::vector<Action> create_actions(const CombatState &combat_state) const override {
    if (combat_state.player.current_energy < cost) {
      return {};
    }

    std::vector<Action> out;
    for (size_t monster_idx = 0; monster_idx < combat_state.monsters.size(); monster_idx++) {
      const auto &m = combat_state.monsters.at(monster_idx);
      if (m.current_hp == 0) {
        continue;
      }
      out.push_back({.descriptor = "Bash " + m.name + " for " + std::to_string(damage_amount) +
                                   " (Cost: " + std::to_string(cost) + ")",
                     .maybe_card = shared_from_this(),
                     .maybe_target = monster_idx,
                     .apply = [this, monster_idx](const CombatState &state) -> CombatState {
                       return CombatUpdater(state)
                           .adjust_energy(-cost)
                           .damage_monster(damage_amount, monster_idx)
                           .move_card_from_to(CardLocation::HAND, CardLocation::DISCARD, *this);
                     }});
    }
    return out;
  }
};
}  // namespace sts::sim::cards
