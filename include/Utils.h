//  U8Widget - Simple widget library based on U8g2 by olikraus
//  Copyright (C) 2024  Tamas Karpati
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.#pragma once

#pragma once

namespace U8W::Utils
{
    template <typename ValueType>
    constexpr ValueType abs(const ValueType n) noexcept
    {
        return n < 0 ? -n : n;
    }

    template <typename ValueType>
    constexpr ValueType min(const ValueType a, const ValueType b) noexcept
    {
        return a < b ? a : b;
    }

    template <typename ValueType>
    constexpr ValueType max(const ValueType a, const ValueType b) noexcept
    {
        return a > b ? a : b;
    }

    template <typename ValueType>
    constexpr ValueType clamp(const ValueType value, const ValueType rangeMin, const ValueType rangeMax)
    {
        return max(rangeMin, min(rangeMax, value));
    }
}
