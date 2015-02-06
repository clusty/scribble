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
   //threshold( grad, grad, 50, 255,3 );
   cv::subtract(cv::Scalar::all(255),grad,grad);
   //adaptiveThreshold(grad, grad, 255,ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 5 );
   threshold( grad, grad, 0, 255,THRESH_BINARY+THRESH_OTSU );
   
   
   return ASM::cvMatToQImage(grad).convertToFormat(QImage::Format_RGB32);
}

inline void Cost( const QImage &qSrc, cv::Mat &grad_x, cv::Mat &grad_y )
{
   using namespace cv;
   Mat src = ASM::QImageToCvMat( qSrc );
   cvtColor( src, src, COLOR_RGB2GRAY );
   GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

   Sobel( src, grad_x, CV_32F, 1, 0, 3, 1, 0, BORDER_DEFAULT ); // Scharr
   Sobel( src, grad_y, CV_32F, 0, 1, 3, 1, 0, BORDER_DEFAULT );
}

#endif // IMAGEPROC_H
