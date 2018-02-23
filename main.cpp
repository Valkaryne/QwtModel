#include "frontend.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setPalette(QColor(190,245,116));
    FrontEnd w;
    w.show();

    return a.exec();
}
