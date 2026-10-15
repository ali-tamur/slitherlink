#ifndef SLITHERLINK_CORNER_H_
#define SLITHERLINK_CORNER_H_

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <variant>
#include <vector>

#include "absl/strings/str_cat.h"
#include "slitherlink/edge.h"
#include "slitherlink/message.h"

namespace slitherlink {

class Face;

class Corner : public IAgent {
 public:
  Corner() = default;
  ~Corner() override = default;

  void SetFaces(Face* upright, Face* downright, Face* downleft, Face* upleft);

  void SetEdges(Edge* up, Edge* right, Edge* down, Edge* left);

  void Init();

  std::string PrettyPrint() const;

  std::string DebugPrint() const override;

  double Score() const {
    if (state_ == 0) {
      return 0;
    }
    return log2(__builtin_popcount(state_));
  }

  std::vector<Message> ConstructMessages();

  MessageResult Receive(const Message& message, ActionStack* stack, MessageStack*) override;
  void Undo() override;
  int Timestamp() override;
  uint16_t State() const override;

  // Testing purposes
  std::vector<Face*> Faces() {
    return std::vector<Face*>{upright_, downright_, downleft_, upleft_};
  };
  std::vector<Edge*>& Edges() { return edges_; }

 private:
  uint16_t NewState(const Message& message) const;

  std::string_view SetPrintChar();

  int timestamp_ = 0;
  std::vector<uint16_t> old_states_;
  // abcd => a is the vertical edge up, going clockwise.
  uint16_t state_;
  Face* upright_;
  Face* downright_;
  Face* downleft_;
  Face* upleft_;
  std::string print_char_;

  std::vector<Edge*> edges_;
};

}  // namespace slitherlink

#endif /* SLITHERLINK_CORNER_H_ */
