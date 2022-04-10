
#include "planning/turn_planner.hh"

#include <iostream>
#include <memory>

#include "gtest/gtest.h"
#include "sim/cards/defend.hh"
#include "sim/cards/strike.hh"

namespace sts::planning {
namespace {
sim::CombatState create_initial_state() {
  return sim::CombatState{.player = {.max_hp = 80,
                                     .current_hp = 55,
                                     .current_block = 0,
                                     .current_energy = 3},
                          .monsters = {{.name = "monster_a",
                                        .max_hp = 32,
                                        .current_hp = 32,
                                        .current_block = 0},
                                       {.name = "monster_b",
                                        .max_hp = 12,
                                        .current_hp = 12,
                                        .current_block = 0}},
                          .deck = {}};
}

int count_leaves(const GameTree &tree) {
  if (tree.successors.empty()) {
    return 1;
  }

  int num_leaves = 0;
  for (const auto &child : tree.successors) {
    num_leaves += count_leaves(child.second);
  }
  return num_leaves;
}

  void enumerate_paths(const GameTree &tree, std::vector<std::string> &&actions = {}) {
    if (tree.successors.empty()) {
      // Leaf node, print out action list
      std::cout << "{" << std::endl;
      for (const auto &action : actions) {
        std::cout << "\t" << action << std::endl;
      }
      std::cout << "}" << std::endl;
      return;
    }

    for (const auto &child : tree.successors) {
      actions.push_back(child.first.descriptor);
      enumerate_paths(child.second, std::move(actions));
      actions.pop_back();
    }
}

}  // namespace
TEST(BuildGameTreeTest, SingleDefendAction) {
  // Setup
  sim::CombatState initial_state = create_initial_state();
  initial_state.deck.hand.insert(std::make_shared<sim::cards::Defend>(1, 5));

  // Action
  const GameTree game_tree = build_game_tree(initial_state);

  // Verification
  EXPECT_EQ(game_tree.successors.size(), 1);
  enumerate_paths(game_tree);
}

TEST(BuildGameTreeTest, SingleStrikeAction) {
  // Setup
  sim::CombatState initial_state = create_initial_state();
  initial_state.deck.hand.insert(std::make_shared<sim::cards::Strike>(1, 5));

  // Action
  const GameTree game_tree = build_game_tree(initial_state);

  // Verification
  EXPECT_EQ(game_tree.successors.size(), 2);
  enumerate_paths(game_tree);
}

TEST(BuildGameTreeTest, StrikeAndDefendAction) {
  // Setup
  sim::CombatState initial_state = create_initial_state();
  initial_state.deck.hand.insert(std::make_shared<sim::cards::Strike>(1, 5));
  initial_state.deck.hand.insert(std::make_shared<sim::cards::Defend>(1, 5));

  // Action
  const GameTree game_tree = build_game_tree(initial_state);

  // Verification
  // We could:
  //  - strike one of two creature and defend or
  //  - defend and strike one of two creatures
  // Note that there are only two unique leaves, but we don't currently do any
  // deduplication of state yet.
  EXPECT_EQ(count_leaves(game_tree), 4);
  enumerate_paths(game_tree);
}
}  // namespace sts::planning
