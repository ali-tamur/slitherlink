#include "slitherlink/loops.h"

#include <functional>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "slitherlink/grid.h"
#include "slitherlink/message.h"

namespace puzzle {

class LoopsTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  MessageResult ToLoops(int row, int col, Orientation o, Loops* l) {
    Message m = Message::FromEdge(f_(row, col, o), l, 1, 0, true);
    MessageResult result = l->Receive(m, &action_stack_, &message_stack_);
    return result;
  }

  Grid grid_ = Grid(2, 3);
  ActionStack action_stack_;
  MessageStack message_stack_;
  std::function<Edge*(int, int, Orientation)> f_ =
      [&](int r, int c, Orientation o) { return &grid_.EdgeAt(r, c, o); };
};

TEST_F(LoopsTest, abc) {
  // . . . .
  // . . . .
  // . . . .
  Loops l;
  l.Init(2, 3, f_);
  EXPECT_THAT(l.NStrings(), 0);

  // . . . .
  // .─. . .
  // . . . .
  EXPECT_THAT(ToLoops(1, 0, Orientation::HORIZONTAL, &l), MessageResult::OK);
  EXPECT_THAT(l.NStrings(), 1);

  // . . . .
  // .─. . .
  // .─. . .
  EXPECT_THAT(ToLoops(2, 0, Orientation::HORIZONTAL, &l), MessageResult::OK);
  EXPECT_THAT(l.NStrings(), 2);

  // . . . .
  // ┌─. . .
  // └─. . .
  EXPECT_THAT(ToLoops(1, 0, Orientation::VERTICAL, &l), MessageResult::OK);
  EXPECT_THAT(l.NStrings(), 1);

  // . . . .
  // ┌─┐ . .
  // └─┘ . .
  EXPECT_THAT(ToLoops(1, 1, Orientation::VERTICAL, &l),
              MessageResult::SOLUTION);

  // . . . .
  // ┌─. . .
  // └─. . .
  l.Undo();
  EXPECT_THAT(l.NStrings(), 1);

  // . . . .
  // ┌─. .─.
  // └─. . .
  EXPECT_THAT(ToLoops(1, 2, Orientation::HORIZONTAL, &l), MessageResult::OK);
  EXPECT_THAT(l.NStrings(), 2);

  EXPECT_THAT(ToLoops(1, 1, Orientation::VERTICAL, &l),
              MessageResult::CONTRADICTION);
  l.Undo();
  // . . . .
  // ┌─┐ .─.
  // └─┘ . .

  // . . . .
  // ┌─. .─.
  // └─. .─.
  EXPECT_THAT(ToLoops(2, 2, Orientation::HORIZONTAL, &l), MessageResult::OK);
  EXPECT_THAT(l.NStrings(), 3);

  // . . . .
  // ┌─. .─┐
  // └─. .─┘
  EXPECT_TRUE(message_stack_.IsEmpty());
  EXPECT_THAT(ToLoops(1, 3, Orientation::VERTICAL, &l), MessageResult::OK);
  EXPECT_TRUE(!message_stack_.IsEmpty());
  auto ms = message_stack_.Pop();
  EXPECT_TRUE(ms.tag == MessageTag::T_ToEdge);
  EXPECT_THAT(ms.source, &l);
  EXPECT_THAT(ms.target, f_(1, 2, Orientation::VERTICAL));
  EXPECT_THAT(ms.edge_state, false);
  EXPECT_THAT(l.NStrings(), 2);

  // . . .
  // ┌─ ─┐
  // └───┘
  EXPECT_THAT(ToLoops(2, 1, Orientation::HORIZONTAL, &l), MessageResult::OK);
  EXPECT_THAT(l.NStrings(), 1);

  // . . .
  // ┌─ ─┐
  // └───┘
  EXPECT_THAT(ToLoops(1, 1, Orientation::HORIZONTAL, &l),
              MessageResult::SOLUTION);
  EXPECT_THAT(l.NStrings(), 0);
}

}  // namespace puzzle
