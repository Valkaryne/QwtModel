#include "frontend.hpp"
#include "ui_frontend.h"

FrontEnd::FrontEnd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FrontEnd)
{
    ui->setupUi(this);

    /* Create spectrum plot, phase plot and waterfall objects */

    QHBoxLayout *spLayout = new QHBoxLayout();
    QHBoxLayout *phLayout = new QHBoxLayout();

    spectrumWaterfall = new SpecWaterfall(this);
    ui->spectrumLayout->addWidget(spectrumWaterfall);

    spectrumPlot = new Plot(this);
    ui->spectrumLayout->addSpacing(10);
    spLayout->addSpacing(12);
    spLayout->addWidget(spectrumPlot);
    ui->spectrumLayout->addLayout(spLayout);

    phasePlot = new Plot(this);
    phLayout->addSpacing(12);
    phLayout->addWidget(phasePlot);
    ui->spectrumLayout->addSpacing(10);
    ui->spectrumLayout->addLayout(phLayout);
    phasePlot->setAxisTitle(QwtPlot::yLeft, "Phase");
    phasePlot->setAxisScale(QwtPlot::yLeft, 0, 360, 90);
}

FrontEnd::~FrontEnd()
{
    delete ui;
}
