#include "puz/face.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace puzzle {

TEST(Face, Init) {
  {
    Face f;
    f.Init();
    EXPECT_FALSE(f.HasValue());
    EXPECT_THAT(f.State(), (1 << 15) - 1);
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
    EXPECT_THAT(f.State(), (1 << 1) + (1 << 2) + (1 << 4) + (1 << 8));
  }
  {
    Face f;
    f.SetValue(2);
    f.Init();
    EXPECT_TRUE(f.HasValue());
    EXPECT_THAT(f.State(), (1 << 3) + (1 << 5) + (1 << 6) + (1 << 9) + (1 << 10) + (1 << 12));
  }
  {
    Face f;
    f.SetValue(3);
    f.Init();
    EXPECT_TRUE(f.HasValue());
    EXPECT_THAT(f.State(), (1 << 7) + (1 << 11) + (1 << 13) + (1 << 14));
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
    EXPECT_EQ(1,1);
  }
}

TEST(Face, NewState) {
  Face f;
  f.SetValue(2);
  f.Init();
  EXPECT_THAT(f.State(), 5736);
  {
    Message m = Message::FromEdge( nullptr, &f, 0, 0, false);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 64 + 1024 + 4096);
  }
  {
    Message m = Message::FromEdge( nullptr, &f, 0, 0, true);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 8 + 32 + 512);
  }
  {
    Message m = Message::FromEdge( nullptr, &f, 0, 1, false);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 32 + 512 + 4096);
  }
  {
    Message m = Message::FromEdge( nullptr, &f, 0, 1, true);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 8 + 64 + 1024);
  }
  {
    Message m = Message::FromEdge( nullptr, &f, 0, 2, false);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 8 + 512 + 1024);
  }
  {
    Message m = Message::FromEdge( nullptr, &f, 0, 2, true);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 32 + 64 + 4096);
  }
  {
    Message m = Message::FromEdge( nullptr, &f, 0, 3, false);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 8 + 32 + 64);
  }
  {
    Message m = Message::FromEdge( nullptr, &f, 0, 3, true);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 512 + 1024 + 4096);
  }
  {
    Message m = Message::FaceAndCorner( nullptr, &f, 0, 6, 0, 1);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 32 + 64 + 512 + 1024);
  }
  {
    Message m = Message::FaceAndCorner( nullptr, &f, 0, 5, 1, 2);
    uint16_t new_state = f.NewState(m);
    EXPECT_THAT(new_state, 32 + 512 + 4096);
  }
}

}  // namespace puzzle
