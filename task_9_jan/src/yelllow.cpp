#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include<sstream>
#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include<math.h>

using namespace cv;
using namespace std;

Mat grad,cdst;
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
  const String window_name = "Sobel";
  int ksize = 0;
  int scale = 0;
  int delta = 0;
  int ddepth = CV_16S;

Mat frame, src,frame_HSV, frame_threshold,frame_threshold2,frame_threshold3, frame_threshold4,frame_threshold5;

const int max_value_H = 360/2;
const int max_value = 255;
const String window_detection_name = "thresholding";
int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;

int smt=0;
int w,h;

int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;

static void sobel(int, void*){

 Sobel(frame_threshold, grad_x, ddepth, 1, 0, (ksize*2)+1, scale, delta, BORDER_DEFAULT);
    Sobel(frame_threshold, grad_y, ddepth, 0, 1, (ksize*2)+1, scale, delta, BORDER_DEFAULT);


}

static void on_low_H_thresh_trackbar(int, void *)
{
    low_H = min(high_H-1, low_H);
    setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
    high_H = max(high_H, low_H+1);
    setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
    low_S = min(high_S-1, low_S);
    setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
    high_S = max(high_S, low_S+1);
    setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
    low_V = min(high_V-1, low_V);
    setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
    high_V = max(high_V, low_V+1);
    setTrackbarPos("High V",  window_detection_name, high_V);
}

void Erosion( int, void* )
{
  int erosion_type = 0;
  if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
  else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
  else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }
  Mat element = getStructuringElement( erosion_type,
                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                       Point( erosion_size, erosion_size ) );
  erode(frame_threshold,frame_threshold, element );
  imshow("ERODE", frame_threshold );
}
void Dilation( int, void* )
{
  int dilation_type = 0;
  if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }
  Mat element = getStructuringElement( dilation_type,
                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                       Point( dilation_size, dilation_size ) );
  dilate( frame_threshold, frame_threshold, element );
 imshow("DILATE", frame_threshold );
}

void smooth( int, void* ){
int i=2*smt+1;
GaussianBlur( frame_threshold,frame_threshold, Size( i, i ), 0,0,BORDER_DEFAULT);
imshow("smooth",frame_threshold);
}


int main(int argc, char **argv)
{
  //VideoCapture cap("/home/devidutta/catkin_ws/src/task6/pq_gate.avi");
 
 w=100;//cap.get(CV_CAP_FRAME_WIDTH); 
h=100;//cap.get(CV_CAP_FRAME_HEIGHT);

VideoWriter vd("/home/devidutta/catkin_ws/src/task5/saved.avi",CV_FOURCC('M','S','Z','H'),10,Size(w,h),true);  
namedWindow("view");
  namedWindow("test");
  namedWindow(window_detection_name);
 namedWindow("ERODE");
 namedWindow("DILATE");
namedWindow("smooth");

 // cv::startWindowThread();

createTrackbar("Low H",  window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
    createTrackbar("High H",  window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
    createTrackbar("Low S",  window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
    createTrackbar("High S",  window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
    createTrackbar("Low V",  window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
    createTrackbar("High V",  window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);
 namedWindow("sobel");
 createTrackbar( "k:", "sobel", &ksize, 15, sobel );
 createTrackbar( "s:", "sobel", &scale, 100, sobel );
 createTrackbar( "d:", "sobel", &delta, 100, sobel );

frame=imread(("/home/devidutta/Downloads/tasks/frames/buckets/frame_75.png"),IMREAD_COLOR);
 while(1)
 {

  //cap >> frame;
   imshow("view",frame);
ROS_INFO("I AM HERE");
  cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
   imshow("test",frame_HSV);

inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);

//ROS_INFO()frame_threshold=frame.clone();
ROS_INFO("I AM HERE");
imshow(window_detection_name, frame_threshold);
  
 


  createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "ERODE",
          &erosion_elem, max_elem,
          Erosion );
  createTrackbar( "Kernel size:\n 2n +1","ERODE" ,
          &erosion_size, max_kernel_size,
          Erosion );
Erosion(0,0);


 //imshow(window_detection_name, frame_threshold2);
createTrackbar("smoothing", "smooth", &smt,15, smooth);
smooth(0,0);
ROS_INFO("I AM HERE");

createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse",  "DILATE",
          &dilation_elem, max_elem,
          Dilation );
  createTrackbar( "Kernel size:\n 2n +1", "DILATE",
          &dilation_size, max_kernel_size,
          Dilation );
Dilation( 0, 0 );
//imshow("DILATE", frame_threshold3);

sobel(0,0);

    // converting back to CV_8U
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    imshow("sobel",grad );
    char key = (char)waitKey(10);
 cvtColor(grad, cdst, COLOR_GRAY2BGR);

// Probabilistic Line Transform
    vector<Vec4i> linesP; // will hold the results of the detection
    HoughLinesP(grad, linesP, 1, CV_PI/180, 50, 50, 10 ); // runs the actual detection
    // Draw the lines
float a, miny=1000, maxy=0,maxx=0;
int c=0;
for( size_t i = 0; i < linesP.size() ; i++ ){
        Vec4i l = linesP[i];
c++;
   line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
	      
    }

ROS_INFO("I AM HERE------ %d ",c);
	imshow("view",frame);
	imshow("linep",cdst);

  int k=waitKey(500);
//cvtColor(frame_threshold4, frame_threshold5, COLOR_HSV2BGR);
 vd.write(frame_threshold4);
if(k==27)break;
  }
//cap.release();
vd.release();  
return 0;
}


