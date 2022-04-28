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

#ifndef GOBB_INSPECT_PROCESSOR_HPP
#define GOBB_INSPECT_PROCESSOR_HPP

#include <cstddef>
#include <deque>
#include <string>
#include <vector>
#include <fmt/core.h>
#include "position_text_creator.hpp"
#include "position.hpp"
#include "inspector.hpp"

using namespace gobb_analyzer;

//
// Class GobbInspectProcessor.
//

class GobbInspectProcessor {
public:
    GobbInspectProcessor() = delete;
    GobbInspectProcessor(Inspector& inspector, PositionTextCreator& viewer, PositionId posId = 0u);
    GobbInspectProcessor(const GobbInspectProcessor& other) = delete;
    GobbInspectProcessor(GobbInspectProcessor&& other) = delete;
    ~GobbInspectProcessor();
    GobbInspectProcessor& operator=(const GobbInspectProcessor& other) = delete;
    GobbInspectProcessor& operator=(GobbInspectProcessor&& other) = delete;

    void do_main_loop();
    void do_show_position_command(const std::vector<std::string>& args);
    void do_goto_position_command(const std::vector<std::string>& args);
    void do_show_moves_command(const std::vector<std::string>& args);
    void do_show_move_backs_command(const std::vector<std::string>& args);
    void do_move_command(const std::vector<std::string>& args);
    void do_move_back_command(const std::vector<std::string>& args);
    void do_show_history_command(const std::vector<std::string>& args);
    void do_goto_history_command(const std::vector<std::string>& args);
    void do_next_command(const std::vector<std::string>& args);
    void do_previous_command(const std::vector<std::string>& args);
    void do_help_command(const std::vector<std::string>& args);

private:
    void show_horizontal_line() const;
    void show_position() const;
    void show_moves() const;
    void show_move_backs() const;
    void show_history() const;
    void show_hint() const;
    void show_line() const;
    void show_line(const std::string& line) const;

    template<typename ...T>
    void show_line(fmt::format_string<T...> fmt, T&&... args) const {
        show_line(fmt::format(fmt, args...));
    }

    void add_history(const PositionInspectionResult& entry);
    std::vector<std::string> split_into_arguments(const std::string& line) const;

    Inspector& inspector_;
    PositionTextCreator& textCreator_;
    Position position_;
    PositionInspectionResult positionInspectionResult_;
    std::vector<MoveInspectionResult> moveInspectionResults_;
    std::vector<MoveInspectionResult> moveBackInspectionResults_;
    std::deque<PositionInspectionResult> historyTable_;
    std::size_t currentHistoryIndex_;

    static constexpr std::size_t maxHistoryTableSize_ = 32u;
    static constexpr std::size_t maxIndexWidth_ = 2u;
    static constexpr std::size_t maxTurnWidth_ = 2u;
};

#endif // GOBB_INSPECT_PROCESSOR_HPP
