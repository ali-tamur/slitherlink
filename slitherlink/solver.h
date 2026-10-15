#ifndef SLITHERLINK_SOLVER_H_
#define SLITHERLINK_SOLVER_H_

#include <algorithm>
#include <vector>

#include "slitherlink/bits.h"
#include "slitherlink/corner.h"
#include "slitherlink/edge.h"
#include "slitherlink/face.h"
#include "slitherlink/grid.h"
#include "slitherlink/message.h"

namespace slitherlink {

using Solution = std::vector<std::tuple<int, int, Orientation>>;

Solution GetSolution(Grid* g);

void InitialConsequences(Grid* g, MessageStack* ms);

class ManualAgent : public IAgent {
 public:
  ManualAgent() = default;
  ~ManualAgent() = default;

  void SetDebugPrint(std::string debug_string) { debug_string_ = debug_string; }

  MessageResult Receive(const Message& m, ActionStack* as, MessageStack* ms) override {
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

// Stop searching after this many solutions: two show the puzzle isn't unique.
constexpr int MAX_SOLUTIONS = 2;

// Returns up to max_solutions solutions of g.
std::vector<Solution> Solve(Grid* g, int max_solutions = MAX_SOLUTIONS);

}  // namespace slitherlink

#endif /* SLITHERLINK_SOLVER_H_ */
