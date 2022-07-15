
#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>


namespace sts::sim {

struct CombatState;
struct Card;
enum class CardLocation { HAND, DRAW, DISCARD, EXHAUST };

struct Action {
  std::string descriptor;
  std::shared_ptr<const Card> maybe_card;
  std::optional<int> maybe_target;
  std::function<CombatState(const CombatState &)> apply;
};

struct PlayerState {
  int max_hp;
  int current_hp;
  int current_block;
  int current_energy;
};

struct MonsterState {
  std::string name;
  int max_hp;
  int current_hp;
  int current_block;
};

struct Card : public std::enable_shared_from_this<Card> {
  enum class Type { ATTACK, SKILL, POWER, CURSE };
  std::string name;
  int cost;
  int id;
  Type type;

  Card(std::string name, int cost, int id, Type type)
      : name(std::move(name)), cost(cost), id(id), type(type) {}
  Card(const Card &other) = default;
  virtual std::vector<Action> create_actions(const CombatState &) const { return {}; };
  virtual ~Card() {}
  bool operator<(const Card &other) const { return id < other.id; }
};

struct DeckState {
  std::set<std::shared_ptr<const Card>> hand;
  std::set<std::shared_ptr<const Card>> discard;
  std::set<std::shared_ptr<const Card>> draw;
  std::set<std::shared_ptr<const Card>> exhaust;
};

struct CombatState {
  PlayerState player;
  std::vector<MonsterState> monsters;
  DeckState deck;
};
}  // namespace sts::sim
