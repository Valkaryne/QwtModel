#include "plot.hpp"
#include <algorithm>

Plot::Plot(QWidget *parent, const QString objectName)
    : QwtPlot(parent)
{
    setObjectName(objectName);
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

    curveMax = new QwtPlotCurve();
    curveMax->setStyle(QwtPlotCurve::NoCurve);
    curveMax->setSymbol(new QwtSymbol(QwtSymbol::Ellipse,
                                      QBrush(Qt::gray), QPen(Qt::gray), QSize(2,2)));
    curveMax->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveMax->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    curveMax->attach(this);

    /* Pickers */
    pickerMarkPr = new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
                QwtPlotPicker::VLineRubberBand,
                QwtPicker::AlwaysOn,
                canvas);
    pickerMarkPr->setRubberBandPen(QColor(Qt::red));
    pickerMarkPr->setTrackerPen(QColor(Qt::white));
    pickerMarkPr->setStateMachine(new QwtPickerDragPointMachine());
    pickerMarkPr->setMousePattern(QwtPlotPicker::MouseSelect1, Qt::LeftButton);

    pickerMarkSec = new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
                QwtPlotPicker::VLineRubberBand,
                QwtPicker::AlwaysOn,
                canvas);
    pickerMarkSec->setRubberBandPen(QColor(Qt::red));
    pickerMarkSec->setTrackerPen(QColor(Qt::white));
    pickerMarkSec->setStateMachine(new QwtPickerDragPointMachine());
    pickerMarkSec->setMousePattern(QwtPlotPicker::MouseSelect1, Qt::RightButton);

    pickerThrPr = new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
                QwtPlotPicker::VLineRubberBand,
                QwtPicker::AlwaysOn,
                canvas);
    pickerThrPr->setRubberBandPen(QColor(Qt::green));
    pickerThrPr->setTrackerPen(QColor(Qt::white));
    pickerThrPr->setStateMachine(new QwtPickerDragPointMachine());
    pickerThrPr->setMousePattern(QwtPlotPicker::MouseSelect1, Qt::LeftButton);

    pickerThrSec = new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
                QwtPlotPicker::HLineRubberBand,
                QwtPicker::AlwaysOn,
                canvas);
    pickerThrSec->setRubberBandPen(QColor(Qt::green));
    pickerThrSec->setTrackerPen(QColor(Qt::white));
    pickerThrSec->setStateMachine(new QwtPickerDragPointMachine());
    pickerThrSec->setMousePattern(QwtPlotPicker::MouseSelect1, Qt::RightButton);

    /* Thresholds */
    thresholdPr = new QwtPlotMarker();
    thresholdPr->setLineStyle(QwtPlotMarker::HLine);
    thresholdPr->setLinePen(QColor(Qt::white), 2, Qt::DashLine);
    thresholdPr->setValue(0, 0);
    thresholdPr->attach(this);

    thresholdSec = new QwtPlotMarker();
    thresholdSec->setLineStyle(QwtPlotMarker::HLine);
    thresholdSec->setLinePen(QColor(Qt::white), 2, Qt::DashLine);
    thresholdSec->setValue(140, 0);
    thresholdSec->attach(this);

    /* Colors */
    colors = {
        *new QColor(220,20,75), /* crimson */
        *new QColor(11,218,81), /* malachite */
        *new QColor(127,199,255), /* skiey */
        *new QColor(255,153,0), /* orange */
        *new QColor(255,170,204), /* rose */
    };

    /* Zoomer */
    zoomer = new QwtPlotZoomer(canvas);
    zoomer->setRubberBandPen(QColor(Qt::darkGreen));
    zoomer->setTrackerMode(QwtPlotPicker::AlwaysOn);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,
                            Qt::RightButton, Qt::ControlModifier);

    /* Other settings */
    for (int i = 0; i < 4096; i++)
        maxSamples.append(0);

    this->cntrFrequency = 70;
    this->expCoeff = 0.996;

    for(int i = 1; i < 6; i++)
        setMarker(i);

    this->markPairNmr = 1;
}

void Plot::setMarker(int number)
{
    if (markerVector.size() < number * 2)
    {
        markerPr = new QwtPlotMarker();
        markerPr->setLineStyle(QwtPlotMarker::VLine);
        markerPr->setLinePen(colors.at((number - 1) % colors.size()), 2, Qt::SolidLine);
        markerPr->setValue(cntrFrequency - 15.45, 0);
        markerPr->attach(this);

        markerSec = new QwtPlotMarker();
        markerSec->setLineStyle(QwtPlotMarker::VLine);
        markerSec->setLinePen(colors.at((number - 1) % colors.size()), 2, Qt::SolidLine);
        markerSec->setValue(cntrFrequency + 15.27, 0);
        markerSec->attach(this);

        markerVector.append(markerPr);
        markerVector.append(markerSec);
    }
    markPairNmr = number * 2 - 1;
}

void Plot::setPickers(bool enable)
{
    pickerMarkPr->setEnabled(enable);
    pickerMarkSec->setEnabled(enable);
}

void Plot::setThresholdPickers(bool enable)
{
    pickerThrPr->setEnabled(enable);
    pickerThrSec->setEnabled(enable);
}

void Plot::setZoomer(bool enable)
{
    zoomer->setEnabled(enable);
}

void Plot::setCentralFrequency(double cntrFrequency)
{
    if (this->cntrFrequency != cntrFrequency)
    {
        this->cntrFrequency = cntrFrequency;
        resetMarkers();
    }
    double xleft = cntrFrequency - SHIFT;
    double xright = cntrFrequency + (30.72 - SHIFT);
    setAxisScale(QwtPlot::xBottom, xleft, xright);
    /* Set Zoom Base */
    QStack<QRectF> stack = zoomer->zoomStack();
    QRectF base;
    if (this->objectName() == "phasePlot")
        base = QRectF(QPointF(xleft, 0), QPointF(xright, 360));
    else
        base = QRectF(QPointF(xleft, 20), QPointF(xright, 140));
    stack.clear();
    stack.append(base);
    zoomer->setZoomStack(stack);
    zoomer->setZoomBase(base);
}

void Plot::setMaxHold(bool holdOn)
{
    if (holdOn)
        curveMax->attach(this);
    else
        curveMax->detach();
}

void Plot::setExpCoefficient(double expCoeff)
{
    if (this->expCoeff != expCoeff)
        this->expCoeff = expCoeff;
}

QwtPlotPicker* Plot::getMarkerPicker(bool prime)
{
    if (prime)
        return pickerMarkPr;
    else
        return pickerMarkSec;
}

QwtPlotPicker* Plot::getThreshPicker(bool prime)
{
    if (prime)
        return pickerThrPr;
    else
        return pickerThrSec;
}

QwtPlotZoomer* Plot::getZoomer()
{
    return zoomer;
}

QVector<int> Plot::getMarkerBounds()
{
    QVector<int> bounds;
    for (int i = 1; i < markerVector.size(); i += 2)
    {
        double bound = markerVector.value(i-1)->xValue();
        bounds.append(2060 + (bound - cntrFrequency) / INCR);
        bound = markerVector.value(i)->xValue();
        bounds.append(2060 + (bound - cntrFrequency) / INCR);
        qSort(bounds.end() - 2, bounds.end());
    }
    return bounds;
}

QVector<int> Plot::getThresholdBounds()
{
    QVector<int> bounds;
    bounds.append(thresholdPr->yValue());
    bounds.append(thresholdSec->yValue());
    qSort(bounds);
    return bounds;
}

void Plot::resetMarkers()
{
    for (int i = 1; i < markerVector.size(); i += 2)
    {
        markerVector.value(i - 1)->setValue(cntrFrequency - 15.45, 0);
        markerVector.value(i)->setValue(cntrFrequency + 15.27, 0);
    }
}

void Plot::moveMarker(double position, bool prime)
{
    if (prime)
        markerVector.at(markPairNmr - 1)->setValue(position, 0);
    else
        markerVector.at(markPairNmr)->setValue(position, 0);
}

void Plot::moveThreshold(double position, bool prime)
{
    if (prime)
        thresholdPr->setValue(0, position);
    else
        thresholdSec->setValue(0, position);
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

    for (int j = 0; j < samplesAm1.size(); j++)
    {
        if (samplesAm1.value(j) > maxSamples.value(j))
            maxSamples.replace(j, expCoeff * samplesAm1.value(j) + (1 - expCoeff) * maxSamples.value(j) + 5);
        else
            maxSamples.replace(j, (1 - expCoeff) * samplesAm1.value(j) + expCoeff * maxSamples.value(j));
    }

    curve1->setSamples(frequency, samplesAm1);
    curve2->setSamples(frequency, samplesAm2);
    curveMax->setSamples(frequency, maxSamples);
    replot();
}

void Plot::equalZoom(const QRectF &rect)
{
    setAxisScale(QwtPlot::xBottom, rect.bottomLeft().x(), rect.bottomRight().x());
}
