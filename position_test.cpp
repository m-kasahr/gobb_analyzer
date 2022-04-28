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
#include "gtest/gtest.h"

using namespace gobb_analyzer;

//
// Test Position::Position(PositionId id).
//
TEST(PositionTest, ConstructorWithPositionId) {
    // Puts small pieces only on the board.
    for (PositionId id = InitialPositionId; id < PieceQuadCombinationNums; id++) {
        Position pos0(id);
        Position pos1 = pos0.transform(TransformerId::Unchange);  // Calculates ID again.
        ASSERT_EQ(id, pos1.id());
    }

    // Puts small pieces only on the board.
    for (PositionId i = InitialPositionId; i < PieceQuadCombinationNums; i++) {
        PositionId id = i * PieceQuadCombinationNums;
        Position pos0(id);
        Position pos1 = pos0.transform(TransformerId::Unchange);
        ASSERT_EQ(id, pos1.id());
    }

    // Puts large pieces only on the board.
    for (PositionId i = InitialPositionId; i < PieceQuadCombinationNums; i++) {
        PositionId id = i * PieceQuadCombinationNums * PieceQuadCombinationNums;
        Position pos0(id);
        Position pos1 = pos0.transform(TransformerId::Unchange);
        ASSERT_EQ(id, pos1.id());
    }

    // Puts pieces of various sizes on the board.
    for (PositionId i0 = InitialPositionId; i0 < PieceQuadCombinationNums; i0++) {
        PositionId i1 = (i0 + 1) % PieceQuadCombinationNums;
        PositionId i2 = (i0 + 2) % PieceQuadCombinationNums;
        PositionId id = i0 + (i1 * PieceQuadCombinationNums) +
            (i2 * PieceQuadCombinationNums * PieceQuadCombinationNums);
        Position pos0(id);
        Position pos1 = pos0.transform(TransformerId::Unchange);  // Calculates ID again.
        ASSERT_EQ(id, pos1.id());
    }

    // Invalid position Id.
    PositionId id = InvalidPositionId;
    Position pos0(id);
    Position pos1 = pos0.transform(TransformerId::Unchange);  // Calculates ID again.
    ASSERT_EQ(id, pos1.id());
}

//
// Test Position::is_winner(PlayerId player).
//
TEST(PositionTest, IsWinner) {
    Position pos;

    // line NW - N - NE
    pos = Position(PlayerColor::Orange,
        {{LocationId::NW,     LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::N,      LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::NE,     LocationId::Out}, {LocationId::Out, LocationId::Out}});
    ASSERT_TRUE(pos.is_winner(PlayerId::Active));
    ASSERT_FALSE(pos.is_winner(PlayerId::Inactive));

    pos = Position(PlayerColor::Orange,
        {{LocationId::Out,    LocationId::Out}, {LocationId::NW, LocationId::Out},
         {LocationId::Out,    LocationId::Out}, {LocationId::N,  LocationId::Out},
         {LocationId::Out,    LocationId::Out}, {LocationId::NE, LocationId::Out}});
    ASSERT_FALSE(pos.is_winner(PlayerId::Active));
    ASSERT_TRUE(pos.is_winner(PlayerId::Inactive));

    // line W - Center - E
    pos = Position(PlayerColor::Orange,
        {{LocationId::W,      LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Center, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::E,      LocationId::Out}, {LocationId::Out, LocationId::Out}});
    ASSERT_TRUE(pos.is_winner(PlayerId::Active));
    ASSERT_FALSE(pos.is_winner(PlayerId::Inactive));

    pos = Position(PlayerColor::Orange,
        {{LocationId::Out, LocationId::Out}, {LocationId::W,      LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Center, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::E,      LocationId::Out}});
    ASSERT_FALSE(pos.is_winner(PlayerId::Active));
    ASSERT_TRUE(pos.is_winner(PlayerId::Inactive));

    // line SW - S - SE
    pos = Position(PlayerColor::Orange,
        {{LocationId::SW,     LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::S,      LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::SE,     LocationId::Out}, {LocationId::Out, LocationId::Out}});
    ASSERT_TRUE(pos.is_winner(PlayerId::Active));
    ASSERT_FALSE(pos.is_winner(PlayerId::Inactive));

    pos = Position(PlayerColor::Orange,
        {{LocationId::Out,    LocationId::Out}, {LocationId::SW, LocationId::Out},
         {LocationId::Out,    LocationId::Out}, {LocationId::S,  LocationId::Out},
         {LocationId::Out,    LocationId::Out}, {LocationId::SE, LocationId::Out}});
    ASSERT_FALSE(pos.is_winner(PlayerId::Active));
    ASSERT_TRUE(pos.is_winner(PlayerId::Inactive));

    // line NW - W - SW
    pos = Position(PlayerColor::Orange,
        {{LocationId::NW,     LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::W,      LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::SW,     LocationId::Out}, {LocationId::Out, LocationId::Out}});
    ASSERT_TRUE(pos.is_winner(PlayerId::Active));
    ASSERT_FALSE(pos.is_winner(PlayerId::Inactive));

    pos = Position(PlayerColor::Orange,
        {{LocationId::Out,    LocationId::Out}, {LocationId::NW, LocationId::Out},
         {LocationId::Out,    LocationId::Out}, {LocationId::W,  LocationId::Out},
         {LocationId::Out,    LocationId::Out}, {LocationId::SW, LocationId::Out}});
    ASSERT_FALSE(pos.is_winner(PlayerId::Active));
    ASSERT_TRUE(pos.is_winner(PlayerId::Inactive));

    // line N - Center - S
    pos = Position(PlayerColor::Orange,
        {{LocationId::N,      LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Center, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::S,      LocationId::Out}, {LocationId::Out, LocationId::Out}});
    ASSERT_TRUE(pos.is_winner(PlayerId::Active));
    ASSERT_FALSE(pos.is_winner(PlayerId::Inactive));

    pos = Position(PlayerColor::Orange,
        {{LocationId::Out, LocationId::Out}, {LocationId::N,      LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Center, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::S,      LocationId::Out}});
    ASSERT_FALSE(pos.is_winner(PlayerId::Active));
    ASSERT_TRUE(pos.is_winner(PlayerId::Inactive));

    // line NE - E - SE
    pos = Position(PlayerColor::Orange,
        {{LocationId::NE,     LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::E,      LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::SE,     LocationId::Out}, {LocationId::Out, LocationId::Out}});
    ASSERT_TRUE(pos.is_winner(PlayerId::Active));
    ASSERT_FALSE(pos.is_winner(PlayerId::Inactive));

    pos = Position(PlayerColor::Orange,
        {{LocationId::Out,    LocationId::Out}, {LocationId::NE, LocationId::Out},
         {LocationId::Out,    LocationId::Out}, {LocationId::E,  LocationId::Out},
         {LocationId::Out,    LocationId::Out}, {LocationId::SE, LocationId::Out}});
    ASSERT_FALSE(pos.is_winner(PlayerId::Active));
    ASSERT_TRUE(pos.is_winner(PlayerId::Inactive));

    // line NW - Center - SE
    pos = Position(PlayerColor::Orange,
        {{LocationId::NW,     LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Center, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::SE,     LocationId::Out}, {LocationId::Out, LocationId::Out}});
    ASSERT_TRUE(pos.is_winner(PlayerId::Active));
    ASSERT_FALSE(pos.is_winner(PlayerId::Inactive));

    pos = Position(PlayerColor::Orange,
        {{LocationId::Out, LocationId::Out}, {LocationId::NW,     LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Center, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::SE,     LocationId::Out}});
    ASSERT_FALSE(pos.is_winner(PlayerId::Active));
    ASSERT_TRUE(pos.is_winner(PlayerId::Inactive));

    // line NE - Center - SW
    pos = Position(PlayerColor::Orange,
        {{LocationId::NE,     LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Center, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::SW,     LocationId::Out}, {LocationId::Out, LocationId::Out}});
    ASSERT_TRUE(pos.is_winner(PlayerId::Active));
    ASSERT_FALSE(pos.is_winner(PlayerId::Inactive));

    pos = Position(PlayerColor::Orange,
        {{LocationId::Out, LocationId::Out}, {LocationId::NE,     LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Center, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::SW,     LocationId::Out}});
    ASSERT_FALSE(pos.is_winner(PlayerId::Active));
    ASSERT_TRUE(pos.is_winner(PlayerId::Inactive));
}

//
// Test Position::transform(TransformerId trans).
//
TEST(PositionTest, Transform) {
    Position posCenter(PlayerColor::Orange,
        {{LocationId::Center, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Center, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Center, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    Position posNW(PlayerColor::Orange,
        {{LocationId::NW, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::NW, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::NW, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    Position posN(PlayerColor::Orange,
        {{LocationId::N, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::N, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::N, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    Position posNE(PlayerColor::Orange,
        {{LocationId::NE, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::NE, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::NE, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    Position posW(PlayerColor::Orange,
        {{LocationId::W, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::W, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::W, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    Position posE(PlayerColor::Orange,
        {{LocationId::E, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::E, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::E, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    Position posSW(PlayerColor::Orange,
        {{LocationId::SW, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::SW, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::SW, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    Position posS(PlayerColor::Orange,
        {{LocationId::S, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::S, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::S, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    Position posSE(PlayerColor::Orange,
        {{LocationId::SE, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::SE, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::SE, LocationId::Out}, {LocationId::Out, LocationId::Out}});
    Position posInvalid = InvalidPositionId;

    // TransformerId::Uncahge
    ASSERT_EQ(posCenter.transform(TransformerId::Unchange), posCenter);
    ASSERT_EQ(posNW.transform(TransformerId::Unchange), posNW);
    ASSERT_EQ(posN.transform(TransformerId::Unchange), posN);
    ASSERT_EQ(posNE.transform(TransformerId::Unchange), posNE);
    ASSERT_EQ(posW.transform(TransformerId::Unchange), posW);
    ASSERT_EQ(posE.transform(TransformerId::Unchange), posE);
    ASSERT_EQ(posSW.transform(TransformerId::Unchange), posSW);
    ASSERT_EQ(posS.transform(TransformerId::Unchange), posS);
    ASSERT_EQ(posSE.transform(TransformerId::Unchange), posSE);
    ASSERT_EQ(posInvalid.transform(TransformerId::Unchange), posInvalid);

    // TransformerId::Rotate90
    ASSERT_EQ(posCenter.transform(TransformerId::Rotate90), posCenter);
    ASSERT_EQ(posNW.transform(TransformerId::Rotate90), posNE);
    ASSERT_EQ(posN.transform(TransformerId::Rotate90), posE);
    ASSERT_EQ(posNE.transform(TransformerId::Rotate90), posSE);
    ASSERT_EQ(posW.transform(TransformerId::Rotate90), posN);
    ASSERT_EQ(posE.transform(TransformerId::Rotate90), posS);
    ASSERT_EQ(posSW.transform(TransformerId::Rotate90), posNW);
    ASSERT_EQ(posS.transform(TransformerId::Rotate90), posW);
    ASSERT_EQ(posSE.transform(TransformerId::Rotate90), posSW);
    ASSERT_EQ(posInvalid.transform(TransformerId::Rotate90), posInvalid);

    // TransformerId::Rotate180
    ASSERT_EQ(posCenter.transform(TransformerId::Rotate180), posCenter);
    ASSERT_EQ(posNW.transform(TransformerId::Rotate180), posSE);
    ASSERT_EQ(posN.transform(TransformerId::Rotate180), posS);
    ASSERT_EQ(posNE.transform(TransformerId::Rotate180), posSW);
    ASSERT_EQ(posW.transform(TransformerId::Rotate180), posE);
    ASSERT_EQ(posE.transform(TransformerId::Rotate180), posW);
    ASSERT_EQ(posSW.transform(TransformerId::Rotate180), posNE);
    ASSERT_EQ(posS.transform(TransformerId::Rotate180), posN);
    ASSERT_EQ(posSE.transform(TransformerId::Rotate180), posNW);
    ASSERT_EQ(posInvalid.transform(TransformerId::Rotate180), posInvalid);

    // TransformerId::Rotate270
    ASSERT_EQ(posCenter.transform(TransformerId::Rotate270), posCenter);
    ASSERT_EQ(posNW.transform(TransformerId::Rotate270), posSW);
    ASSERT_EQ(posN.transform(TransformerId::Rotate270), posW);
    ASSERT_EQ(posNE.transform(TransformerId::Rotate270), posNW);
    ASSERT_EQ(posW.transform(TransformerId::Rotate270), posS);
    ASSERT_EQ(posE.transform(TransformerId::Rotate270), posN);
    ASSERT_EQ(posSW.transform(TransformerId::Rotate270), posSE);
    ASSERT_EQ(posS.transform(TransformerId::Rotate270), posE);
    ASSERT_EQ(posSE.transform(TransformerId::Rotate270), posNE);
    ASSERT_EQ(posInvalid.transform(TransformerId::Unchange), posInvalid);
    ASSERT_EQ(posInvalid.transform(TransformerId::Rotate270), posInvalid);

    // TransformerId::Mirror
    ASSERT_EQ(posCenter.transform(TransformerId::Mirror), posCenter);
    ASSERT_EQ(posNW.transform(TransformerId::Mirror), posNE);
    ASSERT_EQ(posN.transform(TransformerId::Mirror), posN);
    ASSERT_EQ(posNE.transform(TransformerId::Mirror), posNW);
    ASSERT_EQ(posW.transform(TransformerId::Mirror), posE);
    ASSERT_EQ(posE.transform(TransformerId::Mirror), posW);
    ASSERT_EQ(posSW.transform(TransformerId::Mirror), posSE);
    ASSERT_EQ(posS.transform(TransformerId::Mirror), posS);
    ASSERT_EQ(posSE.transform(TransformerId::Mirror), posSW);
    ASSERT_EQ(posInvalid.transform(TransformerId::Mirror), posInvalid);

    // TransformerId::MirrorRotate90
    ASSERT_EQ(posCenter.transform(TransformerId::MirrorRotate90), posCenter);
    ASSERT_EQ(posNW.transform(TransformerId::MirrorRotate90), posSE);
    ASSERT_EQ(posN.transform(TransformerId::MirrorRotate90), posE);
    ASSERT_EQ(posNE.transform(TransformerId::MirrorRotate90), posNE);
    ASSERT_EQ(posW.transform(TransformerId::MirrorRotate90), posS);
    ASSERT_EQ(posE.transform(TransformerId::MirrorRotate90), posN);
    ASSERT_EQ(posSW.transform(TransformerId::MirrorRotate90), posSW);
    ASSERT_EQ(posS.transform(TransformerId::MirrorRotate90), posW);
    ASSERT_EQ(posSE.transform(TransformerId::MirrorRotate90), posNW);
    ASSERT_EQ(posInvalid.transform(TransformerId::MirrorRotate90), posInvalid);

    // TransformerId::MirrorRotate180
    ASSERT_EQ(posCenter.transform(TransformerId::MirrorRotate180), posCenter);
    ASSERT_EQ(posNW.transform(TransformerId::MirrorRotate180), posSW);
    ASSERT_EQ(posN.transform(TransformerId::MirrorRotate180), posS);
    ASSERT_EQ(posNE.transform(TransformerId::MirrorRotate180), posSE);
    ASSERT_EQ(posW.transform(TransformerId::MirrorRotate180), posW);
    ASSERT_EQ(posE.transform(TransformerId::MirrorRotate180), posE);
    ASSERT_EQ(posSW.transform(TransformerId::MirrorRotate180), posNW);
    ASSERT_EQ(posS.transform(TransformerId::MirrorRotate180), posN);
    ASSERT_EQ(posSE.transform(TransformerId::MirrorRotate180), posNE);
    ASSERT_EQ(posInvalid.transform(TransformerId::MirrorRotate180), posInvalid);

    // TransformerId::MirrorRotate270
    ASSERT_EQ(posCenter.transform(TransformerId::MirrorRotate270), posCenter);
    ASSERT_EQ(posNW.transform(TransformerId::MirrorRotate270), posNW);
    ASSERT_EQ(posN.transform(TransformerId::MirrorRotate270), posW);
    ASSERT_EQ(posNE.transform(TransformerId::MirrorRotate270), posSW);
    ASSERT_EQ(posW.transform(TransformerId::MirrorRotate270), posN);
    ASSERT_EQ(posE.transform(TransformerId::MirrorRotate270), posS);
    ASSERT_EQ(posSW.transform(TransformerId::MirrorRotate270), posNE);
    ASSERT_EQ(posS.transform(TransformerId::MirrorRotate270), posE);
    ASSERT_EQ(posSE.transform(TransformerId::MirrorRotate270), posSE);
    ASSERT_EQ(posInvalid.transform(TransformerId::MirrorRotate270), posInvalid);
}

//
// Test Position::move(PieceId piece, LocationId src, LocationId dst).
// in case of MoveResult::Success.
//
TEST(PositionTest, MoveSuccess) {
    Position oldPos, newPos;
    MoveResult result;

    // PiecePair: {Out, Out} -> {A, Out}.
    oldPos = Position(PlayerColor::Orange,
        {{LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    newPos = Position(PlayerColor::Blue,
        {{LocationId::Out, LocationId::Out}, {LocationId::W,   LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});
    result = oldPos.move(PieceId::ActivePlayerSmall, LocationId::Out, LocationId::W);
    ASSERT_EQ(result.status, MoveResultStatus::Success);
    ASSERT_EQ(result.position, newPos);

    // PiecePair: {A, Out} -> {A, B}.
    oldPos = Position(PlayerColor::Orange,
        {{LocationId::W,   LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    newPos = Position(PlayerColor::Blue,
        {{LocationId::Out, LocationId::Out}, {LocationId::W,   LocationId::N},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});
    result = oldPos.move(PieceId::ActivePlayerSmall, LocationId::Out, LocationId::N);
    ASSERT_EQ(result.status, MoveResultStatus::Success);
    ASSERT_EQ(result.position, newPos);

    // PiecePair: {A, Out} -> {B, A}.
    oldPos = Position(PlayerColor::Orange,
        {{LocationId::W,   LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    newPos = Position(PlayerColor::Blue,
        {{LocationId::Out, LocationId::Out}, {LocationId::E,   LocationId::W  },
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});
    result = oldPos.move(PieceId::ActivePlayerSmall, LocationId::Out, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Success);
    ASSERT_EQ(result.position, newPos);

    // PiecePair: {A, Out} -> {B, Out}.
    oldPos = Position(PlayerColor::Orange,
        {{LocationId::W,   LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    newPos = Position(PlayerColor::Blue,
        {{LocationId::Out, LocationId::Out}, {LocationId::E,   LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});
    result = oldPos.move(PieceId::ActivePlayerSmall, LocationId::W, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Success);
    ASSERT_EQ(result.position, newPos);

    // PiecePair: {A, B} -> {A, C}.
    oldPos = Position(PlayerColor::Orange,
        {{LocationId::E,   LocationId::W  }, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    newPos = Position(PlayerColor::Blue,
        {{LocationId::Out, LocationId::Out}, {LocationId::E,   LocationId::N  },
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});
    result = oldPos.move(PieceId::ActivePlayerSmall, LocationId::W, LocationId::N);
    ASSERT_EQ(result.status, MoveResultStatus::Success);
    ASSERT_EQ(result.position, newPos);

    // PiecePair: {A, B} -> {B, C}.
    oldPos = Position(PlayerColor::Orange,
        {{LocationId::E,   LocationId::W  }, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    newPos = Position(PlayerColor::Blue,
        {{LocationId::Out, LocationId::Out}, {LocationId::W,   LocationId::N  },
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});
    result = oldPos.move(PieceId::ActivePlayerSmall, LocationId::E, LocationId::N);
    ASSERT_EQ(result.status, MoveResultStatus::Success);
    ASSERT_EQ(result.position, newPos);
}

//
// Test Position::move(PieceId piece, LocationId src, LocationId dst).
// in case of MoveResult::Lost.
//
TEST(PositionTest, MoveLost) {
    Position oldPos, newPos;
    MoveResult result;

    oldPos = Position(PlayerColor::Orange,
        {{LocationId::Out, LocationId::Out}, {LocationId::NW, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::N,  LocationId::Out},
         {LocationId::NW,  LocationId::Out}, {LocationId::NE, LocationId::Out}});
    result = oldPos.move(PieceId::ActivePlayerLarge, LocationId::NW, LocationId::Center);
    ASSERT_EQ(result.status, MoveResultStatus::Lost);
}

//
// Test Position::move(PieceId piece, LocationId src, LocationId dst).
// in case of MoveResult::Invalid.
//
TEST(PositionTest, MoveInvalid) {
    Position pos;
    MoveResult result;

    // The position is invalid.
    pos = InvalidPositionId;
    result = pos.move(PieceId::InactivePlayerSmall, LocationId::Center, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // `piece` is a piece of the inactive player.
    pos = Position(PlayerColor::Orange,
        {{LocationId::Out, LocationId::Out}, {LocationId::W, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::W, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::W, LocationId::Out}});
    result = pos.move(PieceId::InactivePlayerSmall, LocationId::W, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // `piece` is an invalid piece.
    pos = Position(PlayerColor::Orange,
        {{LocationId::W, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::W, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::W, LocationId::Out}, {LocationId::Out, LocationId::Out}});
    result = pos.move(PieceId::Invalid, LocationId::W, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // `piece` is not located at `src`.
    result = pos.move(PieceId::ActivePlayerLarge, LocationId::N, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // `src` is an invalid location.
    result = pos.move(PieceId::ActivePlayerLarge, LocationId::Invalid, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // `dst` is `Out`.
    result = pos.move(PieceId::ActivePlayerLarge, LocationId::W, LocationId::Out);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // `dst` is an invalid location.
    result = pos.move(PieceId::ActivePlayerLarge, LocationId::W, LocationId::Invalid);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // The `piece` is not a largest piece at `src`.
    result = pos.move(PieceId::ActivePlayerSmall, LocationId::W, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    result = pos.move(PieceId::ActivePlayerMedium, LocationId::W, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // `piece` will not be a largest piece at `dst`.
    pos = Position(PlayerColor::Orange,
        {{LocationId::NW, LocationId::SW}, {LocationId::Out, LocationId::Out},
         {LocationId::N,  LocationId::S},  {LocationId::Out, LocationId::Out},
         {LocationId::NE, LocationId::SE}, {LocationId::Out, LocationId::Out}});
    result = pos.move(PieceId::ActivePlayerSmall, LocationId::NW, LocationId::SW);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    result = pos.move(PieceId::ActivePlayerMedium, LocationId::N, LocationId::S);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    result = pos.move(PieceId::ActivePlayerLarge,  LocationId::NE, LocationId::SE);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // `src` and `dst` are the same.
    result = pos.move(PieceId::ActivePlayerSmall, LocationId::NW, LocationId::NW);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);
}

//
// Test Position::move_back(PieceId piece, LocationId src, LocationId dst).
// in case of MoveResult::Success.
//
TEST(PositionTest, MoveBackSuccess) {
    Position oldPos, newPos;
    MoveResult result;

    // PiecePair: {A, Out} -> {Out, Out}.
    oldPos = Position(PlayerColor::Orange,
        {{LocationId::Out, LocationId::Out}, {LocationId::W,   LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    newPos = Position(PlayerColor::Blue,
        {{LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});
    result = oldPos.move_back(PieceId::InactivePlayerSmall, LocationId::W, LocationId::Out);
    ASSERT_EQ(result.status, MoveResultStatus::Success);
    ASSERT_EQ(result.position, newPos);

    // PiecePair: {A, B} -> {A, Out}.
    oldPos = Position(PlayerColor::Orange,
        {{LocationId::Out, LocationId::Out}, {LocationId::E,   LocationId::W  },
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    newPos = Position(PlayerColor::Blue,
        {{LocationId::E,   LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});
    result = oldPos.move_back(PieceId::InactivePlayerSmall, LocationId::W, LocationId::Out);
    ASSERT_EQ(result.status, MoveResultStatus::Success);
    ASSERT_EQ(result.position, newPos);

    // PiecePair: {A, B} -> {B, Out}.
    oldPos = Position(PlayerColor::Blue,
        {{LocationId::Out, LocationId::Out}, {LocationId::E,   LocationId::W  },
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    newPos = Position(PlayerColor::Orange,
        {{LocationId::W,   LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});
    result = oldPos.move_back(PieceId::InactivePlayerSmall, LocationId::E, LocationId::Out);
    ASSERT_EQ(result.status, MoveResultStatus::Success);
    ASSERT_EQ(result.position, newPos);

    // PiecePair: {A, Out} -> {B, Out}.
    oldPos = Position(PlayerColor::Orange,
        {{LocationId::Out, LocationId::Out}, {LocationId::W,   LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    newPos = Position(PlayerColor::Blue,
        {{LocationId::E,   LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});
    result = oldPos.move_back(PieceId::InactivePlayerSmall, LocationId::W, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Success);
    ASSERT_EQ(result.position, newPos);

    // PiecePair: {A, B} -> {A, C}.
    oldPos = Position(PlayerColor::Orange,
        {{LocationId::Out, LocationId::Out}, {LocationId::E,   LocationId::W},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    newPos = Position(PlayerColor::Blue,
        {{LocationId::E,   LocationId::N},   {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});
    result = oldPos.move_back(PieceId::InactivePlayerSmall, LocationId::W, LocationId::N);
    ASSERT_EQ(result.status, MoveResultStatus::Success);
    ASSERT_EQ(result.position, newPos);

    // PiecePair: {A, B} -> {B, C}.
    oldPos = Position(PlayerColor::Orange,
        {{LocationId::Out, LocationId::Out}, {LocationId::E,   LocationId::W},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});

    newPos = Position(PlayerColor::Blue,
        {{LocationId::W,   LocationId::N  }, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});
    result = oldPos.move_back(PieceId::InactivePlayerSmall, LocationId::E, LocationId::N);
    ASSERT_EQ(result.status, MoveResultStatus::Success);
    ASSERT_EQ(result.position, newPos);
}

//
// Test Position::move_back(PieceId piece, LocationId src, LocationId dst).
// in case of MoveResult::Lost.
//
TEST(PositionTest, MoveBackLost) {
    Position oldPos, newPos;
    MoveResult result;

    oldPos = Position(PlayerColor::Orange,
        {{LocationId::NW, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::N,  LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::NE, LocationId::Out}, {LocationId::NW,  LocationId::Out}});
    result = oldPos.move_back(PieceId::InactivePlayerLarge, LocationId::NW, LocationId::Center);
    ASSERT_EQ(result.status, MoveResultStatus::Lost);
}

//
// Test Position::move_back(PieceId piece, LocationId src, LocationId dst).
// in case of MoveResult::Invalid.
//
TEST(PositionTest, MoveBackInvalid) {
    Position pos;
    MoveResult result;

    // The position is invalid.
    pos = InvalidPositionId;
    result = pos.move_back(PieceId::InactivePlayerSmall, LocationId::Center, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // `piece` is a piece of the turn player's.
    pos = Position(PlayerColor::Orange,
        {{LocationId::W, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::W, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::W, LocationId::Out}, {LocationId::Out, LocationId::Out}});
    result = pos.move_back(PieceId::InactivePlayerSmall, LocationId::W, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // `piece` is an invalid piece.
    pos = Position(PlayerColor::Orange,
        {{LocationId::Out, LocationId::Out}, {LocationId::W, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::W, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::W, LocationId::Out}});
    result = pos.move_back(PieceId::Invalid, LocationId::W, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // `piece` is not located at `src`.
    result = pos.move_back(PieceId::InactivePlayerLarge, LocationId::N, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // `src` is `Out`.
    pos = Position(PlayerColor::Orange,
        {{LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::Out, LocationId::Out}});
    result = pos.move_back(PieceId::InactivePlayerLarge, LocationId::Out, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // `src` is an invalid location.
    pos = Position(PlayerColor::Orange,
        {{LocationId::Out, LocationId::Out}, {LocationId::W, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::W, LocationId::Out},
         {LocationId::Out, LocationId::Out}, {LocationId::W, LocationId::Out}});
    result = pos.move_back(PieceId::InactivePlayerLarge, LocationId::Invalid, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // `dst` is an invalid location.
    result = pos.move_back(PieceId::InactivePlayerLarge, LocationId::W, LocationId::Invalid);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // The `piece` is not a largest piece at `src`.
    result = pos.move_back(PieceId::InactivePlayerSmall, LocationId::W, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    result = pos.move_back(PieceId::InactivePlayerMedium, LocationId::W, LocationId::E);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // `piece` will not be a largest piece at `dst`.
    pos = Position(PlayerColor::Orange,
        {{LocationId::NW, LocationId::SW}, {LocationId::Out, LocationId::Out},
         {LocationId::N,  LocationId::S},  {LocationId::Out, LocationId::Out},
         {LocationId::NE, LocationId::SE}, {LocationId::Out, LocationId::Out}});
    result = pos.move_back(PieceId::InactivePlayerSmall,  LocationId::NW, LocationId::SW);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    result = pos.move_back(PieceId::InactivePlayerMedium, LocationId::N,  LocationId::S);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    result = pos.move_back(PieceId::InactivePlayerLarge,  LocationId::NE, LocationId::SE);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);

    // `src` and `dst` are the same.
    result = pos.move_back(PieceId::InactivePlayerSmall, LocationId::NW, LocationId::NW);
    ASSERT_EQ(result.status, MoveResultStatus::Invalid);
}
