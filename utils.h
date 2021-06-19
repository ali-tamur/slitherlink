#ifndef PUZ_UTILS_H_
#define PUZ_UTILS_H_

#include <cstdint>
#include <tuple>

namespace puzzle {

enum Orientation : uint8_t { VERTICAL, HORIZONTAL };

int RowColToIndex(int rows, int cols, int r, int c);

std::tuple<int, int> IndexToRowCol(int rows, int cols, int index);

constexpr uint16_t CONTRADICTION = 0;
constexpr uint16_t EDGE_OFF = 1;
constexpr uint16_t EDGE_ON = 2;
constexpr uint16_t EDGE_UNKNOWN = 3;

}  // namespace puzzle

#endif /* PUZ_UTILS_H_ */
