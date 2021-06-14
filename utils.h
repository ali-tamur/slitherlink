#ifndef PUZ_UTILS_H_
#define PUZ_UTILS_H_

#include <cstdint>
#include <tuple>

namespace puzzle {

enum Orientation : uint8_t { VERTICAL, HORIZONTAL };

int RowColToIndex(int rows, int cols, int r, int c);

std::tuple<int, int> IndexToRowCol(int rows, int cols, int index);

}  // namespace puzzle

#endif /* PUZ_UTILS_H_ */
