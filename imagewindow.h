#ifndef QIMAGEDISPLAY_IMAGEWINDOW_H
#define QIMAGEDISPLAY_IMAGEWINDOW_H

#include <QMainWindow>

namespace QImageDisplay {

namespace Ui {
class ImageWindow;
}

class ImageWidget;
class ImageAdapter;

class ImageWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageWindow(QWidget *parent = nullptr);
    ~ImageWindow();

    ImageAdapter* currentImage() const;
    void setImage(ImageAdapter* img);

private:
    Ui::ImageWindow *ui;
};


} // namespace QImageDisplay
#endif // QIMAGEDISPLAY_IMAGEWINDOW_H
