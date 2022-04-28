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

#ifndef GOBB_ANALYZER_DEFINITIONS_HPP
#define GOBB_ANALYZER_DEFINITIONS_HPP

#include <cstddef>
#include <cstdint>
#include <array>
#include <string>

///
/// @file   definitions.hpp
/// @brief  Define primitive types, variables and functions.
///
namespace gobb_analyzer {

////////////////////////////////////////////////////////////////////////////

/// Integral type for representing `PlayerColor`.
using PlayerColorUint = std::uint8_t;

///
/// Colors of pieces that each player controls.
///
/// The type is used for distinguishing players between Player A and B.
///
enum class PlayerColor: PlayerColorUint {
    Invalid  = 255u,
    Orange   = 0u,
    Blue     = 1u
};

/// The number of colors..
constexpr std::size_t PlayerColorNums = 2u;

/// A set of concrete colors. (i.e. `Orange` and `Blue`).
extern const std::array<PlayerColor, PlayerColorNums> PlayerColors;

///
/// Invert the color.
///
/// @param   color  a color.
/// @return  the resulting color.
///
/// If `color` is `Orange` or `Blue`, it returns its opposite value.
/// Otherwise, it returns `color`.
///
PlayerColor invert_playerColor(PlayerColor color);

///
/// Return a name of the color.
///
/// @param   color  a color.
/// @return  a name of the color.
///
/// It returns "Orange" for `PlayerColor::Orange` for example.
/// It returns "Invalid" if `color` is not valid.
///
const std::string& playerColor_to_string(PlayerColor color);

/// The maximum length of a string returned from `playerColor_to_string()`.
constexpr std::size_t PlayerColorStringMaxLen = 7u;

/// The maximum length of a string returned from `playerColor_to_string()` for a valid value.
constexpr std::size_t ValidPlayerColorStringMaxLen = 6u;

///
/// Return true if the color is valid.
///
/// @param   color  a color.
/// @return  the validation result.
///
/// It returns true if `id` is `Orange` or `Blue`.
///
inline bool is_valid_playerColor(PlayerColor color) {
    return (color <= PlayerColor::Blue);
}

////////////////////////////////////////////////////////////////////////////

/// Integral type for representing `PlayerId`.
using PlayerIdUint = std::uint8_t;

///
/// Players.
///
/// The type doesn't distinguish players between Player A and B.
/// For that purpose, use `PlayerColor` instead.
///
/// `PlayerId` identifies players as Active (the player having the current turn) and Inactive (the opponent),
/// in order to decrease the number of positions to be analyzed.
///
enum class PlayerId: PlayerIdUint {
    Invalid  = 255u,
    None     = 0u,
    Active   = 1u,    ///< A player having the current turn.
    Inactive = 2u     ///< A player not having the current turn.
};

/// The number of players.
constexpr std::size_t PlayerIdNums = 2u;

/// A set of concrete players (i.e. `Active` and `Inactive`).
extern const std::array<PlayerId, PlayerIdNums> PlayerIds;

///
/// Invert a player ID.
///
/// @param   id  a player ID.
/// @return  the resulting player ID.
///
/// If `id` is `Active` or `Inactive`, it returns its opposite value.
/// Otherwise, it returns `id`.
///
PlayerId invert_playerId(PlayerId id);

///
/// Return a name of the player ID.
///
/// @param   id  a player ID.
/// @return  a name of the player ID.
///
/// It returns "Active" for `PlayerId::Active` for example.
/// It returns "Invalid" if `id` is not valid.
///
const std::string& playerId_to_string(PlayerId id);

/// The maximum length of a string returned from `playerId_to_string()`.
constexpr std::size_t PlayerIdStringMaxLen = 8u;

/// The maximum length of a string returned from `playerId_to_string()` for a valid value.
constexpr std::size_t ValidPlayerIdStringMaxLen = 8u;

///
/// Return true if the player ID is valid.
///
/// @param   id  a player ID.
/// @return  the validation result.
///
/// It returns true if `id` is `None`, `Active` or `Inactive`.
///
inline bool is_valid_playerId(PlayerId id) {
    return (id <= PlayerId::Inactive);
}

////////////////////////////////////////////////////////////////////////////

/// Integral type for representing `PieceSize`.
using PieceSizeUint = std::uint8_t;

///
/// Sizes of pieces.
///
enum class PieceSize: PieceSizeUint {
    Invalid = 255u,
    None    = 0u,
    Small   = 1u,
    Medium  = 2u,
    Large   = 3u
};

/// The number of sizes of concrete pieces.
constexpr std::size_t PieceSizeNums = 3u;

/// A set of sizes of concrete pieces.
/// (i.e. `Small`, `Medium` and `Large`)
extern const std::array<PieceSize, PieceSizeNums> PieceSizes;

///
/// Return a name of the piece size.
///
/// @param   size  size of the piece.
/// @return  a name of the piece size.
///
/// It returns "Small" for `PieceId::Small` for example.
/// It returns "Invalid" if `id` is not valid.
///
const std::string& pieceSize_to_string(PieceSize size);

/// The maximum length of a string returned from `pieceSize_to_string()`.
constexpr std::size_t PieceSizeStringMaxLen = 7u;

/// The maximum length of a string returned from `pieceSize_to_string()` for a valid ID.
constexpr std::size_t ValidPieceSizeStringMaxLen = 6u;

///
/// Return true if the piece size is valid.
///
/// @param   size  a piece size.
/// @return  the validation result.
///
/// It returns true if `id` is `None`, `Small`, `Medium` or `Large`.
///
inline bool is_valid_pieceSize(PieceSize size) {
    return (size <= PieceSize::Large);
}

////////////////////////////////////////////////////////////////////////////

/// Integral type for representing `PieceId`.
using PieceIdUint = std::uint8_t;

///
/// Kinds of pieces.
///
/// To decrease the positions to be analyzed, we represent owners of pieces as `ActivePlayer` (the player
/// having the current turn) and `InactivePlayer` (the opponent), instead of Player A and B.
/// Each time the active player has moved his piece, we have to invert owners of all pieces.
///
enum class PieceId: PieceIdUint {
    Invalid              = 255u,
    None                 = 0u,
    ActivePlayerSmall    = 1u,  ///< A small piece owned the active player.
    InactivePlayerSmall  = 2u,  ///< A medium piece owned the active player.
    ActivePlayerMedium   = 3u,  ///< A large piece owned the active player.
    InactivePlayerMedium = 4u,  ///< A small piece owned the inactive player.
    ActivePlayerLarge    = 5u,  ///< A medium piece owned the inactive player.
    InactivePlayerLarge  = 6u   ///< A large piece owned the in active player.
};

/// The number of kinds of concrete pieces.
constexpr std::size_t PieceIdNums = 6u;

/// The number of kinds of pieces for each player.
constexpr std::size_t PlayerPieceIdNums = 3u;

/// The number of concrete pieces used in a game.
constexpr std::size_t PieceSetNums = 12u;

/// A set of kinds of concrete pieces.
/// (i.e. `ActivePlayerSmall`, `InactivePlayerSmall`, `ActivePlayerMedium`, `InactivePlayerMedium`,
/// `ActivePlayerLarge` and `InactivePlayerLarge`)
extern const std::array<PieceId, PieceIdNums> PieceIds;

/// A set of kinds of concrete pieces for the active player.
/// (i.e. `ActivePlayerSmall`, `ActivePlayerMedium` and `ActivePlayerLarge`)
extern const std::array<PieceId, PlayerPieceIdNums> ActivePlayerPieceIds;

/// A set of kinds of concrete pieces for the inactive player.
/// (i.e. `InactivePlayerSmall`, `InactivePlayerMedium` and `InactivePlayerLarge`)
extern const std::array<PieceId, PlayerPieceIdNums> InactivePlayerPieceIds;

/// A set of concrete pieces used in a game.
/// (i.e. two each of ActivePlayerSmall`, `InactivePlayerSmall`, `ActivePlayerMedium`, `InactivePlayerMedium`,
/// `ActivePlayerLarge` and `InactivePlayerLarge`)
extern const std::array<PieceId, PieceSetNums> PieceSet;

///
/// Return a player ID of the piece.
///
/// @param   id  a piece ID.
/// @return  a player ID.
///
/// It returns `PlayerId::Invalid` if `id` is not valid ID.
///
PlayerId playerId_of_pieceId(PieceId id);

///
/// Return a piece size of the piece.
///
/// @param   id  a piece.
/// @return  size of the piece.
///
/// It returns `PieceSize::Invalid` if `id` is not valid ID.
///
PieceSize pieceSize_of_pieceId(PieceId id);

///
/// Construct a piece ID from a player ID and a piece size.
///
/// @param   player  a player.
/// @param   size    a piece size.
/// @return  the constructed piece.
///
/// It returns `PieceId::Invalid` if `player and/or `size` are not valid.
///
PieceId to_pieceId(PlayerId player, PieceSize size);

///
/// Invert an owner of the piece.
///
/// @param   id  a piece.
/// @return  the resulting piece.
///
/// If `id` is a piece for the active player, it returns a corresponding piece for the inactive player.
/// For example, It returns `InactivePlayerSmall` for `ActivePlayerSmall`.
/// If `id` is a piece for the inactive player, it returns a corresponding piece for the active player.
/// For example, It returns `ActivePlayerSmall` for `InactivePlayerSmall`.
/// Otherwise, it returns `id`.
///
PieceId invert_playerId_of_pieceId(PieceId id);

///
/// Return a name of the piece ID.
///
/// @param   id  a piece.
/// @return  a name of the piece ID.
///
/// It returns "ActivePlayerSmall" for `PieceId::ActivePlayerSmall` for example.
/// It returns "Invalid" if `id` is not valid ID.
///
const std::string& pieceId_to_string(PieceId id);

/// The maximum length of a string returned from `piece_to_string()`.
constexpr std::size_t PieceIdStringMaxLen = 20u;

/// The maximum length of a string returned from `piece_to_string()` for a valid value.
constexpr std::size_t ValidPieceIdStringMaxLen = 20u;

///
/// Return true if the piece ID is valid.
///
/// @param   id  a piece.
/// @return  the validation result.
///
/// It returns true if `id` is `None`, `Small`, `Medium` or `Large`.
///
inline bool is_valid_pieceId(PieceId id) {
    return (id <= PieceId::InactivePlayerLarge);
}

////////////////////////////////////////////////////////////////////////////

/// Integral type for representing `LocationId`.
using LocationIdUint = std::uint8_t;

///
/// Locations of a piece.
///
enum class LocationId: LocationIdUint {
    Invalid = 255u,
    Out     = 0u,    ///< Out of the board.
    None    = 0u,    ///< An alias of `Out`.
    NW      = 1u,    ///< Northwest.
    N       = 2u,    ///< North.
    NE      = 3u,    ///< Northeast.
    W       = 4u,    ///< West.
    Center  = 5u,    ///< Center.
    E       = 6u,    ///< East.
    SW      = 7u,    ///< Southwest.
    S       = 8u,    ///< South.
    SE      = 9u     ///< Southeast.
};

/// The number of locations of a piece.
constexpr std::size_t LocationIdNums = 10u;

/// The number of locations on the board.
constexpr std::size_t OnBoardLocationIdNums = 9u;

/// A set of concrete locations.
/// (i.e. `Out`, `NW`, `N`, `NE`, `W`, `Center`, `E`, `SW`, `S`, and `SE`).
extern const std::array<LocationId, LocationIdNums> LocationIds;

/// A set of locations on the board.
/// (i.e. `NW`, `N`, `NE`, `W`, `Center`, `E`, `SW`, `S`, and `SE`).
extern const std::array<LocationId, OnBoardLocationIdNums> OnBoardLocationIds;

/// The number of rows or columns of the board.
constexpr std::size_t BoardLength = 3u;

///
/// Return a name of the location ID.
///
/// @param   id  a location ID.
/// @return  a name of the location ID.
///
/// It returns "Center" for `LocationId::Center` for example.
/// It returns "Invalid" if `id` is not valid ID.
///
const std::string& locationId_to_string(LocationId id);

/// The maximum length of a string returned from `locationId_to_string()`.
constexpr std::size_t LocationIdStringMaxLen = 7u;

/// The maximum length of a string returned from `locationId_to_string()` for a valid value.
constexpr std::size_t ValidLocationIdStringMaxLen = 6u;

///
/// Return true if the location ID is valid.
///
/// @param   id  a location ID.
/// @return  the validation result.
///
/// It returns true if `id` is `Out`, `NW`, `N', `NE`, `W`, `Center`,`SW`, `S` or `SE`.
///
inline bool is_valid_locationId(LocationId id) {
    return (id <= LocationId::SE);
}

///
/// Return true if the location ID refers to a square on the board.
///
/// @param   id  a location ID.
/// @return  the check result.
///
/// It returns true if `id` is `NW`, `N', `NE`, `W`, `Center`,`SW`, `S` or `SE`.
///
inline bool is_on_board_locationId(LocationId id) {
    return (id >= LocationId::NW && id <= LocationId::SE);
}

} // namespace gobb_analyzer

#endif // GOBB_ANALYZER_DEFINITIONS_HPP
