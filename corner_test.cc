#include "puz/corner.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "puz/edge.h"

namespace puzzle {

// TEST(Corner, Init) {
//   {
//     Corner c;
//     Edge e;
//     c.SetEdges(&e, &e, &e, &e);
//     c.Init();
//     EXPECT_THAT(c.State(), 1 + 8 + 32 + 64 + 512 + 1024 + 4096);
//   }
//   {
//     Corner c;
//     Edge e;
//     c.SetEdges(&e, &e, nullptr, nullptr);
//     c.Init();
//     EXPECT_THAT(c.State(), 1 + 8);
//   }
// }
// 
// TEST(Corner, ConstructMessages) {
//   {
//     Corner c;
//     c.Init();
//     const auto& messages = c.ConstructMessages();
//     EXPECT_THAT(messages.size(), 4);
//     {
//       const auto& message = messages[0];
//       EXPECT_THAT(message.tag, MessageTag::T_FaceAndCorner);
//       EXPECT_THAT(message.source, &f);
//       EXPECT_THAT(message.projected_state, 2 + 4 + 8);
//       EXPECT_THAT(message.index1, 2);
//       EXPECT_THAT(message.index2, 3);
//     }
//     {
//       const auto& message = messages[1];
//       EXPECT_THAT(message.tag, MessageTag::T_FaceAndCorner);
//       M_FaceAndCorner message = std::get<M_FaceAndCorner>(m);
//       EXPECT_THAT(message.source, &f);
//       EXPECT_THAT(message.projected_state, 2 + 4 + 8);
//       EXPECT_THAT(message.index1, 0);
//       EXPECT_THAT(message.index2, 3);
//     }
//     {
//       const auto& message = messages[2];
//       EXPECT_THAT(message.tag, MessageTag::T_FaceAndCorner);
//       M_FaceAndCorner message = std::get<M_FaceAndCorner>(m);
//       EXPECT_THAT(message.source, &f);
//       EXPECT_THAT(message.projected_state, 2 + 4 + 8);
//       EXPECT_THAT(message.index1, 0);
//       EXPECT_THAT(message.index2, 1);
//     }
//     {
//       const auto& message = messages[3];
//       EXPECT_THAT(message.tag, MessageTag::T_FaceAndCorner);
//       M_FaceAndCorner message = std::get<M_FaceAndCorner>(m);
//       EXPECT_THAT(message.source, &f);
//       EXPECT_THAT(message.projected_state, 2 + 4 + 8);
//       EXPECT_THAT(message.index1, 1);
//       EXPECT_THAT(message.index2, 2);
//     }
//     EXPECT_EQ(1,1);
//   }
// }
// 
// TEST(Face, NewState) {
//   Face f;
//   f.SetValue(2);
//   f.Init();
//   EXPECT_THAT(f.State(), 5736);
//   {
//     Message m = Message::FromEdge( nullptr, &f, 0, 0, false);
//     uint16_t new_state = f.NewState(m);
//     EXPECT_THAT(new_state, 64 + 1024 + 4096);
//   }
//   {
//     Message m = Message::FromEdge( nullptr, &f, 0, 0, true);
//     uint16_t new_state = f.NewState(m);
//     EXPECT_THAT(new_state, 8 + 32 + 512);
//   }
//   {
//     Message m = Message::FromEdge( nullptr, &f, 0, 1, false);
//     uint16_t new_state = f.NewState(m);
//     EXPECT_THAT(new_state, 32 + 512 + 4096);
//   }
//   {
//     Message m = Message::FromEdge( nullptr, &f, 0, 1, true);
//     uint16_t new_state = f.NewState(m);
//     EXPECT_THAT(new_state, 8 + 64 + 1024);
//   }
//   {
//     Message m = Message::FromEdge( nullptr, &f, 0, 2, false);
//     uint16_t new_state = f.NewState(m);
//     EXPECT_THAT(new_state, 8 + 512 + 1024);
//   }
//   {
//     Message m = Message::FromEdge( nullptr, &f, 0, 2, true);
//     uint16_t new_state = f.NewState(m);
//     EXPECT_THAT(new_state, 32 + 64 + 4096);
//   }
//   {
//     Message m = Message::FromEdge( nullptr, &f, 0, 3, false);
//     uint16_t new_state = f.NewState(m);
//     EXPECT_THAT(new_state, 8 + 32 + 64);
//   }
//   {
//     Message m = Message::FromEdge( nullptr, &f, 0, 3, true);
//     uint16_t new_state = f.NewState(m);
//     EXPECT_THAT(new_state, 512 + 1024 + 4096);
//   }
//   {
//     Message m = Message::FromEdge( nullptr, &f, 0, 6, 0, 1);
//     uint16_t new_state = f.NewState(m);
//     EXPECT_THAT(new_state, 32 + 64 + 512 + 1024);
//   }
//   {
//     Message m = Message::FromEdge( nullptr, &f, 0, 5, 1, 2);
//     uint16_t new_state = f.NewState(m);
//     EXPECT_THAT(new_state, 32 + 512 + 4096);
//   }
// }
// 
}  // namespace puzzle
