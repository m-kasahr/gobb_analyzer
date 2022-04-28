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

#include "definitions.hpp"

namespace gobb_analyzer {

const std::array<PlayerId, PlayerIdNums> PlayerIds = {
    PlayerId::Active,
    PlayerId::Inactive
};

PieceId to_pieceId(PlayerId player, PieceSize size) {
    if (!is_valid_playerId(player) || !is_valid_pieceSize(size)) {
        return PieceId::Invalid;
    }
    for (PieceId piece: PieceIds) {
        if (playerId_of_pieceId(piece) == player &&
            pieceSize_of_pieceId(piece) == size) {
            return piece;
        }
    }
    return PieceId::None;
}

PlayerId invert_playerId(PlayerId id) {
    static const PlayerId strings[] = {
        PlayerId::None,      // None
        PlayerId::Inactive,  // Active
        PlayerId::Active     // Inactive
    };

    if (id >= PlayerId::None && id <= PlayerId::Inactive) {
        return strings[static_cast<int>(id)];
    } else {
        return PlayerId::Invalid;
    }
}

const std::string& playerId_to_string(PlayerId id) {
    static const std::string strings[] = {
        "None",
        "Active",
        "Inactive"
    };
    static const std::string invalidString = "Invalid";

    if (id >= PlayerId::None && id <= PlayerId::Inactive) {
        return strings[static_cast<int>(id)];
    } else {
        return invalidString;
    }
}

const std::array<PieceSize, PieceSizeNums> PieceSizes = {
    PieceSize::Small,
    PieceSize::Medium,
    PieceSize::Large
};

const std::string& pieceSize_to_string(PieceSize size) {
    static const std::string strings[] = {
        "None",
        "Small",
        "Medium",
        "Large"
    };
    static const std::string& invalidString = "Invalid";

    if (size >= PieceSize::None && size <= PieceSize::Large) {
        return strings[static_cast<int>(size)];
    } else {
        return invalidString;
    }
}

const std::array<PieceId, PieceIdNums> PieceIds = {
    PieceId::ActivePlayerSmall,
    PieceId::InactivePlayerSmall,
    PieceId::ActivePlayerMedium,
    PieceId::InactivePlayerMedium,
    PieceId::ActivePlayerLarge,
    PieceId::InactivePlayerLarge
};

const std::array<PieceId, PlayerPieceIdNums> ActivePlayerPieceIds = {
    PieceId::ActivePlayerSmall,
    PieceId::ActivePlayerMedium,
    PieceId::ActivePlayerLarge
};

const std::array<PieceId, PlayerPieceIdNums> InactivePlayerPieceIds = {
    PieceId::InactivePlayerSmall,
    PieceId::InactivePlayerMedium,
    PieceId::InactivePlayerLarge
};

const std::array<PieceId, PieceSetNums> PieceSet = {
    PieceId::ActivePlayerSmall,
    PieceId::ActivePlayerSmall,
    PieceId::InactivePlayerSmall,
    PieceId::InactivePlayerSmall,
    PieceId::ActivePlayerMedium,
    PieceId::ActivePlayerMedium,
    PieceId::InactivePlayerMedium,
    PieceId::InactivePlayerMedium,
    PieceId::ActivePlayerLarge,
    PieceId::ActivePlayerLarge,
    PieceId::InactivePlayerLarge,
    PieceId::InactivePlayerLarge
};

PlayerId playerId_of_pieceId(PieceId id) {
    static const PlayerId player_ids[] = {
        PlayerId::None,      // None
        PlayerId::Active,    // ActivePlayerSmall
        PlayerId::Inactive,  // InactivePlayerSmall
        PlayerId::Active,    // ActivePlayerMedium
        PlayerId::Inactive,  // InactivePlayerMedium
        PlayerId::Active,    // ActivePlayerLarge
        PlayerId::Inactive   // InactivePlayerLarge
    };

    if (id >= PieceId::None && id <= PieceId::InactivePlayerLarge) {
        return player_ids[static_cast<int>(id)];
    } else {
        return PlayerId::Invalid;
    }
}

PieceSize pieceSize_of_pieceId(PieceId id) {
    static const PieceSize piece_sizes[] = {
        PieceSize::None,    // None
        PieceSize::Small,   // ActivePlayerSmall
        PieceSize::Small,   // InactivePlayerSmall
        PieceSize::Medium,  // ActivePlayerMedium
        PieceSize::Medium,  // InactivePlayerMedium
        PieceSize::Large,   // ActivePlayerLarge
        PieceSize::Large    // InactivePlayerLarge
    };

    if (id >= PieceId::None && id <= PieceId::InactivePlayerLarge) {
        return piece_sizes[static_cast<int>(id)];
    } else {
        return PieceSize::Invalid;
    }
}

PieceId invert_playerId_of_pieceId(PieceId id) {
    static const PieceId piece_ids[] = {
        PieceId::None,                  // None
        PieceId::InactivePlayerSmall,   // ActivePlayerSmall
        PieceId::ActivePlayerSmall,     // InactivePlayerSmall
        PieceId::InactivePlayerMedium,  // ActivePlayerMedium
        PieceId::ActivePlayerMedium,    // InactivePlayerMedium
        PieceId::InactivePlayerLarge,   // ActivePlayerLarge
        PieceId::ActivePlayerLarge      // InactivePlayerLarge
    };

    if (id >= PieceId::None && id <= PieceId::InactivePlayerLarge) {
        return piece_ids[static_cast<int>(id)];
    } else {
        return PieceId::Invalid;
    }
}

const std::string& pieceId_to_string(PieceId id) {
    static const std::string strings[] = {
        "None",
        "ActivePlayerSmall",
        "InactivePlayerSmall",
        "ActivePlayerMedium",
        "InactivePlayerMedium",
        "ActivePlayerLarge",
        "InactivePlayerLarge"
    };
    static const std::string& invalidString = "Invalid";

    if (id >= PieceId::None && id <= PieceId::InactivePlayerLarge) {
        return strings[static_cast<int>(id)];
    } else {
        return invalidString;
    }
}

const std::array<LocationId, LocationIdNums> LocationIds = {
    LocationId::Out,
    LocationId::NW,
    LocationId::N,
    LocationId::NE,
    LocationId::W,
    LocationId::Center,
    LocationId::E,
    LocationId::SW,
    LocationId::S,
    LocationId::SE
};

const std::array<LocationId, OnBoardLocationIdNums> OnBoardLocationIds = {
    LocationId::NW,
    LocationId::N,
    LocationId::NE,
    LocationId::W,
    LocationId::Center,
    LocationId::E,
    LocationId::SW,
    LocationId::S,
    LocationId::SE
};

const std::string& locationId_to_string(LocationId id) {
    static const std::string strings[] = {
        "Out",
        "NW",
        "N",
        "NE",
        "W",
        "Center",
        "E",
        "SW",
        "S",
        "SE",
    };
    static const std::string invalidString = "Invalid";

    if (id >= LocationId::None && id <= LocationId::SE) {
        return strings[static_cast<int>(id)];
    } else {
        return invalidString;
    }
}

PlayerColor invert_playerColor(PlayerColor color) {
    if (color == PlayerColor::Orange) {
        return PlayerColor::Blue;
    } else if (color == PlayerColor::Blue) {
        return PlayerColor::Orange;
    }
    return color;
}

const std::string& playerColor_to_string(PlayerColor color) {
    static const std::string strings[] = {
        "Orange",
        "Blue",
    };
    static const std::string invalidString = "Invalid";

    if (color >= PlayerColor::Orange && color <= PlayerColor::Blue) {
        return strings[static_cast<int>(color)];
    } else {
        return invalidString;
    }
}

} // namespace gobb_analyzer
