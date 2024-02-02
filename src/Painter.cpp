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

#include "Painter.h"

#include "Display.h"
#include "Widget.h"

#include <iostream>
#include <functional>

namespace U8W
{

#define DEBUG_PAINTER 0

Painter::Painter()
{}

void Painter::paintWidget(Widget* const widget)
{
#if DEBUG_PAINTER
    std::cout << __FUNCTION__ << ": widget=" << widget->_name << '\n';
#endif

    updateWidgetRepaintFlags(widget);

    const auto needsDisplayUpdate = paintWidgetRecursive(widget);

    if (needsDisplayUpdate) {
#if DEBUG_PAINTER
        std::cout << __FUNCTION__ << ": updating display, widget=" << widget->_name << '\n';
#endif
        widget->_display->update();
    }

    widget->_display->resetClipRect();
}

void Painter::updateWidgetRepaintFlags(Widget* const w)
{
#if DEBUG_PAINTER
    std::cout << __FUNCTION__ << ": widget=" << w->_name << '\n';
#endif

    // Repaint parent if its child requests it (e.g geometry change)
    for (auto* child : w->_children) {
#if DEBUG_PAINTER
        std::cout << __FUNCTION__ << ": widget=" << w->_name << '\n';
#endif
        w->_needsRepaint |= child->_parentNeedsRepaint;
        child->_parentNeedsRepaint = false;
    }

    // Children must be repainted if parent is repainted
    for (auto* child : w->_children) {
        child->_needsRepaint |= w->_needsRepaint;
        updateWidgetRepaintFlags(child);
    }
}

bool Painter::paintWidgetRecursive(Widget* const w)
{
#if DEBUG_PAINTER
    std::cout << __FUNCTION__ << ": widget=" << w->_name << '\n';
#endif

    auto needsDisplayUpdate = false;

    if (w->_needsRepaint) {
        const auto clipRect = w->calculateClipRect();
        w->_display->setClipRect(clipRect);

        // Clear the background
        if (w->_backgroundEnabled) {
            w->_display->setDrawColor(Display::Color::White);
            w->_display->fillRect(clipRect);
        }

#if DEBUG_PAINTER
        std::cout << __FUNCTION__ <<
            ": painting, widget=" << w->_name
            << ", rect=" << w->_rect
            << ", clipRect=" << clipRect
            << ", backgroundEnabled=" << w->_backgroundEnabled
            << '\n';
#endif

        w->paint();

        w->_needsRepaint = false;

        needsDisplayUpdate = true;
    }

    for (auto* child : w->_children) {
        needsDisplayUpdate |= paintWidgetRecursive(child);
    }

    return needsDisplayUpdate;
}

}
