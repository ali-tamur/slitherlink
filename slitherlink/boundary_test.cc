#include "slitherlink/boundary.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace puzzle {

class BoundaryTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  MessageResult SendMessage(Boundary* b, Edge* e, bool edge_state) {
    Message setedge = Message::ToEdge(nullptr, e, 0, edge_state);
    e->Receive(setedge, &action_stack_, &message_stack_);
    message_stack_.Pop();
    Message to_boundary = Message::FromEdge(e, b, 0, 0, edge_state);
    auto result = b->Receive(to_boundary, &action_stack_, &message_stack_);
    return result;
  }

  ActionStack action_stack_;
  MessageStack message_stack_;
};

void SetEdge(Edge* e, bool state) {}

TEST_F(BoundaryTest, simple) {
  Edge e1;
  Edge e2;
  Edge e3;
  Boundary b;
  b.Init(std::vector<Edge*>{&e1, &e2, &e3});

  // 1 x x
  EXPECT_THAT(SendMessage(&b, &e1, true), MessageResult::OK);
  EXPECT_TRUE(message_stack_.IsEmpty());

  // x x x
  b.Undo();
  e1.Undo();

  // x 1 x
  EXPECT_THAT(SendMessage(&b, &e2, true), MessageResult::OK);
  EXPECT_TRUE(message_stack_.IsEmpty());

  // x 1 0
  EXPECT_THAT(SendMessage(&b, &e3, false), MessageResult::OK);
  EXPECT_TRUE(!message_stack_.IsEmpty());

  const auto& r = message_stack_.Pop();
  EXPECT_THAT(r.target, &e1);
  EXPECT_THAT(r.edge_state, true);

  // x 1 x
  b.Undo();
  e3.Undo();

  // 1 1 x
  EXPECT_THAT(SendMessage(&b, &e1, true), MessageResult::OK);
  EXPECT_TRUE(!message_stack_.IsEmpty());
  message_stack_.Pop();

  // 1 1 1 ==> Contradiction
  EXPECT_THAT(SendMessage(&b, &e3, true), MessageResult::CONTRADICTION);

  b.Undo();
  e3.Undo();
  EXPECT_THAT(SendMessage(&b, &e3, false), MessageResult::OK);
}

}  // namespace puzzle
