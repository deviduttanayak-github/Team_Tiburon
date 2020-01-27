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

//frame=imread(("/home/devidutta/Downloads/tasks/frames/red_flare/frame_62.png"),IMREAD_COLOR);
//VideoCapture cap("/home/devidutta/catkin_ws/src/task5/robosub_path-1.mp4");
namedWindow("view");
int count=1;
while(1){

 std_msgs::String msg;

    std::stringstream ss;
    ss << "/home/devidutta/Downloads/tasks/frames/gate/frame_" << count++<<".png";
    msg.data = ss.str();

    ROS_INFO("%d", count);

frame=imread(msg.data.c_str(),IMREAD_COLOR);




cvtColor(frame, src, COLOR_BGR2HSV);
inRange(src, Scalar(0,100,0), Scalar(60, 255, 255), frame_threshold);
imshow("hsv",frame_threshold);

/*
Mat element2 = getStructuringElement( MORPH_RECT,Size(3, 3 ),Point( 1,1 ) );
erode(frame_threshold,frame_threshold, element2 );

Mat element = getStructuringElement( MORPH_RECT,Size(13, 13 ),Point( 5,5 ) );
Mat element2 = getStructuringElement( MORPH_RECT,Size(7, 7 ),Point( 5,5 ) );
//GaussianBlur( frame_threshold,frame_threshold, Size( 3, 3 ), 0,0,BORDER_DEFAULT);
erode(frame_threshold,frame_threshold, element2 );
imshow("erode",frame_threshold);
GaussianBlur( frame_threshold,frame_threshold, Size( 3, 3 ), 0,0,BORDER_DEFAULT);
imshow("blur",frame_threshold);
dilate( frame_threshold, frame_threshold, element );
imshow("dilate",frame_threshold);
Canny( frame_threshold,grad, 100, 200, 3);
*/
ROS_INFO("I AM HERE");

Sobel(frame_threshold, grad_x, ddepth, 1, 0, (1*2)+1, 10, 0, BORDER_DEFAULT);
    Sobel(frame_threshold, grad_y, ddepth, 0, 1, (1*2)+1,10 , 0, BORDER_DEFAULT);
 convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);
    addWeighted(abs_grad_x, 1, abs_grad_y, 0, 0, grad);
 cvtColor(grad, cdst, COLOR_GRAY2BGR);

ROS_INFO("I AM HERE");
//imshow("hsv",src);

// Probabilistic Line Transform
    vector<Vec4i> linesP; // will hold the results of the detection
    HoughLinesP(grad, linesP, 1, CV_PI/180, 50, 50, 10 ); // runs the actual detection
    // Draw the lines
float a, miny=1000, maxy=0,minx=1000,maxx=0;

for( size_t i = 0; i < linesP.size() ; i++ ){
        Vec4i l = linesP[i];

	if (l[2]!=l[0])	{
	float slope=(float)(l[3]-l[1])/(l[2]-l[0]);
	a=-1*atan(slope)*(180/CV_PI);	
        //ROS_INFO("a= %f",a);

	}		
	else 
	a=90;

int r=abs(l[1]-l[3]);
if(r>80 && abs(a)>85)
   line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
if(r>85){	
float y=min(l[3],l[1]);
	miny=min(miny,y);
float y2=max(l[3],l[1]);
	maxy=max(maxy,y2);
float x=min(l[0],l[2]);
minx=min(minx,x);
float x2=max(l[0],l[2]);
maxx=max(maxx,x2);      
}
    }

line( frame, Point(minx,miny), Point(maxx, miny), Scalar(0,255,0), 3, LINE_AA);
line( frame, Point(minx, miny), Point(minx,maxy), Scalar(0,255,0), 3, LINE_AA);
line( frame, Point(minx, maxy), Point(maxx, maxy), Scalar(0,255,0), 3, LINE_AA);
line( frame, Point(maxx, miny), Point(maxx, maxy), Scalar(0,255,0), 3, LINE_AA);
ROS_INFO("I AM HERE");

//imshow("linep",cdst);
imshow("final",frame);

imshow("view",grad);
waitKey(10);

int key=waitKey(75);
if(key==27)break;

//waitKey(0);

int k2=waitKey(500);
if(k2=='p')
return 0;

}
return 0;
}


