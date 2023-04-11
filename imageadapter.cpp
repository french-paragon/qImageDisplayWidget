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

#include "./imageadapter.h"


namespace QImageDisplay {

ImageAdapter::ImageAdapter(QObject *parent) :
    QObject(parent)
{

}


QImage ImageAdapter::getImage() const {

    QSize size = getImageSize();

    QImage img(size, QImage::Format_RGB32);

    for (int x = 0; x < size.width(); x++) {
        for (int y = 0; y < size.height(); y++) {
            img.setPixelColor(x,y,getColorAtPoint(x,y));
        }
    }

    return img;

}

QVector<ImageAdapter::ChannelInfo> ImageAdapter::getOriginalChannelsInfos(QPoint const& pos) const {
    return QVector<ImageAdapter::ChannelInfo>();
}

} //namespace QImageDisplay
