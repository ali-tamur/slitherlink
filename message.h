#ifndef PUZ_MESSAGE_H_
#define PUZ_MESSAGE_H_

#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

namespace puzzle {

class Edge;
class IAgent;

enum class MessageResult : uint8_t { OK, CONTRADICTION, SOLUTION };

enum class MessageTag {
  T_Uninit, T_FaceAndCorner, T_ToEdge, T_FromEdge
};

struct Message {
  IAgent* source = nullptr;
  IAgent* target = nullptr;
  int timestamp = -1;
  uint16_t projected_state = 0;
  int index1 = -1;
  int index2 = -1;
  bool edge_state = false;
  MessageTag tag = MessageTag::T_Uninit;

  std::string DebugPrint() const;
  static Message FaceAndCorner(IAgent* source, IAgent* target, int timestamp, uint16_t project_state, int index1, int index2) {
    Message r;
    r.tag = MessageTag::T_FaceAndCorner;
    r.source = source;
    r.target= target;
    r.timestamp = timestamp;
    r.projected_state = project_state;
    r.index1 = index1;
    r.index2 = index2;
    return r;
  }

  static Message ToEdge(IAgent* source, IAgent* target, int timestamp, bool edge_state) {
    Message r;
    r.tag = MessageTag::T_ToEdge;
    r.source = source;
    r.target= target;
    r.timestamp = timestamp;
    r.edge_state = edge_state;
    return r;
  }

  static Message FromEdge(IAgent* source, IAgent* target, int timestamp, int index, bool edge_state) {
    Message r;
    r.tag = MessageTag::T_FromEdge;
    r.source = source;
    r.target= target;
    r.timestamp = timestamp;
    r.index1 = index;
    r.edge_state = edge_state;
    return r;
  }


 private:
  std::string DebugPrintFaceAndCorner() const;
  std::string DebugPrintToEdge() const;
  std::string DebugPrintFromEdge() const;
};

// using Message = std::variant<M_FaceAndCorner, M_ToEdge, M_FromEdge>;

std::string PrettyPrintMessage(const Message& m);

class MessageStack {
 public:
  MessageStack() : messages_() {}
  bool IsEmpty();
  Message Pop();

  void PushAll(const std::vector<Message>& s);
  void Push(Message m) { messages_.push_back(m); }
  void Clear() {messages_.clear();}

 private:
  std::vector<Message> messages_;
};

struct Action {
  Action(IAgent* a) {
    agent = a;
    is_it_an_assumption = false;
    assumption = false;
  }
  Action(IAgent* a, bool assume) {
    agent = a;
    is_it_an_assumption = true;
    assumption = assume;
  }

  IAgent* agent;

  // Requires the agent to be an edge.
  bool is_it_an_assumption;

  // Meaningful only if is)it_an_assumption;
  bool assumption;
};

class ActionStack {
 public:
  ActionStack() : actions_() {}

  int Timestamp();
  void AddAction(Action a);

  // Removes elements and undo'es affects until an assumption.
  // Also removes the assumption and returns it.
  // If the vector is empty (no further assumption), returns a
  // "null" assumption, with nullptr as the edge.
  Action Undo();

  bool IsEmpty() {return actions_.empty(); }

 private:
  std::vector<Action> actions_;
  int timestamp_ = 0;
};

class IAgent {
 public:
  IAgent() = default;
  virtual ~IAgent() = default;
  virtual MessageResult Receive(const Message& signal, ActionStack* stack,
                                MessageStack*) = 0;
  virtual void Undo() = 0;
  virtual int Timestamp() = 0;
  virtual uint16_t State() const = 0;
  virtual std::string DebugPrint() const { return "DEBUG_STRING NOT IMPLEMENTED";}
};

}  // namespace puzzle

#endif /* PUZ_MESSAGE_H_ */
