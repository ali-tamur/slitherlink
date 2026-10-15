#include "slitherlink/face.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

// NOLINTBEGIN(readability-magic-numbers)
namespace slitherlink {

TEST(Face, Init) {
  {
    Face f;
    f.Init();
    EXPECT_FALSE(f.HasValue());
    EXPECT_THAT(f.State(), (1U << 15U) - 1);
  }
  {
    Face f;
    f.SetValue(0);
    f.Init();
    EXPECT_TRUE(f.HasValue());
    EXPECT_THAT(f.State(), 1);
  }
  {
    Face f;
    f.SetValue(1);
    f.Init();
    EXPECT_TRUE(f.HasValue());
    EXPECT_THAT(f.State(), (1U << 1U) + (1U << 2U) + (1U << 4U) + (1U << 8U));
  }
  {
    Face f;
    f.SetValue(2);
    f.Init();
    EXPECT_TRUE(f.HasValue());
    EXPECT_THAT(f.State(),
                (1U << 3U) + (1U << 5U) + (1U << 6U) + (1U << 9U) + (1U << 10U) + (1U << 12U));
  }
  {
    Face f;
    f.SetValue(3);
    f.Init();
    EXPECT_TRUE(f.HasValue());
    EXPECT_THAT(f.State(), (1U << 7U) + (1U << 11U) + (1U << 13U) + (1U << 14U));
  }
}

TEST(Face, ConstructMessages) {
  {
    Face f;
    f.SetValue(3);
    f.Init();
    const auto& messages = f.ConstructMessages();
    EXPECT_THAT(messages.size(), 4);
    {
      const auto& message = messages[0];
      EXPECT_THAT(message.tag, MessageTag::T_FaceAndCorner);

      EXPECT_THAT(message.source, &f);
      EXPECT_THAT(message.projected_state, 2 + 4 + 8);
      EXPECT_THAT(message.index1, 3);
      EXPECT_THAT(message.index2, 2);
    }
    {
      const auto& message = messages[1];
      EXPECT_THAT(message.tag, MessageTag::T_FaceAndCorner);
      EXPECT_THAT(message.source, &f);
      EXPECT_THAT(message.projected_state, 2 + 4 + 8);
      EXPECT_THAT(message.index1, 0);
      EXPECT_THAT(message.index2, 3);
    }
    {
      const auto& message = messages[2];
      EXPECT_THAT(message.tag, MessageTag::T_FaceAndCorner);
      EXPECT_THAT(message.source, &f);
      EXPECT_THAT(message.projected_state, 2 + 4 + 8);
      EXPECT_THAT(message.index1, 1);
      EXPECT_THAT(message.index2, 0);
    }
    {
      const auto& message = messages[3];
      EXPECT_THAT(message.tag, MessageTag::T_FaceAndCorner);
      EXPECT_THAT(message.source, &f);
      EXPECT_THAT(message.projected_state, 2 + 4 + 8);
      EXPECT_THAT(message.index1, 1);
      EXPECT_THAT(message.index2, 2);
    }
    EXPECT_EQ(1, 1);
  }
}

TEST(Face, NewState) {
  Face f;
  f.SetValue(2);
  f.Init();
  EXPECT_THAT(f.State(), 5736);
  {
    Message m = Message::FromEdge(nullptr, &f, 0, 0, false);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 64 + 1024 + 4096);
  }
  {
    Message m = Message::FromEdge(nullptr, &f, 0, 0, true);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 8 + 32 + 512);
  }
  {
    Message m = Message::FromEdge(nullptr, &f, 0, 1, false);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 32 + 512 + 4096);
  }
  {
    Message m = Message::FromEdge(nullptr, &f, 0, 1, true);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 8 + 64 + 1024);
  }
  {
    Message m = Message::FromEdge(nullptr, &f, 0, 2, false);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 8 + 512 + 1024);
  }
  {
    Message m = Message::FromEdge(nullptr, &f, 0, 2, true);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 32 + 64 + 4096);
  }
  {
    Message m = Message::FromEdge(nullptr, &f, 0, 3, false);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 8 + 32 + 64);
  }
  {
    Message m = Message::FromEdge(nullptr, &f, 0, 3, true);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 512 + 1024 + 4096);
  }
  {
    Message m = Message::FaceAndCorner(nullptr, &f, 0, 6, 0, 1);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 32 + 64 + 512 + 1024);
  }
  {
    Message m = Message::FaceAndCorner(nullptr, &f, 0, 5, 1, 2);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 32 + 512 + 4096);
  }
}

}  // namespace slitherlink
// NOLINTEND(readability-magic-numbers)
