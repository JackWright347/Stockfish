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

// Experimental, opt-in (EXPERIMENTAL_LEARNED_LMR_CONTINUATION) learned
// override for what happens after a reduced LMR search fails high in
// Search::Worker::search(). Disabled by default and inert until a trained
// model replaces LearnedLMRContinuationModel below. This targets only the
// doDeeperSearch/doShallowerSearch decision, not the initial LMR depth
// itself and not any earlier pruning.

#ifndef LEARNED_LMR_CONTINUATION_H_INCLUDED
#define LEARNED_LMR_CONTINUATION_H_INCLUDED

#ifdef EXPERIMENTAL_LEARNED_LMR_CONTINUATION

namespace Stockfish {

enum class LearnedLMRContinuationAction {
    Keep,
    StopMove,
    ResearchNormalDepth,
    ResearchDeeper,
    ResearchShallower
};

// Snapshot of the decision context right after a reduced-depth search has
// failed high (value > alpha) in Search::Worker::search().
struct LMRContinuationFeatures {
    int  depth;
    int  newDepth;
    int  reducedDepth;
    int  moveCount;
    int  reducedValue;
    int  bestValue;
    int  alpha;
    bool pvNode;
    bool cutNode;
};

// Placeholder for an offline-trained tiny model. This draft always defers to
// the existing doDeeperSearch/doShallowerSearch heuristic.
class LearnedLMRContinuationModel {
   public:
    LearnedLMRContinuationAction predict(const LMRContinuationFeatures& f) const {
        (void) f;
        return LearnedLMRContinuationAction::Keep;
    }

    // Returns false until a calibrated model is trained and wired in, so the
    // engine behaves identically to the unmodified baseline.
    bool confident(const LMRContinuationFeatures& f) const {
        (void) f;
        return false;
    }
};

}  // namespace Stockfish

#endif  // EXPERIMENTAL_LEARNED_LMR_CONTINUATION

#endif  // LEARNED_LMR_CONTINUATION_H_INCLUDED
