#ifndef IMAGE_THRESHING_TOOL_H
#define IMAGE_THRESHING_TOOL_H

#include <QMainWindow>

namespace Ui {
class ImageThreshingTool;
}

class ImageThreshingTool : public QMainWindow {
  Q_OBJECT

public:
  explicit ImageThreshingTool(QWidget *parent = 0);
  ~ImageThreshingTool();

private:
  Ui::ImageThreshingTool *ui;
};

#endif // IMAGE_THRESHING_TOOL_H
