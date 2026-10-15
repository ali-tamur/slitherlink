#include "slitherlink/bits.h"

#include <assert.h>

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace puzzle {

//  N --> M
//  n bits --> m bits <a1, a2, .., am>

// Given a number (that represents a bitset), returns the indexes
// that are set one by one.

BitIterator::BitIterator(uint16_t n) : n_(n) {}

bool BitIterator::HasNext() { return n_ != 0; }
int BitIterator::Next() {
  assert((n_ != 0) && "Next call on empty BitIterator");
  int result = __builtin_ctz(n_);
  n_ = n_ & (n_ - 1);
  return result;
}

// Given n that represents a bitset, returns another bitset
// with only the bits from the given vector of indexes.
//
// Example:
// N: 13 ==> 1101 and indexes: 0, 1, 3
// returns 101 (because 0th bit is set, 1st bit is not, 3rd bit is set)
uint16_t Project2(int n, std::vector<int> indexes) {
  uint16_t result = 0;
  int now = 0;
  for (int index : indexes) {
    if (TestBit(n, index)) {
      result = SetBit(result, now);
    }
    ++now;
  }
  return result;
}

// Given n that represents a bitset:
// 1. for each row, calculate another row ignoring the absent bits.
// 2. take the union of the rows.
// 3. return another bit set that represents the rows.
// Example:
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
uint16_t Project(uint16_t n, std::vector<int> indexes) {
  BitIterator b(n);
  uint16_t result = 0;
  while (b.HasNext()) {
    result = SetBit(result, Project2(b.Next(), indexes));
  }
  return result;
}

// 0 ==> n was 0, already contradiction
// 1 ==> false
// 2 ==> true
// 3 ==> can be both
uint16_t ProjectSingle(uint16_t n, int index) {
  BitIterator b(n);
  uint16_t result = 0;
  while (b.HasNext()) {
    if (TestBit(b.Next(), index)) {
      result = SetBit(result, 1);
    } else {
      result = SetBit(result, 0);
    }
  }
  return result;
}

// Reduce n with respect to m. indexes belong to n.
uint16_t Reduce(uint16_t n, uint16_t m, std::vector<int> n_indexes) {
  BitIterator b(n);
  uint16_t result = 0;
  while (b.HasNext()) {
    auto now_bit = b.Next();
    if (TestBit(m, Project2(now_bit, n_indexes))) {
      result = SetBit(result, now_bit);
    }
  }
  return result;
}

uint16_t ReduceSingle(uint16_t n, int index, bool equals_to) {
  uint16_t result = 0;
  BitIterator b(n);
  while (b.HasNext()) {
    auto now = b.Next();
    if (TestBit(now, index) == equals_to) {
      result = SetBit(result, now);
    }
  }
  return result;
}

}  // namespace puzzle
