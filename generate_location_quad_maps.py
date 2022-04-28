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
# Prints a prolog of C++ program.
#
def print_prolog():
    for line in ("//",
                 "// This file is generated automatically.  Do not edit.",
                 "//",
                 "#ifndef GOBB_ANALYZER_LOCATION_QUAD_MAPS_H",
                 "#define GOBB_ANALYZER_LOCATION_QUAD_MAPS_H",
                 "",
                 '#include "position.hpp"',
                 "",
                 "namespace gobb_analyzer {",
                 "",
                 "const PositionId Position::locationQuadMaps_[] = {"):
        print(line)

#
# Prints a C++ program body.
#
def print_body():
    seq = 0
    maps = {}

    for loc4 in Locations:
        for loc3 in Locations:
            for loc2 in Locations:
                for loc1 in Locations:
                    valid = True
                    if loc1 != 0 and loc1 in (loc2, loc3, loc4):
                        valid = False
                    elif loc2 != 0 and loc2 in (loc3, loc4):
                        valid = False
                    elif loc3 != 0 and loc3 == loc4:
                        valid = False
                    if valid:
                        if loc1 >= loc2:
                            canon_loc1 = loc1
                            canon_loc2 = loc2
                        else:
                            canon_loc1 = loc2
                            canon_loc2 = loc1
                        if loc3 >= loc4:
                            canon_loc3 = loc3
                            canon_loc4 = loc4
                        else:
                            canon_loc3 = loc4
                            canon_loc4 = loc3
                        index = loc1 + \
                            (loc2 * LOCATION_NUMS) + \
                            (loc3 * LOCATION_NUMS ** 2) + \
                            (loc4 * LOCATION_NUMS ** 3)
                        canon_index = canon_loc1 + \
                            (canon_loc2 * LOCATION_NUMS) + \
                            (canon_loc3 * LOCATION_NUMS ** 2) + \
                            (canon_loc4 * LOCATION_NUMS ** 3)
                        if index == canon_index:
                            maps[index] = seq
                            id_ = seq
                            seq += 1
                        else:
                            id_ = maps[canon_index]
                        print("    {:-17d},".format(id_), end="")
                    else:
                        print("    InvalidPositionId,", end="")

                    print("  // {{{:7s} {:7s} {:7s} {:7s}}}".format(location_to_str(loc1) + ",",
                                                                    location_to_str(loc2) + ",",
                                                                    location_to_str(loc3) + ",",
                                                                    location_to_str(loc4)).rstrip())

#
# Prints a C++ program body.
#
def print_epilog():
    for line in ("};",
                 "",
                 "} // namespace gobb_analyzer",
                 "",
                 "#endif // GOBB_ANALYZER_LOCATION_ID_QUAD_MAPS_H"):
        print(line)

print_prolog()
print_body()
print_epilog()
