#ifndef IMAGEPROC_H
#define IMAGEPROC_H
#include "cv2qt.h"
#include "graph.h"

#include <queue>  
#include <limits>
#include <vector>
inline QImage Sobel(const QImage &src)
{
   using namespace cv;
   Mat cvSrc = ASM::QImageToCvMat( src );
   GaussianBlur( cvSrc, cvSrc, Size(3,3), 0, 0, BORDER_DEFAULT );
   Mat src_gray;
   cvtColor( cvSrc, src_gray, COLOR_RGB2GRAY );
   
   
   
   Mat grad_x, grad_y;
   Mat abs_grad_x, abs_grad_y;
   Sobel( src_gray, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT );
   convertScaleAbs( grad_x, abs_grad_x );
   
   Sobel( src_gray, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT );
   convertScaleAbs( grad_y, abs_grad_y );
   Mat grad;
   addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
   cv::subtract(cv::Scalar::all(255),grad,grad);
   
   return ASM::cvMatToQImage(grad).convertToFormat(QImage::Format_RGB32);
}


#endif // IMAGEPROC_H
