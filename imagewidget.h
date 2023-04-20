#ifndef QIMAGEDISPLAYWIDGET_IMAGEWIDGET_H
#define QIMAGEDISPLAYWIDGET_IMAGEWIDGET_H

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

#include <QPoint>
#include <QPointF>
#include <QMap>
#include <QVector>

#include <QWidget>

namespace QImageDisplay {

class ImageAdapter;
class Overlay;

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWidget(QWidget *parent = nullptr);

    bool hasImage() const;

    int zoom() const;
    void setZoom(int zoom_percent);

    ImageAdapter* currentImage() const;
    void setImage(ImageAdapter* img);

    QPoint translation() const;
    void setTranslation(const QPoint &translation);

    void addOverlay(Overlay* drawable);

    inline QTransform getImageToWidgetTransform() const {

        QSize s = rect().size();
        QSize is = (_c_img.isNull()) ? QSize(_img.width()*_zoom/100, _img.height()*_zoom/100) : _c_img.size();

        QTransform img2widget;
        img2widget.translate(_translation.x()-(is.width() - s.width())/2, _translation.y()-(is.height() - s.height())/2);
        img2widget.scale(_zoom/100., _zoom/100.);

        return img2widget;
    }
    QPointF widgetToImageCoordinates(QPoint const& widget_pos) const;
    QPointF imageToWidgetCoordinates(QPointF const& image_pos) const;

Q_SIGNALS:

    void zoomChanged(int zoom_percent);
    void translationChanged(QPoint translation);

protected:

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void cachedZoomed();

    void imageDeleted();
    void resetImage();

    static int clipZoom(int rawZoom);

    void overlayRequestedRepaint(QRect imageRegion);

    QPoint _translation;
    int _zoom;

    ImageAdapter* _currentImageDataBlock;
    QPixmap _img;
    QPixmap _c_img;

    qint64 _activePoint;
    qint64 _drawOnlyPoint;
    bool _showPoints;

    QVector<Overlay*> _overlays;

private:

    Qt::MouseButtons _previously_pressed;
    bool _control_pressed_when_clicked;

    QPoint _motion_origin_pos;
    QPoint _motion_origin_t;

};

} // namespace QImageDisplay

#endif //QIMAGEDISPLAYWIDGET_IMAGEWIDGET_H
