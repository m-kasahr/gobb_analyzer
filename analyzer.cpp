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

#include "analyzer.hpp"

namespace gobb_analyzer {

//
// AnalysisStatus.
//
const std::string& analysisStatus_to_string(AnalysisStatus status) {
    static const std::string strings[] = {
        "Unfixed",
        "Lost",
        "LostStalemate",
        "Won",
        "WonStalemate",
        "Transformed",
        "Contradictory",
    };
    static const std::string invalidString = "Invalid";

    if (status <= AnalysisStatus::Contradictory) {
        return strings[static_cast<int>(status)];
    } else {
        return invalidString;
    }
}

//
// struct AnalysisStatistics.
//
AnalysisStatistics::AnalysisStatistics() noexcept
    : lostNums(0u),
      lostStalemateNums(0u),
      wonNums(0u),
      transformedNums(0u),
      contradictoryNums(0u),
      unfixedNums(0u) {
}

void AnalysisStatistics::clear() noexcept {
    lostNums          = 0u;
    lostStalemateNums = 0u;
    wonNums           = 0u;
    transformedNums   = 0u;
    contradictoryNums = 0u;
    unfixedNums       = 0u;
}

void AnalysisStatistics::add(const AnalysisStatistics& other) noexcept {
    unfixedNums -= \
        other.lostNums + other.lostStalemateNums + other.wonNums + other.transformedNums + other.contradictoryNums;
    lostNums          += other.lostNums;
    lostStalemateNums += other.lostStalemateNums;
    wonNums           += other.wonNums;
    transformedNums   += other.transformedNums;
    contradictoryNums += other.contradictoryNums;
}

//
// Class AnalysisDataIOHandler.
//
AnalysisDataIOHandler::~AnalysisDataIOHandler() {
}

//
// Class AnalysisLogger.
//
AnalysisLogger::~AnalysisLogger() {
}

//
// Class Analyzer.
//
Analyzer::Analyzer(AnalysisLogger& logger)
    : generation_(InvalidGeneration),
      storedGeneration_(InvalidGeneration),
      analysisDataTable_(nullptr),
      statistics_(),
      logger_(logger) {
    analysisDataTable_ = new AnalysisData [AnalysisDataTableSize];
}

Analyzer::~Analyzer() {
    delete[] analysisDataTable_;
}

bool Analyzer::start(AnalysisDataIOHandler& handler, AnalysisDataIOMode ioMode) {
    generation_ = 0u;
    logger_.notice("start the generation 0 (initialization).");
    if (!initialize()) {
        return false;
    }
    log_statistics(0, statistics_);

    if (ioMode == AnalysisDataIOMode::StoreEveryGenerations) {
        if (!handler.store(0u, statistics_, analysisDataTable_, AnalysisDataTableSize * sizeof(AnalysisData))) {
            logger_.error("failed to store the initial analysis data.");
            return false;
        }
        storedGeneration_ = 0u;
        logger_.notice("stored analysis data of the generation 0 (initialization).");
    }

    generation_ = 1u;
    return analyze(handler, ioMode);
}

bool Analyzer::resume(AnalysisDataIOHandler& handler, AnalysisDataIOMode ioMode) {
    Generation generation = handler.find_latest();
    if (generation == InvalidGeneration) {
        logger_.warn("no analysis data found.");
        return start(handler, ioMode);
    }
    logger_.notice("found the analysis data of the generation {}.", static_cast<int>(generation));
    return resume(handler, ioMode, generation);
}

bool Analyzer::resume(AnalysisDataIOHandler& handler, AnalysisDataIOMode ioMode, Generation generation) {
    if (!handler.load(generation, statistics_, analysisDataTable_, AnalysisDataTableSize * sizeof(AnalysisData))) {
        logger_.error("failed to load the analysis data of the generation {}.", static_cast<int>(generation));
        return false;
    }
    generation_ = generation + 1;
    storedGeneration_ = generation;
    logger_.notice("resume analysis from the generation {}.", static_cast<int>(generation_));
    return analyze(handler, ioMode);
}

bool Analyzer::analyze(AnalysisDataIOHandler& handler, AnalysisDataIOMode ioMode) {
    while (generation_ <= MaxGeneration) {
        logger_.notice("analyze the generation {}.", static_cast<int>(generation_));

        AnalysisStatistics generationStats;
        bool updated = analyze_generation(generationStats);
        statistics_.add(generationStats);
        log_statistics(generation_, generationStats);

        bool needsStoring = false;
        if (updated) {
            needsStoring = (ioMode == AnalysisDataIOMode::StoreEveryGenerations);
        } else if (storedGeneration_ == InvalidGeneration || storedGeneration_ + 1 < generation_) {
            needsStoring = (ioMode != AnalysisDataIOMode::StoreNoGeneration);
        }

        if (needsStoring) {
            if (!handler.store(generation_, statistics_, analysisDataTable_,
                AnalysisDataTableSize * sizeof(AnalysisData))) {
                logger_.error("failed to store analysis data of the generation {}.", static_cast<int>(generation_));
                return false;
            }
            storedGeneration_ = generation_;
            logger_.notice("stored analysis data of the generation {}.", static_cast<int>(generation_));
        }

        if (!updated) {
            logger_.notice("no update occurred. the analysis is complete.");
            break;
        }
        generation_++;
        logger_.notice();
    }

    if (generation_ > MaxGeneration) {
        logger_.warn("the generation exceeds its limit. give up the analysis.");
    }
    return true;
}

bool Analyzer::initialize() noexcept {
    bool updated = false;

    //
    // Set initial values.
    //
    AnalysisData initialData = to_analysisData(false, MaxTurn, AnalysisStatus::Unfixed);
    for (PositionId i = 0u; i < AnalysisDataTableSize; i++) {
        analysisDataTable_[i] = initialData;
    }

    for (PositionId i = 0u; i < AnalysisDataTableSize; i++) {
        if (status_of_analysisData(analysisDataTable_[i]) == AnalysisStatus::Transformed) {
            statistics_.transformedNums++;
            continue;
        }

        Position pos(i);
        for (TransformerId trans: EffectiveTransformerIds) {
            //
            // If the position can be transformed to another symmetric position with a smaller position ID,
            // the position is marked with Transformed.
            //
            Position transPos = pos.transform(trans);
            PositionId transPosId = transPos.id();
            if (transPosId > i && transPosId < AnalysisDataTableSize) {
                analysisDataTable_[transPosId] = to_analysisData(false, 0u, AnalysisStatus::Transformed);
            }
        }

        //
        // At the beginning of the turn, if three pieces of the active player have already been lined up
        // in a row, the position is marked with Contradictory.
        //
        if (pos.is_winner(PlayerId::Active)) {
            analysisDataTable_[i] = to_analysisData(false, 0u, AnalysisStatus::Contradictory);
            statistics_.contradictoryNums++;
            continue;
        }

        //
        // At the beginning of the turn, if the active player has not placed any piece on the board yet,
        // but the inactive player has placed two or more pieces, the position is marked with Contradictory.
        //
        int activePieceNums = on_board_piece_nums(pos, PlayerId::Active);
        int inactivePieceNums = on_board_piece_nums(pos, PlayerId::Inactive);
        if (activePieceNums == 0 && inactivePieceNums >= 2) {
            analysisDataTable_[i] = to_analysisData(false, 0u, AnalysisStatus::Contradictory);
            statistics_.contradictoryNums++;
            continue;
        }

        //
        // At the beginning of the turn, if the inactive player has not placed any piece on the board yet,
        // but the active player has placed one or more pieces, the position is marked with Contradictory.
        //
        if (inactivePieceNums == 0 && activePieceNums >= 1) {
            analysisDataTable_[i] = to_analysisData(false, 0u, AnalysisStatus::Contradictory);
            statistics_.contradictoryNums++;
            continue;
        }

        //
        // At the beginning of the turn, if three pieces of the inactive player have already been
        // lined up in a row, the position is marked with Lost.  It means the inactive player won.
        // We sets the number of remained turns to 0, because the game was over in the previous turn.
        //
        if (pos.is_winner(PlayerId::Inactive)) {
            analysisDataTable_[i] = to_analysisData(true, 0u, AnalysisStatus::Lost);
            updated = true;
            statistics_.lostNums++;
            continue;
        }

        //
        // If there is no possible moves, the position is marked with LostStalemate.
        // The active player must pick up one of his pieces on the board, but it causes that three pieces
        // of the inactive player are lined up in a row.
        // We sets the number of remained turns to 1, because the game is over during the current turn.
        //
        if (move_nums(pos) == 0) {
            analysisDataTable_[i] = to_analysisData(true, 1u, AnalysisStatus::LostStalemate);
            updated = true;
            statistics_.lostStalemateNums++;
            continue;
        }

        statistics_.unfixedNums++;
    }

    return updated;
}

bool Analyzer::analyze_generation(AnalysisStatistics& stats) noexcept {
    bool updated = false;

    for (PositionId i = 0u; i < AnalysisDataTableSize; i++) {
        AnalysisData& data = analysisDataTable_[i];
        if (!updateFlag_of_analysisData(data)) {
            continue;
        }
        data = set_updateFlag_of_analysisData(data, false);

        AnalysisStatus status = status_of_analysisData(data);
        if (status == AnalysisStatus::Lost || status == AnalysisStatus::LostStalemate) {
            Position pos(i);
            if (analyze_move_backs_from_active_player_lost(stats, pos)) {
                updated = true;
            }
        } else if (status == AnalysisStatus::Won) {
            Position pos(i);
            if (analyze_move_backs_from_active_player_won(stats, pos)) {
                updated = true;
            }
        } else if (status == AnalysisStatus::Unfixed) {
            Position pos(i);
            if (analyze_unfixed(stats, pos)) {
                analyze_move_backs_from_active_player_lost(stats, pos);
                updated = true;
            }
        }
    }

    return updated;
}

bool Analyzer::analyze_move_backs_from_active_player_lost(AnalysisStatistics& stats, const Position& pos) noexcept {
    bool updated = false;

    Turn turn = turn_of_analysisData(analysisDataTable_[pos.id()]);
    Turn nextTurn;
    if (turn == MaxTurn) {
        nextTurn = turn;
    } else {
        nextTurn = turn + 1u;
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

                AnalysisData& dstData = analysisDataTable_[moveResult.position.minimize_id()];
                if (status_of_analysisData(dstData) == AnalysisStatus::Unfixed) {
                    dstData = to_analysisData(true, nextTurn, AnalysisStatus::Won);
                    stats.wonNums++;
                    updated = true;
                } else if (status_of_analysisData(dstData) == AnalysisStatus::Won &&
                    turn_of_analysisData(dstData) > nextTurn) {
                    dstData = set_turn_of_analysisData(dstData, nextTurn);
                }
            }

            if (locPair.locations[0] == locPair.locations[1]) {
                break;
            }
        }
    }

    return updated;
}

bool Analyzer::analyze_move_backs_from_active_player_won(AnalysisStatistics& stats, const Position& pos) noexcept {
    static_cast<void>(stats);
    bool updated = false;

    for (PieceId piece: InactivePlayerPieceIds) {
        LocationIdPair locPair = pos.locations_of_piece(piece);

        for (int i = 0; i < 2; i++) {
            LocationId src = locPair.locations[i];

            for (LocationId dst: LocationIds) {
                MoveResult moveResult = pos.move_back(piece, src, dst);
                if (moveResult.status != MoveResultStatus::Success) {
                    continue;
                }

                AnalysisData& dstData = analysisDataTable_[moveResult.position.minimize_id()];
                if (status_of_analysisData(dstData) == AnalysisStatus::Unfixed) {
                    dstData = set_updateFlag_of_analysisData(dstData, true);
                    updated = true;
                }
            }
            if (locPair.locations[0] == locPair.locations[1]) {
                break;
            }
        }
    }

    return updated;
}

bool Analyzer::analyze_unfixed(AnalysisStatistics& stats, const Position& pos) noexcept {
    Turn nextTurn = 0u;

    for (PieceId piece: ActivePlayerPieceIds) {
        LocationIdPair locPair = pos.locations_of_piece(piece);

        for (int i = 0; i < 2; i++) {
            LocationId src = locPair.locations[i];

            for (LocationId dst: OnBoardLocationIds) {
                MoveResult moveResult = pos.move(piece, src, dst);
                if (moveResult.status != MoveResultStatus::Success) {
                    continue;
                }

                AnalysisData& dstData = analysisDataTable_[moveResult.position.minimize_id()];
                if (status_of_analysisData(dstData) != AnalysisStatus::Won) {
                    return false;
                }
                Turn turn = turn_of_analysisData(dstData);
                if (turn + 1 == MaxTurn) {
                    nextTurn = MaxTurn;
                } else if (turn + 1 > nextTurn) {
                    nextTurn = turn + 1;
                }
            }
            if (locPair.locations[0] == locPair.locations[1]) {
                break;
            }
        }
    }

    analysisDataTable_[pos.id()] = to_analysisData(false, nextTurn, AnalysisStatus::Lost);
    stats.lostNums++;
    return true;
}

int Analyzer::move_nums(const Position& pos) const noexcept {
    int nums = 0;

    for (PieceId piece: ActivePlayerPieceIds) {
        LocationIdPair locPair = pos.locations_of_piece(piece);

        for (int i = 0; i < 2; i++) {
            LocationId src = locPair.locations[i];

            for (LocationId dst: OnBoardLocationIds) {
                MoveResult moveResult = pos.move(piece, src, dst);
                if (moveResult.status == MoveResultStatus::Success) {
                    nums++;
                }
            }
        }
    }

    return nums;
}

int Analyzer::on_board_piece_nums(const Position& pos, PlayerId player) const noexcept {
    int nums = 0;

    if (player == PlayerId::Active) {
        for (PieceId piece: ActivePlayerPieceIds) {
            const LocationIdPair locPair = pos.locations_of_piece(piece);
            for (int i = 0; i < 2; i++) {
                if (locPair.locations[i] != LocationId::Out) {
                    nums++;
                }
            }
        }
    } else if (player == PlayerId::Inactive) {
        for (PieceId piece: InactivePlayerPieceIds) {
            const LocationIdPair locPair = pos.locations_of_piece(piece);
            for (int i = 0; i < 2; i++) {
                if (locPair.locations[i] != LocationId::Out) {
                    nums++;
                }
            }
        }
    }

    return nums;
}

void Analyzer::log_statistics(Generation generation, AnalysisStatistics& stats) {
    if (generation == 0) {
        logger_.info("analysis result of the initialization:");
    } else {
        logger_.info("analysis result of the generation {}:", static_cast<int>(generation));
        logger_.info("  fixed positions during this generation:");
        logger_.info("    lost          = {}", stats.lostNums);
        logger_.info("    won           = {}", stats.wonNums);
    }

    logger_.info("  total:");
    logger_.info("    lost          = {}", statistics_.lostNums);
    logger_.info("    lostStalemate = {}", statistics_.lostStalemateNums);
    logger_.info("    won           = {}", statistics_.wonNums);
    logger_.info("    transformed   = {}", statistics_.transformedNums);
    logger_.info("    contradictory = {}", statistics_.contradictoryNums);
    logger_.info("    unfixed       = {}", statistics_.unfixedNums);
    logger_.info();
}

} // namespace gobb_analyzer
