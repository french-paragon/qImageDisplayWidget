/*qImageDisplayWidget is a qt widget to display images
  that support advanced features like zoom, translation and overlays.

Copyright (C) 2023  Paragon<french.paragon@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "./overlay.h"

QImageDisplay::Overlay::Overlay(QWidget *parent) :
    QObject(parent),
    _display(true)
{

}


namespace QImageDisplay {

bool Overlay::display() const
{
    return _display;
}

void Overlay::setDisplay(bool newDisplay)
{
    if (_display == newDisplay)
        return;
    _display = newDisplay;
    Q_EMIT displayChanged();
    Q_EMIT repaintingRequested(QRect());

}

}
