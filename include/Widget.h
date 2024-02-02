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
#include "Rect.h"
#include "Size.h"

#include <iostream>
#include <string>
#include <vector>

namespace U8W
{

class Display;

class Widget
{
    friend class Painter;

public:
    explicit Widget(Display* display);
    explicit Widget(Widget* parent);

    virtual ~Widget();

    Widget(const Widget&) = delete;
    Widget(Widget&&) = delete;
    Widget& operator=(const Widget&) = delete;
    Widget& operator=(Widget&&) = delete;

    void setName(std::string name);

    const inline Point pos() const
    {
        return _rect.topLeft();
    }

    const inline Size size() const
    {
        return _rect.size();
    }

    const inline Rect& rect() const
    {
        return _rect;
    }

    void setPos(Point p);
    void setSize(Size s);
    void setWidth(int width);
    void setHeight(int height);
    void setRect(Rect rect);

    void setBackgroundEnabled(bool enabled);

    Point mapToGlobal(const Point& p) const;
    Rect mapToGlobal(const Rect& r) const;
    Point mapToParent(const Point& p) const;
    Rect mapToParent(const Rect& r) const;

protected:
    Display* const _display;
    Widget* const _parent;
    std::string _name;
    std::vector<Widget*> _children;
    Rect _rect;
    bool _needsRepaint = true;
    bool _parentNeedsRepaint = true;
    bool _backgroundEnabled = true;

    virtual void paint();

    virtual void onResize() {};

    Rect calculateClipRect() const;
};

}
