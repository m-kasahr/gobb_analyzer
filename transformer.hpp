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

#ifndef GOBB_ANALYZER_TRANSFORMER_HPP
#define GOBB_ANALYZER_TRANSFORMER_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include "definitions.hpp"

///
/// @file   transformer.hpp
/// @brief  Define the class `Transformer` and its related types.
///
namespace gobb_analyzer {

/// Integral type for representing `TransformerId`.
using TransformerIdInt = std::uint8_t;

///
/// Transformation methods.
///
/// To decrease the number of position to be analyzed, we transform a position to a symmetric one
/// during the analysis.
///
enum class TransformerId: TransformerIdInt {
    Unchange        = 0,  ///< No transformation.
    Rotate90        = 1,  ///< Rotate 90 degrees clockwise.
    Rotate180       = 2,  ///< Rotate 180 degrees clockwise.
    Rotate270       = 3,  ///< Rotate 270 degrees clockwise.
    Mirror          = 4,  ///< Flip horizontal.
    MirrorRotate90  = 5,  ///< Flip horizontal and then rotate 90 degrees clockwise.
    MirrorRotate180 = 6,  ///< Flip horizontal and then rotate 180 degrees clockwise.
    MirrorRotate270 = 7   ///< Flip horizontal and then rotate 270 degrees clockwise.
};

/// The number of transformations.
constexpr std::size_t TransformerIdNums = 8;

/// A set of transformations.
extern const std::array<TransformerId, TransformerIdNums> TransformerIds;

/// A mapping table for antitransformations.
/// It is recommended to use `invert_Transformer()` and `antitransform_LocationId()` instead of
/// using the array directly.
extern const std::array<TransformerId, TransformerIdNums> AntitransformerIds;

/// The number of effective transformations.
/// (i.e. it removes `Unchange` from `TransformerIdNums`)
constexpr std::size_t EffectiveTransformerIdNums = 7;

/// A set of effctive transformations.
extern const std::array<TransformerId, EffectiveTransformerIdNums> EffectiveTransformerIds;

/// The conversion vectors for the transformations.
/// It is recommended to use `transform_LocationId()` and `antitransform_LocationId()` instead of
/// using the array directly.
extern const LocationId TransformationVectors[TransformerIdNums][LocationIdNums];

///
/// Return a name of the transformer ID.
///
/// @param   id  a transformer ID.
/// @return  a name of the transformer ID.
///
/// It returns "Invalid" if `id` is not valid.
///
const std::string& TransformerId_to_string(TransformerId id);

/// The maximum length of a string returned from `TransformerId_to_string()`.
constexpr std::size_t TransformerIdStringMaxLen = 15u;

/// The maximum length of a string returned from `TransformerId_to_string()` for a valid ID.
constexpr std::size_t ValidTransformerIdStringMaxLen = 15u;

///
/// Return true if the transformer ID is valid.
///
/// @param   id  a transformer ID.
/// @return  the validation result.
///
/// It returns true if `id` is `Unchange`, `Rotate90`, `Rotate180`, `Rotate270`, `Mirror`, `MirrorRotate90`,
/// `MirrorRotate180` or `MirrorRotate270`.
///
inline bool is_valid_TransformerId(TransformerId id) {
    return (id <= TransformerId::MirrorRotate270);
}

///
/// Return a transformer which does antitransformation.
///
/// @param   id  a transformer ID.
/// @return  a transformer.
///
/// Any location transformed by `id` is transformed to the original location by calling `invert_Transformer(id)`.
/// It returns `id` if `id` is not valid.
///
inline TransformerId invert_Transformer(TransformerId id) {
    if (is_valid_TransformerId(id)) {
        return AntitransformerIds[static_cast<int>(id)];
    } else {
        return id;
    }
}

///
/// Transform a location.
///
/// @param   id   a transformer ID.
/// @param   loc  a location.
/// @return  the transformed location.
///
/// It converts `loc` with the transformer `id`.
/// It returns `LocationId::Invalid` if `id` and/or `loc` are not valid.
///
inline LocationId transform_LocationId(TransformerId id, LocationId loc) {
    if (is_valid_TransformerId(id) && is_valid_locationId(loc)) {
        return TransformationVectors[static_cast<int>(id)][static_cast<int>(loc)];
    } else {
        return LocationId::Invalid;
    }
}

///
/// Antitransform a location.
///
/// @param   id   a transformer ID.
/// @param   loc  a location.
/// @return  the transformed location.
///
/// It converts `loc` with the antitransformation method of `id`.
/// It returns `LocationId::Invalid` if `id` and/or `loc` are not valid.
///
inline LocationId antitransform_LocationId(TransformerId id, LocationId loc) {
    if (is_valid_TransformerId(id) && is_valid_locationId(loc)) {
        TransformerId invId = AntitransformerIds[static_cast<int>(id)];
        return TransformationVectors[static_cast<int>(invId)][static_cast<int>(loc)];
    } else {
        return LocationId::Invalid;
    }
}

} // namespace gobb_analyzer

#endif // GOBB_ANALYZER_TRANSFORMER_HPP
