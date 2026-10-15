#ifndef SLITHERLINK_GRID_H_
#define SLITHERLINK_GRID_H_

#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "absl/strings/str_cat.h"
#include "slitherlink/boundary.h"
#include "slitherlink/corner.h"
#include "slitherlink/edge.h"
#include "slitherlink/face.h"
#include "slitherlink/loops.h"
#include "slitherlink/message.h"
#include "slitherlink/utils.h"

namespace slitherlink {

class Face;
class Edge;
class Corner;

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
  void LinkFaces();
  void LinkCorners();
  void LinkVerticalEdges();
  void LinkHorizontalEdges();
  void InitBoundaries();
  void InitBoundary(int row, int col, int bn);
  std::string DrawLine(int row);
  std::string DrawRow(int row);

  int rows_, cols_;
  std::vector<Face> faces_;
  std::vector<Corner> corners_;
  std::vector<Edge> edges_;
  std::vector<Boundary> boundaries_;
  Loops loops_;
};

}  // namespace slitherlink

#endif /* SLITHERLINK_GRID_H_ */
