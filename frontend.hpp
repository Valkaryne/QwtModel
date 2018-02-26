#ifndef FRONTEND_HPP
#define FRONTEND_HPP

#include <QMainWindow>
#include "plot.hpp"
#include "specwaterfall.hpp"
#include "polarplot.hpp"
#include "backend.hpp"
#include "pseudoudpchannel.hpp"

#include <QAbstractButton>

const QString serverIP = "192.168.159.2";
const QString clientIP = "192.168.159.10";
constexpr int serverPort = 8995;
constexpr int clientPort = 25000;

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
    /* Canvas signals */
    void leftClickOnCanvas(const QPoint &pos);
    void rightClickOnCanvas(const QPoint &pos);
    void thresholdPrChosen(const QPoint &pos);
    void thresholdSecChosen(const QPoint &pos);

    /* Buttons */
    void on_applyBtn_clicked();
    void on_setRangeBtn_clicked();
    void on_plotRspBtn_clicked(bool checked);
    void on_clearDiagramBtn_clicked();

    /* Button Groups */
    void on_buttonGroupMode_buttonClicked(QAbstractButton *button);
    void on_buttonGroupMarkers_buttonClicked(QAbstractButton *button);

    /* Other */
    void on_cbMaxHold_stateChanged(int arg1);
    void on_exponentSpinBox_valueChanged(double arg1);
    void on_isReal_stateChanged(int arg1);

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
