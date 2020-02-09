#include "tiburon_gui/mainwin.h"
#include "ui_mainwin.h"
#include <iostream>

mainwin::mainwin(ros::NodeHandle _nh, QWidget *parent) : QMainWindow(parent), ui(new Ui::mainwin), nh(_nh){
  LOGO_PATH = "/home/devidutta/Pictures/tiburon.jpeg";
  VIDEO_PATH = "/home/devidutta/Downloads/tasks/videos/Gate_YFlare_Buckets.avi";

  ui->setupUi(this);
	timer = new QTimer(this);
     	connect(timer, SIGNAL(timeout()),this,SLOT(loop()));
    	timer->start(1);
	cap.open(VIDEO_PATH);
  	frame_save=cv::imread(LOGO_PATH);
  	cv::cvtColor(frame_save, frame_save, CV_BGR2RGB);
  	ui->logo->setPixmap(QPixmap::fromImage(
      	QImage(frame_save.data, frame_save.cols, frame_save.rows,
            frame_save.step, QImage::Format_RGB888)));

  connect(ui->gate, SIGNAL(pressed()), this, SLOT(Gate()));
  connect(ui->redBucket, SIGNAL(pressed()), this, SLOT(RedBucket()));
connect(ui->blueBucket, SIGNAL(pressed()), this, SLOT(BlueBucket()));
connect(ui->redFlare, SIGNAL(pressed()), this, SLOT(RedFlare()));
connect(ui->yellowFlare, SIGNAL(pressed()), this, SLOT(YellowFlare()));
connect(ui->pause, SIGNAL(pressed()), this, SLOT(pau()));
 

}

mainwin::~mainwin() { delete ui; }
void mainwin::loop()
{
  //cv::Mat src;
  if( cap.isOpened())
	{		 
		if(f!=1)		
		cap >> src;   
    		 if(!src.empty()) {
			if(f!=1)
 		 	cv::cvtColor(src, src, CV_BGR2RGB);
			ui->vid->setPixmap(QPixmap::fromImage(
			QImage(src.data, src.cols, src.rows,
				    src.step, QImage::Format_RGB888)));
			if(gateui>0)
				gateui->feed(src);
			if(yellowflareui>0)
				yellowflareui->feed(src);
                        if(bluebucketui>0)
				bluebucketui->feed(src);
                        if(redbucketui>0)
				redbucketui->feed(src);
			if(redflareui>0)
				redflareui->feed(src);


		}

	}
}

void mainwin::pau(){
if(f==0) f=1;
else f=0;
}

void mainwin::Gate()
{
	if(gateui>0){
	gateui->close();
	gateui=NULL;
	}else{
	gateui=new gate();
	gateui->show();
	}
}
void mainwin::RedBucket()
{
	if(redbucketui>0){
	redbucketui->close();
	redbucketui=NULL;
	}else{

	redbucketui=new redbucket();
	redbucketui->show();
	}
}
void mainwin::BlueBucket()
{
	if(bluebucketui>0){
	bluebucketui->close();
	bluebucketui=NULL;
	}else{
	bluebucketui=new bluebucket();
	bluebucketui->show();
	}
}void mainwin::RedFlare()
{
	if(redflareui>0){
	redflareui->close();
	redflareui=NULL;
	}else{
	redflareui=new redflare();
	redflareui->show();
	}
}void mainwin::YellowFlare()
{
	if(yellowflareui>0){
	yellowflareui->close();
	yellowflareui=NULL;
	}else{
	yellowflareui=new yellowflare();
	yellowflareui->show();
	}
}
