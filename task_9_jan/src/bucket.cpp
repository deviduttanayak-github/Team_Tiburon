#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include<sstream>
#include <iostream>
#include<math.h>
#include"opencv2/imgcodecs.hpp"
#include"opencv2/imgproc.hpp"
#include"opencv2/imgproc.hpp"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>



using namespace cv;
using namespace std;

RNG rng(12345);

Mat frame, src,frame_HSV, frame_threshold,cdst,grad_x,grad_y,abs_grad_x,abs_grad_y,grad;
int ddepth = CV_16S;


int main(int argc, char **argv){

//frame=imread(("/home/devidutta/Downloads/tasks/frames/buckets/frame_75.png"),IMREAD_COLOR);
//VideoCapture cap("/home/devidutta/catkin_ws/src/task5/robosub_path-1.mp4");
namedWindow("view");
int count=50;
while(1){

 //std_msgs::String msg;

    std::stringstream ss;
    ss << "/home/devidutta/Downloads/tasks/frames/buckets/frame_" << count++<<".png";
    //msg.data = ss.str();

    ROS_INFO("%d", count);

//frame=imread(msg.data.c_str(),IMREAD_COLOR);
frame=imread(ss.str(),IMREAD_COLOR);


cvtColor(frame, src, COLOR_BGR2HSV);

inRange(src, Scalar(0, 80,0), Scalar(15, 255, 255), frame_threshold);

//imshow("hsv",frame_threshold);
Mat element = getStructuringElement( MORPH_RECT,Size(7, 7 ),Point( 1,1 ) );

//erode(frame_threshold,frame_threshold, element );
//GaussianBlur( frame_threshold,frame_threshold, Size( 7, 7 ), 0,0,BORDER_DEFAULT);
//imshow("blur",frame_threshold);
dilate( frame_threshold, frame_threshold, element );
//imshow("dilate",frame_threshold);

GaussianBlur( frame_threshold,frame_threshold, Size( 7, 7 ), 0,0,BORDER_DEFAULT);

Canny( frame_threshold,grad, 100, 200, 3);


/*
Sobel(frame_threshold, grad_x, ddepth, 1, 0, (1*2)+1, 10, 0, BORDER_DEFAULT);
    Sobel(frame_threshold, grad_y, ddepth, 0, 1, (1*2)+1,10 , 0, BORDER_DEFAULT);
 convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);
    addWeighted(abs_grad_x, 1, abs_grad_y, 0, 0, grad);
 cvtColor(grad, cdst, COLOR_GRAY2BGR);

*/
//imshow("hsv",src);


// Probabilistic Line Transform
 //  vector<Vec4i> linesP; // will hold the results of the detection


   vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

findContours(grad,contours, hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);
vector<vector<Point> > contours_poly(contours.size());
vector<Rect> boundRect(contours.size());
vector<Point2f> centres(contours.size());

for(size_t i=0;i<contours.size();i++){

approxPolyDP(contours[i],contours_poly[i],9,true);
boundRect[i]=boundingRect(contours_poly[i]);
}
Mat drawing=Mat::zeros(grad.size(), CV_8UC3);

for(size_t i=0;i<contours.size();i++){
drawContours(drawing,contours_poly,int(i),Scalar(0,255,255));
rectangle(frame,boundRect[i].tl(), boundRect[i].br(),Scalar(0,255,255));
}

ROS_INFO("I AM HERE");
//imshow("linep",cdst);
imshow("final",frame);

imshow("view",frame_threshold);
waitKey(10);

int key=waitKey(75);
if(key==27)break;

//waitKey(0);

waitKey(500);

}
return 0;
}

