#include "slitherlink/edge.h"

#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "absl/strings/str_cat.h"
#include "slitherlink/bits.h"
#include "slitherlink/corner.h"
#include "slitherlink/face.h"
#include "slitherlink/message.h"
#include "slitherlink/utils.h"

namespace puzzle {

namespace {
// Boundary and Loops ignore the message index; these values only make the
// messages easy to recognize when debugging.
constexpr int BOUNDARY_MESSAGE_INDEX = -389;
constexpr int LOOPS_MESSAGE_INDEX = -499;
}  // namespace

uint16_t Edge::State() const { return state_; }

double Edge::Score() const {
  double score = 0;
  for (const auto& f : faces_) {
    score += std::get<0>(f)->Score();
  }
  for (const auto& c : corners_) {
    score += std::get<0>(c)->Score();
  }
  return score;
}

std::string Edge::PrettyPrint() const {
  std::string r;
  if (state_ != EDGE_ON) {
    return " ";
  }
  r += "\033[93;1m";
  r += print_char_;
  r += "\033[0m";
  return r;
}

std::string Edge::DebugPrint() const {
  std::string result;
  result += "E(";
  result += std::to_string(row_);
  result += ',';
  result += std::to_string(col_);
  result += ',';
  result += o_ == Orientation::VERTICAL ? "V" : "H";
  result += ')';
  return result;
}

std::vector<Message> Edge::ConstructMessages() {
  std::vector<Message> r;
  bool bool_state = state_ == EDGE_ON;
  for (auto& tup : faces_) {
    Face* face = std::get<0>(tup);
    auto index = std::get<1>(tup);
    r.push_back(Message::FromEdge(this, face, timestamp_, index, bool_state));
  }
  for (auto& tup : corners_) {
    Corner* corner = std::get<0>(tup);
    assert((corner != nullptr) && "NULL CORNER");
    auto index = std::get<1>(tup);
    r.push_back(Message::FromEdge(this, corner, timestamp_, index, bool_state));
  }

  for (auto* b : boundaries_) {
    r.push_back(Message::FromEdge(this, b, timestamp_, BOUNDARY_MESSAGE_INDEX, bool_state));
  }

  r.push_back(Message::FromEdge(this, loops_, timestamp_, LOOPS_MESSAGE_INDEX, bool_state));

  return r;
}

uint16_t Edge::NewState(const Message& m) const {
  assert((m.tag == MessageTag::T_ToEdge) && "Wrong MessageType");
  return (m.edge_state ? EDGE_ON : EDGE_OFF) & state_;
}

MessageResult Edge::Receive(const Message& message, ActionStack* stack, MessageStack* messages) {
  uint16_t new_state = NewState(message);
  if (state_ == new_state) {
    return MessageResult::OK;
  }
  old_states_.push_back(state_);
  state_ = new_state;
  stack->AddAction(Action(this));
  timestamp_ = stack->Timestamp();
  if (new_state == CONTRADICTION) {
    return MessageResult::CONTRADICTION;
  }
  const auto& new_messages = ConstructMessages();
  messages->PushAll(new_messages);
  return MessageResult::OK;
}

void Edge::Undo() {
  assert(!old_states_.empty() && "Edge undo with empty stack");
  state_ = old_states_.back();
  old_states_.pop_back();
}

int Edge::Timestamp() { return timestamp_; }
}  // namespace puzzle
