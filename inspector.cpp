//
// Copyright (C) 2022 Motoyuki Kasahara.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>
//

#include "inspector.hpp"
#include "transformer.hpp"

namespace gobb_analyzer {

//
// Class Inspector.
//
Inspector::Inspector()
    : analysisDataTable_(nullptr),
      statistics_() {
    analysisDataTable_ = new AnalysisData [AnalysisDataTableSize];
}

Inspector::~Inspector() {
    delete[] analysisDataTable_;
}

bool Inspector::load(AnalysisDataIOHandler& handler, Generation generation) {
    return handler.load(generation, statistics_, analysisDataTable_, AnalysisDataTableSize * sizeof(AnalysisData));
}

Generation Inspector::load_latest(AnalysisDataIOHandler& handler) {
    return handler.load_latest(statistics_, analysisDataTable_, AnalysisDataTableSize * sizeof(AnalysisData));
}

PositionInspectionResult Inspector::inspect_position(PositionId id) const noexcept {
    PositionInspectionResult result;

    if (!is_valid_positionId(id)) {
        return result;
    }
    Position pos(id);

    AnalysisData analysisData = analysisDataTable_[pos.minimize_id()];
    return PositionInspectionResult {id, turn_of_analysisData(analysisData), status_of_analysisData(analysisData)};
}

std::vector<MoveInspectionResult> Inspector::inspect_moves(PositionId id) const noexcept {
    std::vector<MoveInspectionResult> result;

    if (!is_valid_positionId(id)) {
        return result;
    }
    Position pos(id);

    if (status_of_analysisData(analysisDataTable_[pos.minimize_id()]) == AnalysisStatus::Contradictory ||
        pos.is_winner(PlayerId::Active) ||
        pos.is_winner(PlayerId::Inactive)) {
        return result;
    }

    for (PieceId piece: ActivePlayerPieceIds) {
        LocationIdPair locPair = pos.locations_of_piece(piece);

        for (int i = 0; i < 2; i++) {
            LocationId src = locPair.locations[i];

            for (LocationId dst: OnBoardLocationIds) {
                MoveResult moveResult = pos.move(piece, src, dst);
                if (moveResult.status != MoveResultStatus::Success) {
                    continue;
                }

                //
                // We invert analysis status of the position after the move.
                // The status code recorded in `analysisData` is the status of the active player at the next turn,
                // but what we want here is the status of the active player at the current turn.
                //
                AnalysisData analysisData = analysisDataTable_[moveResult.position.minimize_id()];
                AnalysisStatus analysisStatus = invert_analysisStatus(status_of_analysisData(analysisData));

                if (analysisStatus == AnalysisStatus::Contradictory ||
                    analysisStatus == AnalysisStatus::Transformed ||
                    analysisStatus == AnalysisStatus::Invalid) {
                    continue;
                }
                result.push_back(
                    MoveInspectionResult {piece, src, dst, moveResult.position.id(),
                        turn_of_analysisData(analysisData), analysisStatus, false});
            }
            if (locPair.locations[0] == locPair.locations[1]) {
                break;
            }
        }
    }

    mark_best_move(result);
    return result;
}

std::vector<MoveInspectionResult> Inspector::inspect_move_backs(PositionId id) const noexcept {
    std::vector<MoveInspectionResult> result;

    if (!is_valid_positionId(id)) {
        return result;
    }

    Position pos(id);
    if (status_of_analysisData(analysisDataTable_[pos.minimize_id()]) == AnalysisStatus::Contradictory) {
        return result;
    }

    for (PieceId piece: InactivePlayerPieceIds) {
        LocationIdPair locPair = pos.locations_of_piece(piece);

        for (int i = 0; i < 2; i++) {
            LocationId src = locPair.locations[i];

            for (LocationId dst: LocationIds) {
                MoveResult moveResult = pos.move_back(piece, src, dst);
                if (moveResult.status != MoveResultStatus::Success) {
                    continue;
                }

                //
                // We invert analysis status of the position after the move.
                // The status code recorded in `analysisData` is the status of the active player at the next turn,
                // but what we want here is the status of the active player at the current turn.
                //
                AnalysisData analysisData = analysisDataTable_[moveResult.position.minimize_id()];
                AnalysisStatus analysisStatus = invert_analysisStatus(status_of_analysisData(analysisData));
                if (analysisStatus == AnalysisStatus::Contradictory ||
                    analysisStatus == AnalysisStatus::Transformed ||
                    analysisStatus == AnalysisStatus::Invalid) {
                    continue;
                }

                result.push_back(
                    MoveInspectionResult {piece, src, dst, moveResult.position.id(),
                        turn_of_analysisData(analysisData), analysisStatus, false});
                if (locPair.locations[0] == locPair.locations[1]) {
                    break;
                }
            }
        }
    }

    mark_best_move(result);
    return result;
}

void Inspector::mark_best_move(std::vector<MoveInspectionResult>& moveInspectionResults) const noexcept {
    AnalysisStatus bestStatus = AnalysisStatus::Contradictory;
    Turn bestTurn = MaxTurn;

    for (MoveInspectionResult& res: moveInspectionResults) {
        if (res.analysisStatus == AnalysisStatus::Lost || res.analysisStatus == AnalysisStatus::LostStalemate) {
            if (bestStatus == AnalysisStatus::Lost || bestStatus == AnalysisStatus::LostStalemate) {
                if (res.turn > bestTurn) {
                    bestTurn = res.turn;
                }
            } else if (bestStatus == AnalysisStatus::Contradictory) {
                bestStatus = res.analysisStatus;
                bestTurn = res.turn;
            }
        } else if (res.analysisStatus == AnalysisStatus::Unfixed) {
            if (bestStatus != AnalysisStatus::Won && bestStatus != AnalysisStatus::WonStalemate) {
                bestStatus = res.analysisStatus;
            }
        } else if (res.analysisStatus == AnalysisStatus::Won || res.analysisStatus == AnalysisStatus::WonStalemate) {
            if (bestStatus == AnalysisStatus::Won || bestStatus == AnalysisStatus::WonStalemate) {
                if (res.turn < bestTurn) {
                    bestTurn = res.turn;
                }
            } else {
                bestStatus = res.analysisStatus;
                bestTurn = res.turn;
            }
        }
    }

    if (bestStatus == AnalysisStatus::Lost || bestStatus == AnalysisStatus::LostStalemate) {
        for (MoveInspectionResult& res: moveInspectionResults) {
            if (res.analysisStatus == AnalysisStatus::Lost || res.analysisStatus == AnalysisStatus::LostStalemate) {
                if (res.turn == bestTurn) {
                    res.isBestMove = true;
                }
            }
        }
    } else if (bestStatus == AnalysisStatus::Unfixed) {
        for (MoveInspectionResult& res: moveInspectionResults) {
            if (res.analysisStatus == bestStatus) {
                res.isBestMove = true;
            }
        }
    } else if (bestStatus == AnalysisStatus::Won || bestStatus == AnalysisStatus::WonStalemate) {
        for (MoveInspectionResult& res: moveInspectionResults) {
            if (res.analysisStatus == AnalysisStatus::Won || res.analysisStatus == AnalysisStatus::WonStalemate) {
                if (res.turn == bestTurn) {
                    res.isBestMove = true;
                }
            }
        }
    }
}

} // namespace gobb_analyzer
