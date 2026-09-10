/*
  Stockfish, a UCI chess playing engine derived from Glaurung 2.1
  Copyright (C) 2004-2026 The Stockfish developers (see AUTHORS file)

  Stockfish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Stockfish is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Experimental, opt-in (EXPERIMENTAL_LEARNED_LMR) learned override for the
// final LMR search depth decision in Search::Worker::search(). Disabled by
// default and inert until a trained model replaces LearnedLMRModel below.
// See the "Rethink the Search" discussion (official-stockfish/Stockfish#7144)
// for the experiment design and acceptance criteria (SPRT vs. baseline).

#ifndef LEARNED_LMR_H_INCLUDED
#define LEARNED_LMR_H_INCLUDED

#ifdef EXPERIMENTAL_LEARNED_LMR

#include "types.h"

namespace Stockfish {

enum class LearnedLMRAction {
    Keep,
    ReduceLess,
    NoReduction
};

// Snapshot of the decision context at the point where Stockfish would
// otherwise apply its heuristic LMR depth. Field names mirror the local
// variables available at that point in Search::Worker::search().
struct LMRFeatures {
    int   depth;
    int   moveCount;
    int   reduction;
    int   statScore;
    int   correctionValue;
    int   alpha;
    int   eval;
    bool  pvNode;
    bool  cutNode;
    bool  allNode;
    bool  improving;
    bool  ttMove;
    bool  ttCapture;
    bool  capture;
    bool  givesCheck;
};

// Placeholder for an offline-trained tiny model (e.g. quantized linear model
// or lookup table). This draft always defers to the existing heuristic.
class LearnedLMRModel {
   public:
    LearnedLMRAction predict(const LMRFeatures& f) const {
        (void) f;
        return LearnedLMRAction::Keep;
    }

    // Returns false until a calibrated model is trained and wired in, so the
    // engine behaves identically to the unmodified baseline.
    bool confident(const LMRFeatures& f) const {
        (void) f;
        return false;
    }
};

// Training record for offline dataset generation: pairs the decision context
// with the outcome of the (heuristically) reduced search and a deeper
// reference search from the same recorded context.
struct LMRTrainingRecord {
    LMRFeatures features;
    Depth       reducedDepth;
    Value       reducedValue;
    Depth       fullDepth;
    Value       fullValue;
    Move        move;
};

}  // namespace Stockfish

#endif  // EXPERIMENTAL_LEARNED_LMR

#endif  // LEARNED_LMR_H_INCLUDED
