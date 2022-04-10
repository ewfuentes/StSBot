
#include "sim/combat_updater.hh"

#include "gtest/gtest.h"

namespace sts::sim {
namespace {

CombatState create_dummy_combat_state() {
  return CombatState{
      .player = {.max_hp = 100,
                 .current_hp = 50,
                 .current_block = 10,
                 .current_energy = 3},
      .monsters = {{.name = "monster_a",
                    .max_hp = 25,
                    .current_hp = 10,
                    .current_block = 0},
                   {.name = "monster_b",
                    .max_hp = 30,
                    .current_hp = 5,
                    .current_block = 10},
                   {.name = "monster_c",
                    .max_hp = 30,
                    .current_hp = 5,
                    .current_block = 10}},
      .deck = {},
  };
}
}  // namespace

TEST(CombatUpdaterTest, AdjustEnergyTest) {
  // Setup
  constexpr int DOWN_UPDATE = -1;
  constexpr int UP_UPDATE = 2;
  const CombatState state = create_dummy_combat_state();
  CombatUpdater up_updater(state);
  CombatUpdater down_updater(state);

  // Action
  const CombatState down_state = down_updater.adjust_energy(DOWN_UPDATE);
  const CombatState up_state = up_updater.adjust_energy(UP_UPDATE);

  // Verification
  EXPECT_EQ(state.player.current_energy + UP_UPDATE,
            up_state.player.current_energy);
  EXPECT_EQ(state.player.current_energy + DOWN_UPDATE,
            down_state.player.current_energy);
}

TEST(CombatUpdaterTest, DamageMonsterTest) {
  // Setup
  constexpr int DAMAGE_AMOUNT_1 = 5;
  constexpr int DAMAGE_AMOUNT_2 = 6;
  constexpr int DAMAGE_AMOUNT_3 = 12;
  const CombatState state = create_dummy_combat_state();
  CombatUpdater updater(state);

  // Action
  const CombatState new_state = updater.damage_monster(DAMAGE_AMOUNT_1, 0)
                                    .damage_monster(DAMAGE_AMOUNT_2, 1)
                                    .damage_monster(DAMAGE_AMOUNT_3, 2);

  // Verification
  const std::array<std::tuple<int, int>, 3> expected_monster_hp_and_block{
      {{5, 0}, {5, 4}, {3, 0}}};
  for (size_t i = 0; i < new_state.monsters.size(); i++) {
    const auto &monster = new_state.monsters.at(i);
    const auto &[expected_hp, expected_block] =
        expected_monster_hp_and_block.at(i);

    EXPECT_EQ(monster.current_hp, expected_hp);
    EXPECT_EQ(monster.current_block, expected_block);
  }
}

TEST(CombatUpdaterTest, ApplyPlayerBlock) {
  // Setup
  constexpr int BLOCK_AMOUNT = 20;
  const CombatState state = create_dummy_combat_state();
  CombatUpdater updater(state);

  // Action
  const CombatState new_state = updater.apply_player_block(BLOCK_AMOUNT);

  // Verification
  EXPECT_EQ(new_state.player.current_block,
            state.player.current_block + BLOCK_AMOUNT);
}

TEST(CombatUpdaterTest, MoveCards) {
  // Setup
  const std::shared_ptr<Card> card_1 = std::make_shared<Card>("DummyAttack", 0, 10, Card::Type::ATTACK);
  const std::shared_ptr<Card> card_2 = std::make_shared<Card>("DummySkill", 0, 11, Card::Type::SKILL);
  const std::shared_ptr<Card> card_3 = std::make_shared<Card>("DummyPower", 0, 12, Card::Type::POWER);
  const std::shared_ptr<Card> card_4 = std::make_shared<Card>("DummyCurse", 0, 13, Card::Type::CURSE);
  CombatState state = create_dummy_combat_state();
  state.deck.draw.insert(card_1);
  state.deck.hand.insert(card_2);
  state.deck.exhaust.insert(card_3);
  state.deck.discard.insert(card_4);
  CombatUpdater updater(state);

  // Action
  updater.move_card_from_to(CardLocation::DRAW, CardLocation::HAND, *card_1)
      .move_card_from_to(CardLocation::HAND, CardLocation::DISCARD, *card_2)
    .move_card_from_to(CardLocation::EXHAUST, CardLocation::DRAW, *card_3)
      // Note that this one is invalid
      .move_card_from_to(CardLocation::HAND, CardLocation::DRAW, *card_4);
  const CombatState new_state = updater;

  // Verification
  EXPECT_EQ(new_state.deck.hand.size(), 1);
  EXPECT_EQ(new_state.deck.discard.size(), 2);
  EXPECT_EQ(new_state.deck.draw.size(), 1);
  EXPECT_EQ(new_state.deck.exhaust.size(), 0);

  EXPECT_TRUE(new_state.deck.hand.contains(card_1));
  EXPECT_TRUE(new_state.deck.discard.contains(card_2));
  EXPECT_TRUE(new_state.deck.draw.contains(card_3));
  EXPECT_TRUE(new_state.deck.discard.contains(card_4));
}

}  // namespace sts::sim
