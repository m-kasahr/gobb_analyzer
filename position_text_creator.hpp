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

#ifndef GOBB_ANALYZER_POSITION_TEXT_CREATOR_HPP
#define GOBB_ANALYZER_POSITION_TEXT_CREATOR_HPP

#include <string>
#include <vector>
#include "definitions.hpp"
#include "position.hpp"

///
/// @file   position_text_creator.hpp
/// @brief  Define the class `PositionTextCreator` and `PositionAsciiCreator`.
///
namespace gobb_analyzer {

///
/// Create a text showing a position.
///
/// This is an abstract class.
///
class PositionTextCreator {
public:
    ///
    /// Destructor.
    ///
    virtual ~PositionTextCreator();

    ///
    /// Create a text showing the position.
    ///
    /// @param   pos  a position to be drawn as text.
    /// @return  a text showing the position.
    ///
    /// Each string in the vector returned from this function represents a line, but it doesn't contain
    /// a newline character.  It assumes that the strings in the vector will be output from top to bottom.
    ///
    virtual std::vector<std::string> position_to_string(const Position& pos) const = 0;

    ///
    /// Return a text showing the piece.
    ///
    /// @param   color  a color of the piece.
    /// @param   size   size of the piece.
    /// @return  a text showing the piece.
    ///
    /// Note that the string from this function doesn't contain a newline character.
    ///
    virtual const std::string& piece_to_string(PlayerColor color, PieceSize size) const noexcept = 0;

    ///
    /// Return a color symbol of the piece.
    ///
    /// @param   color  a color of the piece.
    /// @return  a symbol of the piece.
    ///
    /// It returns `"=="` for `PlayerColor::Orange`.
    /// It returns `"**"` for `PlayerColor::Blue`.
    ///
    virtual const std::string& piece_symbol(PlayerColor color) const noexcept = 0;
};

///
/// Create an ASCII text showing a position.
///
class PositionAsciiCreator: public PositionTextCreator {
public:
    ///
    /// Constructor.
    ///
    /// @param   enableEscape  whether or not to use escape sequences.
    ///
    /// If `enableEscape` is true, `position_to_string()`, `piece_to_string()` and
    /// `player_symbol()` return a text with ANSI escape sequences for coloring pieces.
    ///
    PositionAsciiCreator(bool enableEscape = false) noexcept;

    ///
    /// Destructor.
    ///
    virtual ~PositionAsciiCreator();

    ///
    /// Copy constructor.
    ///
    /// @param   other  an instance to copy from.
    ///
    /// It constructs an instance with a copy of the contents of `other`.
    ///
    PositionAsciiCreator(const PositionAsciiCreator& other) = default;

    ///
    /// Move constructor.
    ///
    /// @param   other  an instance to move from.
    ///
    /// It constructs an instance with the contents of `other` using move semantics.
    ///
    PositionAsciiCreator(PositionAsciiCreator&& other) = default;

    ///
    /// Copy assignment operator.
    ///
    /// @param   other  an instance to copy from.
    /// @return  a reference of `*this`.
    ///
    /// It replaces the contents with a copy of `other`.
    ///
    PositionAsciiCreator& operator=(const PositionAsciiCreator& other) = default;

    ///
    /// Move assignment operator.
    ///
    /// @param   other  an instance to move from.
    /// @return  a reference of `*this`.
    ///
    /// It replaces the contents with those of `other` using move semantics.
    ///
    PositionAsciiCreator& operator=(PositionAsciiCreator&& other) = default;

    ///
    /// Return a text showing the position.
    ///
    /// @param   pos  a position to be drawn as text.
    /// @return  a text showing the position.
    ///
    /// Each string in the vector returned from this function represents a line, but it doesn't contain
    /// a newline character.  It assumes that the strings in the vector will be output from top to bottom.
    ///
    virtual std::vector<std::string> position_to_string(const Position& pos) const;

    ///
    /// Return a text showing the piece.
    ///
    /// @param   color  a color of the piece.
    /// @param   size   size of the piece.
    /// @return  a text showing the piece.
    ///
    /// It returns `"== S =="`, `"== M =="` or `"== L =="` for pieces of `PlayerColor::Orange`.
    /// It returns `"** S **"`, `"** M **"` or `"** L **"` for pieces of `PlayerColor::Blue`.
    ///
    virtual const std::string& piece_to_string(PlayerColor color, PieceSize size) const noexcept;

    ///
    /// Return a color symbol of the piece.
    ///
    /// @param   color  a color of the piece.
    /// @return  a symbol of the piece.
    ///
    /// It returns `"=="` for `PlayerColor::Orange`.
    /// It returns `"**"` for `PlayerColor::Blue`.
    ///
    virtual const std::string& piece_symbol(PlayerColor color) const noexcept;

private:
    ///
    /// Return a text representing a piece without escape sequences.
    ///
    /// @param   color  a color of the piece.
    /// @param   size   size of the piece.
    /// @return  a colorless text showing the piece.
    ///
    /// It returns `"== S =="`, `"== M =="` or `"== L =="` for pieces of `PlayerColor::Orange`.
    /// It returns `"** S **"`, `"** M **"` or `"** L **"` for pieces of `PlayerColor::Blue`.
    ///
    const std::string& piece_to_string_without_escape(PlayerColor color, PieceSize size) const noexcept;

    ///
    /// Return a text representing a piece with escape sequences.
    ///
    /// @param   color  a color of the piece.
    /// @param   size   size of the piece.
    /// @return  a colored text showing the piece.
    ///
    /// It returns `"== S =="`, `"== M =="` or `"== L =="` for pieces of `PlayerColor::Orange`.
    /// It returns `"** S **"`, `"** M **"` or `"** L **"` for pieces of `PlayerColor::Blue`.
    ///
    const std::string& piece_to_string_with_escape(PlayerColor color, PieceSize size) const noexcept;

    ///
    /// Return a color symbol without escape sequences.
    ///
    /// @param   color  a color of the piece.
    /// @return  a player symbol.
    ///
    /// It returns `"=="` for `PlayerColor::Orange`.
    /// It returns `"**"` for `PlayerColor::Blue`.
    ///
    const std::string& piece_symbol_without_escape(PlayerColor color) const noexcept;

    ///
    /// Return a color symbol with escape sequences.
    ///
    /// @param   color  a color of the piece.
    /// @return  a symbol of the piece.
    ///
    /// It returns `"=="` for `PlayerColor::Orange`.
    /// It returns `"**"` for `PlayerColor::Blue`.
    ///
    const std::string& piece_symbol_with_escape(PlayerColor color) const noexcept;

    /// A pointer to an internal fucntion which processes `piece_to_string()`.
    const std::string& (PositionAsciiCreator::* piece_to_string_)(PlayerColor, PieceSize) const noexcept;

    /// A pointer to an internal fucntion which processes `apiece_symbol()`.
    const std::string& (PositionAsciiCreator::* piece_symbol_)(PlayerColor) const noexcept;
};

} // namespace gobb_analyzer

#endif // GOBB_ANALYZER_POSITION_TEXT_CREATOR_HPP
