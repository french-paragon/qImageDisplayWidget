#include "imagewindow.h"
#include "ui_imagewindow.h"

#include "imagewidget.h"
#include "imageadapter.h"

#include <QEvent>
#include <QMouseEvent>

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
    ui(new Ui::ImageWindow)
{
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
