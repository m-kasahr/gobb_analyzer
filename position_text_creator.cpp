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

#include "position_text_creator.hpp"

namespace gobb_analyzer {

PositionTextCreator::~PositionTextCreator() {
}

PositionAsciiCreator::PositionAsciiCreator(bool enableEscape) noexcept
    : PositionTextCreator() {
    if (enableEscape) {
        piece_to_string_ = &PositionAsciiCreator::piece_to_string_with_escape;
        piece_symbol_ = &PositionAsciiCreator::piece_symbol_with_escape;
    } else {
        piece_to_string_ = &PositionAsciiCreator::piece_to_string_without_escape;
        piece_symbol_ = &PositionAsciiCreator::piece_symbol_without_escape;
    }
}

PositionAsciiCreator::~PositionAsciiCreator() {
}

std::vector<std::string> PositionAsciiCreator::position_to_string(const Position& pos) const {
    static const char* horizontalLine = "+-------+-------+-------+";

    static const LocationId xyToLocationMaps[BoardLength][BoardLength] = {
        {LocationId::NW, LocationId::W,      LocationId::SW},
        {LocationId::N,  LocationId::Center, LocationId::S },
        {LocationId::NE, LocationId::E,      LocationId::SE}
    };

    std::vector<std::string> lines;
    if (!pos.is_valid()) {
        lines.push_back("invalid position");
        return lines;
    }

    PlayerColor activePlayerColor = pos.active_player_color();
    PlayerColor inactivePlayerColor = pos.inactive_player_color();

    for (std::size_t y = 0u; y < BoardLength; y++) {
        lines.push_back(horizontalLine);
        for (int sizeIndex = PieceSizeNums - 1; sizeIndex >= 0; sizeIndex--) {
            std::string line("|");
            for (std::size_t x = 0u; x < BoardLength; x++) {
                std::function<bool(PieceId, LocationId)> filter = [&](PieceId piece, LocationId loc) -> bool {
                    return (loc == xyToLocationMaps[x][y] &&
                        pieceSize_of_pieceId(piece) == PieceSizes[sizeIndex]);
                };
                std::vector<PieceId> pieces = pos.find_pieces(filter);
                if (pieces.size() == 0) {
                    line.append((this->*piece_to_string_)(PlayerColor::Orange, PieceSize::None));
                } else if (playerId_of_pieceId(pieces[0]) == PlayerId::Active) {
                    line.append((this->*piece_to_string_)(activePlayerColor, pieceSize_of_pieceId(pieces[0])));
                } else {
                    line.append((this->*piece_to_string_)(inactivePlayerColor, pieceSize_of_pieceId(pieces[0])));
                }
                line.append("|");
            }
            lines.push_back(line);
        }
    }

    lines.push_back(horizontalLine);
    return lines;
}

const std::string& PositionAsciiCreator::piece_to_string(PlayerColor color, PieceSize size) const noexcept {
    return (this->*piece_to_string_)(color, size);
}

const std::string& PositionAsciiCreator::piece_symbol(PlayerColor color) const noexcept {
    return (this->*piece_symbol_)(color);
}

const std::string& PositionAsciiCreator::piece_to_string_without_escape(PlayerColor color, PieceSize size) const
    noexcept {
    static const std::string orange_strings[PieceSizeNums + 1] = {
        std::string("       "),
        std::string("== S =="),
        std::string("== M =="),
        std::string("== L ==")
    };

    static const std::string blue_strings[PieceSizeNums + 1] = {
        std::string("       "),
        std::string("** S **"),
        std::string("** M **"),
        std::string("** L **")
    };

    if (color == PlayerColor::Orange) {
        return orange_strings[static_cast<int>(size)];
    } else {
        return blue_strings[static_cast<int>(size)];
    }
}

const std::string& PositionAsciiCreator::piece_to_string_with_escape(PlayerColor color, PieceSize size) const noexcept {
    static const std::string orange_strings[PieceSizeNums + 1] = {
        std::string("       "),
        std::string("\x1b[38;5;208m== S ==\x1b[0m"),
        std::string("\x1b[38;5;208m== M ==\x1b[0m"),
        std::string("\x1b[38;5;208m== L ==\x1b[0m")
    };

    static const std::string blue_strings[PieceSizeNums + 1] = {
        std::string("       "),
        std::string("\x1b[38;5;32m** S **\x1b[0m"),
        std::string("\x1b[38;5;32m** M **\x1b[0m"),
        std::string("\x1b[38;5;32m** L **\x1b[0m")
    };

    if (color == PlayerColor::Orange) {
        return orange_strings[static_cast<int>(size)];
    } else {
        return blue_strings[static_cast<int>(size)];
    }
}

const std::string& PositionAsciiCreator::piece_symbol_without_escape(PlayerColor color) const noexcept {
    static const std::string strings[PlayerColorNums] = {
        std::string("=="),
        std::string("**")
    };

    if (color == PlayerColor::Orange) {
        return strings[0];
    } else {
        return strings[1];
    }
}

const std::string& PositionAsciiCreator::piece_symbol_with_escape(PlayerColor color) const noexcept {
    static const std::string strings[PlayerColorNums] = {
        std::string("\x1b[38;5;208m==\x1b[0m"),
        std::string("\x1b[38;5;32m**\x1b[0m")
    };

    if (color == PlayerColor::Orange) {
        return strings[0];
    } else {
        return strings[1];
    }
}

} // namespace gobb_analyzer
