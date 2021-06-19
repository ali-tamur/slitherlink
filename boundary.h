#ifndef PUZ_BOUNDARY_H_
#define PUZ_BOUNDARY_H_

#include <vector>

#include "puz/edge.h"
#include "puz/message.h"

namespace puzzle {

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
  std::vector<Edge*>& Edges() { return edges_; }

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
