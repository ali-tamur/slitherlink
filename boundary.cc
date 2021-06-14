#include "puz/boundary.h"

#include <vector>

#include "puz/edge.h"
#include "puz/message.h"

namespace puzzle {

  void Boundary::Init(std::vector<Edge*> edges) {edges_ = edges;}

  MessageResult Boundary::Receive(const Message& m, ActionStack* action_stack,
                        MessageStack* message_stack) {
    assert((m.tag == MessageTag::T_FromEdge) && "Wrong MessageType");

    auto* source = m.source;
    bool edge_state = m.edge_state;

    stack_.push_back(parity_);
    parity_ = parity_ ^ edge_state;
    action_stack->AddAction(Action(this));
    timestamp_ = action_stack->Timestamp();
    ++n_;
    if (n_ == edges_.size() && parity_) {
      return MessageResult::CONTRADICTION;
    }
    if (n_ < edges_.size() - 1) {
      return MessageResult::OK;
    }
    // Exactly one edge is not set.
    for (auto* e : edges_) {
      if (e->State() == 3) {
        message_stack->PushAll({Message::ToEdge(this, (IAgent*)e, timestamp_, parity_)});
        break;
      }
    }
    return MessageResult::OK;
  }

  void Boundary::Undo() {
    --n_;
    parity_ = stack_.back();
    stack_.pop_back();
  }

  int Boundary::Timestamp() { return timestamp_; }

  uint16_t Boundary::State() const {
    // assert(false && "State() call on boundary");
    return 0;
  }

  std::string Boundary::DebugPrint() const { 
   std::string result;
   result += "B(";
   for (auto* e : edges_) {
     result += e->DebugPrint();
   }
   result += ")";
   return result;
  }

}  // namespace puzzle

