#include "image_threshing_tool/image_threshing_tool.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageThreshingTool w;
    w.show();

    return a.exec();
}
