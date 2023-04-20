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

#include "./imagewidget.h"

#include "./imageadapter.h"
#include "./overlay.h"

#include <QGuiApplication>
#include <QMouseEvent>

namespace QImageDisplay {

ImageWidget::ImageWidget(QWidget *parent) :
    QWidget(parent),
    _translation(0, 0),
    _zoom(100),
    _currentImageDataBlock(nullptr),
    _img(),
    _c_img(),
    _activePoint(-1),
    _drawOnlyPoint(-1),
    _showPoints(true),
    _control_pressed_when_clicked(false)
{

}

bool ImageWidget::hasImage() const {
    return !_img.isNull();
}

int ImageWidget::zoom() const {
    return _zoom;
}

void ImageWidget::cachedZoomed() {

    if (_img.isNull() or _zoom > 100) {
        _c_img = QPixmap();
        return;
    }

    int w = _img.width()*_zoom/100;
    int h = _img.height()*_zoom/100;

    _c_img = _img.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

}

void ImageWidget::imageDeleted() {
    _img = QPixmap();
    _c_img = QPixmap();
}


void ImageWidget::resetImage() {

    _img = QPixmap();
    _c_img = QPixmap();

    if (_currentImageDataBlock != nullptr) {

        _img = QPixmap::fromImage(_currentImageDataBlock->getImage());

    }

    cachedZoomed();
    update();
}

int ImageWidget::clipZoom(int rawZoom) {

    int zoom_percent = rawZoom;

    if (zoom_percent <= 0) {
        zoom_percent = 1;
    }

    if (zoom_percent > 1000) {
        zoom_percent = 1000;
    }

    return zoom_percent;
}

void ImageWidget::overlayRequestedRepaint(QRect imageRegion) {

    if (!imageRegion.isValid()) {
        update();
        return;
    }

    QTransform img2Widget = getImageToWidgetTransform();
    QRect trsf = img2Widget.mapRect(imageRegion);

    update(trsf);

}

void ImageWidget::addOverlay(Overlay* drawable) {
    _overlays.push_back(drawable);
    connect(drawable, &Overlay::repaintingRequested, this, &ImageWidget::overlayRequestedRepaint);
}

void ImageWidget::setZoom(int zp) {

    int zoom_percent = clipZoom(zp);

    if (zoom_percent != _zoom) {
        _zoom = zoom_percent;
        emit zoomChanged(zoom_percent);

        if (hasImage()) {
            cachedZoomed();
            setTranslation(translation());
            update();
        }
    }
}

ImageAdapter* ImageWidget::currentImage() const {
    return _currentImageDataBlock;
}

void ImageWidget::setImage(ImageAdapter *img) {

    if (_currentImageDataBlock != nullptr) {

        disconnect(_currentImageDataBlock, &QObject::destroyed, this, &ImageWidget::imageDeleted);
        disconnect(_currentImageDataBlock, &ImageAdapter::imageSizeChanged, this, &ImageWidget::resetImage);
        disconnect(_currentImageDataBlock, &ImageAdapter::imageValuesChanged, this, &ImageWidget::resetImage);
    }

    _currentImageDataBlock = img;

    if (img != nullptr) {

        connect(_currentImageDataBlock, &QObject::destroyed, this, &ImageWidget::imageDeleted);
        connect(_currentImageDataBlock, &ImageAdapter::imageSizeChanged, this, &ImageWidget::resetImage);
        connect(_currentImageDataBlock, &ImageAdapter::imageValuesChanged, this, &ImageWidget::resetImage);

    }

    resetImage();
}

QPoint ImageWidget::translation() const
{
    return _translation;
}

void ImageWidget::setTranslation(const QPoint &translation)
{
    QPoint n_t = translation;

    QSize s = rect().size();
    QSize is = (_c_img.isNull()) ? QSize(_img.width()*_zoom/100, _img.height()*_zoom/100) : _c_img.size();

    int max_w = std::abs(s.width() - is.width())/2;
    int max_h = std::abs(s.height() - is.height())/2;

    if (n_t.x() < -max_w) {
        n_t.setX(-max_w);
    } else if (n_t.x() > max_w) {
        n_t.setX(max_w);
    }

    if (n_t.y() < -max_h) {
        n_t.setY(-max_h);
    } else if (n_t.y() > max_h) {
        n_t.setY(max_h);
    }

    if (_translation != n_t) {
        _translation = n_t;
        update();
    }
}

QPointF ImageWidget::widgetToImageCoordinates(QPoint const& widget_pos) const {

    QPoint middle = QPoint(rect().size().width(), rect().size().height())/2;
    QPointF src_img_coord = widget_pos - middle - _translation;

    src_img_coord /= _zoom;
    src_img_coord *= 100;

    QPointF middle_img = QPoint(_img.size().width(), _img.size().height())/2;
    src_img_coord += middle_img;

    return src_img_coord;
}

QPointF ImageWidget::imageToWidgetCoordinates(QPointF const& image_pos) const {

    QPointF middle_img = QPoint(_img.size().width(), _img.size().height())/2;
    QPointF local_img_coord = image_pos - middle_img;

    local_img_coord *= _zoom;
    local_img_coord /= 100;

    QPoint middle = QPoint(rect().size().width(), rect().size().height())/2;
    QPointF wCoord = local_img_coord + middle + _translation;

    return wCoord;
}

void ImageWidget::paintEvent(QPaintEvent *) {

    QPainter painter(this);

    QSize s = rect().size();
    painter.fillRect(0, 0, s.width(), s.height(), QColor(255, 255, 255));

    if (_img.isNull()) {
        return;
    }

    QSize is = (_c_img.isNull()) ? QSize(_img.width()*_zoom/100, _img.height()*_zoom/100) : _c_img.size();

    int pw_x = (is.width() >= s.width()) ? 0 : (s.width() - is.width())/2 + _translation.x();
    int pw_y = (is.height() >= s.height()) ? 0 : (s.height() - is.height())/2 + _translation.y();

    int pi_x = (is.width() < s.width()) ? 0 : (is.width() - s.width())/2 - _translation.x();
    int pi_y = (is.height() < s.height()) ? 0 : (is.height() - s.height())/2 - _translation.y();

    int pi_w = (is.width() < s.width()) ? is.width() : s.width();
    int pi_h = (is.height() < s.height()) ? is.height() : s.height();

    QRectF imageCover = (_c_img.isNull()) ? QRectF(pi_x*100./_zoom, pi_y*100./_zoom, pi_w*100./_zoom, pi_h*100./_zoom) : QRectF(pi_x, pi_y, pi_w, pi_h);

    painter.drawPixmap(QRectF(pw_x, pw_y, pi_w, pi_h), (_c_img.isNull()) ? _img : _c_img, imageCover);

    QTransform img2widget;
    img2widget.translate(_translation.x()-(is.width() - s.width())/2, _translation.y()-(is.height() - s.height())/2);
    img2widget.scale(_zoom/100., _zoom/100.);

    for (Overlay* o : _overlays) {
        o->paintItem(&painter, img2widget, size());
    }

}

void ImageWidget::resizeEvent(QResizeEvent *) {
    setTranslation(_translation);
    update();
}

void ImageWidget::mousePressEvent(QMouseEvent *e) {

    QGuiApplication* gapp = qGuiApp;

    if (gapp != nullptr) {
        Qt::KeyboardModifiers kmods = gapp->keyboardModifiers();

        if (kmods & Qt::ControlModifier) {
            _control_pressed_when_clicked = true;
        } else {
            _control_pressed_when_clicked = false;
        }
    }

    _previously_pressed = e->buttons();

    if (_previously_pressed == Qt::LeftButton or _previously_pressed == Qt::MiddleButton) {
        _motion_origin_pos = e->pos();
        _motion_origin_t = translation();

        e->accept();
    } else {
        e->ignore();
    }

}
void ImageWidget::mouseReleaseEvent(QMouseEvent *e) {

    _previously_pressed = e->buttons();
    e->ignore();
}

void ImageWidget::mouseMoveEvent(QMouseEvent *e) {

    if (_currentImageDataBlock == nullptr) {
        return;
    }

    if (_previously_pressed == Qt::MiddleButton) {
        QPoint delta_t = e->pos() - _motion_origin_pos;
        setTranslation(_motion_origin_t + delta_t);
        e->accept();
    } else {
        e->ignore();
    }

}

void ImageWidget::wheelEvent(QWheelEvent *e) {

    QPoint d = e->angleDelta();
    if (d.y() == 0){
        e->ignore();
        return;
    }

    if (e->buttons() == Qt::NoButton) {
        int delta = d.y()*zoom()/500;
        if (delta == 0) {
            delta = (d.y() > 0) - (d.y() < 0);
        }
        int old_zoom = zoom();
        int n_zoom = clipZoom(old_zoom + delta);

        QPoint mousePos = e->position().toPoint();
        QPoint oldTranslation = translation();

        QPoint wCenter(width()/2, height()/2);

        QPoint imPt = mousePos - wCenter - oldTranslation;

        imPt.rx() *= n_zoom;
        imPt.rx() /= old_zoom;

        imPt.ry() *= n_zoom;
        imPt.ry() /= old_zoom;

        _translation = mousePos - wCenter -imPt;

        setZoom(n_zoom);

        e->accept();
    } else {
        e->ignore();
    }

}

} //namespace QImageDisplay
