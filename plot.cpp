#include "plot.hpp"

Plot::Plot(QWidget *parent)
    : QwtPlot(parent)
{
    /* Canvas */
    QwtPlotCanvas *canvas = new QwtPlotCanvas;
    canvas->setBorderRadius(10);
    setCanvas(canvas);
    setCanvasBackground(QColor(29, 100, 141));

    /* Grid */
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->setMajorPen(Qt::white,0,Qt::DotLine);
    grid->attach(this);

    /* Axis */
    setAxisTitle(QwtPlot::xBottom, "Frequency, MHz");
    setAxisTitle(QwtPlot::yLeft, "Power");
    setAxisScale(QwtPlot::yLeft, 20, 140, 20);
    setAxisScale(QwtPlot::xBottom, 54.55, 85.27, 3);
    setAxisAutoScale(QwtPlot::xBottom, false);
    setAxisAutoScale(QwtPlot::yLeft, false);

    /* Curves */
    curve1 = new QwtPlotCurve();
    curve1->setStyle(QwtPlotCurve::Lines);
    curve1->setPen(Qt::yellow);
    curve1->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve1->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    curve1->attach(this);

    curve2 = new QwtPlotCurve();
    curve2->setStyle(QwtPlotCurve::Lines);
    curve2->setPen(Qt::cyan);
    curve2->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve2->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    curve2->attach(this);

    /* Other settings */

    this->cntrFrequency = 70;
}

void Plot::updateCurve(const QVector<double> &samplesPh)
{
    int size = samplesPh.size();
    QVector<double> frequency;

    for (double i = cntrFrequency - SHIFT; i < size * INCR + (cntrFrequency - SHIFT); i += INCR)
        frequency.push_back(i);

    curve1->setSamples(frequency, samplesPh);
    replot();
}

void Plot::updateCurve(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2)
{
    int size = samplesAm1.size();
    QVector<double> frequency;

    for (double i = cntrFrequency - SHIFT; i < size * INCR + (cntrFrequency - SHIFT); i += INCR)
        frequency.push_back(i);

    curve1->setSamples(frequency, samplesAm1);
    curve2->setSamples(frequency, samplesAm2);
    replot();
}
