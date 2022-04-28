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

#ifndef STRING_TO_UINT_HPP
#define STRING_TO_UINT_HPP

#include <cstdlib>
#include <string>

///
/// @file   string_to_uint.hpp
/// @brief  Define a utility function `string_to_uint()`.
///

///
/// Convert a C string to an unsigned integral value.
///
/// @param   str    a C string.
/// @param   value  a reference to an unsigned integral value.
/// @return  true if the conversion is succeeded.
///
/// `str` must consist of digits ('0' to '9') and '\0' only.
/// If `str` starts with '0', the next character must be '\0'.
///
template <typename T>
bool string_to_uint(const char* str, T& value) {
    if (*str == '\0') {
        return false;
    }
    if (*str == '0') {
        if (*(str + 1) == '\0') {
            value = 0u;
            return true;
        }
        return false;
    }

    const char* s = str;
    unsigned long long ullValue = 0u;

    while (*s != '\0') {
        unsigned long long ullValueTmp;
        switch (*s) {
        case '0':
            ullValueTmp = ullValue * 10u;
            break;
        case '1':
            ullValueTmp = ullValue * 10u + 1;
            break;
        case '2':
            ullValueTmp = ullValue * 10u + 2;
            break;
        case '3':
            ullValueTmp = ullValue * 10u + 3;
            break;
        case '4':
            ullValueTmp = ullValue * 10u + 4;
            break;
        case '5':
            ullValueTmp = ullValue * 10u + 5;
            break;
        case '6':
            ullValueTmp = ullValue * 10u + 6;
            break;
        case '7':
            ullValueTmp = ullValue * 10u + 7;
            break;
        case '8':
            ullValueTmp = ullValue * 10u + 8;
            break;
        case '9':
            ullValueTmp = ullValue * 10u + 9;
            break;
        default:
            return false;
        }
        if (ullValueTmp < ullValue) {
            return false;
        }
        ullValue = ullValueTmp;
        s++;
    }

    T tValue = static_cast<T>(ullValue);
    if (ullValue != tValue) {
        return false;
    }
    value = tValue;
    return true;
}

///
/// Convert a string to an unsigned integral value.
///
/// @param   str    a string.
/// @param   value  a reference to an unsigned integral value.
/// @return  true if the conversion is succeeded.
///
/// `str` must consist of digits ('0' to '9') only.
/// If `str` starts with '0', the next character must be '\0'.
///
template <typename T>
bool string_to_uint(const std::string& str, T& value) {
    return string_to_uint(str.data(), value);
}

#endif // STRING_TO_UINT_HPP
