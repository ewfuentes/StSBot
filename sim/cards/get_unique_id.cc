
#include "sim/cards/get_unique_id.hh"

namespace sts::sim::cards {
  int get_unique_id() {
    static int id_counter;
    return id_counter++;
  }
}
