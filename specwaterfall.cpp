#include "specwaterfall.hpp"

class ColorMap : public QwtLinearColorMap
{
public:
    ColorMap():
        QwtLinearColorMap(Qt::blue, Qt::red)
    {
        addColorStop(0.5, Qt::green);
    }
};

SpecWaterfall::SpecWaterfall(QWidget *parent) :
    QwtPlot(parent)
{
    /* Canvas */
    QwtPlotCanvas *canvas = new QwtPlotCanvas;
    canvas->setBorderRadius(10);
    setCanvas(canvas);

    /* Axis */
    enableAxis(QwtPlot::yLeft, false);
    setAxisAutoScale(QwtPlot::yLeft, false);
    setAxisAutoScale(QwtPlot::xBottom, false);
    setAxisScale(QwtPlot::yLeft, 100, 0, 20);
    setAxisScale(QwtPlot::xBottom, 54.55, 85.27, 4);

    /* Plot */
    plot = new QwtPlotSpectrogram();
    plot->setRenderThreadCount(0);
    plot->setColorMap(new ColorMap());

    /* Plot Data */
    plotData = new QwtMatrixRasterData();
    plotData->setResampleMode(QwtMatrixRasterData::ResampleMode::BilinearInterpolation);
    plotData->setInterval(Qt::XAxis, QwtInterval(0, 4095, QwtInterval::ExcludeMaximum));
    plotData->setInterval( Qt::YAxis, QwtInterval(0, 100, QwtInterval::ExcludeMaximum));
    plotData->setInterval(Qt::ZAxis, QwtInterval(0, 100));

    const QwtInterval interval(0, 100);
    cntrFrequency = 70;

    /* Color bar */
    QwtScaleWidget *leftAxis = axisWidget(QwtPlot::yLeft);
    leftAxis->setTitle("Power");
    leftAxis->setColorBarEnabled(true);

    setAxisScale(QwtPlot::yLeft, 0, 100);
    enableAxis(QwtPlot::yLeft);

    leftAxis->setColorMap(interval, new ColorMap());

    vec = new QVector<double>(102400);

    for (int i = 0; i < 102400; i++)
        vec->append(interval.minValue());

    plotData->setValueMatrix(*vec, 1024);
    plot->setData(plotData);
    plot->attach(this);
}

void SpecWaterfall::setCentralFrequency(double cntrFrequency)
{
    if (this->cntrFrequency != cntrFrequency)
        this->cntrFrequency = cntrFrequency;

    double xleft = cntrFrequency - LSHIFT;
    double xright = cntrFrequency + RSHIFT;
    setAxisScale(QwtPlot::xBottom, xleft, xright);
}

void SpecWaterfall::updateSpectrogram(const QVector<double> samplesAm1, const QVector<double> samplesAm2)
{
    vec->remove(0, samplesAm1.size() / 4);

    for (int i = 0; i < samplesAm1.size(); i += 4)
        vec->append(samplesAm1.at(i));

    plotData->setInterval(Qt::XAxis, QwtInterval(cntrFrequency - LSHIFT, cntrFrequency + RSHIFT, QwtInterval::ExcludeMaximum));
    plotData->setValueMatrix(*vec, 1024);
    plot->setData(plotData);
    replot();
}
