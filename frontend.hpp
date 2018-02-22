#ifndef FRONTEND_HPP
#define FRONTEND_HPP

#include <QMainWindow>
#include "plot.hpp"
#include "specwaterfall.hpp"
#include "polarplot.hpp"
#include "backend.hpp"
#include "pseudoudpchannel.hpp"

namespace Ui {
class FrontEnd;
}

class FrontEnd : public QMainWindow
{
    Q_OBJECT

public:
    explicit FrontEnd(QWidget *parent = 0);
    ~FrontEnd();

private slots:

private:
    Ui::FrontEnd    *ui;
    BackEnd         *backEnd;
    Plot            *spectrumPlot;
    Plot            *phasePlot;
    SpecWaterfall   *spectrumWaterfall;
    PolarPlot       *polarPlot;
    PseudoUdpChannel    *pseudoUdpChannel;
};

#endif // FRONTEND_HPP
