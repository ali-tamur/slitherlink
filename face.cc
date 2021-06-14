#include "puz/face.h"

#include <math.h>

#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "absl/strings/str_cat.h"
#include "puz/bits.h"
#include "puz/corner.h"
#include "puz/message.h"

namespace puzzle {

void Face::SetValue(int val) {
  has_value_ = true;
  value_ = val;
}

void Face::Init() {
  uint16_t new_state = 0;
  // skip i == 15; it forms a loop.
  for (int i = 0; i < 15; ++i) {
    if (!has_value_ || __builtin_popcount(i) == value_) {
      new_state |= (1 << i);
    }
  }
  state_ = new_state;
  old_states_.clear();
}

int Face::Value() const {
  assert(has_value_ && "Face has no value");
  return value_;
}

bool Face::HasValue() const { return has_value_; }

std::string Face::PrettyPrint() const {
  if (!HasValue()) {
    return " ";
  }
  std::string r;
  r += "\033[1;31m";
  r += std::to_string(Value());
  r += "\033[0m";
  return r;
}

uint16_t Face::State() const { return state_; }

int Face::Timestamp() { return timestamp_; }

void Face::Undo() {
  assert(!old_states_.empty() && "Face undo with empty stack");
  state_ = old_states_.back();
  old_states_.pop_back();
}

std::vector<Message> Face::ConstructMessages() {
  std::vector<Message> r;
  for (int i = 0; i < edges_.size(); ++i) {
    uint16_t projected = ProjectSingle(state_, i);
    if (projected == 1 || projected == 2) {
      bool bool_state = projected == 2;
      r.push_back(Message::ToEdge(this, edges_[i], timestamp_, bool_state));
    }
  }
  {
    uint16_t projected_state = Project(state_, std::vector<int>{0, 1});
    if (projected_state != 15) {
      r.push_back(
          Message::FaceAndCorner(this, upright_, timestamp_, projected_state, 3, 2));
    }
  }
  {
    uint16_t projected_state = Project(state_, std::vector<int>{1, 2});
    if (projected_state != 15) {
      r.push_back(
          Message::FaceAndCorner(this, downright_, timestamp_, projected_state, 0, 3));
    }
  }
  {
    uint16_t projected_state = Project(state_, std::vector<int>{2, 3});
    if (projected_state != 15) {
      r.push_back(
          Message::FaceAndCorner(this, downleft_, timestamp_, projected_state, 1, 0));
    }
  }
  {
    uint16_t projected_state = Project(state_, std::vector<int>{0, 3});
    if (projected_state != 15) {
      r.push_back(
          Message::FaceAndCorner(this, upleft_, timestamp_, projected_state, 1, 2));
    }
  }
  return r;
}

uint16_t Face::NewState(const Message& m) {
  assert((m.tag == MessageTag::T_FaceAndCorner || m.tag == MessageTag::T_FromEdge) &&
         "Wrong MessageType");
  if (m.tag == MessageTag::T_FromEdge){
    return ReduceSingle(state_, m.index1, m.edge_state);
  }
  {
    return Reduce2(state_, m.projected_state,
                   std::vector<int>{m.index1, m.index2});
  }
}

MessageResult Face::Receive(const Message& message, ActionStack* stack,
                            MessageStack* messages) {
  // assuming the sender already checked that the message is not obsolete.
  uint16_t new_state = NewState(message);
  if (state_ == new_state) {
    return MessageResult::OK;
  }
  old_states_.push_back(state_);
  state_ = new_state;
  stack->AddAction(Action(this));
  timestamp_ = stack->Timestamp();
  if (new_state == 0) {
    return MessageResult::CONTRADICTION;
  }
  const auto& new_messages = ConstructMessages();
  messages->PushAll(new_messages);
  return MessageResult::OK;
}

}  // namespace puzzle
