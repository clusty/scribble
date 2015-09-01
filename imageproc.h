#ifndef IMAGEPROC_H
#define IMAGEPROC_H
#include "cv2qt.h"
#include "graph.h"

#include <queue>  
#include <limits>
#include <vector>

cv::Mat mat2gray(const cv::Mat& src)
{
   using namespace cv;
    Mat dst;
    normalize(src, dst, 0.0, 255.0, cv::NORM_MINMAX, CV_8U);

    return dst;
}



inline QImage Sobel(const QImage &src)
{
   using namespace cv;
   Mat cvSrc = ASM::QImageToCvMat( src );
   GaussianBlur( cvSrc, cvSrc, Size(11,11), 0, 0, BORDER_DEFAULT );
   Mat src_gray;
   cvtColor( cvSrc, src_gray, COLOR_RGB2GRAY );
   
   
   
   Mat grad_x, grad_y;
   Mat abs_grad_x, abs_grad_y;
   Sobel( src_gray, grad_x, CV_32F, 1, 0, 3, 1, 1, BORDER_DEFAULT );
   //convertScaleAbs( grad_x, abs_grad_x );
   
   Sobel( src_gray, grad_y, CV_32F, 0, 1, 3, 1, 1, BORDER_DEFAULT );
   //convertScaleAbs( grad_y, abs_grad_y );
   Mat gradm;
   
   magnitude(grad_x, grad_y, gradm);
   Mat gradf;
   phase(grad_x, grad_y, gradf, true);
   //addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
   //threshold( grad, grad, 50, 255,3 );
   //cv::subtract(cv::Scalar::all(255),grad,grad);
   
   double max;
   minMaxLoc( gradm,0, &max );
   divide(gradm, cv::Scalar::all(max), gradm);
   cv::subtract(cv::Scalar::all(1),gradm,gradm);
   Mat lapl;
   Laplacian( src_gray, lapl, CV_32F, 3, 1, 0, BORDER_DEFAULT );
   
   
   //convertScaleAbs( lapl, lapl );
   //cv::subtract(cv::Scalar::all(255),lapl,lapl);
         
   
   Mat result;
   //cv::addWeighted(grad, 0.3, lapl, 0.0, 0,  result, CV_32F);
   result = mat2gray(gradm);
   //convertScaleAbs( grad, result );
  // cv::subtract(cv::Scalar::all(255),result,result);
   //adaptiveThreshold(grad, grad, 255,ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 5 );
   // threshold( result, result, 0, 255,THRESH_BINARY+THRESH_OTSU );
   
   
   return ASM::cvMatToQImage(result).convertToFormat(QImage::Format_RGB32);
}


#endif // IMAGEPROC_H
