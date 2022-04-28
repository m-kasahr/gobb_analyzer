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

#include "transformer.hpp"

namespace gobb_analyzer {

const std::array<TransformerId, TransformerIdNums> TransformerIds = {
    TransformerId::Unchange,
    TransformerId::Rotate90,
    TransformerId::Rotate180,
    TransformerId::Rotate270,
    TransformerId::Mirror,
    TransformerId::MirrorRotate90,
    TransformerId::MirrorRotate180,
    TransformerId::MirrorRotate270
};

const std::array<TransformerId, TransformerIdNums> InverseTransformerIds = {
    TransformerId::Unchange,
    TransformerId::Rotate270,
    TransformerId::Rotate180,
    TransformerId::Rotate90,
    TransformerId::Mirror,
    TransformerId::MirrorRotate90,
    TransformerId::MirrorRotate180,
    TransformerId::MirrorRotate270
};

const std::array<TransformerId, EffectiveTransformerIdNums> EffectiveTransformerIds = {
    TransformerId::Rotate90,
    TransformerId::Rotate180,
    TransformerId::Rotate270,
    TransformerId::Mirror,
    TransformerId::MirrorRotate90,
    TransformerId::MirrorRotate180,
    TransformerId::MirrorRotate270
};

const LocationId TransformationVectors[TransformerIdNums][LocationIdNums] = {
    {
        // Unchange
        LocationId::Out,
        LocationId::NW,  LocationId::N,      LocationId::NE,
        LocationId::W,   LocationId::Center, LocationId::E,
        LocationId::SW,  LocationId::S,      LocationId::SE
    },
    {
        // Rotate90
        LocationId::Out,
        LocationId::NE,  LocationId::E,      LocationId::SE,
        LocationId::N,   LocationId::Center, LocationId::S,
        LocationId::NW,  LocationId::W,      LocationId::SW
    },
    {
        // Rotate180
        LocationId::Out,
        LocationId::SE,  LocationId::S,      LocationId::SW,
        LocationId::E,   LocationId::Center, LocationId::W,
        LocationId::NE,  LocationId::N,      LocationId::NW
    },
    {
        // Rotate270
        LocationId::Out,
        LocationId::SW,  LocationId::W,      LocationId::NW,
        LocationId::S,   LocationId::Center, LocationId::N,
        LocationId::SE,  LocationId::E,      LocationId::NE
    },
    {
        // Mirror
        LocationId::Out,
        LocationId::NE,  LocationId::N,      LocationId::NW,
        LocationId::E,   LocationId::Center, LocationId::W,
        LocationId::SE,  LocationId::S,      LocationId::SW
    },
    {
        // MirrorRotate90
        LocationId::Out,
        LocationId::SE,  LocationId::E,      LocationId::NE,
        LocationId::S,   LocationId::Center, LocationId::N,
        LocationId::SW,  LocationId::W,      LocationId::NW
    },
    {
        // MirrorRotate180
        LocationId::Out,
        LocationId::SW,  LocationId::S,      LocationId::SE,
        LocationId::W,   LocationId::Center, LocationId::E,
        LocationId::NW,  LocationId::N,      LocationId::NE
    },
    {
        // MirrorRotate270
        LocationId::Out,
        LocationId::NW,  LocationId::W,      LocationId::SW,
        LocationId::N,   LocationId::Center, LocationId::S,
        LocationId::NE,  LocationId::E,      LocationId::SE
    }
};

const std::string& TransformerId_to_string(TransformerId id) {
    static const std::string strings[] = {
        "Unchange",
        "Rotate90",
        "Rotate180",
        "Rotate270",
        "Mirror",
        "MirrorRotate90",
        "MirrorRotate180",
        "MirrorRotate270"
    };
    static const std::string invalidString("Invalid");

    if (id >= TransformerId::Unchange && id <= TransformerId::MirrorRotate270) {
        return strings[static_cast<int>(id)];
    } else {
        return invalidString;
    }
}

} // namespace gobb_analyzer
