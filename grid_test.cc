#include "puz/grid.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::ElementsAre;
using ::testing::UnorderedElementsAre;

namespace puzzle {

TEST(Grid, mock) { 
  Grid g(3, 3);
  g.Init();
  Face& F00 = g.FaceAt(0,0);
  Face& F01 = g.FaceAt(0,1);
  Face& F02 = g.FaceAt(0,2);
  Face& F10 = g.FaceAt(1,0);
  Face& F11 = g.FaceAt(1,1);
  Face& F12 = g.FaceAt(1,2);
  Face& F20 = g.FaceAt(2,0);
  Face& F21 = g.FaceAt(2,1);
  Face& F22 = g.FaceAt(2,2);
  Corner& C00 = g.CornerAt(0,0);
  Corner& C01 = g.CornerAt(0,1);
  Corner& C02 = g.CornerAt(0,2);
  Corner& C03 = g.CornerAt(0,3);
  Corner& C10 = g.CornerAt(1,0);
  Corner& C11 = g.CornerAt(1,1);
  Corner& C12 = g.CornerAt(1,2);
  Corner& C13 = g.CornerAt(1,3);
  Corner& C20 = g.CornerAt(2,0);
  Corner& C21 = g.CornerAt(2,1);
  Corner& C22 = g.CornerAt(2,2);
  Corner& C23 = g.CornerAt(2,3);
  Corner& C30 = g.CornerAt(3,0);
  Corner& C31 = g.CornerAt(3,1);
  Corner& C32 = g.CornerAt(3,2);
  Corner& C33 = g.CornerAt(3,3);
  Edge& H00 = g.EdgeAt(0, 0, Orientation::HORIZONTAL);
  Edge& H01 = g.EdgeAt(0, 1, Orientation::HORIZONTAL);
  Edge& H02 = g.EdgeAt(0, 2, Orientation::HORIZONTAL);
  Edge& H10 = g.EdgeAt(1, 0, Orientation::HORIZONTAL);
  Edge& H11 = g.EdgeAt(1, 1, Orientation::HORIZONTAL);
  Edge& H12 = g.EdgeAt(1, 2, Orientation::HORIZONTAL);
  Edge& H20 = g.EdgeAt(2, 0, Orientation::HORIZONTAL);
  Edge& H21 = g.EdgeAt(2, 1, Orientation::HORIZONTAL);
  Edge& H22 = g.EdgeAt(2, 2, Orientation::HORIZONTAL);
  Edge& H30 = g.EdgeAt(3, 0, Orientation::HORIZONTAL);
  Edge& H31 = g.EdgeAt(3, 1, Orientation::HORIZONTAL);
  Edge& H32 = g.EdgeAt(3, 2, Orientation::HORIZONTAL);
  Edge& V00 = g.EdgeAt(0, 0, Orientation::VERTICAL);
  Edge& V01 = g.EdgeAt(0, 1, Orientation::VERTICAL);
  Edge& V02 = g.EdgeAt(0, 2, Orientation::VERTICAL);
  Edge& V03 = g.EdgeAt(0, 3, Orientation::VERTICAL);
  Edge& V10 = g.EdgeAt(1, 0, Orientation::VERTICAL);
  Edge& V11 = g.EdgeAt(1, 1, Orientation::VERTICAL);
  Edge& V12 = g.EdgeAt(1, 2, Orientation::VERTICAL);
  Edge& V13 = g.EdgeAt(1, 3, Orientation::VERTICAL);
  Edge& V20 = g.EdgeAt(2, 0, Orientation::VERTICAL);
  Edge& V21 = g.EdgeAt(2, 1, Orientation::VERTICAL);
  Edge& V22 = g.EdgeAt(2, 2, Orientation::VERTICAL);
  Edge& V23 = g.EdgeAt(2, 3, Orientation::VERTICAL);

  EXPECT_THAT(F00.Corners(), ElementsAre(&C01, &C11, &C10, &C00));
  EXPECT_THAT(F01.Corners(), ElementsAre(&C02, &C12, &C11, &C01));
  EXPECT_THAT(F02.Corners(), ElementsAre(&C03, &C13, &C12, &C02));
  EXPECT_THAT(F10.Corners(), ElementsAre(&C11, &C21, &C20, &C10));
  EXPECT_THAT(F11.Corners(), ElementsAre(&C12, &C22, &C21, &C11));
  EXPECT_THAT(F12.Corners(), ElementsAre(&C13, &C23, &C22, &C12));
  EXPECT_THAT(F20.Corners(), ElementsAre(&C21, &C31, &C30, &C20));
  EXPECT_THAT(F21.Corners(), ElementsAre(&C22, &C32, &C31, &C21));
  EXPECT_THAT(F22.Corners(), ElementsAre(&C23, &C33, &C32, &C22));

  EXPECT_THAT(F00.Edges(), ElementsAre(&H00, &V01, &H10, &V00));
  EXPECT_THAT(F01.Edges(), ElementsAre(&H01, &V02, &H11, &V01));
  EXPECT_THAT(F02.Edges(), ElementsAre(&H02, &V03, &H12, &V02));
  EXPECT_THAT(F10.Edges(), ElementsAre(&H10, &V11, &H20, &V10));
  EXPECT_THAT(F11.Edges(), ElementsAre(&H11, &V12, &H21, &V11));
  EXPECT_THAT(F12.Edges(), ElementsAre(&H12, &V13, &H22, &V12));
  EXPECT_THAT(F20.Edges(), ElementsAre(&H20, &V21, &H30, &V20));
  EXPECT_THAT(F21.Edges(), ElementsAre(&H21, &V22, &H31, &V21));
  EXPECT_THAT(F22.Edges(), ElementsAre(&H22, &V23, &H32, &V22));

  EXPECT_THAT(C00.Faces(), ElementsAre(nullptr, &F00, nullptr, nullptr));
  EXPECT_THAT(C01.Faces(), ElementsAre(nullptr, &F01, &F00, nullptr));
  EXPECT_THAT(C02.Faces(), ElementsAre(nullptr, &F02, &F01, nullptr));
  EXPECT_THAT(C03.Faces(), ElementsAre(nullptr, nullptr, &F02, nullptr));
  EXPECT_THAT(C10.Faces(), ElementsAre(&F00, &F10, nullptr, nullptr));
  EXPECT_THAT(C11.Faces(), ElementsAre(&F01, &F11, &F10, &F00));
  EXPECT_THAT(C12.Faces(), ElementsAre(&F02, &F12, &F11, &F01));
  EXPECT_THAT(C13.Faces(), ElementsAre(nullptr, nullptr, &F12, &F02));
  EXPECT_THAT(C20.Faces(), ElementsAre(&F10, &F20, nullptr, nullptr));
  EXPECT_THAT(C21.Faces(), ElementsAre(&F11, &F21, &F20, &F10));
  EXPECT_THAT(C22.Faces(), ElementsAre(&F12, &F22, &F21, &F11));
  EXPECT_THAT(C23.Faces(), ElementsAre(nullptr, nullptr, &F22, &F12));
  EXPECT_THAT(C30.Faces(), ElementsAre(&F20, nullptr, nullptr, nullptr));
  EXPECT_THAT(C31.Faces(), ElementsAre(&F21, nullptr, nullptr, &F20));
  EXPECT_THAT(C32.Faces(), ElementsAre(&F22, nullptr, nullptr, &F21));
  EXPECT_THAT(C33.Faces(), ElementsAre(nullptr, nullptr, nullptr, &F22));

  EXPECT_THAT(C00.Edges(), ElementsAre(nullptr, &H00, &V00, nullptr));
  EXPECT_THAT(C01.Edges(), ElementsAre(nullptr, &H01, &V01, &H00));
  EXPECT_THAT(C02.Edges(), ElementsAre(nullptr, &H02, &V02, &H01));
  EXPECT_THAT(C03.Edges(), ElementsAre(nullptr, nullptr, &V03, &H02));
  EXPECT_THAT(C10.Edges(), ElementsAre(&V00, &H10, &V10, nullptr));
  EXPECT_THAT(C11.Edges(), ElementsAre(&V01, &H11, &V11, &H10));
  EXPECT_THAT(C12.Edges(), ElementsAre(&V02, &H12, &V12, &H11));
  EXPECT_THAT(C13.Edges(), ElementsAre(&V03, nullptr, &V13, &H12));
  EXPECT_THAT(C20.Edges(), ElementsAre(&V10, &H20, &V20, nullptr));
  EXPECT_THAT(C21.Edges(), ElementsAre(&V11, &H21, &V21, &H20));
  EXPECT_THAT(C22.Edges(), ElementsAre(&V12, &H22, &V22, &H21));
  EXPECT_THAT(C23.Edges(), ElementsAre(&V13, nullptr, &V23, &H22));
  EXPECT_THAT(C30.Edges(), ElementsAre(&V20, &H30, nullptr, nullptr));
  EXPECT_THAT(C31.Edges(), ElementsAre(&V21, &H31, nullptr, &H30));
  EXPECT_THAT(C32.Edges(), ElementsAre(&V22, &H32, nullptr, &H31));
  EXPECT_THAT(C33.Edges(), ElementsAre(&V23, nullptr, nullptr, &H32));

  EXPECT_THAT(H00.Faces(), UnorderedElementsAre(std::make_tuple(&F00, 0)));
  EXPECT_THAT(H01.Faces(), UnorderedElementsAre(std::make_tuple(&F01, 0)));
  EXPECT_THAT(H02.Faces(), UnorderedElementsAre(std::make_tuple(&F02, 0)));
  EXPECT_THAT(H10.Faces(), UnorderedElementsAre(std::make_tuple(&F00, 2), std::make_tuple(&F10, 0)));
  EXPECT_THAT(H11.Faces(), UnorderedElementsAre(std::make_tuple(&F01, 2), std::make_tuple(&F11, 0)));
  EXPECT_THAT(H12.Faces(), UnorderedElementsAre(std::make_tuple(&F02, 2), std::make_tuple(&F12, 0)));
  EXPECT_THAT(H20.Faces(), UnorderedElementsAre(std::make_tuple(&F10, 2), std::make_tuple(&F20, 0)));
  EXPECT_THAT(H21.Faces(), UnorderedElementsAre(std::make_tuple(&F11, 2), std::make_tuple(&F21, 0)));
  EXPECT_THAT(H22.Faces(), UnorderedElementsAre(std::make_tuple(&F12, 2), std::make_tuple(&F22, 0)));
  EXPECT_THAT(H30.Faces(), UnorderedElementsAre(std::make_tuple(&F20, 2)));
  EXPECT_THAT(H31.Faces(), UnorderedElementsAre(std::make_tuple(&F21, 2)));
  EXPECT_THAT(H32.Faces(), UnorderedElementsAre(std::make_tuple(&F22, 2)));

  EXPECT_THAT(V00.Faces(), UnorderedElementsAre(std::make_tuple(&F00, 3)));
  EXPECT_THAT(V01.Faces(), UnorderedElementsAre(std::make_tuple(&F00, 1), std::make_tuple(&F01, 3)));
  EXPECT_THAT(V02.Faces(), UnorderedElementsAre(std::make_tuple(&F01, 1), std::make_tuple(&F02, 3)));
  EXPECT_THAT(V03.Faces(), UnorderedElementsAre(std::make_tuple(&F02, 1)));
  EXPECT_THAT(V10.Faces(), UnorderedElementsAre(std::make_tuple(&F10, 3)));
  EXPECT_THAT(V11.Faces(), UnorderedElementsAre(std::make_tuple(&F10, 1), std::make_tuple(&F11, 3)));
  EXPECT_THAT(V12.Faces(), UnorderedElementsAre(std::make_tuple(&F11, 1), std::make_tuple(&F12, 3)));
  EXPECT_THAT(V13.Faces(), UnorderedElementsAre(std::make_tuple(&F12, 1)));
  EXPECT_THAT(V20.Faces(), UnorderedElementsAre(std::make_tuple(&F20, 3)));
  EXPECT_THAT(V21.Faces(), UnorderedElementsAre(std::make_tuple(&F20, 1), std::make_tuple(&F21, 3)));
  EXPECT_THAT(V22.Faces(), UnorderedElementsAre(std::make_tuple(&F21, 1), std::make_tuple(&F22, 3)));
  EXPECT_THAT(V23.Faces(), UnorderedElementsAre(std::make_tuple(&F22, 1)));

  EXPECT_THAT(H00.Corners(), UnorderedElementsAre(std::make_tuple(&C00, 1), std::make_tuple(&C01, 3)));
  EXPECT_THAT(H01.Corners(), UnorderedElementsAre(std::make_tuple(&C01, 1), std::make_tuple(&C02, 3)));
  EXPECT_THAT(H02.Corners(), UnorderedElementsAre(std::make_tuple(&C02, 1), std::make_tuple(&C03, 3)));
  EXPECT_THAT(H10.Corners(), UnorderedElementsAre(std::make_tuple(&C10, 1), std::make_tuple(&C11, 3)));
  EXPECT_THAT(H11.Corners(), UnorderedElementsAre(std::make_tuple(&C11, 1), std::make_tuple(&C12, 3)));
  EXPECT_THAT(H12.Corners(), UnorderedElementsAre(std::make_tuple(&C12, 1), std::make_tuple(&C13, 3)));
  EXPECT_THAT(H20.Corners(), UnorderedElementsAre(std::make_tuple(&C20, 1), std::make_tuple(&C21, 3)));
  EXPECT_THAT(H21.Corners(), UnorderedElementsAre(std::make_tuple(&C21, 1), std::make_tuple(&C22, 3)));
  EXPECT_THAT(H22.Corners(), UnorderedElementsAre(std::make_tuple(&C22, 1), std::make_tuple(&C23, 3)));
  EXPECT_THAT(H30.Corners(), UnorderedElementsAre(std::make_tuple(&C30, 1), std::make_tuple(&C31, 3)));
  EXPECT_THAT(H31.Corners(), UnorderedElementsAre(std::make_tuple(&C31, 1), std::make_tuple(&C32, 3)));
  EXPECT_THAT(H32.Corners(), UnorderedElementsAre(std::make_tuple(&C32, 1), std::make_tuple(&C33, 3)));

  EXPECT_THAT(V00.Corners(), UnorderedElementsAre(std::make_tuple(&C00, 2), std::make_tuple(&C10, 0)));
  EXPECT_THAT(V01.Corners(), UnorderedElementsAre(std::make_tuple(&C01, 2), std::make_tuple(&C11, 0)));
  EXPECT_THAT(V02.Corners(), UnorderedElementsAre(std::make_tuple(&C02, 2), std::make_tuple(&C12, 0)));
  EXPECT_THAT(V03.Corners(), UnorderedElementsAre(std::make_tuple(&C03, 2), std::make_tuple(&C13, 0)));
  EXPECT_THAT(V10.Corners(), UnorderedElementsAre(std::make_tuple(&C10, 2), std::make_tuple(&C20, 0)));
  EXPECT_THAT(V11.Corners(), UnorderedElementsAre(std::make_tuple(&C11, 2), std::make_tuple(&C21, 0)));
  EXPECT_THAT(V12.Corners(), UnorderedElementsAre(std::make_tuple(&C12, 2), std::make_tuple(&C22, 0)));
  EXPECT_THAT(V13.Corners(), UnorderedElementsAre(std::make_tuple(&C13, 2), std::make_tuple(&C23, 0)));
  EXPECT_THAT(V20.Corners(), UnorderedElementsAre(std::make_tuple(&C20, 2), std::make_tuple(&C30, 0)));
  EXPECT_THAT(V21.Corners(), UnorderedElementsAre(std::make_tuple(&C21, 2), std::make_tuple(&C31, 0)));
  EXPECT_THAT(V22.Corners(), UnorderedElementsAre(std::make_tuple(&C22, 2), std::make_tuple(&C32, 0)));
  EXPECT_THAT(V23.Corners(), UnorderedElementsAre(std::make_tuple(&C23, 2), std::make_tuple(&C33, 0)));

//   B00_UR = g.Boundaries()[0];
//   B00_DR = g.Boundaries()[1];
//   B00_DL = g.Boundaries()[2];
//   B00_UL = g.Boundaries()[3];

//  EXPECT_THAT(B00_UR.Edges(), UnorderedElementsAre(&H00



}

}  // namespace puzzle
