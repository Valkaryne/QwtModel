#ifndef FRONTEND_HPP
#define FRONTEND_HPP

#include <QMainWindow>

namespace Ui {
class FrontEnd;
}

class FrontEnd : public QMainWindow
{
    Q_OBJECT

public:
    explicit FrontEnd(QWidget *parent = 0);
    ~FrontEnd();

private:
    Ui::FrontEnd *ui;
};

#endif // FRONTEND_HPP
