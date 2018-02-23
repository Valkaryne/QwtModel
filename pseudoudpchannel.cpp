#include "pseudoudpchannel.hpp"
#include <QVector>
#include <QDebug>

PseudoUdpChannel::PseudoUdpChannel(int timerInterval, QObject *parent) :
    QObject(parent)
{
    timer.setInterval(timerInterval);
    timer.start();
    connect(&timer, SIGNAL(timeout()), this, SLOT(readPseudoDatagram()));
}

void PseudoUdpChannel::readPseudoDatagram()
{
    /* First (yellow) channel */
    QVector<double> vecAm1;
    int init1 = qrand() % 4095 - BAND;
    for (int i = 0; i < init1; i++)
        vecAm1.push_back(75 - qrand() % 15);

    for (int j = init1; j < init1 + BAND; j++)
        vecAm1.push_back(121 - qrand() % 7);

    for (int k = init1 + BAND; k < 4096; k++)
        vecAm1.push_back(75 - qrand() % 15);

    /* Second (blue) channel */
    QVector<double> vecAm2;
    int init2 = init1 - qrand() % 4;
    for (int i = 0; i < init2; i++)
        vecAm2.push_back(75 - qrand() % 15);

    for (int j = init2; j < init2 + BAND2; j++)
        vecAm2.push_back(121 - qrand() % 7);

    for (int k = init2 + BAND2; k < 4096; k++)
        vecAm2.push_back(75 - qrand() % 15);

    /* Phase Difference */
    QVector<double> vecPh;
    for (int i = 0; i < 4096; i++)
    {
        if (vecAm1.at(i) > 75)
            vecPh.push_back(qrand() % (15 + i % 7) + 175 + i % 7);
        else
            vecPh.push_back(qrand() % 360);
    }

    emit(pseudoSamplesReceived(vecAm1, vecAm2, vecPh));
}
