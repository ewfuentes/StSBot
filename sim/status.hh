
#pragma once

#include <variant>

#include "sim/cards/effect.pb.h"

namespace sts::sim {

struct Status {
  cards::proto::StatusType::Value type;
  int value;
};

}  // namespace sts::sim
