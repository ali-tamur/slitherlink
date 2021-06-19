#ifndef PUZ_SOLVER_H_
#define PUZ_SOLVER_H_

#include <algorithm>
#include <vector>

#include "puz/bits.h"
#include "puz/corner.h"
#include "puz/edge.h"
#include "puz/face.h"
#include "puz/grid.h"
#include "puz/message.h"

namespace puzzle {

using Solution = std::vector<std::tuple<int, int, Orientation>>;

Solution GetSolution(Grid* g);

void InitialConsequences(Grid* g, MessageStack* ms);

class ManualAgent : public IAgent {
 public:
  ManualAgent() = default;
  ~ManualAgent() = default;

  void SetDebugPrint(std::string debug_string) { debug_string_ = debug_string; }

  MessageResult Receive(const Message& m, ActionStack* as,
                        MessageStack* ms) override {
    return MessageResult::OK;
  }
  void Undo() override {}
  int Timestamp() override { return 0; }
  uint16_t State() const override { return 0; }

  static ManualAgent* Initial() {
    static ManualAgent instance_for_initial = ManualAgent();
    instance_for_initial.SetDebugPrint("Initial");
    return &instance_for_initial;
  }

  static ManualAgent* Assume() {
    static ManualAgent instance_for_assume = ManualAgent();
    instance_for_assume.SetDebugPrint("Assume");
    return &instance_for_assume;
  }

  static ManualAgent* Therefore() {
    static ManualAgent instance_for_therefore = ManualAgent();
    instance_for_therefore.SetDebugPrint("Therefore");
    return &instance_for_therefore;
  }

  static ManualAgent* ThreeThree() {
    static ManualAgent instance_for_threethree = ManualAgent();
    instance_for_threethree.SetDebugPrint("ThreeThree");
    return &instance_for_threethree;
  }

  std::string DebugPrint() const override { return debug_string_; }

 private:
  std::string debug_string_;
};

void InitialThreeThree(Grid* g, MessageStack* ms);

Edge* MakeAssumption(Grid* g);

std::vector<Solution> Solve(Grid* g);

}  // namespace puzzle

#endif /* PUZ_SOLVER_H_ */
