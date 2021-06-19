#ifndef PUZ_CORNER_H_
#define PUZ_CORNER_H_

#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "absl/strings/str_cat.h"
#include "puz/edge.h"
#include "puz/message.h"

namespace puzzle {

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

  MessageResult Receive(const Message& signal, ActionStack* stack,
                        MessageStack*) override;
  void Undo() override;
  int Timestamp() override;
  uint16_t State() const override;

  // Testing purposes
  std::vector<Face*> Faces() {
    return std::vector<Face*>{upright_, downright_, downleft_, upleft_};
  };
  std::vector<Edge*>& Edges() { return edges_; }

 private:
  uint16_t NewState(const Message& message);

  std::string SetPrintChar();

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

}  // namespace puzzle

#endif /* PUZ_CORNER_H_ */
