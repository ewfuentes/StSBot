
#include <functional>
#include <set>
#include <string>
#include <vector>

namespace sts::sim {

struct CombatState;

struct Action {

  std::string descriptor;
  std::function<CombatState(const CombatState&)> apply;
};

struct PlayerState {
  int max_hp;
  int current_hp;
  int current_block;
  int current_energy;
};

struct MonsterState {
  int name;
  int max_hp;
  int current_hp;
  int current_block;
};

struct Card {
  enum class Type { ATTACK, SKILL, POWER, CURSE };
  std::string name;
  int cost;
  int id;
  Type type;
  std::vector<Action> create_actions(const CombatState &);
  bool operator<(const Card &other) const {
    return id < other.id;
  }
};

struct DeckState {
  std::set<Card> hand;
  std::set<Card> discard;
  std::set<Card> draw;
  std::set<Card> exhaust;
};

struct CombatState {
  PlayerState player;
  std::vector<MonsterState> monsters;
  DeckState deck;
};
}  // namespace sts::sim
