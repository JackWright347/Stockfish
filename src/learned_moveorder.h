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

// Experimental, opt-in (EXPERIMENTAL_LEARNED_MOVEORDER) bounded correction to
// quiet-move ordering in MovePicker::score(). Disabled by default and inert
// until a trained model replaces LearnedMoveOrderModel below. The existing
// history-based score always remains authoritative; the model may only add
// or subtract one small, fixed unit, never replace the score outright.

#ifndef LEARNED_MOVEORDER_H_INCLUDED
#define LEARNED_MOVEORDER_H_INCLUDED

#ifdef EXPERIMENTAL_LEARNED_MOVEORDER

namespace Stockfish {

enum class LearnedMoveOrderAction {
    Keep,
    PromoteSlightly,
    DemoteSlightly
};

// Snapshot of the decision context at the point where a quiet move's history
// score has just been computed in MovePicker::score().
struct MoveOrderFeatures {
    int  historyScore;
    int  ply;
    bool givesCheck;
    bool threatened;
};

// Placeholder for an offline-trained tiny classifier. This draft always
// defers to the existing history-based score.
class LearnedMoveOrderModel {
   public:
    LearnedMoveOrderAction predict(const MoveOrderFeatures& f) const {
        (void) f;
        return LearnedMoveOrderAction::Keep;
    }

    // Returns false until a calibrated model is trained and wired in, so the
    // engine behaves identically to the unmodified baseline.
    bool confident(const MoveOrderFeatures& f) const {
        (void) f;
        return false;
    }
};

// Fixed correction magnitude, deliberately small relative to typical history
// score ranges so a wrong prediction can only cause a minor reordering.
constexpr int MoveOrderCorrectionUnit = 64;

}  // namespace Stockfish

#endif  // EXPERIMENTAL_LEARNED_MOVEORDER

#endif  // LEARNED_MOVEORDER_H_INCLUDED
