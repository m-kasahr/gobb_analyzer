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

#
# Locations.
#
OUT     = 0
NW      = 1
N       = 2
NE      = 3
W       = 4
CENTER  = 5
E       = 6
SW      = 7
S       = 8
SE      = 9

LOCATION_NUMS = 10

Locations = range(OUT, LOCATION_NUMS)

#
# Converts a integral location value to a string.
#
def location_to_str(loc):
    return ("Out",
            "NW",
            "N",
            "NE",
            "W",
            "Center",
            "E",
            "SW",
            "S",
            "SE")[loc]
