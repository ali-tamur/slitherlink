#include "slitherlink/loops.h"

#include <assert.h>

#include <functional>
#include <memory>
#include <tuple>
#include <vector>

#include "slitherlink/edge.h"
#include "slitherlink/message.h"
#include "slitherlink/utils.h"

namespace puzzle {

Node* Node::Merge(Node* other) {
  auto tup1 = Root();
  int n1 = std::get<0>(tup1);
  Node* root1 = std::get<1>(tup1);

  auto tup2 = other->Root();
  int n2 = std::get<0>(tup2);
  Node* root2 = std::get<1>(tup2);

  int e1 = root1->endpoint1_ + root1->endpoint2_ - my_index_;
  int e2 = root2->endpoint1_ + root2->endpoint2_ - other->my_index_;

  if (root1 == root2) {
    return nullptr;
  }
  if (n1 < n2) {
    root1->parent_ = root2;
    root2->Merged(e1, e2);
    return root1;
  } else {
    root2->parent_ = root1;
    root1->Merged(e1, e2);
    return root2;
  }
}

std::tuple<int, int> Node::Endpoints() {
  return std::make_tuple(endpoint1_, endpoint2_);
}

bool Node::IsString() { return parent_ != nullptr || endpoint1_ != endpoint2_; }

void Node::Unparent() {
  parent_->Undo();
  parent_ = nullptr;
}

Node* Node::Parent() { return parent_; }

void Node::Merged(int e1, int e2) {
  stack_.push_back(std::make_tuple(endpoint1_, endpoint2_));
  endpoint1_ = e1;
  endpoint2_ = e2;
}

void Node::Undo() {
  endpoint1_ = std::get<0>(stack_.back());
  endpoint2_ = std::get<1>(stack_.back());
  stack_.pop_back();
}

std::tuple<int, Node*> Node::Root() {
  Node* now = this;
  int n = 0;
  while (now->parent_ != nullptr) {
    now = now->parent_;
    ++n;
  }
  return std::make_tuple(n, now);
}

void Loops::Init(int rows, int cols,
                 std::function<Edge*(int, int, Orientation)> f) {
  rows_ = rows;
  cols_ = cols;
  edge_finder_ = f;
  for (int i = 0; i < (rows + 1) * (cols + 1); ++i) {
    nodes_.push_back(Node(i));
  }
}

MessageResult Loops::Receive(const Message& m, ActionStack* action_stack,
                             MessageStack* message_stack) {
  assert((m.tag == MessageTag::T_FromEdge) && "Wrong Message Type to Loops");
  const Edge* source = (Edge*)m.source;

  // Don't care about turned off edges.
  if (!m.edge_state) {
    return MessageResult::OK;
  }

  int edge_row = source->Row();
  int edge_col = source->Col();
  Orientation o = source->Orientation();

  auto tup_a = EdgeToCornerIndexes(edge_row, edge_col, o);
  int i1 = std::get<0>(tup_a);
  int i2 = std::get<1>(tup_a);

  // Find out the new number of strings after merge.
  bool is_string1 = nodes_[i1].IsString();
  bool is_string2 = nodes_[i2].IsString();
  int delta = (is_string1 && is_string2)     ? -1
              : (!is_string1 && !is_string2) ? 1
                                             : 0;

  Node* undo_node = nodes_[i1].Merge(&nodes_[i2]);
  stack_.push_back(std::make_tuple(undo_node, n_strings_));
  n_strings_ += delta;
  action_stack->AddAction(this);
  timestamp_ = action_stack->Timestamp();

  if (undo_node == nullptr && n_strings_ == 0) {
    // does not have to be a solution but needs to be checked.
    return MessageResult::SOLUTION;
  }
  if (undo_node == nullptr) {
    return MessageResult::CONTRADICTION;
  }
  if (n_strings_ == 1) {
    return MessageResult::OK;
  }
  if (!is_string1 && !is_string2) {
    return MessageResult::OK;
  }
  const auto& tup1 = undo_node->Parent()->Endpoints();
  int e1 = std::get<0>(tup1);
  int e2 = std::get<1>(tup1);
  Edge* maybe_common_edge = CommonEdge(e1, e2);
  if (maybe_common_edge != nullptr && maybe_common_edge != source) {
    message_stack->Push(
        Message::ToEdge(this, maybe_common_edge, timestamp_, false));
  }
  return MessageResult::OK;
}

void Loops::Undo() {
  assert(!stack_.empty() && "Undo on Empty stack");
  auto tup = stack_.back();
  auto* last_merged = std::get<0>(tup);
  int prev_nstrings = std::get<1>(tup);
  if (last_merged != nullptr) {
    last_merged->Unparent();
  }
  n_strings_ = prev_nstrings;
  stack_.pop_back();
}

int Loops::Timestamp() { return timestamp_; }
uint16_t Loops::State() const { return -1; }

std::tuple<int, int> Loops::EdgeToCornerIndexes(int row, int col,
                                                Orientation o) {
  if (o == VERTICAL) {
    return std::make_tuple(RowColToIndex(rows_ + 1, cols_ + 1, row, col),
                           RowColToIndex(rows_ + 1, cols_ + 1, row + 1, col));
  }
  return std::make_tuple(RowColToIndex(rows_ + 1, cols_ + 1, row, col),
                         RowColToIndex(rows_ + 1, cols_ + 1, row, col + 1));
}

// nullptr unless i1 and i2 point to neighboor corners, in which case
// returns the edge.
Edge* Loops::CommonEdge(int i1, int i2) {
  const auto& tup1 = IndexToRowCol(rows_ + 1, cols_ + 1, i1);
  int x1 = std::get<0>(tup1);
  int y1 = std::get<1>(tup1);

  const auto& tup2 = IndexToRowCol(rows_ + 1, cols_ + 1, i2);
  int x2 = std::get<0>(tup2);
  int y2 = std::get<1>(tup2);

  if (abs(x2 - x1) + abs(y2 - y1) != 1) {
    return nullptr;
  }
  if (y1 == y2) {
    return edge_finder_(std::min(x1, x2), y1, Orientation::VERTICAL);
  }
  return edge_finder_(x1, std::min(y1, y2), Orientation::HORIZONTAL);
}

}  // namespace puzzle
