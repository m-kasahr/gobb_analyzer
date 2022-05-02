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

#ifndef GOBB_ANALYZER_POSITION_HPP
#define GOBB_ANALYZER_POSITION_HPP

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <vector>
#include "definitions.hpp"
#include "transformer.hpp"

///
/// @file   position.hpp
/// @brief  Define the class `Position` and its related types.
///
namespace gobb_analyzer {

///
/// A position ID.
///
/// Any position of Gobblet Gobblers can be represented as an integer of 64bits.
/// We calculate a position ID by the following procedures:
///
/// 1. List locations of the four small pieces in the order {ActivePlayerSmall #1, ActivePlayerSmall #2,
///    InactivePlayerSmall #1, InactivePlayerSmall #2}, where the location ID of #1 must be greater or
///    equal to that of #2.
/// 2. Get an index number S of the four small pieces from the table `Position::locationQuadMaps_` defined
///    in `location_quad_maps.cpp` (The file is created in the build directory by a Python script,)
/// 3. Get an index number M of the four medium pieces, same as the small pieces.
/// 4. Get an index number L of the four large pieces, likewise.
/// 5. Calcuate the value: P = (L * 1423 * 1423) + (M * 1423) + S.
/// 6. If the player having the turn is Orange, the position ID is P.  Otherwise, the position ID is
///    P + PieceSetCombinationNums, where PieceSetCombinationNums is (1423 * 1423 * 1423).
///
using PositionId = std::uint64_t;

/// A position ID of the starting position at a game.
constexpr PositionId InitialPositionId = 0u;

/// A special position ID representing 'invalid'.
constexpr PositionId InvalidPositionId = 0xffff'ffff'ffff'ffffu;

///
/// The number of combinations of locations where four pieces with the same size reside.
///
/// See `piece_quad_index_maps.cpp` in the build directory (generated by a Python script) for a complete list
/// of the combinations.
///
constexpr PositionId PieceQuadCombinationNums = 1423u;

///
/// The number of combinations of locations where all the pieces reside.
///
constexpr PositionId PieceSetCombinationNums =
    PieceQuadCombinationNums * PieceQuadCombinationNums * PieceQuadCombinationNums;

/// The number of positions of Gobblet Gobblers.
constexpr PositionId PositionIdNums = PieceSetCombinationNums * 2;

/// The maximum width of `PositionId` values.
constexpr std::size_t MaxPositionIdWidth = 10u;

///
/// Return true if the given position ID is valid.
///
/// @param   id  a position ID.
/// @return  the validation result.
///
inline bool is_valid_positionId(PositionId id) {
    return (id < PositionIdNums);
}

///
/// A pair of locations.
///
/// It is used for representing the locations of a kind of pieces.
///
struct LocationIdPair {
    LocationId locations[2];  ///< Locations.

    ///
    /// Return true if both locations are valid.
    ///
    /// @return  the validation result.
    ///
    bool is_valid() const noexcept;

    ///
    /// Change one of the locations from `src` to `dst`.
    ///
    /// @param   src  a current location
    /// @param   dst  a new location
    /// @return  true upon success.
    ///
    /// it exchanges `locations[0]` and `locations[1]`, if `locations[0]` is less than `locations[1]` after
    /// the operation.
    /// It returns false if neither location is `src`.
    ///
    bool update_either(LocationId src, LocationId dst) noexcept;

    ///
    /// Transform the locations.
    ///
    /// @param   trans  a transformer.
    /// @return  the resulting locations.
    ///
    /// it exchanges `locations[0]` and `locations[1]`, if `locations[0]` is less than `locations[1]` after
    /// the operation.
    /// Not valid locations are transformed to `LocationId::Invalid`.
    ///
    LocationIdPair transform(TransformerId trans) const noexcept;
};

///
/// A result code of moving a piece.
///
enum class MoveResultStatus {
    Invalid = 255,  ///< Invalid.
    Success = 0,    ///< Success.
    Lost    = 1     ///< The active player loses immediately.  He can pick up his piece on the board, but it
                    ///< causes three pieces of the inactive player are lined up in a row.
};

struct MoveResult;
struct MinimizationResult;

///
/// A position.
///
/// Both the type `PositionId` and the class `Position` represent a position.
/// `PositionId` is an alias of an integral type and it is used for representing an index number of a position,
/// while `Position` is suitable for operating moves of pieces and judging end of the game.
///
class Position {
private:
    ///
    /// Locations of four pieces, used for calculating a position ID.
    ///
    struct LocationIdQuad {
        LocationId locations[4];     ///< four locations.

        int index() const noexcept;  ///< an index number of `locations`.
    };

public:
    ///
    /// Default constructor.
    ///
    /// It constructs an instance without any initialization.
    ///
    Position() noexcept;

    ///
    /// Constructor.
    ///
    /// @param   id  a position ID.
    ///
    /// It constructs an instance with the specified position ID.
    /// If `id` is out of range, it constructs an instance of `PositionId::Invalid`.
    ///
    Position(PositionId id) noexcept;

    ///
    /// Constructor.
    ///
    /// @param   color a color of the player having the current turn.
    /// @param   init  6 pairs of location IDs.
    ///
    /// It constructs `Position` instance out of a color and 6 pairs of location IDs in the following order:
    ///
    /// 1. the locations of `PieceId::ActivePlayerSmall` #1 and #2.
    /// 2. the locations of `PieceId::InactivePlayerSmall` #1 and #2.
    /// 3. the locations of `PieceId::ActivePlayerMedium` #1 and #2.
    /// 4. the locations of `PieceId::InactivePlayerMedium` #1 and #2.
    /// 5. the locations of `PieceId::ActivePlayerLarge` #1 and #2.
    /// 6. the locations of `PieceId::InactivePlayerLarge` #1 and #2.
    ///
    Position(PlayerColor color, std::initializer_list<LocationIdPair> init) noexcept;

    ///
    /// Copy constructor.
    ///
    /// @param   other  an instance to copy from.
    ///
    /// It constructs an instance with a copy of the contents of `other`.
    ///
    Position(const Position& other) noexcept = default;

    ///
    /// Move constructor.
    ///
    /// @param   other  an instance to move from.
    ///
    /// It constructs an instance with the contents of `other` using move semantics.
    ///
    Position(Position&& other) noexcept = default;

    ///
    /// Copy assignment operator.
    ///
    /// @param   id  a position ID.
    /// @return  a reference of `*this`.
    ///
    /// It replaces the contents with the specified position ID.
    ///
    Position& operator=(PositionId id) noexcept;

    ///
    /// Copy assignment operator.
    ///
    /// @param   other  an instance to copy from.
    /// @return  a reference of `*this`.
    ///
    /// It replaces the contents with a copy of `other`.
    ///
    Position& operator=(const Position& other) noexcept = default;

    ///
    /// Move assignment operator.
    ///
    /// @param   other  an instance to move from.
    /// @return  a reference of `*this`.
    ///
    /// It replaces the contents with those of `other` using move semantics.
    ///
    Position& operator=(Position&& other) noexcept = default;

    ///
    /// Destructor.
    ///
    virtual ~Position();

    ///
    /// Returns a position ID.
    ///
    /// @return  a position ID.
    ///
    /// It returns `PositionId::Invalid` If the position is not valid.
    ///
    inline PositionId id() const noexcept {
        return id_;
    }

    ///
    /// Returns a color of the active player.
    ///
    /// @return  the color of the active player.
    ///
    /// It returns `PlayerColor::Invalid` If the position is not valid.
    ///
    inline PlayerColor active_player_color() const noexcept {
        return activePlayerColor_;
    }

    ///
    /// Returns a color of the inactive player.
    ///
    /// @return  the color of the inactive player.
    ///
    /// It returns `PlayerColor::Invalid` If the position is not valid.
    ///
    inline PlayerColor inactive_player_color() const noexcept {
        return invert_playerColor(activePlayerColor_);
    }

    ///
    /// Return true if the position ID is valid.
    ///
    /// @return  the validation result.
    ///
    inline bool is_valid() const noexcept {
        return is_valid_positionId(id_);
    }

    ///
    /// Return locations of the piece.
    ///
    /// @return  locations of the piece.
    ///
    /// Since there are two pieces of each kind, it returns two locations as an instance of `LocationIdPair`.
    /// it returns a pair of `LocationId::Invalid` if `piece` is not valid,.
    ///
    inline const LocationIdPair locations_of_piece(PieceId piece) const noexcept {
        if (is_valid_pieceId(piece)) {
            return piecePairs_[static_cast<int>(piece)];
        } else {
            return {LocationId::Invalid, LocationId::Invalid};
        }
    }

    ///
    /// Return the largest piece at the location on the board.
    ///
    /// @param   loc  a location
    /// @return  the largest piece at the location.
    ///
    /// If there is no piece at the location, it returns `PieceId::None`.
    /// If `loc` is not a location on the board, it returns `PieceId::Invalid`.
    ///
    inline PieceId largetst_piece_at_location(LocationId loc) const noexcept {
        if (is_on_board_locationId(loc)) {
            return largestPieces_[static_cast<int>(loc)];
        } else {
            return PieceId::Invalid;
        }
    }

    ///
    /// Return true if the position ID of `this` is equal to that of `other`.
    ///
    /// @return  the comparison result.
    ///
    inline bool operator==(const Position& other) const noexcept {
        return (id_ == other.id_);
    }

    ///
    /// Return true if the position ID of `this` is not equal to that of `other`.
    ///
    /// @return  the comparison result.
    ///
    inline bool operator!=(const Position& other) const noexcept {
        return (id_ != other.id_);
    }

    ///
    /// Return true if the position ID of `this` is less than that of `other`.
    ///
    /// @return  the comparison result.
    ///
    inline bool operator<(const Position& other) const noexcept {
        return (id_ < other.id_);
    }

    ///
    /// Return true if the position ID of `this` is less than or equal to that of `other`.
    ///
    /// @return  the comparison result.
    ///
    inline bool operator<=(const Position& other) const noexcept {
        return (id_ <= other.id_);
    }

    ///
    /// Return true if the position ID of `this` is greater than that of `other`.
    ///
    /// @return  the comparison result.
    ///
    inline bool operator>(const Position& other) const noexcept {
        return (id_ > other.id_);
    }

    ///
    /// Return true if the position ID of `this` is greater than or equal to that of `other`.
    ///
    /// @return  the comparison result.
    ///
    inline bool operator>=(const Position& other) const {
        return (id_ >= other.id_);
    }

    ///
    /// Return pieces that have passed through the given filter.
    ///
    /// @param   filter  a filter.
    /// @return  pieces.
    ///
    /// For each piece, the function calls `filter`.  The function returns a vector with all pieces that
    /// the filter returns true.
    ///
    std::vector<PieceId> find_pieces(std::function<bool(PieceId piece, LocationId loc)>& filter) const;

    ///
    /// Check if three pieces of `player` are lined up in a row.
    ///
    /// @param   player  a player.
    /// @return  true if three pieces of `player` are lined up in a row.
    ///
    bool is_winner(PlayerId player) const noexcept;

    ///
    /// Move an active player's piece from `src` to `dst`.
    ///
    /// @param   piece  a piece to be moved.
    /// @param   src    a source location of the piece.
    /// @param   dst    a destination of the the piece.
    /// @return  The result of the move.
    ///
    /// The data member `position` of `MoveResult` instance returned from this function is meaningful
    /// only when the data member `status` is `MoveResultStatus::Success`.
    ///
    MoveResult move(PieceId piece, LocationId src, LocationId dst) const noexcept;

    ///
    /// Move an inactive player's piece from `src` to `dst` retrogradely.
    ///
    /// @param   piece  a piece to be moved.
    /// @param   src    a source location of the piece.
    /// @param   dst    a destination of the the piece.
    /// @return  The result of the move.
    ///
    /// The data member `position` of `MoveResult` instance returned from this function is meaningful
    /// only when the data member `status` is `MoveResultStatus::Success`.
    ///
    MoveResult move_back(PieceId piece, LocationId src, LocationId dst) const noexcept;

    ///
    /// Transform the position.
    ///
    /// @param   trans  a transformer ID.
    /// @return  the resulting position.
    ///
    /// If `trans` and/or `this` are not valid, it returns `Position::Invalid`.
    ///
    Position transform(TransformerId trans) const noexcept;

    ///
    /// Return the smallest position ID among the symmetric positions.
    ///
    /// @return  a position ID.
    ///
    /// The function also removes a color information (a color of the player having the turn), so that
    /// it returns an integer between 0 to `PieceSetCombinationNums` - 1.
    /// If `this` is not valid, it returns `Position::Invalid`.
    ///
    PositionId minimize_id() const noexcept;

private:
    ///
    /// Invert owners of all pieces.
    ///
    void invert_player() noexcept;

    ///
    /// Update `largestPieces_`.
    ///
    void update_largestPieces() noexcept;

    ///
    /// Calculate the position ID again.
    ///
    void update_id() noexcept;

    /// The position ID.
    PositionId id_;

    /// The position ID.
    PlayerColor activePlayerColor_;

    ///
    /// Locations of pieces.
    ///
    LocationIdPair piecePairs_[PieceIdNums + 1];

    ///
    /// Largest pieces at the squares.
    ///
    PieceId largestPieces_[LocationIdNums];

    /// A table used for calculating locations of pieces to a position ID.
    static const LocationIdQuad pieceQuadIndexMaps_[PieceQuadCombinationNums];

    /// A table used for calculating locations of pieces to a position ID.
    static const PositionId locationQuadMaps_[LocationIdNums * LocationIdNums * LocationIdNums * LocationIdNums];
};

///
/// A result object of try moving a piece.
///
/// It is returned from `Position::move()` and `Position::move_back()`.
///
struct MoveResult {
    MoveResultStatus status;  ///< the result code of the movement.
    Position position;        ///< the position after the movement.
};

} // namespace gobb_analyzer

#endif // GOBB_ANALYZER_POSITION_HPP
