#ifndef FRONTEND_HPP
#define FRONTEND_HPP

#include <QMainWindow>
#include "plot.hpp"
#include "specwaterfall.hpp"

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
    Ui::FrontEnd    *ui;
    Plot            *spectrumPlot;
    Plot            *phasePlot;
    SpecWaterfall   *spectrumWaterfall;
};

#endif // FRONTEND_HPP
