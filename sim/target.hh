
#pragma once

#include <variant>

namespace sts::sim {
struct TargetCard {};
struct TargetMonster {
  int idx;
};
struct TargetPlayer {};
struct None {};

using Target = std::variant<TargetCard, TargetMonster, TargetPlayer, None>;
}  // namespace sts::sim
