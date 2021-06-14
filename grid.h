#ifndef PUZ_GRID_H_
#define PUZ_GRID_H_

#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "absl/strings/str_cat.h"
#include "puz/boundary.h"
#include "puz/corner.h"
#include "puz/edge.h"
#include "puz/face.h"
#include "puz/loops.h"
#include "puz/message.h"

namespace puzzle {

class Face;
class Edge;
class Corner;

int RowColToIndex(int rows, int cols, int r, int c);
std::tuple<int, int> IndexToRowCol(int rows, int cols, int index);

class Grid {
 public:
  Grid(int rows, int cols);

  void Init();

  int NRows() { return rows_; }
  int NCols() { return cols_; }

  Corner& CornerAt(int row, int col);

  Face& FaceAt(int row, int col);

  Edge& EdgeAt(int row, int col, Orientation o);



  std::vector<Edge>& Edges() { return edges_; };
  std::vector<Face>& Faces() { return faces_; };
  std::vector<Corner>& Corners() { return corners_; };
  std::vector<Boundary>& Boundaries() { return boundaries_; }

  std::string PrettyPrint();

 private:
  void InitBoundary(int row, int col, int boundary_index);
  std::string DrawLine(int row);
  std::string DrawRow(int row);

  int rows_, cols_;
  std::vector<Face> faces_;
  std::vector<Corner> corners_;
  std::vector<Edge> edges_;
  std::vector<Boundary> boundaries_;
  Loops loops_;
};

}  // namespace puzzle

#endif /* PUZ_GRID_H_ */
