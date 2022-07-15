
#include "sim/combat_updater.hh"

#include <type_traits>

namespace sts::sim {

namespace detail {
template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
template <typename T>
inline constexpr bool always_false_v = false;

CombatState apply_effect(CombatState in, const cards::proto::EnergyAdjust &effect,
                         const Target &target) {
  // This will throw if the target type is not none;
  std::get<None>(target);
  // TODO: Check that target is empty
  in.player.current_energy += effect.value();
  return in;
}

CombatState apply_effect(CombatState in, const cards::proto::ApplyBlock &effect,
                         const Target &target) {
  // This will throw if the target type is not none;
  std::get<None>(target);
  in.player.current_block += effect.value();
  return in;
}

CombatState apply_effect(CombatState in, const cards::proto::ApplyDamage &effect,
                         const Target &target) {
  const auto damage_actor = [&](auto &actor) {
    const int num_hits = std::max(effect.times(), 1);
    for (int i = 0; i < num_hits; i++) {
      const int blocked_damage =
          effect.ignores_block() ? 0 : std::min(effect.value(), actor.current_block);
      const int unblocked_damage = effect.value() - blocked_damage;

      // Reduce the block by the blocked damage and reduce the health by the unblocked damage
      actor.current_block -= blocked_damage;
      actor.current_hp -= std::min(unblocked_damage, actor.current_hp);
    }
  };

  std::visit(overloaded{
                 [&](TargetPlayer &&) -> void { damage_actor(in.player); },
                 [&](TargetMonster &&target) -> void { damage_actor(in.monsters.at(target.idx)); },
                 [&](auto &&) -> void {},
             },
             Target(target));
  return in;
}

CombatState apply_effect(CombatState in, const cards::proto::ApplyStatus &, const Target &) {
  return in;
}

CombatState apply_effect(CombatState in, const cards::proto::Effect &effect, const Target &target) {
  switch (effect.effect_oneof_case()) {
    case cards::proto::Effect::kEnergyAdjust: {
      in = apply_effect(in, effect.energy_adjust(), target);
      break;
    }
    case cards::proto::Effect::kApplyBlock: {
      in = apply_effect(in, effect.apply_block(), target);
      break;
    }
    case cards::proto::Effect::kApplyDamage: {
      in = apply_effect(in, effect.apply_damage(), target);
      break;
    }
    case cards::proto::Effect::kApplyStatus: {
      in = apply_effect(in, effect.apply_status(), target);
      break;
    }
    case cards::proto::Effect::EFFECT_ONEOF_NOT_SET: {
      break;
    }
  }
  return in;
}
}  // namespace detail

CombatUpdater &CombatUpdater::update(const cards::proto::Card &card, const Target &target) {
  for (const auto &effect : card.effects()) {
    state_ = detail::apply_effect(std::move(state_), effect, target);
  }
  return *this;
}

}  // namespace sts::sim
