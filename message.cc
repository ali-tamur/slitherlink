#include "puz/message.h"

#include <assert.h>

#include "puz/edge.h"

namespace puzzle {

bool MessageStack::IsEmpty() { return messages_.empty(); }

Message MessageStack::Pop() {
  assert(!messages_.empty() && "MessageStack.pop on empty");
  Message result = messages_.back();
  messages_.pop_back();
  return result;
}

void MessageStack::PushAll(const std::vector<Message>& s) {
  for (auto it = s.rbegin(); it != s.rend(); ++it) {
    messages_.push_back(*it);
  }
}

int ActionStack::Timestamp() { return timestamp_; }
void ActionStack::AddAction(Action a) {
  actions_.push_back(a);
  ++timestamp_;
}

// Removes elements and undo'es affects until an assumption.
// Also removes the assumption and returns it.
// If the vector is empty (no further assumption), returns a
// "null" assmption, with nullptr as the edge.
Action ActionStack::Undo() {
  while (!actions_.empty() && !actions_.back().is_it_an_assumption) {
    actions_.back().agent->Undo();
    actions_.pop_back();
  }
  if (actions_.empty()) {
    return Action(nullptr, false);
  }
  auto result = actions_.back();
  actions_.pop_back();
  return result;
}

std::string Message::DebugPrint() const {
  if (tag == MessageTag::T_FaceAndCorner) {
    return DebugPrintFaceAndCorner();
  }
  if (tag ==MessageTag::T_FromEdge) {
    return DebugPrintFromEdge();
  }
  if (tag ==MessageTag::T_ToEdge) {
    return DebugPrintToEdge();
  }
  return "*";
}

  std::string Message::DebugPrintFaceAndCorner() const {
    std::string result;
    result += "FC(Source=";
    result += source == nullptr ? "nullptr" : source->DebugPrint();
    result += ", Target=";
    result += target == nullptr ? "nullptr" : target->DebugPrint();
    result += ", Time=";
    result += std::to_string(timestamp);
    result += ", Projection=";
    result += std::to_string(projected_state);
    result += ", Indexes=";
    result += std::to_string(index1);
    result += ",";
    result += std::to_string(index2);
    result += ")";
    return result;
  }

  std::string Message::DebugPrintToEdge() const {
    std::string result;
    result += "To_E(Source=";
    result += source == nullptr ? "nullptr" : source->DebugPrint();
    result += ", Target=";
    result += target == nullptr ? "nullptr" : target->DebugPrint();
    result += ", Time=";
    result += std::to_string(timestamp);
    result += ", State=";
    result += edge_state ? "T" : "F";
    result += ")";
    return result;
  }

  std::string Message::DebugPrintFromEdge() const {
    std::string result;
    result += "Fr_Edge(Source=";
    result += source == nullptr ? "nullptr" : source->DebugPrint();
    result += ", Target=";
    result += target == nullptr ? "nullptr" : target->DebugPrint();
    result += ", Time=";
    result += std::to_string(timestamp);
    result += ", Index=";
    result += std::to_string(index1);
    result += ", State=";
    result += edge_state ? "T" : "F";
    result += ")";
    return result;
  }

}  // namespace puzzle
