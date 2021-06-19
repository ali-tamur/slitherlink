#include "puz/corner.h"

#include <optional>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "absl/strings/str_cat.h"
#include "puz/bits.h"
#include "puz/face.h"
#include "puz/message.h"

namespace puzzle {

uint16_t Corner::State() const { return state_; }

void Corner::Init() {
  uint16_t new_state;
  // skip i == 15; it cannot have four edges.
  for (int i = 0; i < 15; ++i) {
    if (__builtin_popcount(i) % 2 == 0) {
      new_state |= (1 << i);
    }
  }
  for (int i = 0; i < 4; ++i) {
    if (edges_[i] == nullptr) {
      new_state = ReduceSingle(new_state, i, false);
    }
  }
  state_ = new_state;
  old_states_.clear();
}

std::vector<Message> Corner::ConstructMessages() {
  std::vector<Message> r;
  for (int i = 0; i < edges_.size(); ++i) {
    if (edges_[i] == nullptr) {
      continue;
    }
    uint16_t projected = ProjectSingle(state_, i);
    if (projected == 1 || projected == 2) {
      bool bool_state = projected == 2;
      r.push_back(Message::ToEdge(this, edges_[i], timestamp_, bool_state));
    }
  }
  if (upright_ != nullptr) {
    uint16_t projected_state = Project(state_, std::vector<int>{0, 1});
    if (projected_state != 15) {
      r.push_back(Message::FaceAndCorner(this, upright_, timestamp_,
                                         projected_state, 3, 2));
    }
  }
  if (downright_ != nullptr) {
    uint16_t projected_state = Project(state_, std::vector<int>{1, 2});
    if (projected_state != 15) {
      r.push_back(Message::FaceAndCorner(this, downright_, timestamp_,
                                         projected_state, 0, 3));
    }
  }
  if (downleft_ != nullptr) {
    uint16_t projected_state = Project(state_, std::vector<int>{2, 3});
    if (projected_state != 15) {
      r.push_back(Message::FaceAndCorner(this, downleft_, timestamp_,
                                         projected_state, 1, 0));
    }
  }
  if (upleft_ != nullptr) {
    uint16_t projected_state = Project(state_, std::vector<int>{0, 3});
    if (projected_state != 15) {
      r.push_back(Message::FaceAndCorner(this, upleft_, timestamp_,
                                         projected_state, 1, 2));
    }
  }
  return r;
}

std::string Corner::PrettyPrint() const {
  int n = __builtin_popcount(state_);
  if (n != 1) {
    return " ";
  }
  int i = __builtin_ctz(state_);
  if (i == 3) {
    return "\033[93;1m└\033[0m";
  } else if (i == 5) {
    return "\033[93;1m│\033[0m";
  } else if (i == 6) {
    return "\033[93;1m┌\033[0m";
  } else if (i == 9) {
    return "\033[93;1m┘\033[0m";
  } else if (i == 10) {
    return "\033[93;1m─\033[0m";
  } else if (i == 12) {
    return "\033[93;1m┐\033[0m";
  }
  return " ";
}

std::string Corner::SetPrintChar() {
  if (upleft_ != nullptr && downleft_ != nullptr && upright_ != nullptr &&
      downright_ != nullptr) {
    return "┼";
  }
  if (upleft_ == nullptr && downleft_ == nullptr && upright_ == nullptr) {
    return "┌";
  }
  if (upleft_ == nullptr && downleft_ == nullptr && downright_ == nullptr) {
    return "└";
  }
  if (upleft_ == nullptr && upright_ == nullptr && downright_ == nullptr) {
    return "┐";
  }
  if (upright_ == nullptr && downright_ == nullptr && downleft_ == nullptr) {
    return "┘";
  }
  if (upleft_ == nullptr && upright_ == nullptr) {
    return "┬";
  }
  if (downleft_ == nullptr && downright_ == nullptr) {
    return "┴";
  }
  if (upleft_ == nullptr && downleft_ == nullptr) {
    return "├";
  }
  if (upright_ == nullptr && downright_ == nullptr) {
    return "┤";
  }
  return "?";
}

uint16_t Corner::NewState(const Message& m) {
  assert((m.tag == MessageTag::T_FaceAndCorner ||
          m.tag == MessageTag::T_FromEdge) &&
         "Wrong MessageType");
  if (m.tag == MessageTag::T_FromEdge) {
    return ReduceSingle(state_, m.index1, m.edge_state);
  }
  return Reduce2(state_, m.projected_state,
                 std::vector<int>{m.index1, m.index2});
}

MessageResult Corner::Receive(const Message& message, ActionStack* stack,
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

void Corner::Undo() {
  assert(!old_states_.empty() && "Corner undo with empty stack");
  state_ = old_states_.back();
  old_states_.pop_back();
}

int Corner::Timestamp() { return timestamp_; }

}  // namespace puzzle
