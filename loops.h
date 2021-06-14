#ifndef PUZ_LOOPS_H_
#define PUZ_LOOPS_H_

#include <assert.h>

#include <functional>
#include <memory>
#include <tuple>
#include <vector>

#include "puz/edge.h"
#include "puz/message.h"
#include "puz/utils.h"

namespace puzzle {

class Node {
 public:
  Node(int endpoint)
      : endpoint1_(endpoint), endpoint2_(endpoint), my_index_(endpoint), parent_(nullptr) {}

  // Returns the node that made a link.
  // Returns nullptr if the merge makes a loop.
  Node* Merge(Node* other);

  std::tuple<int, int> Endpoints();
  bool IsString();
  void Unparent();
  Node* Parent();
  void Merged(int e1, int e2);
  void Undo();

 private:
  std::tuple<int, Node*> Root();

  int endpoint1_ = -1;
  int endpoint2_ = -1;
  int my_index_ = -1;
  Node* parent_;
  std::vector<std::tuple<int, int>> stack_;
};

class Loops : public IAgent {
 public:

  void Init(int rows, int cols,
        std::function<Edge*(int, int, Orientation)> edge_finder);

  MessageResult Receive(const Message& message, ActionStack* action_stack,
                        MessageStack* message_stack) override;

  void Undo() override;

  int Timestamp() override;
  uint16_t State() const override;
  int NStrings() { return n_strings_; }
  std::string DebugPrint() const override { return "Loops";}

 private:
  std::tuple<int, int> EdgeToCornerIndexes(int row, int col, Orientation o);

  // nullptr unless i1 and i2 point to neighboor corners, in which case
  // returns the edge.
  Edge* CommonEdge(int i1, int i2);

  // rows_, cols_ is number of rows/cols of faces, not corners.
  int rows_, cols_;
  int n_strings_ = 0;
  int timestamp_ = 0;
  std::function<Edge*(int, int, Orientation)> edge_finder_;
  std::vector<Node> nodes_;
  std::vector<std::tuple<Node*, int>> stack_;
};

}  // namespace puzzle

#endif /* PUZ_LOOPS_H_ */
