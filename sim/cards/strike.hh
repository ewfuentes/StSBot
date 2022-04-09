
#include "sim/cards/get_unique_id.hh"
#include "sim/combat.hh"

namespace sts::sim::cards {
struct Strike : public Card {
  int damage_amount;

  Strike(int energy_cost, int damage_amount)
      : Card("Strike", energy_cost, get_unique_id(), Type::ATTACK),
        damage_amount(damage_amount) {}

  std::vector<Action> create_actions(const CombatState &combat_state) override {
    if (combat_state.player.current_energy < cost) {
      return {};
    }

    std::vector<Action> out;
    for (size_t i = 0; i < combat_state.monsters.size(); i++) {
      const auto &m = combat_state.monsters.at(i);
      out.push_back(
          {.descriptor = "Attack " + m.name + " for " +
                         std::to_string(damage_amount) +
                         " (Cost: " + std::to_string(cost) + ")",
           .apply = [this, i](const CombatState &state) -> CombatState {
             CombatState out = state;
             // Apply player effects
             out.player.current_energy -= cost;
             // Apply monster effects
             const int excess_damage =
                 std::max(damage_amount - out.monsters.at(i).current_block, 0);
             out.monsters.at(i).current_block =
                 std::max(out.monsters.at(i).current_block - damage_amount, 0);
             out.monsters.at(i).current_hp -=
                 std::min(excess_damage, out.monsters.at(i).current_hp);
             // Apply power effects
             // Apply relic effects
             // Move to relevant pile
             out.deck.discard.insert(out.deck.hand.extract(*this));
             return out;
           }});
    }
    return out;
  }
};
}  // namespace sts::sim::cards
