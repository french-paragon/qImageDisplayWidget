#ifndef QIMAGEDISPLAY_IMAGEWINDOW_H
#define QIMAGEDISPLAY_IMAGEWINDOW_H

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

    QString _lastFileSaveDir;
};


} // namespace QImageDisplay
#endif // QIMAGEDISPLAY_IMAGEWINDOW_H
