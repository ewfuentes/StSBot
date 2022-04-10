
#pragma once

#include "sim/cards/strike.hh"

namespace sts::sim::cards {
struct Bash : public Strike {
  Bash(int energy_cost, int damage_amount)
      : Card("Bash", energy_cost, get_unique_id(), Type::ATTACK),
        damage_amount(damage_amount) {}
};
}  // namespace sts::sim::cards
