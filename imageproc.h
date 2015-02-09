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

inline void Cost( const QImage &qSrc, cv::Mat &lGrad, cv::Mat &pGrad, cv::Mat &laZ )
{
   using namespace cv;
   Mat src = ASM::QImageToCvMat( qSrc );
   cvtColor( src, src, COLOR_RGB2GRAY );
   GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

   cv::Mat grad_x, grad_y;
   
   Sobel( src, grad_x, CV_32F, 1, 0, 3, 1, 0, BORDER_DEFAULT ); // Scharr
   Sobel( src, grad_y, CV_32F, 0, 1, 3, 1, 0, BORDER_DEFAULT );
   
   cv::magnitude(grad_x, grad_y, lGrad);
   
   
   pGrad = lGrad.clone();
   
   for ( int i=0; i<pGrad.rows; ++i)
      for ( int j=0; j<pGrad.cols; ++j)
      {
         const float len = lGrad.at<float>(i,j);
         const float dx = grad_x.at<float>(i,j) / len;
         const float dy = grad_y.at<float>(i,j) / len;

         const float res = atan2(dy, dx) + M_PI;
         pGrad.at<float>(i,j) = res;
      }
   
   cv::add(cv::Scalar::all(M_PI),pGrad,pGrad);
   
   
   double minG, maxG;
   cv::minMaxLoc(lGrad, &minG, &maxG);

   // 1-G/max(G)
   cv::divide( lGrad,cv::Scalar::all(maxG), lGrad );
   cv::subtract(cv::Scalar::all(1),lGrad,lGrad);
   

   cv::Mat lapl;
   Laplacian( src, lapl, CV_32F, 3, 1, 0, BORDER_DEFAULT );
   
   // Create the iterators
   cv::Mat_<float>::const_iterator it= lapl.begin<float>()+lapl.step1();
   cv::Mat_<float>::const_iterator itend= lapl.end<float>();
   cv::Mat_<float>::const_iterator itup= lapl.begin<float>();

   // Binary image initialize to white
   cv::Mat binary(lapl.size(),CV_8U,cv::Scalar(255));
   cv::Mat_<uchar>::iterator itout= binary.begin<uchar>()+binary.step1();
   
   int thresh = -50;

   for ( ; it!= itend; ++it, ++itup, ++itout) {

      // if the product of two adjascent pixel is negative
      // then there is a sign change
      if (*it * *(it-1) < thresh)
         *itout= 0; // horizontal zero-crossing
      else if (*it * *itup < thresh)
         *itout= 0; // vertical zero-crossing
   }

   laZ = binary.clone();
   
}

#endif // IMAGEPROC_H
