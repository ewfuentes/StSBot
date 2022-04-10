
#include "sim/combat_updater.hh"

namespace sts::sim {
CombatUpdater &CombatUpdater::adjust_energy(const int amount) {
  state_.player.current_energy += amount;
  return *this;
}

CombatUpdater &CombatUpdater::damage_monster(const int amount,
                                             const int target) {
  auto &monster = state_.monsters.at(target);
  const int excess_damage = std::max(0, amount - monster.current_block);
  monster.current_block = std::max(0, monster.current_block - amount);
  monster.current_hp -= excess_damage;
  return *this;
}

CombatUpdater &CombatUpdater::apply_player_block(const int amount) {
  state_.player.current_block += amount;
  return *this;
}

CombatUpdater &CombatUpdater::move_card_from_to(const CardLocation src,
                                                const CardLocation dst,
                                                const Card &card) {
  auto location_from_enum =
      [&deck = state_.deck](const auto loc) -> std::set<Card> & {
    if (loc == CardLocation::HAND) {
      return deck.hand;
    } else if (loc == CardLocation::DISCARD) {
      return deck.discard;
    } else if (loc == CardLocation::DRAW) {
      return deck.draw;
    } else {
      return deck.exhaust;
    }
  };

  auto &src_set = location_from_enum(src);
  auto &dst_set = location_from_enum(dst);
  dst_set.insert(src_set.extract(card));
  return *this;
}
}  // namespace sts::sim
