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

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "gobb_inspect_processor.hpp"
#include "string_to_uint.hpp"

using namespace gobb_analyzer;

GobbInspectProcessor::GobbInspectProcessor(Inspector& inspector, PositionTextCreator& textCreator,
    PositionId posId)
    : inspector_(inspector),
      textCreator_(textCreator),
      position_(posId),
      positionInspectionResult_(inspector.inspect_position(posId)),
      moveInspectionResults_(inspector.inspect_moves(posId)),
      moveBackInspectionResults_(inspector.inspect_move_backs(posId)),
      historyTable_(),
      currentHistoryIndex_(0u) {
    historyTable_.push_back(inspector_.inspect_position(posId));
}

GobbInspectProcessor::~GobbInspectProcessor() {
}

void GobbInspectProcessor::do_main_loop() {
    std::string line;

    show_position();
    show_moves();

    for (;;) {
        std::cout << "gobb_inspect> " << std::flush;
        if (!std::getline(std::cin, line)) {
            break;
        }

        std::vector<std::string> args = split_into_arguments(line);
        if (args.size() == 0) {
            continue;
        }

        if (args[0] == "show-position" || args[0] == "sp") {
            do_show_position_command(args);
        } else if (args[0] == "goto-position" || args[0] == "gp") {
            do_goto_position_command(args);
        } else if (args[0] == "show-moves" || args[0] == "sm") {
            do_show_moves_command(args);
        } else if (args[0] == "show-movebacks" || args[0] == "smb") {
            do_show_move_backs_command(args);
        } else if (args[0] == "move" || args[0] == "m") {
            do_move_command(args);
        } else if (args[0] == "moveback" || args[0] == "mb") {
            do_move_back_command(args);
        } else if (args[0] == "show-history" || args[0] == "sh") {
            do_show_history_command(args);
        } else if (args[0] == "goto-history" || args[0] == "gh") {
            do_goto_history_command(args);
        } else if (args[0] == "next" || args[0] == "n") {
            do_next_command(args);
        } else if (args[0] == "previous" || args[0] == "p") {
            do_previous_command(args);
        } else if (args[0] == "help" || args[0] == "?") {
            do_help_command(args);
        } else if (args[0] == "exit") {
            break;
        } else {
            show_line("invalid command");
            show_hint();
        }
    }
}

void GobbInspectProcessor::do_show_position_command(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        show_line("invalid arguments to 'show-position' command");
        show_hint();
        return;
    }
    show_position();
}

void GobbInspectProcessor::do_show_moves_command(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        show_line("invalid arguments to 'show-moves' command");
        show_hint();
        return;
    }
    show_moves();
}

void GobbInspectProcessor::do_show_move_backs_command(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        show_line("invalid arguments to 'show-movebacks' command");
        show_hint();
        return;
    }
    show_move_backs();
}

void GobbInspectProcessor::do_goto_position_command(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        show_line("invalid arguments to 'position' command");
        show_hint();
        return;
    }
    PositionId posId;
    if (!string_to_uint(args[1], posId)) {
        show_line("invalid position");
        return;
    }
    position_ = posId;
    positionInspectionResult_ = inspector_.inspect_position(posId);
    moveInspectionResults_ = inspector_.inspect_moves(posId);
    moveBackInspectionResults_ = inspector_.inspect_move_backs(posId);

    show_horizontal_line();
    show_position();
    show_moves();
    add_history(positionInspectionResult_);
}

void GobbInspectProcessor::do_move_command(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        show_line("invalid arguments for 'move' command");
        show_hint();
        return;
    }

    unsigned long index = 0u;
    if (!string_to_uint(args[1], index)) {
        show_line("invalid index");
        return;
    }
    if (index >= moveInspectionResults_.size()) {
        show_line("invalid index for 'move' command");
        return;
    }

    MoveResult moveResult = position_.move(
        moveInspectionResults_[index].piece,
        moveInspectionResults_[index].source,
        moveInspectionResults_[index].destination);
    position_ = moveResult.position;
    positionInspectionResult_ = inspector_.inspect_position(position_.id());
    moveInspectionResults_ = inspector_.inspect_moves(position_.id());
    moveBackInspectionResults_ = inspector_.inspect_move_backs(position_.id());

    show_horizontal_line();
    show_position();
    show_moves();
    add_history(positionInspectionResult_);
}

void GobbInspectProcessor::do_move_back_command(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        show_line("invalid arguments for 'moveback' command");
        show_hint();
        return;
    }

    unsigned long index = 0u;
    if (!string_to_uint(args[1], index)) {
        show_line("invalid index");
    }
    if (index >= moveBackInspectionResults_.size()) {
        show_line("invalid index for 'moveback' command");
        return;
    }

    MoveResult moveResult = position_.move_back(
        moveBackInspectionResults_[index].piece,
        moveBackInspectionResults_[index].source,
        moveBackInspectionResults_[index].destination);
    position_ = moveResult.position;
    positionInspectionResult_ = inspector_.inspect_position(position_.id());
    moveInspectionResults_ = inspector_.inspect_moves(position_.id());
    moveBackInspectionResults_ = inspector_.inspect_move_backs(position_.id());

    show_horizontal_line();
    show_position();
    show_moves();
    add_history(positionInspectionResult_);
}

void GobbInspectProcessor::do_show_history_command(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        show_line("invalid arguments to 'show-history' command");
        show_hint();
        return;
    }
    show_history();
}

void GobbInspectProcessor::do_goto_history_command(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        show_line("invalid arguments to 'history' command");
        show_hint();
        return;
    }

    unsigned long index = 0u;
    if (!string_to_uint(args[1], index)) {
        show_line("invalid index");
        return;
    }
    if (index >= historyTable_.size()) {
        show_line("invalid index for 'history' command");
        return;
    }

    currentHistoryIndex_ = index;
    PositionId posId = historyTable_[currentHistoryIndex_].positionId;
    position_ = posId;
    positionInspectionResult_ = inspector_.inspect_position(posId);
    moveInspectionResults_ = inspector_.inspect_moves(posId);
    moveBackInspectionResults_ = inspector_.inspect_move_backs(posId);

    show_horizontal_line();
    show_position();
    show_moves();
}

void GobbInspectProcessor::do_next_command(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        show_line("invalid arguments to 'next' command");
        show_hint();
        return;
    }
    if (currentHistoryIndex_ + 1 >= historyTable_.size()) {
        show_line("no next entry in the history table");
        return;
    }
    currentHistoryIndex_++;
    PositionId posId = historyTable_[currentHistoryIndex_].positionId;
    position_ = posId;
    positionInspectionResult_ = inspector_.inspect_position(posId);
    moveInspectionResults_ = inspector_.inspect_moves(posId);
    moveBackInspectionResults_ = inspector_.inspect_move_backs(posId);

    show_position();
    show_moves();
}

void GobbInspectProcessor::do_previous_command(const std::vector<std::string>& args){
    if (args.size() != 1) {
        show_line("invalid arguments to 'previous' command");
        show_hint();
        return;
    }
    if (currentHistoryIndex_ == 0) {
        show_line("no previous entry in the history table");
        return;
    }

    currentHistoryIndex_--;
    PositionId posId = historyTable_[currentHistoryIndex_].positionId;
    position_ = posId;
    positionInspectionResult_ = inspector_.inspect_position(posId);
    moveInspectionResults_ = inspector_.inspect_moves(posId);
    moveBackInspectionResults_ = inspector_.inspect_move_backs(posId);

    show_position();
    show_moves();
}

void GobbInspectProcessor::do_help_command(const std::vector<std::string>& args) {
    static_cast<void>(args);

    show_line("Position:");
    show_line("  (sp)  show-position     show the current position");
    show_line("  (gp)  goto-position ID  go to the position ID");

    show_line("Move:");
    show_line("  (sm)  show-moves        show possible moves");
    show_line("  (smb) show-movebacks    show possible retrograde moves");
    show_line("  (m)   move NUM          execute the movement of the possible move NUM");
    show_line("  (mb)  moveback NUM      execute the movement of the possible");
    show_line("                          retrograde move NUM");

    show_line("History:");
    show_line("  (sh)  show-history      show the history table");
    show_line("  (gh)  goto-history NUM  go to the position of the history NUM");
    show_line("  (n)   next              go to the next position of the history");
    show_line("  (p)   previous          go to the previous position of the history");
    show_line("");

    show_line("Miscellaneous:");
    show_line("  (?)   help              print this help");
    show_line("        exit              exit the program");
}

void GobbInspectProcessor::show_horizontal_line() const {
    show_line("----------------------------------------");
}

void GobbInspectProcessor::show_position() const {
    show_line(fmt::format("position = {}, remainingTurns = {}, {}",
            position_.id(),
            positionInspectionResult_.turn,
            analysisStatus_to_string(positionInspectionResult_.analysisStatus)));

    if (!position_.is_valid()) {
        show_line("### the position is not valid. ###");
        return;
    }

    std::vector<std::string> lines = textCreator_.position_to_string(position_);
    for (std::string& line: lines) {
        show_line(line);
    }

    PlayerColor activePlayerColor = position_.active_player_color();
    show_line("(the player having the turn: \"{}\")", textCreator_.piece_symbol(activePlayerColor));
    show_line();
}

void GobbInspectProcessor::show_moves() const {
    show_line("possible moves:");

    std::string bestMark;
    std::size_t index = 0u;
    for (MoveInspectionResult insRes: moveInspectionResults_) {
        if (insRes.isBestMove) {
            bestMark = " [best]";
        } else {
            bestMark = "";
        }

        show_line("  {:{}d}| {:{}s}, {:{}s} -> {:{}s}, position = {:{}d}, remainingTurns = {:{}d}, {}{}",
            index, maxIndexWidth_,
            pieceSize_to_string(pieceSize_of_pieceId(insRes.piece)), ValidPieceSizeStringMaxLen,
            locationId_to_string(insRes.source), ValidLocationIdStringMaxLen,
            locationId_to_string(insRes.destination), ValidLocationIdStringMaxLen,
            insRes.positionId, MaxPositionIdWidth,
            insRes.turn, maxTurnWidth_,
            analysisStatus_to_string(insRes.analysisStatus),
            bestMark);
        index++;
    }
}

void GobbInspectProcessor::show_move_backs() const {
    show_line("possible retrograde moves:");

    std::string bestMark;
    std::size_t index = 0u;
    for (MoveInspectionResult insRes: moveBackInspectionResults_) {
        if (insRes.isBestMove) {
            bestMark = " [best]";
        } else {
            bestMark = "";
        }

        show_line("  {:{}d}| {:{}s}, {:{}s} -> {:{}s}, position = {:{}d}, remainingTurns = {:{}d}, {}{}",
            index, maxIndexWidth_,
            pieceSize_to_string(pieceSize_of_pieceId(insRes.piece)), ValidPieceSizeStringMaxLen,
            locationId_to_string(insRes.source), ValidLocationIdStringMaxLen,
            locationId_to_string(insRes.destination), ValidLocationIdStringMaxLen,
            insRes.positionId, MaxPositionIdWidth,
            insRes.turn, maxTurnWidth_,
            analysisStatus_to_string(insRes.analysisStatus),
            bestMark);
        index++;
    }
}

void GobbInspectProcessor::show_history() const {
    show_line("history:");

    std::string hereMark;
    std::size_t index = 0u;
    for (PositionInspectionResult entry: historyTable_) {
        if (index == currentHistoryIndex_) {
            hereMark = " [here]";
        } else {
            hereMark = "";
        }

        show_line("  {:{}d}| position = {:{}d}, remainingTurns = {:{}d}, {}{}",
            index, maxIndexWidth_,
            entry.positionId, MaxPositionIdWidth,
            entry.turn, maxTurnWidth_,
            analysisStatus_to_string(entry.analysisStatus),
            hereMark);
        index++;
    }
}

void GobbInspectProcessor::show_hint() const {
    show_line("Try 'help' or '?' for more information.");
}

void GobbInspectProcessor::add_history(const PositionInspectionResult& entry) {
    for (std::size_t i = historyTable_.size() - 1; i > currentHistoryIndex_; i--) {
        historyTable_.pop_back();
    }
    if (historyTable_.size() == maxHistoryTableSize_) {
        historyTable_.pop_front();
        if (currentHistoryIndex_ > 0u) {
            currentHistoryIndex_--;
        }
    }
    historyTable_.push_back(entry);
    currentHistoryIndex_++;
}

void GobbInspectProcessor::show_line() const {
    std::cout << std::endl;
}

void GobbInspectProcessor::show_line(const std::string& line) const {
    std::cout << line << std::endl;
}

std::vector<std::string> GobbInspectProcessor::split_into_arguments(const std::string& line) const {
    std::vector<std::string> args;
    std::size_t lineSize = line.size();
    std::size_t index = 0u;

    for (;;) {
        while (index < lineSize) {
            char ch = line[index];
            if (ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n') {
                break;
            }
            index++;
        }
        if (index == lineSize) {
            break;
        }

        std::size_t argStartIndex = index++;
        while (index < lineSize) {
            char ch = line[index];
            if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
                break;
            }
            index++;
        }
        args.push_back(line.substr(argStartIndex, index - argStartIndex));
        if (index == lineSize) {
            break;
        }
    }

    return args;
}
