#ifndef PUZ_UTILS_H_
#define PUZ_UTILS_H_

#include <cstddef>
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

// Corner and Face states are 16-bit sets: bit i is set if the combination of
// their four edges encoded by i (bit k = edge k is on) is still possible.
constexpr unsigned ALL_FOUR_EDGES = 0b1111;  // the combination with every edge on
constexpr std::size_t STATE_BITS = 16;
// A state projected onto two edges has four combinations; all four still
// possible means the projection carries no information.
constexpr uint16_t NO_INFORMATION = 0b1111;

}  // namespace puzzle

#endif /* PUZ_UTILS_H_ */
