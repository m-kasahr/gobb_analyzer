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

#include "position.hpp"

namespace gobb_analyzer {

//
// Class LocationIdPair.
//
bool LocationIdPair::is_valid() const noexcept {
    return (is_valid_locationId(locations[0]) && is_valid_locationId(locations[1]));
}

bool LocationIdPair::update_either(LocationId oldLoc, LocationId newLoc) noexcept {
    if (locations[0] == oldLoc) {
        if (locations[1] <= newLoc) {
            locations[0] = newLoc;
        } else {
            locations[0] = locations[1];
            locations[1] = newLoc;
        }
    } else if (locations[1] == oldLoc) {
        if (locations[0] >= newLoc) {
            locations[1] = newLoc;
        } else {
            locations[1] = locations[0];
            locations[0] = newLoc;
        }
    } else {
        return false;
    }
    return true;
}

LocationIdPair LocationIdPair::transform(TransformerId trans) const noexcept {
    LocationIdPair locPair;
    LocationId loc0 = transform_LocationId(trans, locations[0]);
    LocationId loc1 = transform_LocationId(trans, locations[1]);
    if (loc0 >= loc1) {
        locPair.locations[0] = loc0;
        locPair.locations[1] = loc1;
    } else {
        locPair.locations[0] = loc1;
        locPair.locations[1] = loc0;
    }
    return locPair;
}

//
// Class LocationIdQuad.
//
int Position::LocationIdQuad::index() const noexcept {
    return static_cast<int>(locations[0]) +
        static_cast<int>(locations[1]) * LocationIdNums +
        static_cast<int>(locations[2]) * (LocationIdNums * LocationIdNums) +
        static_cast<int>(locations[3]) * (LocationIdNums * LocationIdNums * LocationIdNums);
}

//
// Class Position.
//
Position::Position() noexcept {
}

Position::Position(PositionId id) noexcept {
    *this = id;
}

Position::Position(PlayerColor activePlayerColor, std::initializer_list<LocationIdPair> init) noexcept {
    if (!is_valid_playerColor(activePlayerColor)) {
        id_ = InvalidPositionId;
        return;
    }
    activePlayerColor_ = activePlayerColor;

    if (init.size() != PieceIdNums) {
        id_ = InvalidPositionId;
        return;
    }
    const LocationIdPair* initPairs = init.begin();
    for (PieceSize size: PieceSizes) {
        if (!initPairs[static_cast<PieceSizeUint>(size)].is_valid()) {
            id_ = InvalidPositionId;
            return;
        }
    }

    for (std::size_t i = 0u; i < PieceIdNums; i += 2) {
        LocationIdQuad quad = {
            initPairs[i].locations[0],
            initPairs[i].locations[1],
            initPairs[i + 1u].locations[0],
            initPairs[i + 1u].locations[1]
        };
        if (locationQuadMaps_[quad.index()] == InvalidPositionId) {
            id_ = InvalidPositionId;
            return;
        }
    }

    piecePairs_[0] = {LocationId::Out, LocationId::Out};
    for (std::size_t i = 0u; i < PieceIdNums; i++) {
        piecePairs_[i + 1u] = initPairs[i];
    }

    update_largestPieces();
    update_id();
}

Position::~Position() {
}

Position& Position::operator=(PositionId id) noexcept {
    if (!is_valid_positionId(id)) {
        id_ = InvalidPositionId;
        return *this;
    }

    PositionId val = id;
    if (id < PieceSetCombinationNums) {
        activePlayerColor_ = PlayerColor::Orange;
    } else {
        activePlayerColor_ = PlayerColor::Blue;
        val -= PieceSetCombinationNums;
    }

    PositionId smallPositionId = val % PieceQuadCombinationNums;
    val /= PieceQuadCombinationNums;
    PositionId mediumPositionId = val % PieceQuadCombinationNums;
    val /= PieceQuadCombinationNums;
    PositionId largePositionId = val;

    const LocationIdQuad& smallQuad = pieceQuadIndexMaps_[smallPositionId];
    piecePairs_[static_cast<PieceIdUint>(PieceId::ActivePlayerSmall)].locations[0] = smallQuad.locations[0];
    piecePairs_[static_cast<PieceIdUint>(PieceId::ActivePlayerSmall)].locations[1] = smallQuad.locations[1];
    piecePairs_[static_cast<PieceIdUint>(PieceId::InactivePlayerSmall)].locations[0] = smallQuad.locations[2];
    piecePairs_[static_cast<PieceIdUint>(PieceId::InactivePlayerSmall)].locations[1] = smallQuad.locations[3];

    const LocationIdQuad& mediumQuad = pieceQuadIndexMaps_[mediumPositionId];
    piecePairs_[static_cast<PieceIdUint>(PieceId::ActivePlayerMedium)].locations[0] = mediumQuad.locations[0];
    piecePairs_[static_cast<PieceIdUint>(PieceId::ActivePlayerMedium)].locations[1] = mediumQuad.locations[1];
    piecePairs_[static_cast<PieceIdUint>(PieceId::InactivePlayerMedium)].locations[0] = mediumQuad.locations[2];
    piecePairs_[static_cast<PieceIdUint>(PieceId::InactivePlayerMedium)].locations[1] = mediumQuad.locations[3];

    const LocationIdQuad& largeQuad = pieceQuadIndexMaps_[largePositionId];
    piecePairs_[static_cast<PieceIdUint>(PieceId::ActivePlayerLarge)].locations[0] = largeQuad.locations[0];
    piecePairs_[static_cast<PieceIdUint>(PieceId::ActivePlayerLarge)].locations[1] = largeQuad.locations[1];
    piecePairs_[static_cast<PieceIdUint>(PieceId::InactivePlayerLarge)].locations[0] = largeQuad.locations[2];
    piecePairs_[static_cast<PieceIdUint>(PieceId::InactivePlayerLarge)].locations[1] = largeQuad.locations[3];

    piecePairs_[static_cast<PieceIdUint>(PieceId::None)] = {LocationId::Out, LocationId::Out};

    id_ = id;
    update_largestPieces();
    return *this;
}

std::vector<PieceId> Position::find_pieces(std::function<bool(PieceId, LocationId)>& filter) const {
    std::vector<PieceId> pieces;

    for (PieceId piece: PieceIds) {
        for (int i = 0; i < 2; i++) {
            if (filter(piece, piecePairs_[static_cast<PieceIdUint>(piece)].locations[i])) {
                pieces.push_back(piece);
            }
        }
    }
    return pieces;
}

bool Position::is_winner(PlayerId player) const noexcept {
    static const LocationId lines[][BoardLength] = {
        {LocationId::NW, LocationId::N,      LocationId::NE},
        {LocationId::W,  LocationId::Center, LocationId::E},
        {LocationId::SW, LocationId::S,      LocationId::SE},
        {LocationId::NW, LocationId::W,      LocationId::SW},
        {LocationId::N,  LocationId::Center, LocationId::S},
        {LocationId::NE, LocationId::E,      LocationId::SE},
        {LocationId::NW, LocationId::Center, LocationId::SE},
        {LocationId::NE, LocationId::Center, LocationId::SW}
    };
    static constexpr int linesNums = sizeof(lines) / sizeof(LocationId[BoardLength]);

    if (!is_valid_positionId(id_)) {
        return false;
    }

    for (int i = 0; i < linesNums; i++) {
        const LocationId* line = lines[i];
        if (playerId_of_pieceId(largestPieces_[static_cast<int>(line[0])]) == player &&
            playerId_of_pieceId(largestPieces_[static_cast<int>(line[1])]) == player &&
            playerId_of_pieceId(largestPieces_[static_cast<int>(line[2])]) == player) {
            return true;
        }
    }
    return false;
}

MoveResult Position::move(PieceId piece, LocationId src, LocationId dst) const noexcept {
    MoveResult result;
    result.status = MoveResultStatus::Invalid;

    // The result status is Invalid if the current position is not valid.
    if (!is_valid_positionId(id_)) {
        return result;
    }

    // The result status is Invalid if an onwer of `piece` is not the active player.
    if (playerId_of_pieceId(piece) != PlayerId::Active) {
        return result;
    }

    // The result status is Invalid if `piece` is not located at `src`.
    LocationIdPair piecePair = piecePairs_[static_cast<int>(piece)];
    if (piecePair.locations[0] != src && piecePair.locations[1] != src) {
        return result;
    }

    // The result status is Invalid if `src` is not a valid location.
    if (!is_valid_locationId(src)) {
        return result;
    }

    // The result status is Invalid if `dst` is not a location on the board.
    if (!is_on_board_locationId(dst)) {
        return result;
    }

    // The result status is Invalid if `src` is a location on the board but `piece` is not a largest piece at there.
    if (src != LocationId::Out && largestPieces_[static_cast<int>(src)] != piece) {
        return result;
    }

    // The result status is Invalid if a piece at `dst` is not smaller than `piece`.
    if (pieceSize_of_pieceId(largestPieces_[static_cast<int>(dst)]) >= pieceSize_of_pieceId(piece)) {
        return result;
    }

    // The result status is Invalid if `src` and `dst` are the same location.
    if (src == dst) {
        return result;
    }

    // The result status is Lost if three pieces of the inactive player are lined up in a row,
    result.position = *this;
    if (src != LocationId::Out) {
        result.position.piecePairs_[static_cast<int>(piece)].update_either(src, LocationId::Out);
        result.position.update_largestPieces();
        if (result.position.is_winner(PlayerId::Inactive)) {
            result.status = MoveResultStatus::Lost;
            return result;
        }
    }

    // Put `piece` on `dst`.
    // The result status is Success.
    result.position.piecePairs_[static_cast<int>(piece)].update_either(LocationId::Out, dst);
    result.position.update_largestPieces();
    result.status = MoveResultStatus::Success;
    result.position.invert_player();
    result.position.update_id();

    return result;
}

MoveResult Position::move_back(PieceId piece, LocationId src, LocationId dst) const noexcept {
    MoveResult result;
    result.status = MoveResultStatus::Invalid;

    // if the current position is not valid, it returns `Invalid`.
    if (!is_valid_positionId(id_)) {
        return result;
    }

    // The result status is Invalid if an onwer of `piece` is not the inactive player.
    if (playerId_of_pieceId(piece) != PlayerId::Inactive) {
        return result;
    }

    // The result status is Invalid if `piece` is not located at `src`.
    LocationIdPair piecePair = piecePairs_[static_cast<int>(piece)];
    if (piecePair.locations[0] != src && piecePair.locations[1] != src) {
        return result;
    }

    // The result status is Invalid if `src` is not a location on the board.
    if (!is_on_board_locationId(src)) {
        return result;
    }

    // The result status is Invalid if `dst` is not a valid location.
    if (!is_valid_locationId(dst)) {
        return result;
    }

    // The result status is Invalid if `piece` is not a largest piece at `src`.
    if (largestPieces_[static_cast<int>(src)] != piece) {
        return result;
    }

    // The result status is Invalid if `dst` is a location on the board but `piece` is not a largest piece at there.
    if (dst != LocationId::Out &&
        pieceSize_of_pieceId(largestPieces_[static_cast<int>(dst)]) >= pieceSize_of_pieceId(piece)) {
        return result;
    }

    // The result status is Invalid if `src` and `dst` are the same location.
    if (src == dst) {
        return result;
    }

    // Pick up `piece`.
    // The result status is Lost if three pieces of the active player are lined up in a row,
    result.position = *this;
    result.position.piecePairs_[static_cast<int>(piece)].update_either(src, LocationId::Out);
    result.position.update_largestPieces();
    if (result.position.is_winner(PlayerId::Active)) {
        result.status = MoveResultStatus::Lost;
        return result;
    }

    // Put `piece` on `dst`.
    // The result status is Lost if three pieces of the active player are lined up in a row,
    result.position.piecePairs_[static_cast<int>(piece)].update_either(LocationId::Out, dst);
    result.position.update_largestPieces();
    result.status = MoveResultStatus::Success;
    result.position.invert_player();
    result.position.update_id();

    return result;
}

Position Position::transform(TransformerId trans) const noexcept {
    if (!is_valid_positionId(id_)) {
        return Position(*this);
    }
    if (!is_valid_TransformerId(trans)) {
        return Position(InvalidPositionId);
    }

    Position pos(*this);
    for (PieceId piece: PieceIds) {
        PieceIdUint index = static_cast<PieceIdUint>(piece);
        pos.piecePairs_[index] = piecePairs_[index].transform(trans);
    }
    pos.update_largestPieces();
    pos.update_id();
    return pos;
}

PositionId Position::minimize_id() const noexcept {
    PositionId minId = id_;

    for (TransformerId trans: EffectiveTransformerIds) {
        Position transPos = transform(trans);
        if (transPos.id_ < minId) {
            minId = transPos.id_;
        }
    }

    if (minId >= PieceSetCombinationNums) {
        minId -= PieceSetCombinationNums;
    }
    return minId;
}

void Position::update_largestPieces() noexcept {
    for (LocationId loc: OnBoardLocationIds) {
        largestPieces_[static_cast<LocationIdUint>(loc)] = PieceId::None;
    }

    for (PieceId piece: PieceIds) {
        LocationId loc0 = piecePairs_[static_cast<PieceIdUint>(piece)].locations[0];
        if (loc0 != LocationId::None) {
            largestPieces_[static_cast<LocationIdUint>(loc0)] = piece;
        }
        LocationId loc1 = piecePairs_[static_cast<PieceIdUint>(piece)].locations[1];
        if (loc1 != LocationId::None) {
            largestPieces_[static_cast<LocationIdUint>(loc1)] = piece;
        }
    }
}

void Position::update_id() noexcept {
    LocationIdQuad smallQuad = {
        piecePairs_[static_cast<PieceIdUint>(PieceId::ActivePlayerSmall)].locations[0],
        piecePairs_[static_cast<PieceIdUint>(PieceId::ActivePlayerSmall)].locations[1],
        piecePairs_[static_cast<PieceIdUint>(PieceId::InactivePlayerSmall)].locations[0],
        piecePairs_[static_cast<PieceIdUint>(PieceId::InactivePlayerSmall)].locations[1]
    };
    PositionId smallId = locationQuadMaps_[smallQuad.index()];

    LocationIdQuad mediumQuad = {
        piecePairs_[static_cast<PieceIdUint>(PieceId::ActivePlayerMedium)].locations[0],
        piecePairs_[static_cast<PieceIdUint>(PieceId::ActivePlayerMedium)].locations[1],
        piecePairs_[static_cast<PieceIdUint>(PieceId::InactivePlayerMedium)].locations[0],
        piecePairs_[static_cast<PieceIdUint>(PieceId::InactivePlayerMedium)].locations[1]
    };
    PositionId mediumId = locationQuadMaps_[mediumQuad.index()];

    LocationIdQuad largeQuad = {
        piecePairs_[static_cast<PieceIdUint>(PieceId::ActivePlayerLarge)].locations[0],
        piecePairs_[static_cast<PieceIdUint>(PieceId::ActivePlayerLarge)].locations[1],
        piecePairs_[static_cast<PieceIdUint>(PieceId::InactivePlayerLarge)].locations[0],
        piecePairs_[static_cast<PieceIdUint>(PieceId::InactivePlayerLarge)].locations[1]
    };
    PositionId largeId = locationQuadMaps_[largeQuad.index()];

    if (activePlayerColor_ == PlayerColor::Orange) {
        id_ = smallId + \
            mediumId * PieceQuadCombinationNums +
            largeId * PieceQuadCombinationNums * PieceQuadCombinationNums;
    } else {
        id_ = smallId + \
            mediumId * PieceQuadCombinationNums +
            largeId * PieceQuadCombinationNums * PieceQuadCombinationNums +
            PieceSetCombinationNums;
    }
}

void Position::invert_player() noexcept {
    activePlayerColor_ = invert_playerColor(activePlayerColor_);

    for (PieceId activePiece: ActivePlayerPieceIds) {
        PieceIdUint activeIndex = static_cast<PieceIdUint>(activePiece);
        PieceIdUint inactiveIndex = static_cast<PieceIdUint>(invert_playerId_of_pieceId(activePiece));
        LocationIdPair tmpPair = piecePairs_[activeIndex];
        piecePairs_[activeIndex] = piecePairs_[inactiveIndex];
        piecePairs_[inactiveIndex] = tmpPair;
    }

    for (LocationId loc: OnBoardLocationIds) {
        LocationIdUint index = static_cast<LocationIdUint>(loc);
        largestPieces_[index] = invert_playerId_of_pieceId(largestPieces_[index]);
    }
}

} // namespace gobb_analyzer
