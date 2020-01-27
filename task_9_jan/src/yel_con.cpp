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

//frame=imread(("/home/devidutta/Downloads/tasks/frames/yellow_flare/frame_0.png"),IMREAD_COLOR);
//VideoCapture cap("/home/devidutta/catkin_ws/src/task5/robosub_path-1.mp4");
namedWindow("view");
int count=21;
while(1){

 std_msgs::String msg;

    std::stringstream ss;
    ss << "/home/devidutta/Downloads/tasks/frames/yellow_flare/frame_" << count++<<".png";
    msg.data = ss.str();

    ROS_INFO("%d", count);

frame=imread(msg.data.c_str(),IMREAD_COLOR);



cvtColor(frame, src, COLOR_BGR2HSV);
inRange(src, Scalar(10, 129,0), Scalar(40, 255, 255), frame_threshold);
//imshow("hsv",frame_threshold);
Mat element = getStructuringElement( MORPH_RECT,Size(13, 13 ),Point( 5,5 ) );
//erode(frame_threshold,frame_threshold, element );
GaussianBlur( frame_threshold,frame_threshold, Size( 7, 7 ), 0,0,BORDER_DEFAULT);
//imshow("blur",frame_threshold);
dilate( frame_threshold, frame_threshold, element );
//imshow("dilate",frame_threshold);
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
drawContours(drawing,contours_poly,int(i),Scalar(0,255,0));
rectangle(frame,boundRect[i].tl(), boundRect[i].br(),Scalar(0,255,0));
}


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


/*
   HoughLinesP(grad, linesP, 1, CV_PI/180, 50, 50, 10 ); // runs the actual detection

    // Draw the lines
float maxx=0, minx=1000,dymin=1000,dymax=0;
float meany,t;
int c=0,flag=1,k=0;

for( size_t i = 0; i < linesP.size() ; i++ ){
        Vec4i l = linesP[i];
	Vec4i l2 = lin[i];
ROS_INFO("-----###------");

for(size_t j=0; j<lin.size(); j++){
ROS_INFO("-----###------");
	Vec4i l2 = lin[j];	
if(abs(l[0]-l2[0]) >15 )
flag=flag*1;
else{
flag=flag*-1;
break;
}
}
ROS_INFO("-----###------");
   
c++;
ROS_INFO("-----###------");
if(flag=1){
//lin[k++]=linesP[i];


 Vec4i l1 = linesP[i];
lin[k++]={l1[0],l1[1],l[2],l[3]};

l1[0]=l2[0];
l1[1]=l2[1];
l1[2]=l2[2];
l1[3]=l2[3];


}

ROS_INFO("-----###------");



float x=min(l[0],l[2]);	
minx=min(minx,x);
x=max(l[0],l[2]);	
maxx=max(maxx,x);

 t=abs(l[1]-l[3]);
meany=(l[1]+l[3])/2;

dymin=min(dymin,t);
dymax=max(dymax,t);
if(flag==1)
  line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);

ROS_INFO("-----###------");
flag=1;	      
    }



ROS_INFO("-----%d------",c,k);

 line( frame, Point(minx,(meany+ dymin*0.5)), Point(minx,(meany-dymin*0.5)), Scalar(0,255,0), 3, LINE_AA);
line( frame, Point((minx+50),(meany+ dymin*0.5)), Point((minx+50),(meany-dymin*0.5)), Scalar(0,255,0), 3, LINE_AA);
line( frame, Point(minx,(meany+ dymin*0.5)), Point((minx+50),(meany+dymin*0.5)), Scalar(0,255,0), 3, LINE_AA);
line( frame, Point(minx,(meany- dymin*0.5)), Point((minx+50),(meany-dymin*0.5)), Scalar(0,255,0), 3, LINE_AA);

*/
