#ifndef QIMAGEDISPLAYWIDGET_ADAPTER_H
#define QIMAGEDISPLAYWIDGET_ADAPTER_H

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

#include <QObject>

#include <QPoint>
#include <QSize>
#include <QRect>
#include <QColor>
#include <QImage>

namespace QImageDisplay {

class ImageAdapter : public QObject
{
    Q_OBJECT
public:
    explicit ImageAdapter(QObject *parent = nullptr);

    virtual QSize getImageSize() const;

    virtual QColor getColorAtPoint(int x, int y) const = 0;

    inline QColor getColorAtPoint(QPoint const& pos) const {
        return getColorAtPoint(pos.x(), pos.y());
    }

    virtual QImage getImage() const;

Q_SIGNALS:

    void imageSizeChanged(QSize newSize);
    void imageValuesChanged(QRect regionChanged);

};

} // namespace QImageDisplay

#endif //QIMAGEDISPLAYWIDGET_ADAPTER_H

