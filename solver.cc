#include "puz/solver.h"

#include <algorithm>
#include <bitset>
#include <vector>

#include "puz/bits.h"
#include "puz/corner.h"
#include "puz/edge.h"
#include "puz/face.h"
#include "puz/grid.h"
#include "puz/message.h"

namespace puzzle {

using Solution = std::vector<std::tuple<int, int, Orientation>>;

Solution GetSolution(Grid* g) {
  std::vector<Edge>& edges = g->Edges();

  Solution result;
  for (auto& e : g->Edges()) {
    if (e.State() == EDGE_ON) {
      result.push_back(std::make_tuple(e.Row(), e.Col(), e.Orientation()));
    }
  }
  return result;
}

void InitialConsequences(Grid* g, MessageStack* ms) {
  for (Corner& c : g->Corners()) {
    ms->PushAll(c.ConstructMessages());
  }
  for (Face& f : g->Faces()) {
    ms->PushAll(f.ConstructMessages());
  }
  // Tobe used in debugging (setting some edges beforehand)
  for (Edge& e : g->Edges()) {
    if (e.State() != EDGE_UNKNOWN) {
      ms->PushAll(e.ConstructMessages());
    }
  }
}

void InitialThreeThree(Grid* g, MessageStack* ms) {
  for (int r = 0; r < g->NRows(); ++r) {
    for (int c = 0; c < g->NCols() - 1; ++c) {
      auto& f1 = g->FaceAt(r, c);
      if (!f1.HasValue() || f1.Value() != 3) {
        continue;
      }
      auto& f2 = g->FaceAt(r, c + 1);
      if (!f2.HasValue() || f2.Value() != 3) {
        continue;
      }
      std::vector<Edge*> edges = {&g->EdgeAt(r, c, Orientation::VERTICAL),
                                  &g->EdgeAt(r, c + 1, Orientation::VERTICAL),
                                  &g->EdgeAt(r, c + 2, Orientation::VERTICAL)};
      for (auto* e : edges) {
        Message m = Message::ToEdge(ManualAgent::ThreeThree(), e, 0, true);
        ms->Push(m);
      }
      if (r > 0) {
        Edge* e = &g->EdgeAt(r - 1, c + 1, Orientation::VERTICAL);
        Message m = Message::ToEdge(ManualAgent::ThreeThree(), e, 0, false);
        ms->Push(m);
      }
      if (r < g->NRows() - 1) {
        Edge* e = &g->EdgeAt(r + 1, c + 1, Orientation::VERTICAL);
        Message m = Message::ToEdge(ManualAgent::ThreeThree(), e, 0, false);
        ms->Push(m);
      }
    }
  }

  for (int r = 0; r < g->NRows() - 1; ++r) {
    for (int c = 0; c < g->NCols(); ++c) {
      auto& f1 = g->FaceAt(r, c);
      if (!f1.HasValue() || f1.Value() != 3) {
        continue;
      }
      auto& f2 = g->FaceAt(r + 1, c);
      if (!f2.HasValue() || f2.Value() != 3) {
        continue;
      }
      std::vector<Edge*> edges = {
          &g->EdgeAt(r, c, Orientation::HORIZONTAL),
          &g->EdgeAt(r + 1, c, Orientation::HORIZONTAL),
          &g->EdgeAt(r + 2, c, Orientation::HORIZONTAL)};
      for (auto* e : edges) {
        Message m = Message::ToEdge(ManualAgent::ThreeThree(), e, 0, true);
        ms->Push(m);
      }
      if (c > 0) {
        Edge* e = &g->EdgeAt(r + 1, c - 1, Orientation::HORIZONTAL);
        Message m = Message::ToEdge(ManualAgent::ThreeThree(), e, 0, false);
        ms->Push(m);
      }
      if (c < g->NCols() - 1) {
        Edge* e = &g->EdgeAt(r + 1, c + 1, Orientation::HORIZONTAL);
        Message m = Message::ToEdge(ManualAgent::ThreeThree(), e, 0, false);
        ms->Push(m);
      }
    }
  }
}

bool CheckSolution(Grid* g) {
  for (Face& f : g->Faces()) {
    if (!f.HasValue()) {
      continue;
    }
    int count = 0;
    for (Edge* e : f.Edges()) {
      if (e->State() == EDGE_ON) {
        ++count;
      }
    }
    if (count != f.Value()) {
      return false;
    }
  }
  return true;
}

Edge* MakeAssumption(Grid* g) {
  Edge* best_edge = nullptr;
  double best_score = -1;
  for (auto& e : g->Edges()) {
    Edge* edge = &e;
    if (edge->State() != EDGE_UNKNOWN) {
      continue;
    }
    double now_score = edge->Score();
    if (now_score > best_score || best_edge == nullptr) {
      best_score = now_score;
      best_edge = edge;
    }
  }
  return best_edge;
}

std::vector<Solution> Solve(Grid* g) {
  std::vector<Solution> solutions;
  ActionStack as;
  as.AddAction(Action(nullptr, true));
  MessageStack ms;
  InitialConsequences(g, &ms);
  InitialThreeThree(g, &ms);
  int i = 0;
  while (!as.IsEmpty()) {
    while (!ms.IsEmpty()) {
      Message m = ms.Pop();
      ++i;
      IAgent* source = m.source;
      IAgent* target = m.target;
      int timestamp = m.timestamp;
      if (source != ManualAgent::Initial() && source != ManualAgent::Assume() &&
          source != ManualAgent::Therefore() &&
          source != ManualAgent::ThreeThree() &&
          timestamp < source->Timestamp()) {
        continue;
      }

      auto target_old_state = target->State();
      MessageResult r = target->Receive(m, &as, &ms);
      auto target_new_state = target->State();

      if (target_old_state != target_new_state) {
        auto message_pp = m.DebugPrint();
        std::cout << i << ":" << message_pp << " "
                  << std::bitset<16>(target_old_state) << " ==> "
                  << std::bitset<16>(target_new_state) << "\n";
      }

      if (r == MessageResult::OK) {
        continue;
      }
      if (r == MessageResult::SOLUTION && CheckSolution(g)) {
        solutions.push_back(GetSolution(g));
        std::cout << g->PrettyPrint() << "\n";
      }

      std::cout << "CONTADICTION: " << m.DebugPrint() << "\n";

      Action last_assumption = as.Undo();
      if (last_assumption.agent == nullptr) {
        return solutions;
      }
      ms.Clear();
      Edge* e = static_cast<Edge*>(last_assumption.agent);
      Message proof_by_contradiction =
          Message::ToEdge(ManualAgent::Therefore(), e, 0, false);
      ms.Push(proof_by_contradiction);
    }
    Edge* assumption_edge = MakeAssumption(g);
    if (assumption_edge == nullptr) {
      break;
    }
    Message new_assumption =
        Message::ToEdge(ManualAgent::Assume(), assumption_edge, 0, true);
    ms.Push(new_assumption);
    as.AddAction(Action(assumption_edge, true));
  }
  return solutions;
}

}  // namespace puzzle
