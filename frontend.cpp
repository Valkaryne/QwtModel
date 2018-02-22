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

    spectrumPlot = new Plot(this);
    spectrumPlot->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    ui->spectrumLayout->addSpacing(10);
    spLayout->addSpacing(12);
    spLayout->addWidget(spectrumPlot);
    ui->spectrumLayout->addLayout(spLayout);

    phasePlot = new Plot(this);
    phasePlot->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    phLayout->addSpacing(12);
    phLayout->addWidget(phasePlot);
    ui->spectrumLayout->addSpacing(10);
    ui->spectrumLayout->addLayout(phLayout);
    phasePlot->setAxisTitle(QwtPlot::yLeft, "Phase");
    phasePlot->setAxisScale(QwtPlot::yLeft, 0, 360, 90);

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
}

FrontEnd::~FrontEnd()
{
    //delete backEnd;
    delete ui;
}
