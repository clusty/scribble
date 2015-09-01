#include <QtWidgets>

#include "scribblearea.h"

#include "imageproc.h"

static bool displayData = false;

ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    myPenWidth = 1;
    myPenColor = Qt::blue;
}

bool ScribbleArea::openImage(const QString &fileName)
{
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    dataImage = Sobel(loadedImage);
   
    //dataImage = ASM::cvMatToQImage(viewM).convertToFormat(QImage::Format_RGB32);
    displayImage = dataImage;
    update();
    
    g = Graph(dataImage.width(), dataImage.height(), &dataImage );
    update();
    return true;
}
bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = displayImage;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        return true;
    } else {
        return false;
    }
}

void ScribbleArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void ScribbleArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void ScribbleArea::clearImage()
{
    displayImage.fill(qRgb(255, 255, 255));

    update();
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos()-QPoint(1, 1);
        g.setStart(std::make_pair(lastPoint.x(), lastPoint.y()));
        path.insert(path.end(), currpath.rbegin(), currpath.rend());
        currpath.clear();
    }
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, displayImage, dirtyRect);
    update();
}

void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if (width() > displayImage.width() || height() > displayImage.height()) {
        int newWidth = qMax(width() + 128, displayImage.width());
        int newHeight = qMax(height() + 128, displayImage.height());
        resizeImage(&displayImage, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}


void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

bool ScribbleArea::eventFilter(QObject *, QEvent *event)
{
   if (event->type() == QEvent::MouseMove && 
       QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
   {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
      //qDebug() << "x: "<<mouseEvent->x()<<"y: "<<mouseEvent->y();
      
      displayImage = displayData ? dataImage : loadedImage; 
      Graph::Location start, target;
      target.first = mouseEvent->x(); target.second = mouseEvent->y();
      start.first = lastPoint.x(); start.second = lastPoint.y();
      g.setStart(start);
      currpath = g.aStar(target );      
      
      QPainter painter(&displayImage);
      
      if ( !currpath.empty())
      {
         painter.setPen(QPen(QColor(255,0,0), 2, Qt::SolidLine, Qt::RoundCap,
                             Qt::RoundJoin));
         
         std::vector<Graph::Location>::const_iterator it = currpath.begin();
         QPoint a = QPoint( it->first, it->second );
         for ( std::vector<Graph::Location>::const_iterator it = currpath.begin();
               it != currpath.end(); ++it)
         {
            QPoint b(it->first, it->second);
            painter.drawLine(a,b);
            a = b;
         }
      }
      
      if ( !path.empty())
      {
         painter.setPen(QPen(QColor(0,255,0), 2, Qt::SolidLine, Qt::RoundCap,
                             Qt::RoundJoin));
         
         std::vector<Graph::Location>::const_iterator it = path.begin();
         QPoint a = QPoint( it->first, it->second );
         for ( std::vector<Graph::Location>::const_iterator it = path.begin();
               it != path.end(); ++it)
         {
            QPoint b(it->first, it->second);
            painter.drawLine(a,b);
            a = b;
         }
      }
      
      update();
   }
   else if ( event->type() == QEvent::KeyPress &&
             QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier) )
   {
      qDebug()<<"Shift";   
      displayData = !displayData;
      update();
      return true;
   }
   
   return false;
}
