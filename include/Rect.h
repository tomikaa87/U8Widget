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

#include "Point.h"
#include "Size.h"

#include <ostream>

namespace U8W
{

class Rect
{
public:
    constexpr inline Rect() noexcept = default;

    constexpr inline Rect(const Point& topLeft, const Point& bottomRight) noexcept
        : _x1{ topLeft.x() }
        , _y1{ topLeft.y() }
        , _x2{ bottomRight.x() }
        , _y2{ bottomRight.y() }
    {}

    constexpr inline Rect(const Point& topLeft, const Size& size) noexcept
        : _x1{ topLeft.x() }
        , _y1{ topLeft.y() }
        , _x2{ topLeft.x() + size.width() - 1 }
        , _y2{ topLeft.y() + size.height() - 1 }
    {}

    constexpr inline Rect(const int left, const int top, const int width, const int height) noexcept
        : _x1{ left }
        , _y1{ top }
        , _x2{ left + width - 1 }
        , _y2{ top + height - 1 }
    {}

    [[nodiscard]] constexpr inline bool isNull() const noexcept
    {
        return _x2 == _x1 - 1 && _y2 == _y1 - 1;
    }

    [[nodiscard]] constexpr inline bool isEmpty() const noexcept
    {
        return _x1 > _x2 || _y1 > _y2;
    }

    [[nodiscard]] constexpr inline bool isValid() const noexcept
    {
        return _x1 <= _x2 && _y1 <= _y2;
    }

    [[nodiscard]] constexpr inline int left() const noexcept
    {
        return _x1;
    }

    [[nodiscard]] constexpr inline int top() const noexcept
    {
        return _y1;
    }

    [[nodiscard]] constexpr inline int right() const noexcept
    {
        return _x2;
    }

    [[nodiscard]] constexpr inline int bottom() const noexcept
    {
        return _y2;
    }

    constexpr inline void setLeft(const int pos) noexcept
    {
        _x1 = pos;
    }

    constexpr inline void setTop(const int pos) noexcept
    {
        _y1 = pos;
    }

    constexpr inline void setRight(const int pos) noexcept
    {
        _x2 = pos;
    }

    constexpr inline void setBottom(const int pos) noexcept
    {
        _y2 = pos;
    }

    [[nodiscard]] constexpr inline int x() const noexcept
    {
        return _x1;
    }

    [[nodiscard]] constexpr inline int y() const noexcept
    {
        return _y1;
    }

    constexpr inline void setX(int pos) noexcept
    {
        _x1 = pos;
    }

    constexpr inline void setY(int pos) noexcept
    {
        _y1 = pos;
    }

    [[nodiscard]] constexpr inline Point topLeft() const noexcept
    {
        return Point{ _x1, _y1 };
    }

    [[nodiscard]] constexpr inline Point bottomRight() const noexcept
    {
        return Point{ _x2, _y2 };
    }

    [[nodiscard]] constexpr inline Point topRight() const noexcept
    {
        return Point{ _x2, _y1 };
    }

    [[nodiscard]] constexpr inline Point bottomLeft() const noexcept
    {
        return Point{ _x1, _y2 };
    }

    [[nodiscard]] constexpr inline Point center() const noexcept
    {
        return Point{ (_x1 + _x2) / 2, (_y1 + _y2) / 2 };
    }

    constexpr inline void moveLeft(const int pos) noexcept
    {
        _x2 += pos - _x1;
        _x1 = pos;
    }

    constexpr inline void moveTop(const int pos) noexcept
    {
        _y2 += pos - _y1;
        _y1 = pos;
    }

    constexpr inline void moveRight(const int pos) noexcept
    {
        _x1 += pos - _x2;
        _x2 = pos;
    }

    constexpr inline void moveBottom(const int pos) noexcept
    {
        _y1 += pos - _y2;
        _y2 = pos;
    }

    constexpr inline void moveTopLeft(const Point& p) noexcept
    {
        moveLeft(p.x());
        moveTop(p.y());
    }

    constexpr inline void moveBottomRight(const Point& p) noexcept
    {
        moveRight(p.x());
        moveBottom(p.y());
    }

    constexpr inline void moveTopRight(const Point& p) noexcept
    {
        moveRight(p.x());
        moveTop(p.y());
    }

    constexpr inline void moveBottomLeft(const Point& p) noexcept
    {
        moveLeft(p.x());
        moveBottom(p.y());
    }

    constexpr inline void moveCenter(const Point& p) noexcept
    {
        const int w = _x2 - _x1;
        const int h = _y2 - _y1;
        _x1 = p.x() - w / 2;
        _y1 = p.y() - h / 2;
        _x2 = _x1 + w;
        _y2 = _y1 + h;
    }

    constexpr inline void moveTo(const int x, const int y) noexcept
    {
        _x2 += x - _x1;
        _y2 += y - _y1;
        _x1 = x;
        _x2 = y;
    }

    constexpr inline void moveTo(const Point& p) noexcept
    {
        moveTo(p.x(), p.y());
    }

    [[nodiscard]] constexpr inline Rect translated(const int dx, const int dy) const noexcept
    {
        return Rect{
            Point{ _x1 + dx, _y1 + dy },
            Point{ _x2 + dx, _y2 + dy }
        };
    }

    [[nodiscard]] constexpr inline Rect translated(const Point& p) const noexcept
    {
        return translated(p.x(), p.y());
    }

    [[nodiscard]] constexpr inline Rect transposed() const noexcept
    {
        return Rect{
            topLeft(),
            size().transposed()
        };
    }

    constexpr inline void translate(const int dx, const int dy) noexcept
    {
        _x1 += dx;
        _y1 += dy;
        _x2 += dx;
        _y2 += dy;
    }

    constexpr inline void translate(const Point& p) noexcept
    {
        translate(p.x(), p.y());
    }

    [[nodiscard]] constexpr inline Size size() const noexcept
    {
        return Size{ width(), height() };
    }

    [[nodiscard]] constexpr inline int width() const noexcept
    {
        return _x2 - _x1 + 1;
    }

    [[nodiscard]] constexpr inline int height() const noexcept
    {
        return _y2 - _y1 + 1;
    }

    constexpr inline void setWidth(const int w) noexcept
    {
        _x2 = _x1 + w - 1;
    }

    constexpr inline void setHeight(const int h) noexcept
    {
        _y2 = _y1 + h - 1;
    }

    constexpr inline void setSize(const Size& s) noexcept
    {
        setWidth(s.width());
        setHeight(s.height());
    }

    [[nodiscard]] Rect operator|(const Rect& r) const noexcept;
    [[nodiscard]] Rect operator&(const Rect& r) const noexcept;

    inline Rect& operator|=(const Rect& r) noexcept
    {
        *this = *this | r;
        return *this;
    }

    inline Rect& operator&=(const Rect& r) noexcept
    {
        *this = *this & r;
        return *this;
    }

    [[nodiscard]] inline Rect intersected(const Rect& r) const noexcept
    {
        return *this & r;
    }

    [[nodiscard]] inline Rect united(const Rect& r) const noexcept
    {
        return *this | r;
    }

    constexpr inline void adjust(const int x1, const int y1, const int x2, const int y2) noexcept
    {
        _x1 += x1;
        _y1 += y1;
        _x2 += x2;
        _y2 += y2;
    }

    [[nodiscard]] constexpr inline Rect adjusted(const int x1, const int y1, const int x2, int const y2) const noexcept
    {
        return Rect{
            Point{ _x1 + x1, _y1 + y1 },
            Point{ _x2 + x2, _y2 + y2 }
        };
    }

    // bool contains(const Rect& r, bool proper = false) const noexcept;
    // bool contains(const Point& p, bool proper = false) const noexcept;
    // inline bool contains(int x, int y) const noexcept;
    // inline bool contains(int x, int y, bool proper) const noexcept;
    // bool intersects(const Rect& r) const noexcept;

    // [[nodiscard]] static constexpr inline Rect span(const Point& p1, const Point& p2) noexcept;

    [[nodiscard]] friend constexpr inline bool operator==(const Rect& r1, const Rect& r2) noexcept
    {
        return
            r1._x1 == r2._x1
            && r1._x2 == r2._x2
            && r1._y1 == r2._y1
            && r1._y2 == r2._y2;
    }

    [[nodiscard]] friend constexpr inline bool operator!=(const Rect& r1, const Rect& r2) noexcept
    {
        return
            r1._x1 != r2._x1
            || r1._x2 != r2._x2
            || r1._y1 != r2._y1
            || r1._y2 != r2._y2;
    }

    friend std::ostream& operator<<(std::ostream& os, const Rect& r);

private:
    int _x1 = 0;
    int _y1 = 0;
    int _x2 = -1;
    int _y2 = -1;
};

}
