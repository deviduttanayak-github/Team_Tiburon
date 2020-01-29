#include "image_threshing_tool/image_threshing_tool.h"
#include "ui_image_threshing_tool.h"

ImageThreshingTool::ImageThreshingTool(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ImageThreshingTool) {
  ui->setupUi(this);
}

ImageThreshingTool::~ImageThreshingTool() { delete ui; }
