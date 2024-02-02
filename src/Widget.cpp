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

#include "Widget.h"

#include "Display.h"

#include <algorithm>

namespace U8W
{

Widget::Widget(Display* const display)
    : _display{ display }
    , _parent{ nullptr }
{}

Widget::Widget(Widget* parent)
    : _display{ parent->_display }
    , _parent{ parent }
{
    parent->_children.push_back(this);
}

Widget::~Widget()
{
    if (_parent) {
        _children.erase(
            std::remove(
                std::begin(_children),
                std::end(_children),
                this
            )
        );
    }
}

void Widget::setName(std::string name)
{
    _name = std::move(name);
}

void Widget::setPos(Point p)
{
    _rect.moveTopLeft(std::move(p));
    _needsRepaint = true;
    _parentNeedsRepaint = true;
}

void Widget::setSize(Size s)
{
    _rect.setSize(std::move(s));
    _needsRepaint = true;
    _parentNeedsRepaint = true;

    onResize();
}

void Widget::setWidth(const int width)
{
    _rect.setWidth(width);
    _needsRepaint = true;
    _parentNeedsRepaint = true;

    onResize();
}

void Widget::setHeight(const int height)
{
    _rect.setHeight(height);
    _needsRepaint = true;
    _parentNeedsRepaint = true;

    onResize();
}

void Widget::setRect(Rect r)
{
    _rect = std::move(r);
    _needsRepaint = true;
    _parentNeedsRepaint = true;

    onResize();
}

void Widget::setBackgroundEnabled(const bool enabled)
{
    _backgroundEnabled = enabled;
    _needsRepaint = true;
}

Point Widget::mapToGlobal(const Point& p) const
{
    Point mappedPoint = p;
    const Widget* w = this;

    while (w) {
        mappedPoint = w->mapToParent(mappedPoint);
        w = w->_parent;
    }

    return mappedPoint;
}

Rect Widget::mapToGlobal(const Rect& r) const
{
    return Rect{
        mapToGlobal(r.topLeft()),
        r.size()
    };
}

Point Widget::mapToParent(const Point& p) const
{
    if (!_parent) {
        return p;
    }

    return Point{
        p.x() + _parent->pos().x(),
        p.y() + _parent->pos().y()
    };
}

Rect Widget::mapToParent(const Rect& r) const
{
    return Rect{
        mapToParent(r.topLeft()),
        r.size()
    };
}

void Widget::paint()
{
#if DEBUG_WIDGET
    _display->setDrawColor(Display::Color::Black);
    _display->drawRect(mapToGlobal(_rect));
#endif
}

Rect Widget::calculateClipRect() const
{
    if (!_parent) {
        return _rect;
    }

    const auto selfGlobalRect = mapToGlobal(_rect);
    const auto parentGlobalRect = _parent->mapToGlobal(_parent->_rect);

    return parentGlobalRect & selfGlobalRect;
}

}
