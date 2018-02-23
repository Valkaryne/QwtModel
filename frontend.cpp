#include "frontend.hpp"
#include "ui_frontend.h"

FrontEnd::FrontEnd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FrontEnd)
{
    ui->setupUi(this);

    backEnd = new BackEnd();
    connect(this, SIGNAL(destroyed()), backEnd, SLOT(quit()));

    /* Create spectrum plot, phase plot and waterfall objects */

    QHBoxLayout *spLayout = new QHBoxLayout();
    QHBoxLayout *phLayout = new QHBoxLayout();

    spectrumWaterfall = new SpecWaterfall(this);
    spectrumWaterfall->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    ui->spectrumLayout->addWidget(spectrumWaterfall);

    spectrumPlot = new Plot(this, "spectrumPlot");
    spectrumPlot->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    ui->spectrumLayout->addSpacing(10);
    spLayout->addSpacing(12);
    spLayout->addWidget(spectrumPlot);
    ui->spectrumLayout->addLayout(spLayout);
    spectrumPlot->setPickers(false);
    spectrumPlot->setZoomer(true);
    spectrumPlot->setThresholdPickers(false);
    spectrumPlot->setObjectName("spectrumPlot");

    phasePlot = new Plot(this, "phasePlot");
    phasePlot->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    phLayout->addSpacing(12);
    phLayout->addWidget(phasePlot);
    ui->spectrumLayout->addSpacing(10);
    ui->spectrumLayout->addLayout(phLayout);
    phasePlot->setAxisTitle(QwtPlot::yLeft, "Phase");
    phasePlot->setAxisScale(QwtPlot::yLeft, 0, 360, 90);
    phasePlot->setPickers(false);
    phasePlot->setZoomer(false);
    phasePlot->setThresholdPickers(false);
    phasePlot->setObjectName("phasePlot");

    polarPlot = new PolarPlot(this);
    polarPlot->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    ui->polarLayout->addWidget(polarPlot);

    backEnd->moveToThread(backEnd);

    connect(backEnd, SIGNAL(amplitudeSamplesReady(QVector<double>,QVector<double>)),
            this->spectrumPlot, SLOT(updateCurve(QVector<double>,QVector<double>)));
    connect(backEnd, SIGNAL(amplitudeSamplesReady(QVector<double>,QVector<double>)),
            this->spectrumWaterfall, SLOT(updateSpectrogram(QVector<double>,QVector<double>)));
    connect(backEnd, SIGNAL(phaseSamplesReady(QVector<double>)),
            this->phasePlot, SLOT(updateCurve(QVector<double>)));

    backEnd->start();

    pseudoUdpChannel = new PseudoUdpChannel(10, this);
    connect(pseudoUdpChannel, SIGNAL(pseudoSamplesReceived(QVector<double>,QVector<double>,QVector<double>)),
            backEnd, SLOT(samplesHandler(QVector<double>,QVector<double>,QVector<double>)));

    connect(spectrumPlot->getMarkerPicker(true), SIGNAL(appended(const QPoint &)),
            this, SLOT(leftClickOnCanvas(const QPoint &)));
    connect(spectrumPlot->getMarkerPicker(false), SIGNAL(appended(const QPoint &)),
            this, SLOT(rightClickOnCanvas(const QPoint &)));
    connect(spectrumPlot->getThreshPicker(true), SIGNAL(appended(const QPoint &)),
            this, SLOT(thresholdPrChosen(const QPoint &)));
    connect(spectrumPlot->getThreshPicker(false), SIGNAL(appended(const QPoint &)),
            this, SLOT(thresholdSecChosen(const QPoint &)));
    connect(spectrumPlot->getZoomer(), SIGNAL(zoomed(const QRectF &)),
            phasePlot, SLOT(equalZoom(const QRectF &)));
}

FrontEnd::~FrontEnd()
{
    //delete backEnd;
    delete ui;
}

void FrontEnd::leftClickOnCanvas(const QPoint &pos)
{
    double xleft = spectrumPlot->invTransform(QwtPlot::xBottom, pos.x());
    spectrumPlot->moveMarker(xleft, true);
}

void FrontEnd::rightClickOnCanvas(const QPoint &pos)
{
    double xright = spectrumPlot->invTransform(QwtPlot::xBottom, pos.x());
    spectrumPlot->moveMarker(xright, false);
}

void FrontEnd::thresholdPrChosen(const QPoint &pos)
{
    double yup = spectrumPlot->invTransform(QwtPlot::yLeft, pos.y());
    spectrumPlot->moveThreshold(yup, true);
}

void FrontEnd::thresholdSecChosen(const QPoint &pos)
{
    double ylow = spectrumPlot->invTransform(QwtPlot::yLeft, pos.y());
    spectrumPlot->moveThreshold(ylow, false);
}

void FrontEnd::on_applyBtn_clicked()
{
    spectrumPlot->setCentralFrequency(ui->frequencySpinBox->value());
    phasePlot->setCentralFrequency(ui->frequencySpinBox->value());
    spectrumWaterfall->setCentralFrequency(ui->frequencySpinBox->value());
    QVector<int> bounds = spectrumPlot->getMarkerBounds();
    QVector<int> thresholds = spectrumPlot->getThresholdBounds();
    qDebug() << "bounds: " << bounds;
    qDebug() << "Thresholds: " << thresholds;
}

void FrontEnd::on_buttonGroupMode_buttonClicked(QAbstractButton *button)
{
    QString name = button->text();
    if (name == "Markers")
    {
        spectrumPlot->setPickers(true);
        spectrumPlot->setZoomer(false);
        spectrumPlot->setThresholdPickers(false);
    }
    else if (name == "Zoom")
    {
        spectrumPlot->setZoomer(true);
        spectrumPlot->setThresholdPickers(false);
        spectrumPlot->setPickers(false);
    }
    else if (name == "Threshold")
    {
        spectrumPlot->setThresholdPickers(true);
        spectrumPlot->setPickers(false);
        spectrumPlot->setZoomer(false);
    }
}

void FrontEnd::on_buttonGroupMarkers_buttonClicked(QAbstractButton *button)
{
    int number = button->text().toInt();
    spectrumPlot->setMarker(number);
}
