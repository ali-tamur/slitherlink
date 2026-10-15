# slitherlink

A solver for [Slitherlink](https://en.wikipedia.org/wiki/Slitherlink) puzzles:
draw a single closed loop along the grid lines so that each numbered cell has
exactly that many of its sides on the loop.

## How it works

Every part of the grid is an agent that keeps the set of states still possible
for it and sends messages to its neighbours when that set shrinks:

- **Face** (a cell) and **Corner** track which combinations of their four
  edges are still possible; a numbered cell allows only combinations with that
  many edges, a corner only 0 or 2.
- **Edge** is on, off or unknown, and forwards changes to its faces, corners,
  boundaries and the loop tracker.
- **Boundary** enforces parity: a closed loop crosses any closed boundary an
  even number of times.
- **Loops** tracks the strings of connected edges, so a loop can't close early.

When propagation stalls, the solver assumes an edge is on and continues; if
that leads to a contradiction it undoes the assumption and sets the edge off
(proof by contradiction). Known patterns such as two adjacent 3s seed the
search.

## Build and run

Requires Bazel (via Bazelisk) and the machine-wide LLVM toolchain configured in
`MODULE.bazel`.

```sh
bazel run -c opt //slitherlink:grid_main   # solve the built-in 15x15 puzzle
bazel test //slitherlink/...               # unit tests
```

## Development

- `tools/format.sh` — clang-format (Google style, 100 columns).
- `python3 tools/compile_commands.py` — `compile_commands.json` for clangd.
- `.clang-tidy` — the checks the code is kept clean against.
