/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "scribblearea.h"

#include "imageproc.h"

//! [0]
ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    scribbling = false;
    myPenWidth = 1;
    myPenColor = Qt::blue;
}
//! [0]

//! [1]
bool ScribbleArea::openImage(const QString &fileName)
//! [1] //! [2]
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);

    Cost(loadedImage, lGrad, pGrad, lapZ);
    cv::Mat viewM;
    cv::convertScaleAbs(lGrad, viewM, 255);
    dataImage = ASM::cvMatToQImage(viewM).convertToFormat(QImage::Format_RGB32);
    displayImage = dataImage;
    
    g = Graph(dataImage.width(), dataImage.height(), &lGrad, &pGrad, &lapZ );
    //g = Graph(dataImage.width(), dataImage.height(), gradx, grady );
    modified = false;
    update();
    return true;
}
//! [2]

//! [3]
bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
//! [3] //! [4]
{
    QImage visibleImage = displayImage;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}
//! [4]

//! [5]
void ScribbleArea::setPenColor(const QColor &newColor)
//! [5] //! [6]
{
    myPenColor = newColor;
}
//! [6]

//! [7]
void ScribbleArea::setPenWidth(int newWidth)
//! [7] //! [8]
{
    myPenWidth = newWidth;
}
//! [8]

//! [9]
void ScribbleArea::clearImage()
//! [9] //! [10]
{
    displayImage.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}
//! [10]

//! [11]
void ScribbleArea::mousePressEvent(QMouseEvent *event)
//! [11] //! [12]
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos()-QPoint(1, 1);
        scribbling = true;
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->pos(), myPenWidth);
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos(), myPenWidth);
        scribbling = false;
    }
}

//! [12] //! [13]
void ScribbleArea::paintEvent(QPaintEvent *event)
//! [13] //! [14]
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, displayImage, dirtyRect);
}
//! [14]

//! [15]
void ScribbleArea::resizeEvent(QResizeEvent *event)
//! [15] //! [16]
{
    if (width() > displayImage.width() || height() > displayImage.height()) {
        int newWidth = qMax(width() + 128, displayImage.width());
        int newHeight = qMax(height() + 128, displayImage.height());
        resizeImage(&displayImage, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}
//! [16]

//! [17]
void ScribbleArea::drawLineTo(const QPoint &endPoint, int width)
//! [17] //! [18]
{
    QPainter painter(&displayImage);
    painter.setPen(QPen(myPenColor, width, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    int rad = (width / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}
//! [18]

//! [19]
void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
//! [19] //! [20]
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}
//! [20]


bool ScribbleArea::eventFilter(QObject *, QEvent *event)
{
  if (event->type() == QEvent::MouseMove && 
      QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
  {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    //qDebug() << "x: "<<mouseEvent->x()<<"y: "<<mouseEvent->y();
    if (modified)
    {
      displayImage = dataImage; 
      Graph::Location start, target;
      std::vector<Graph::Location> path;
      start.first = mouseEvent->x(); start.second = mouseEvent->y();
      target.first = lastPoint.x(); target.second = lastPoint.y();
      path = aStar(g, start, target );
      
      QPainter painter(&displayImage);
      
      painter.setPen(QPen(QColor(255,0,0), 2, Qt::SolidLine, Qt::RoundCap,
                          Qt::RoundJoin));
      QPoint a = lastPoint;
      
      for ( std::vector<Graph::Location>::const_iterator it = path.begin();
            it != path.end(); ++it)
      {
         QPoint b(it->first, it->second);
         painter.drawLine(b, a);
         a = b;
      }
      
      update(QRect(QPoint(0,0), displayImage.size() ) );
    }
  }

  return false;
}
//! [22]
