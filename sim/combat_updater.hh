
#pragma once

#include "sim/combat.hh"

namespace sts::sim {
class CombatUpdater {
 public:

  CombatUpdater(const CombatState &combat_state) : state_(combat_state){};
  operator CombatState(){return state_; }

  CombatUpdater &adjust_energy(const int amount);
  CombatUpdater &damage_monster(const int amount, const int target);
  CombatUpdater &apply_player_block(const int amount);
  CombatUpdater &move_card_from_to(const CardLocation src,
                                   const CardLocation dst, const Card &card);

 private:
  CombatState state_;
};
}  // namespace sts::sim
