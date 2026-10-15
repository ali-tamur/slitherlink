#include "slitherlink/grid.h"

#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "absl/strings/str_cat.h"
#include "slitherlink/bits.h"
#include "slitherlink/corner.h"
#include "slitherlink/face.h"
#include "slitherlink/message.h"
#include "slitherlink/utils.h"

namespace puzzle {

void Grid::Init() {
  for (auto& f : faces_) {
    f.Init();
  }
  for (auto& c : corners_) {
    c.Init();
  }
}

Grid::Grid(int rows, int cols) : rows_(rows), cols_(cols) {
  for (int i = 0; i < rows_ * cols_; ++i) {
    faces_.push_back(Face());
  }
  for (int i = 0; i < (rows_ + 1) * (cols_ + 1); ++i) {
    corners_.push_back(Corner());
  }
  for (int i = 0; i < (rows_ * (cols_ + 1)) + ((rows_ + 1) * cols_); ++i) {
    edges_.push_back(Edge());
  }

  for (int i = 0; i < rows_ * cols_; ++i) {
    auto tup = IndexToRowCol(rows_, cols_, i);
    auto row = std::get<0>(tup);
    auto col = std::get<1>(tup);
    faces_[i].SetCorners(&CornerAt(row, col + 1), &CornerAt(row + 1, col + 1),
                         &CornerAt(row + 1, col), &CornerAt(row, col));
    faces_[i].SetEdges(
        &EdgeAt(row, col, Orientation::HORIZONTAL), &EdgeAt(row, col + 1, Orientation::VERTICAL),
        &EdgeAt(row + 1, col, Orientation::HORIZONTAL), &EdgeAt(row, col, Orientation::VERTICAL));
  }
  for (int i = 0; i < (rows_ + 1) * (cols_ + 1); ++i) {
    auto tup = IndexToRowCol(rows_ + 1, cols_ + 1, i);
    auto row = std::get<0>(tup);
    auto col = std::get<1>(tup);
    corners_[i].SetFaces(row == 0 || col == cols_ ? nullptr : &FaceAt(row - 1, col),
                         row == rows_ || col == cols_ ? nullptr : &FaceAt(row, col),
                         col == 0 || row == rows_ ? nullptr : &FaceAt(row, col - 1),
                         row == 0 || col == 0 ? nullptr : &FaceAt(row - 1, col - 1));
    corners_[i].SetEdges(row == 0 ? nullptr : &EdgeAt(row - 1, col, Orientation::VERTICAL),
                         col == cols_ ? nullptr : &EdgeAt(row, col, Orientation::HORIZONTAL),
                         row == rows_ ? nullptr : &EdgeAt(row, col, Orientation::VERTICAL),
                         col == 0 ? nullptr : &EdgeAt(row, col - 1, Orientation::HORIZONTAL));
  }

  // Vertical Edges
  for (int i = 0; i < rows_ * (cols_ + 1); ++i) {
    auto tup = IndexToRowCol(rows_, cols_ + 1, i);
    auto row = std::get<0>(tup);
    auto col = std::get<1>(tup);
    std::vector<std::tuple<Face*, int>> face_list;
    if (col > 0) {
      face_list.push_back(std::make_tuple(&FaceAt(row, col - 1), 1));
    }
    if (col < cols_) {
      face_list.push_back(std::make_tuple(&FaceAt(row, col), 3));
    }
    edges_[i].SetCoordinate(row, col, Orientation::VERTICAL);
    edges_[i].SetFaces(face_list);
    edges_[i].SetCorners(
        {std::make_tuple(&CornerAt(row, col), 2), std::make_tuple(&CornerAt(row + 1, col), 0)});
  }
  // Horizontal Edges
  int base = rows_ * (cols_ + 1);
  for (int i = 0; i < (rows_ + 1) * cols_; ++i) {
    auto tup = IndexToRowCol(rows_ + 1, cols_, i);
    auto row = std::get<0>(tup);
    auto col = std::get<1>(tup);
    std::vector<std::tuple<Face*, int>> face_list;
    if (row > 0) {
      face_list.push_back(std::make_tuple(&FaceAt(row - 1, col), 2));
    }
    if (row < rows_) {
      face_list.push_back(std::make_tuple(&FaceAt(row, col), 0));
    }
    edges_[base + i].SetCoordinate(row, col, Orientation::HORIZONTAL);
    edges_[base + i].SetFaces(face_list);
    edges_[base + i].SetCorners(
        {std::make_tuple(&CornerAt(row, col), 1), std::make_tuple(&CornerAt(row, col + 1), 3)});
  }

  // Boundaries
  for (int r = 0; r < rows_; ++r) {
    for (int c = 0; c < cols_; ++c) {
      for (int n = 0; n < 4; ++n) {
        boundaries_.push_back(Boundary());
      }
    }
  }
  int bn = 0;
  for (int r = 0; r < rows_; ++r) {
    for (int c = 0; c < cols_; ++c) {
      InitBoundary(r, c, bn);
      bn += 4;
    }
  }

  loops_.Init(rows_, cols_, [&](int r, int c, Orientation o) { return &EdgeAt(r, c, o); });
  for (auto& e : edges_) {
    e.SetLoops(&loops_);
  }
}

void Grid::InitBoundary(int row, int col, int bn) {
  std::vector<Edge*> upleft;
  std::vector<Edge*> upright;
  std::vector<Edge*> downleft;
  std::vector<Edge*> downright;

  for (int r = 0; r <= rows_; ++r) {
    Edge* e = &EdgeAt(r, col, Orientation::HORIZONTAL);
    if (r <= row) {
      upleft.push_back(e);
      upright.push_back(e);
    } else {
      downleft.push_back(e);
      downright.push_back(e);
    }
  }

  for (int c = 0; c <= cols_; ++c) {
    Edge* e = &EdgeAt(row, c, Orientation::VERTICAL);
    if (c <= col) {
      upleft.push_back(e);
      downleft.push_back(e);
    } else {
      upright.push_back(e);
      downright.push_back(e);
    }
  }

  int dn = 0;
  for (auto& v : std::vector<std::vector<Edge*>>{upright, downright, downleft, upleft}) {
    boundaries_[bn + dn].Init(v);
    for (Edge* e : v) {
      e->AddBoundary(&boundaries_[bn + dn]);
    }
    ++dn;
  }
}

Corner& Grid::CornerAt(int row, int col) {
  return corners_[RowColToIndex(rows_ + 1, cols_ + 1, row, col)];
}

Face& Grid::FaceAt(int row, int col) { return faces_[RowColToIndex(rows_, cols_, row, col)]; }

Edge& Grid::EdgeAt(int row, int col, Orientation o) {
  if (o == VERTICAL) {
    return edges_[RowColToIndex(rows_, cols_ + 1, row, col)];
  }
  int result = (cols_ + 1) * rows_;
  return edges_[result + RowColToIndex(rows_ + 1, cols_, row, col)];
}

std::string Grid::PrettyPrint() {
  std::string result;
  for (int r = 0; r < rows_; ++r) {
    result += DrawLine(r);
    result += DrawRow(r);
  }
  result += DrawLine(rows_);
  return result;
}

std::string Grid::DrawLine(int r) {
  std::string result;
  for (int c = 0; c < cols_; ++c) {
    result += CornerAt(r, c).PrettyPrint();
    result += EdgeAt(r, c, Orientation::HORIZONTAL).PrettyPrint();
  }
  result += CornerAt(r, cols_).PrettyPrint();
  result += "\n";
  return result;
}

std::string Grid::DrawRow(int r) {
  std::string result;
  for (int c = 0; c < cols_; ++c) {
    result += EdgeAt(r, c, Orientation::VERTICAL).PrettyPrint();
    result += FaceAt(r, c).PrettyPrint();
  }
  result += EdgeAt(r, cols_, Orientation::VERTICAL).PrettyPrint();
  result += "\n";
  return result;
}

}  // namespace puzzle
