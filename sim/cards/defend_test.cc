
#include "sim/cards/defend.hh"

#include "gtest/gtest.h"

namespace sts::sim::cards {
TEST(DefendTest, HappyCase) {
  // SETUP
  // Create Defend Card
  constexpr int ENERGY_COST = 2;
  constexpr int DEFEND_AMOUNT = 3;

  std::shared_ptr<Defend> defend_card = std::make_shared<Defend>(ENERGY_COST, DEFEND_AMOUNT);

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
              {.name = "monster_a", .max_hp = 20, .current_hp = 15, .current_block = 0},
          },
      .deck = {.hand = {defend_card}, .discard = {}, .draw = {}, .exhaust = {}}};

  // ACTION + VERIFICATION
  std::vector<Action> actions = defend_card->create_actions(combat_state);
  ASSERT_EQ(actions.size(), 1);

  const CombatState new_combat_state = actions.at(0).apply(combat_state);

  EXPECT_EQ(new_combat_state.player.current_block,
            combat_state.player.current_block + DEFEND_AMOUNT);
  EXPECT_EQ(new_combat_state.player.current_energy,
            combat_state.player.current_energy - ENERGY_COST);
  EXPECT_TRUE(new_combat_state.deck.hand.empty());
  EXPECT_EQ(new_combat_state.deck.discard.size(), 1);
}

TEST(DefendTest, NotEnoughEnergyProducesNoActions) {
  // SETUP
  // Create Defend Card
  constexpr int ENERGY_COST = 2;
  constexpr int DEFEND_AMOUNT = 3;

  std::shared_ptr<Defend> defend_card = std::make_shared<Defend>(ENERGY_COST, DEFEND_AMOUNT);

  // Create CombatState
  const CombatState combat_state{
      .player =
          {
              .max_hp = 100,
              .current_hp = 50,
              .current_block = 20,
              .current_energy = 1,
          },
      .monsters = {},
      .deck = {.hand = {defend_card}, .discard = {}, .draw = {}, .exhaust = {}}};

  // ACTION + VERIFICATION
  std::vector<Action> actions = defend_card->create_actions(combat_state);
  ASSERT_TRUE(actions.empty());
}
}  // namespace sts::sim::cards
