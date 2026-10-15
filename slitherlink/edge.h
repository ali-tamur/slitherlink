#ifndef PUZ_EDGE_H_
#define PUZ_EDGE_H_

#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "absl/strings/str_cat.h"
#include "slitherlink/message.h"
#include "slitherlink/utils.h"

namespace puzzle {

class Face;
class Corner;

class Edge : public IAgent {
 public:
  Edge() = default;
  ~Edge() override = default;
  void SetCoordinate(int row, int col, Orientation orientation) {
    row_ = row;
    col_ = col;
    o_ = orientation;
    print_char_ = PrintChar();
  }

  void SetFaces(std::vector<std::tuple<Face*, int>> faces) { faces_ = faces; }
  void SetCorners(std::vector<std::tuple<Corner*, int>> corners) { corners_ = corners; }
  void AddBoundary(IAgent* b) { boundaries_.push_back(b); }
  void SetLoops(IAgent* l) { loops_ = l; }

  int Row() const { return row_; }
  int Col() const { return col_; }
  Orientation Orientation() const { return o_; }
  double Score() const;

  MessageResult Receive(const Message& message, ActionStack* stack, MessageStack*) override;
  void Undo() override;
  int Timestamp() override;
  uint16_t State() const override;
  std::string PrettyPrint() const;
  std::string DebugPrint() const override;

  std::vector<Message> ConstructMessages();

  // For testing purposes
  std::vector<std::tuple<Face*, int>>& Faces() { return faces_; }
  std::vector<std::tuple<Corner*, int>>& Corners() { return corners_; }
  void SetState(uint16_t s) { state_ = s; }

 private:
  uint16_t NewState(const Message& m) const;

  std::string PrintChar() { return o_ == Orientation::HORIZONTAL ? "─" : "│"; }

  int row_;
  int col_;
  enum Orientation o_;
  int timestamp_ = 0;
  std::vector<uint16_t> old_states_;
  uint16_t state_ = 3;
  // TODO: Let these be collections of IAgents.
  std::vector<std::tuple<Face*, int>> faces_;
  std::vector<std::tuple<Corner*, int>> corners_;
  IAgent* loops_;
  std::vector<IAgent*> boundaries_;
  std::string print_char_;
};

}  // namespace puzzle

#endif /* PUZ_EDGE_H_ */
