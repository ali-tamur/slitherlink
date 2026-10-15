#include "slitherlink/utils.h"

namespace puzzle {

int RowColToIndex(int rows, int cols, int r, int c) { return (cols * r) + c; }

std::tuple<int, int> IndexToRowCol(int rows, int cols, int index) {
  return std::make_tuple((index / cols), index % cols);
}

}  // namespace puzzle
