#include "slitherlink/solver.h"

#include <algorithm>
#include <utility>
#include <vector>

#include "slitherlink/bits.h"
#include "slitherlink/corner.h"
#include "slitherlink/edge.h"
#include "slitherlink/face.h"
#include "slitherlink/grid.h"
#include "slitherlink/message.h"

namespace slitherlink {

using Solution = std::vector<std::tuple<int, int, Orientation>>;

Solution GetSolution(Grid* g) {
  Solution result;
  for (auto& e : g->Edges()) {
    if (e.State() == EDGE_ON) {
      result.emplace_back(e.Row(), e.Col(), e.Orientation());
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

namespace {

bool IsThree(const Face& f) { return f.HasValue() && f.Value() == 3; }

void PushThreeThree(MessageStack* ms, Edge* e, bool on) {
  ms->Push(Message::ToEdge(ManualAgent::ThreeThree(), e, 0, on));
}

// Two 3s side by side: their three vertical edges are on, and the vertical
// edges above and below the shared edge are off.
void ThreeThreeSideBySide(Grid* g, MessageStack* ms) {
  for (int r = 0; r < g->NRows(); ++r) {
    for (int c = 0; c < g->NCols() - 1; ++c) {
      if (!IsThree(g->FaceAt(r, c)) || !IsThree(g->FaceAt(r, c + 1))) {
        continue;
      }
      for (int k = 0; k < 3; ++k) {
        PushThreeThree(ms, &g->EdgeAt(r, c + k, Orientation::VERTICAL), true);
      }
      if (r > 0) {
        PushThreeThree(ms, &g->EdgeAt(r - 1, c + 1, Orientation::VERTICAL), false);
      }
      if (r < g->NRows() - 1) {
        PushThreeThree(ms, &g->EdgeAt(r + 1, c + 1, Orientation::VERTICAL), false);
      }
    }
  }
}

// Two 3s stacked: their three horizontal edges are on, and the horizontal
// edges left and right of the shared edge are off.
void ThreeThreeStacked(Grid* g, MessageStack* ms) {
  for (int r = 0; r < g->NRows() - 1; ++r) {
    for (int c = 0; c < g->NCols(); ++c) {
      if (!IsThree(g->FaceAt(r, c)) || !IsThree(g->FaceAt(r + 1, c))) {
        continue;
      }
      for (int k = 0; k < 3; ++k) {
        PushThreeThree(ms, &g->EdgeAt(r + k, c, Orientation::HORIZONTAL), true);
      }
      if (c > 0) {
        PushThreeThree(ms, &g->EdgeAt(r + 1, c - 1, Orientation::HORIZONTAL), false);
      }
      if (c < g->NCols() - 1) {
        PushThreeThree(ms, &g->EdgeAt(r + 1, c + 1, Orientation::HORIZONTAL), false);
      }
    }
  }
}

}  // namespace

void InitialThreeThree(Grid* g, MessageStack* ms) {
  ThreeThreeSideBySide(g, ms);
  ThreeThreeStacked(g, ms);
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
  double best_score = 0;
  for (auto& e : g->Edges()) {
    Edge* edge = &e;
    if (edge->State() != EDGE_UNKNOWN) {
      continue;
    }
    double now_score = edge->Score();
    if (best_edge == nullptr || now_score < best_score) {
      best_score = now_score;
      best_edge = edge;
    }
  }
  return best_edge;
}

std::vector<Solution> Solve(Grid* g, int max_solutions) {
  std::vector<Solution> solutions;
  ActionStack as;
  as.AddAction(Action(nullptr, true));
  MessageStack ms;
  InitialConsequences(g, &ms);
  InitialThreeThree(g, &ms);
  while (!as.IsEmpty()) {
    while (!ms.IsEmpty()) {
      Message m = ms.Pop();
      IAgent* source = m.source;
      IAgent* target = m.target;
      int timestamp = m.timestamp;
      if (source != ManualAgent::Initial() && source != ManualAgent::Assume() &&
          source != ManualAgent::Therefore() && source != ManualAgent::ThreeThree() &&
          timestamp < source->Timestamp()) {
        continue;
      }

      MessageResult r = target->Receive(m, &as, &ms);

      if (r == MessageResult::OK) {
        continue;
      }
      if (r == MessageResult::SOLUTION && CheckSolution(g)) {
        solutions.push_back(GetSolution(g));
        std::cout << g->PrettyPrint() << "\n";
        if (std::cmp_greater_equal(solutions.size(), max_solutions)) {
          return solutions;
        }
      }

      Action last_assumption = as.Undo();
      if (last_assumption.agent == nullptr) {
        return solutions;
      }
      ms.Clear();
      Edge* e = static_cast<Edge*>(last_assumption.agent);
      Message proof_by_contradiction = Message::ToEdge(ManualAgent::Therefore(), e, 0, false);
      ms.Push(proof_by_contradiction);
    }
    Edge* assumption_edge = MakeAssumption(g);
    if (assumption_edge == nullptr) {
      break;
    }
    Message new_assumption = Message::ToEdge(ManualAgent::Assume(), assumption_edge, 0, true);
    ms.Push(new_assumption);
    as.AddAction(Action(assumption_edge, true));
  }
  return solutions;
}

}  // namespace slitherlink
