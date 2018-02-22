#include "frontend.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setPalette(Qt::lightGray);
    FrontEnd w;
    w.show();

    return a.exec();
}
