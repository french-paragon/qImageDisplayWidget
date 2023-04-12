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

#include "imagewindow.h"
#include "ui_imagewindow.h"

#include "imagewidget.h"
#include "imageadapter.h"

#include <QEvent>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

inline void initGuiResources() { Q_INIT_RESOURCE(icons); }

namespace QImageDisplay {

class ImageWindowMouseEventFilter : public QObject {
    Q_OBJECT
public:
    ImageWindowMouseEventFilter(QObject* parent) :
        QObject(parent)
    {

    }

Q_SIGNALS:
    void moved(QPoint widgetPos);

protected:

    bool eventFilter(QObject *watched, QEvent *event) override {

        if (event->type() != QEvent::MouseMove) {
            return false;
        }

        QMouseEvent* mEvent = dynamic_cast<QMouseEvent*>(event);

        QPoint wpos = mEvent->pos();
        Q_EMIT moved(wpos);

        return false;

    }


};

ImageWindow::ImageWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageWindow),
    _lastFileSaveDir("")
{

    initGuiResources();

    ui->setupUi(this);

    ui->imageWidget->setMouseTracking(true);

    ImageWindowMouseEventFilter* mouseEventFilter = new ImageWindowMouseEventFilter(this);
    ui->imageWidget->installEventFilter(mouseEventFilter);

    connect(mouseEventFilter, &ImageWindowMouseEventFilter::moved, this, [this] (QPoint wPos) {

        QPoint imagePos = ui->imageWidget->widgetToImageCoordinates(wPos).toPoint();

        QRect area(QPoint(0,0), ui->imageWidget->currentImage()->getImageSize());

        if (!area.contains(imagePos)) {
            ui->statusbar->showMessage("");
            return;
        }

        QString tmpl = (tr("Position x:%1 y:%2 | %3"));

        auto descr = ui->imageWidget->currentImage()->getOriginalChannelsInfos(imagePos);
        QString valueStr = "";

        if (descr.empty()) {
            QColor col = ui->imageWidget->currentImage()->getColorAtPoint(imagePos);
            valueStr = QString("r:%1 g:%2 b:%3").arg(col.red()).arg(col.green()).arg(col.blue());
        } else {
            for (auto const& info : descr) {
                valueStr += QString("%1:%2").arg(info.channelName).arg(info.channelValue);
            }
        }

        QString message = tmpl.arg(imagePos.x()).arg(imagePos.y()).arg(valueStr);

        ui->statusbar->showMessage(message);

    });

    connect(ui->actionsave_image, &QAction::triggered, this, [this] () {

        if (ui->imageWidget->currentImage() == nullptr) {
            return;
        }

        QString saveDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);

        if (!_lastFileSaveDir.isEmpty()) {
            saveDir = _lastFileSaveDir;
        }

        QString filter= tr(" bmp (*.bmp);; jpg (*.jpg *.jpeg);; png (*.png);; tiff (*.tif *.tiff);;");

        QString filePath = QFileDialog::getSaveFileName(this, tr("Save image as"), saveDir, filter);

        if (filePath.isEmpty()) {
            return;
        }

        QFileInfo infos(filePath);

        _lastFileSaveDir = infos.absoluteDir().path();

        QString saveFilePath = infos.absoluteFilePath();

        QImage img = ui->imageWidget->currentImage()->getImage();

        bool ok = img.save(saveFilePath);

        if (!ok) {
            QMessageBox::warning(this, tr("Could not save image"), tr("An error occured while saving the image"));
        }

    });
}

ImageWindow::~ImageWindow()
{
    delete ui;
}

ImageAdapter* ImageWindow::currentImage() const {
    return ui->imageWidget->currentImage();
}
void ImageWindow::setImage(ImageAdapter* img) {
    ui->imageWidget->setImage(img);
}

} // namespace QImageDisplay

#include "imagewindow.moc"
