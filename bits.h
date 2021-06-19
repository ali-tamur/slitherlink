#ifndef PUZ_BITS_H_
#define PUZ_BITS_H_

#include <assert.h>

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace puzzle {

template <typename T>
T SetBit(T input, int index) {
  return input | (1 << index);
}

template <typename T>
T UnsetBit(T input, int index) {
  return input & ~(1 << index);
}

template <typename T>
bool TestBit(T input, int index) {
  return (input & (1 << index)) != 0;
}

//  N --> M
//  n bits --> m bits <a1, a2, .., am>

// Given a number (that represents a bitset), returns the indexes
// that are set one by one.
class BitIterator {
 public:
  BitIterator(uint16_t n);

  bool HasNext();
  int Next();

 private:
  uint16_t n_;
};

// Given n that represents a bitset, returns another bitset
// with only the bits from the given vector of indexes.
//
// Example:
// N: 13 ==> 1101 and indexes: 0, 1, 3
// returns 101 (because 0th bit is set, 1st bit is not, 3rd bit is set)
//
// Declared in the header for testing.
uint16_t Project2(int n, std::vector<int> indexes);

// Given n that represents a bitset:
// 1. for each row, calculate another row ignoring the absent bits.
// 2. take the union of the rows.
// 3. return another bit set that represents the rows.
// Example:
// Project2(42, {0, 2}) ==>
// n = 42 ==> 00000101010
//        ==> 1st, 3rd, 5th rows are present
//        ==> 0001, 0011, 0101
// project only for 0th and 2nd bits:
// 0001 => 01, 0011 => 01, 0101 => 11
// ==> Therefore the result is 01, 11
// Representing as a bit set:
// ==> 11:Yes 10:No 01:Yes 00:No
// ==>      1     0      1     0
// ==> 10
uint16_t Project(uint16_t n, std::vector<int> indexes);

// 0 ==> n was 0, already contradiction
// 1 ==> false
// 2 ==> true
// 3 ==> can be both
uint16_t ProjectSingle(uint16_t n, int index);

// Reduce n with respect to m. indexes belong to n.
uint16_t Reduce(uint16_t n, uint16_t m, std::vector<int> n_indexes);

uint16_t ReduceSingle(uint16_t n, int index, bool equals_to);

}  // namespace puzzle

#endif /* PUZ_BITS_H_ */
