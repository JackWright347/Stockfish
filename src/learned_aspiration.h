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

// Experimental, opt-in (EXPERIMENTAL_LEARNED_ASPIRATION) learned override for
// the initial aspiration-window half-width in
// Search::Worker::iterative_deepening(). Disabled by default and inert until
// a trained model replaces LearnedAspirationModel below. This is a root-level,
// once-per-iteration decision; it does not touch move ordering, pruning,
// extensions, or evaluation, and it does not remove the existing re-search
// loop on fail-high/fail-low.

#ifndef LEARNED_ASPIRATION_H_INCLUDED
#define LEARNED_ASPIRATION_H_INCLUDED

#ifdef EXPERIMENTAL_LEARNED_ASPIRATION

namespace Stockfish {

enum class LearnedAspirationAction {
    Keep,
    Narrower,
    Wider
};

// Snapshot of the decision context right before the aspiration window is
// computed for a new root iteration.
struct AspirationFeatures {
    int    delta;
    int    avg;
    int    rootDepth;
    int    multiPV;
    double previousTimeReduction;
};

// Placeholder for an offline-trained tiny model. This draft always defers to
// the existing delta heuristic.
class LearnedAspirationModel {
   public:
    LearnedAspirationAction predict(const AspirationFeatures& f) const {
        (void) f;
        return LearnedAspirationAction::Keep;
    }

    // Returns false until a calibrated model is trained and wired in, so the
    // engine behaves identically to the unmodified baseline.
    bool confident(const AspirationFeatures& f) const {
        (void) f;
        return false;
    }
};

}  // namespace Stockfish

#endif  // EXPERIMENTAL_LEARNED_ASPIRATION

#endif  // LEARNED_ASPIRATION_H_INCLUDED
