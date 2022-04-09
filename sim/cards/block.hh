
#include "sim/combat.hh"

namespace sts::sim::cards {
struct Block : public Card {
  Block(int cost, int block_amount)
      : Card{.name = "Block", .cost = cost, .id = 0, .type = Type::SKILL},
        block_amount(block_amount) {}

  int block_amount;

  std::vector<Action> create_actions(const CombatState &combat_state) {
    if (combat_state.player.current_energy < cost) {
      return {};
    }

    std::vector<Action> out;
    out.push_back({.descriptor = "Apply " + name + " for " +
                                 std::to_string(block_amount) +
                                 "(Cost: " + std::to_string(cost) + ")",
                   .apply = [this](const CombatState &state) -> CombatState {
                     CombatState out = state; // Apply player effects
                     out.player.current_block += block_amount;
                     out.player.current_energy -= cost;
                     // Apply monster effects
                     // Apply power effects
                     // Apply relic effects
                     // Move to relevant pile
                     out.deck.discard.insert(out.deck.hand.extract(*this));
                     return out;
                   }});
    return out;
  }
};
}  // namespace sts::sim::cards
