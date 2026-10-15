#include <iostream>

#include "corner.h"
#include "edge.h"
#include "face.h"
#include "grid.h"
#include "slitherlink/utils.h"
#include "solver.h"

namespace puzzle {
void SetEdge(Edge* e) {
  ActionStack a;
  MessageStack ms;
  Message m = Message::ToEdge(nullptr, e, 1, true);
  e->Receive(m, &a, &ms);
}

void InitGrid(Grid* g, std::string s) {
  for (int i = 0; i < s.size(); ++i) {
    char c = s[i];
    if (c == ' ') {
      continue;
    }
    auto tup = IndexToRowCol(g->NRows(), g->NCols(), i);
    int x = std::get<0>(tup);
    int y = std::get<1>(tup);
    int value = c - '0';
    g->FaceAt(x, y).SetValue(value);
  }
}

void Run() {
  //   Grid g(5, 5);
  //   g.FaceAt(0, 2).SetValue(2);
  //   g.FaceAt(1, 0).SetValue(3);
  //   g.FaceAt(1, 4).SetValue(2);
  //   g.FaceAt(2, 0).SetValue(2);
  //   g.FaceAt(2, 1).SetValue(1);
  //   g.FaceAt(2, 4).SetValue(3);
  //   g.FaceAt(3, 0).SetValue(1);
  //   g.FaceAt(3, 3).SetValue(1);
  //   g.FaceAt(4, 0).SetValue(3);
  //   g.FaceAt(4, 2).SetValue(3);
  //   g.FaceAt(4, 4).SetValue(3);
  //   g.Init();

  //  Grid g(7,7);
  //  InitGrid(&g, "3 132  2 2  12     23 021223 33 2      21  2 233 ");

  //   Grid g(10,10);
  //   InitGrid(&g,
  // "33   33   "
  // " 1 3 2  22"
  // " 220 1 1 1"
  // "  2 3   3 "
  // "2 2   1 12"
  // "0 2  3  3 "
  // "2     1   "
  // "2     21  "
  // " 3221  231"
  // "   32 22 3" );

  //   Grid g(2,2);
  //   g.FaceAt(0,0).SetValue(1);

  //   Grid g(3,3);
  //   InitGrid(&g, "3 01    1");
  //

  //   Grid g(15,15);
  //   InitGrid(&g,
  // "22  22223      "
  // " 3 1  12       "
  // " 3  2 2 12 3 2 "
  // " 2 3123   2  3 "
  // " 1  2     3 0 2"
  // "   2 2 1 2    3"
  // "   2   21  31  "
  // "  21 2   22  23"
  // "31 2 12 2    0 "
  // "   22    31  2 "
  // " 02  2 3 2 3 1 "
  // "3     3 22   32"
  // "   33  32 2 1 2"
  // "2   131  313   "
  // "  222 2    22 3");

  Grid g(15, 15);
  InitGrid(&g,
           "    2222  2   3"
           "21 2 2 12     2"
           "321 1    221  1"
           "1  23 22       "
           "22    32211312 "
           " 3  31  23  2  "
           "1 1222   1    2"
           "23 3  3 3 322 1"
           "   22 3 2  1  2"
           "1 2   222 22 23"
           "2  2 131    3 2"
           "       2  2   3"
           "322  231   3 2 "
           " 12221      2 3"
           "3  3  3232  223");

  g.Init();

  std::vector<Solution> solutions = Solve(&g);
  std::cout << "NUMBER OF SOLUTIONS:" << solutions.size() << "\n";
}

}  // namespace puzzle

int main() {
  puzzle::Run();
  return 0;
}
