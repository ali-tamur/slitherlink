#ifndef PUZ_BOUNDARY_H_
#define PUZ_BOUNDARY_H_

#include <vector>

#include "puz/edge.h"
#include "puz/message.h"

namespace puzzle {

// Represents a rectangular region on the grid that contains one corner
// and cuts through the edges. THe idea is that the loop has to cross
// a boundary an even number of times.
// So a boundary observes its set of edges:
// a) If a single edge e is undecided, sends a message to e to make the
//    number of set edges even.
// b) If no edge is undecided and there is an odd number of set edges
//    fires a contradiciton.
class Boundary : public IAgent {
 public:
  Boundary() = default;
  ~Boundary() override = default;

  void Init(std::vector<Edge*> edges);

  MessageResult Receive(const Message& m, ActionStack* action_stack,
                        MessageStack* message_stack) override;

  void Undo() override;

  int Timestamp() override;

  uint16_t State() const override;

  std::string DebugPrint() const override;

 private:
  bool parity_ = false;
  std::vector<Edge*> edges_;
  std::vector<bool> stack_;
  // number of edges set.
  int n_ = 0;
  int timestamp_ = 0;
};

}  // namespace puzzle

#endif /* PUZ_BOUNDARY_H_ */
