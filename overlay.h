#ifndef QIMAGEDISPLAYWIDGET_OVERLAY_H
#define QIMAGEDISPLAYWIDGET_OVERLAY_H

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

#include <QList>
#include <QColor>
#include <QPointF>
#include <QPolygonF>

#include <QPainter>
#include <QWidget>

namespace QImageDisplay {

class Overlay : public QObject
{
    Q_OBJECT
public:
    explicit Overlay(QWidget *parent = nullptr);

    inline void paintItem(QPainter* painter, QTransform const& imageToPaintArea) const {
        _imageToPaintArea = imageToPaintArea;
        paintItemImpl(painter);
    }

protected:

    virtual void paintItemImpl(QPainter* painter) const = 0;

    inline void drawPoint(QPainter* painter, QPointF const& point, QColor color, float radius) const {
        QPen pen;
        pen.setColor(color);
        pen.setWidthF(radius);
        painter->setPen(pen);
        painter->drawPoint(_imageToPaintArea.map(point));
    }

    inline void drawLine(QPainter* painter, QLineF const& line, QColor color, float width) const {
        QPen pen;
        pen.setColor(color);
        pen.setWidthF(width);
        painter->setPen(pen);
        painter->drawLine(_imageToPaintArea.map(line));
    }

    inline void drawPoints(QPainter* painter, QPolygonF const& points, QColor color, float radius) const {
        QPen pen;
        pen.setColor(color);
        pen.setWidthF(radius);
        painter->setPen(pen);
        painter->drawPoints(_imageToPaintArea.map(points));
    }

    inline void drawLines(QPainter* painter, QList<QLineF> const& lines, QColor color, float width) const {
        QPen pen;
        pen.setColor(color);
        pen.setWidthF(width);
        painter->setPen(pen);

        QVector<QLineF> transformedLines;
        transformedLines.reserve(lines.size());

        for (QLineF const& line : lines) {
            transformedLines.push_back(_imageToPaintArea.map(line));
        }

        painter->drawLines(transformedLines);
    }

    inline void drawLines(QPainter* painter, QPolygonF const& points, QColor color, float width) const {
        QPen pen;
        pen.setColor(color);
        pen.setWidthF(width);
        painter->setPen(pen);
        painter->drawPolyline(_imageToPaintArea.map(points));
    }

private:

    mutable QTransform _imageToPaintArea;
};

} // namespace QImageDisplay

#endif //QIMAGEDISPLAYWIDGET_OVERLAY_H
