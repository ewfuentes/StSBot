
#include "sim/cards/strike.hh"

#include "gtest/gtest.h"

namespace sts::sim::cards {
TEST(StrikeTest, MultipleMonstersProduceMultipleActions) {
  // SETUP
  // Create Block Card
  constexpr int ENERGY_COST = 2;
  constexpr int DAMAGE_AMOUNT = 3;

  Strike strike_card(ENERGY_COST, DAMAGE_AMOUNT);

  // Create CombatState
  const CombatState combat_state{
      .player =
          {
              .max_hp = 100,
              .current_hp = 50,
              .current_block = 20,
              .current_energy = 10,
          },
      .monsters =
          {
              {.name = "monster_a",
               .max_hp = 20,
               .current_hp = 15,
               .current_block = 0},
              {.name = "monster_b",
               .max_hp = 30,
               .current_hp = 20,
               .current_block = 0},
          },
      .deck = {
          .hand = {strike_card}, .discard = {}, .draw = {}, .exhaust = {}}};

  // ACTION + VERIFICATION
  std::vector<Action> actions = strike_card.create_actions(combat_state);
  ASSERT_EQ(actions.size(), 2);

  for (size_t i = 0; i < actions.size(); i++) {
    const CombatState new_combat_state = actions.at(i).apply(combat_state);
    EXPECT_EQ(new_combat_state.monsters.at(i).current_block,
              combat_state.monsters.at(i).current_block);
    EXPECT_EQ(new_combat_state.monsters.at(i).current_hp,
              combat_state.monsters.at(i).current_hp - DAMAGE_AMOUNT);
    EXPECT_EQ(new_combat_state.player.current_energy,
              combat_state.player.current_energy - ENERGY_COST);
    EXPECT_TRUE(new_combat_state.deck.hand.empty());
    EXPECT_EQ(new_combat_state.deck.discard.size(), 1);
  }
}

TEST(StrikeTest, BlockHandledAppropriately) {
  // SETUP
  // Create Block Card
  constexpr int ENERGY_COST = 2;
  constexpr int DAMAGE_AMOUNT = 15;

  Strike strike_card(ENERGY_COST, DAMAGE_AMOUNT);

  // Create CombatState
  const CombatState combat_state{
      .player =
          {
              .max_hp = 100,
              .current_hp = 50,
              .current_block = 20,
              .current_energy = 10,
          },
      .monsters =
          {
              {.name = "monster_a",
               .max_hp = 20,
               .current_hp = 15,
               .current_block = 10},
              {.name = "monster_b",
               .max_hp = 30,
               .current_hp = 20,
               .current_block = 0},
              {.name = "monster_c",
               .max_hp = 40,
               .current_hp = 30,
               .current_block = 20},
          },
      .deck = {
          .hand = {strike_card}, .discard = {}, .draw = {}, .exhaust = {}}};

  // ACTION + VERIFICATION
  std::vector<Action> actions = strike_card.create_actions(combat_state);
  ASSERT_EQ(actions.size(), 3);

  const std::array<std::tuple<int, int>, 3> expected_monster_hp_and_block{
      std::make_tuple(10, 0), std::make_tuple(5, 0), std::make_tuple(30, 5)};

  for (size_t i = 0; i < actions.size(); i++) {
    const CombatState new_combat_state = actions.at(i).apply(combat_state);
    const auto &[expected_hp, expected_block] =
        expected_monster_hp_and_block.at(i);
    EXPECT_EQ(new_combat_state.monsters.at(i).current_block, expected_block);
    EXPECT_EQ(new_combat_state.monsters.at(i).current_hp, expected_hp);
    EXPECT_EQ(new_combat_state.player.current_energy,
              combat_state.player.current_energy - ENERGY_COST);
    EXPECT_TRUE(new_combat_state.deck.hand.empty());
    EXPECT_EQ(new_combat_state.deck.discard.size(), 1);
  }
}

TEST(StrikeTest, NotEnoughEnergyProducesNoActions) {
  // SETUP
  // Create Block Card
  constexpr int ENERGY_COST = 2;
  constexpr int DAMAGE_AMOUNT = 3;

  Strike strike_card(ENERGY_COST, DAMAGE_AMOUNT);

  // Create CombatState
  const CombatState combat_state{
      .player =
          {
              .max_hp = 100,
              .current_hp = 50,
              .current_block = 20,
              .current_energy = 1,
          },
      .monsters =
          {
              {.name = "monster_a",
               .max_hp = 20,
               .current_hp = 15,
               .current_block = 0},
              {.name = "monster_b",
               .max_hp = 30,
               .current_hp = 20,
               .current_block = 0},
          },
      .deck = {
          .hand = {strike_card}, .discard = {}, .draw = {}, .exhaust = {}}};

  // ACTION + VERIFICATION
  std::vector<Action> actions = strike_card.create_actions(combat_state);
  ASSERT_TRUE(actions.empty());
}
}  // namespace sts::sim::cards
