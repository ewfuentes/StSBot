
#pragma once

#include <optional>

#include "sim/cards/card.pb.h"
#include "sim/combat.hh"
#include "sim/target.hh"

namespace sts::sim {
class CombatUpdater {
 public:
  CombatUpdater(const CombatState &combat_state) : state_(combat_state){};
  operator CombatState() { return state_; }

  CombatUpdater &update(const cards::proto::Card &card, const Target &target);

 private:
  CombatState state_;
};

namespace detail {
CombatState apply_effect(CombatState in, const cards::proto::EnergyAdjust &effect,
                         const Target &target = None{});
CombatState apply_effect(CombatState in, const cards::proto::ApplyBlock &effect,
                         const Target &target = None{});
CombatState apply_effect(CombatState in, const cards::proto::ApplyDamage &effect,
                         const Target &target);
CombatState apply_effect(CombatState in, const cards::proto::ApplyStatus &effect,
                         const Target &target);
}  // namespace detail
}  // namespace sts::sim
