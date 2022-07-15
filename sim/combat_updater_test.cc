
#include "sim/combat_updater.hh"

#include "gtest/gtest.h"

namespace sts::sim {
namespace {

CombatState create_dummy_combat_state() {
  return CombatState{
      .player = {.max_hp = 100, .current_hp = 50, .current_block = 10, .current_energy = 3},
      .monsters = {{.name = "monster_a", .max_hp = 25, .current_hp = 10, .current_block = 0},
                   {.name = "monster_b", .max_hp = 30, .current_hp = 5, .current_block = 10},
                   {.name = "monster_c", .max_hp = 30, .current_hp = 5, .current_block = 10}},
      .deck = {},
  };
}

cards::proto::EnergyAdjust make_energy_adjust(const int value) {
  auto msg = cards::proto::EnergyAdjust();
  msg.set_value(value);
  return msg;
}
}  // namespace

TEST(CombatUpdaterTest, AdjustEnergyTest) {
  // Setup
  constexpr int DOWN_UPDATE = -1;
  constexpr int UP_UPDATE = 2;
  const CombatState state = create_dummy_combat_state();

  // Action + Verification
  for (const auto amount : {DOWN_UPDATE, UP_UPDATE}) {
    const auto energy_adjust = make_energy_adjust(amount);
    const CombatState new_state = detail::apply_effect(state, energy_adjust);

    EXPECT_EQ(state.player.current_energy + amount, new_state.player.current_energy);
  }
}

TEST(CombatUpdaterTest, ApplyPlayerBlock) {
  // Setup
  constexpr int BLOCK_AMOUNT = 20;
  const CombatState state = create_dummy_combat_state();
  cards::proto::ApplyBlock effect;
  effect.set_value(BLOCK_AMOUNT);

  // Action
  const auto new_state = detail::apply_effect(state, effect);

  // Verification
  EXPECT_EQ(new_state.player.current_block, state.player.current_block + BLOCK_AMOUNT);
}

TEST(CombatUpdaterTest, DamageMonsterTest) {
  // Setup
  constexpr int DAMAGE_AMOUNT_1 = 5;
  constexpr int DAMAGE_AMOUNT_2 = 6;
  constexpr int DAMAGE_AMOUNT_3 = 12;
  const CombatState state = create_dummy_combat_state();

  // Action
  CombatState new_state = state;
  for (auto &[idx, amount] :
       {std::make_pair(0, DAMAGE_AMOUNT_1), std::make_pair(1, DAMAGE_AMOUNT_2),
        std::make_pair(2, DAMAGE_AMOUNT_3)}) {
    cards::proto::ApplyDamage effect;
    effect.set_value(amount);
    new_state = detail::apply_effect(std::move(new_state), effect, TargetMonster{.idx = idx});
  }

  // Verification
  const std::array<std::tuple<int, int>, 3> expected_monster_hp_and_block{{{5, 0}, {5, 4}, {3, 0}}};
  for (size_t i = 0; i < new_state.monsters.size(); i++) {
    const auto &monster = new_state.monsters.at(i);
    const auto &[expected_hp, expected_block] = expected_monster_hp_and_block.at(i);

    EXPECT_EQ(monster.current_hp, expected_hp);
    EXPECT_EQ(monster.current_block, expected_block);
  }
}

TEST(CombatUpdaterTest, IgnoreBlockDamageMonsterTest) {
  // Setup
  constexpr int DAMAGE_AMOUNT_1 = 5;
  constexpr int DAMAGE_AMOUNT_2 = 3;
  constexpr int DAMAGE_AMOUNT_3 = 12;
  constexpr int DAMAGE_AMOUNT_4 = 20;
  const CombatState state = create_dummy_combat_state();

  // Action
  CombatState new_state = state;
  for (auto &[target, amount] :
       std::vector<std::pair<Target, int>>{std::make_pair(TargetMonster{0}, DAMAGE_AMOUNT_1),
                                           std::make_pair(TargetMonster{1}, DAMAGE_AMOUNT_2),
                                           std::make_pair(TargetMonster{2}, DAMAGE_AMOUNT_3),
                                           std::make_pair(TargetPlayer{}, DAMAGE_AMOUNT_4)}) {
    cards::proto::ApplyDamage effect;
    effect.set_value(amount);
    effect.set_ignores_block(true);
    new_state = detail::apply_effect(std::move(new_state), effect, target);
  }

  // Verification
  // Check the monsters
  const std::array<std::tuple<int, int>, 3> expected_monster_hp_and_block{{{5, 0}, {2, 10}, {0, 10}}};
  for (size_t i = 0; i < new_state.monsters.size(); i++) {
    const auto &monster = new_state.monsters.at(i);
    const auto &[expected_hp, expected_block] = expected_monster_hp_and_block.at(i);

    EXPECT_EQ(monster.current_hp, expected_hp);
    EXPECT_EQ(monster.current_block, expected_block);
  }

  // Check the player
  EXPECT_EQ(new_state.player.current_hp, state.player.current_hp - DAMAGE_AMOUNT_4);
  EXPECT_EQ(new_state.player.current_block, state.player.current_block);
}
}  // namespace sts::sim
