
#include <iostream>

#include <QApplication>
#include <QImage>

#include "imagewidget.h"
#include "imageadapter.h"

class BasicImageAdapter : public QImageDisplay::ImageAdapter {

public:
    explicit BasicImageAdapter(QObject *parent = nullptr) :
        QImageDisplay::ImageAdapter(parent),
        _loadedImage()
    {

    }

    QSize getImageSize() const override {
        return _loadedImage.size();
    }

    QColor getColorAtPoint(int x, int y) const override {
        return _loadedImage.pixelColor(x,y);
    }

    QImage getImage() const override {
        return _loadedImage;
    }

    void setImage(QString imagePath) {
        QSize oldSize = _loadedImage.size();

        _loadedImage.load(imagePath);

        if (_loadedImage.size() != oldSize) {
            Q_EMIT imageSizeChanged(_loadedImage.size());
        }

        Q_EMIT imageValuesChanged(QRect());
    }

protected:

    QImage _loadedImage;
};

int main(int argc, char** argv) {

    QApplication app(argc, argv);

    if (argc != 2) {
        std::cout << "wrong number of argument provided" << std::endl;
        return 1;
    }

    BasicImageAdapter imageAdapter;
    imageAdapter.setImage(QString(argv[1]));

    QImageDisplay::ImageWidget widget;
    widget.setImage(&imageAdapter);

    widget.resize(800, 600);
    widget.show();

    return app.exec();
}
