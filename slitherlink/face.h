#ifndef SLITHERLINK_FACE_H_
#define SLITHERLINK_FACE_H_

#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "absl/strings/str_cat.h"
#include "slitherlink/edge.h"
#include "slitherlink/message.h"

namespace slitherlink {

class Edge;
class Corner;

class Face : public IAgent {
 public:
  Face() = default;
  ~Face() override = default;

  void SetCorners(Corner* upright, Corner* downright, Corner* downleft, Corner* upleft) {
    upright_ = upright;
    downright_ = downright;
    downleft_ = downleft;
    upleft_ = upleft;
  }

  void SetEdges(Edge* up, Edge* right, Edge* down, Edge* left) { edges_ = {up, right, down, left}; }

  void SetValue(int val);
  int Value() const;
  bool HasValue() const;
  void Init();
  double Score() const {
    if (state_ == 0) {
      return 0;
    }
    return log2(__builtin_popcount(state_));
  }

  std::string PrettyPrint() const;
  std::string DebugPrint() const override {
    std::string result;
    result += "F(";
    result += std::to_string(edges_[0]->Row());
    result += ",";
    result += std::to_string(edges_[0]->Col());
    result += ")";
    return result;
  }

  uint16_t State() const override;
  int Timestamp() override;
  std::vector<Message> ConstructMessages();
  const std::vector<Edge*>& Edges() const { return edges_; }

  void Undo() override;
  MessageResult Receive(const Message& message, ActionStack* stack, MessageStack*) override;

  uint16_t NewState(const Message& message) const;

  // Testing purposes
  std::vector<Corner*> Corners() {
    return std::vector<Corner*>{upright_, downright_, downleft_, upleft_};
  };
  std::vector<Edge*>& Edges() { return edges_; }

 private:
  int value_ = -1;
  bool has_value_ = false;
  int timestamp_ = 0;
  std::vector<uint16_t> old_states_;
  Corner* upright_;
  Corner* downright_;
  Corner* downleft_;
  Corner* upleft_;
  std::vector<Edge*> edges_;

  // abcd => a is the top edge, going clockwise.
  // e.g. 0010 means only bottom edge is on.
  uint16_t state_;
};

}  // namespace slitherlink

#endif /* SLITHERLINK_FACE_H_ */
