#! /usr/bin/python3
#
# Copyright (C) 2022 Motoyuki Kasahara.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>
#

from gobb_analyzer_common import *

#
# Yields a tuple of two locations.
# The tuples returned by the function are:
#   (Out, Out)
# or
#   (loc1, loc2), that satisfies loc1 > loc2
#
def each_location_pair():
    yield (OUT, OUT)
    for loc2 in range(OUT, SE):
        for loc1 in range(loc2 + 1, SE + 1):
            yield (loc1, loc2)

#
# Prints a prolog of C++ program.
#
def print_prolog():
    for line in ("//",
                 "// This file is generated automatically.  Do not edit.",
                 "//",
                 "#ifndef GOBB_ANALYZER_PIECE_QUAD_INDEX_MAPS_H",
                 "#define GOBB_ANALYZER_PIECE_QUAD_INDEX_MAPS_H",
                 "",
                 '#include "position.hpp"',
                 "",
                 "namespace gobb_analyzer {",
                 "",
                 "const Position::LocationIdQuad Position::pieceQuadIndexMaps_[] = {"):
        print(line)

#
# Prints an epilog of C++ program.
#
def print_epilog():
    for line in ("};",
                 "",
                 "} // namespace gobb_analyzer",
                 "",
                 "#endif // GOBB_ANALYZER_PIECE_QUAD_INDEX_MAPS"):
        print(line)

#
# Prints a C++ program body.
#
def print_body():
    seq = 0
    for pair2 in each_location_pair():
        bitmap2 = ((1 << pair2[0]) | (1 << pair2[1])) & 0x3fe
        for pair1 in each_location_pair():
            bitmap1 = ((1 << pair1[0]) | (1 << pair1[1])) & 0x3fe
            if (bitmap1 == bitmap2 == 0) or (bitmap1 != bitmap2 and bitmap1 | bitmap2 == bitmap1 ^ bitmap2):
                print('    {{LocationId::{:7s} LocationId::{:6s}, LocationId::{:7s} LocationId::{:6s}}},'
                      .format(location_to_str(pair1[0]) + ",",
                              location_to_str(pair1[1]),
                              location_to_str(pair2[0]) + ",",
                              location_to_str(pair2[1])),
                      end="")
                print("  // {:4d}".format(seq))
                seq += 1

print_prolog()
print_body()
print_epilog()
