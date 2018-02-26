#include "backend.hpp"
#include <QDebug>
#include <QMetaType>
#include <QVector>

BackEnd::BackEnd(QObject *parent)
    : QThread(parent)
{
    qRegisterMetaType<QVector<double>>("QVector<double>");
    this->xleft = 2310;
    this->xright = 2311;
}

BackEnd::~BackEnd()
{
    qDebug() << "BackEnd destoyed";
}

void BackEnd::setRangeBounds(const int xleft, const int xright)
{
    this->xleft = xleft;
    this->xright = xright;
}

void BackEnd::samplesHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                        const QVector<double> samplesPh)
{
    //QThread::msleep(150);
    QVector<double> reserve1, reserve2, reserve3;

    for (int j = samplesAm1.size() / 2; j < samplesAm1.size(); j++)
    {
        reserve1.push_back(samplesAm1[j] / 16.0);
        reserve2.push_back(samplesAm2[j] / 16.0);
        reserve3.push_back(samplesPh[j]);
    }

    for (int i = 0; i < samplesAm1.size() / 2; i++)
    {
        reserve1.push_back(samplesAm1[i] / 16.0);
        reserve2.push_back(samplesAm2[i] / 16.0);
        reserve3.push_back(samplesPh[i]);
    }

    emit(amplitudeSamplesReady(reserve1, reserve2));
    emit(phaseSamplesReady(reserve3));
}

void BackEnd::responsePointsHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                                    const QVector<double> samplesPh)
{
    QVector<double> reserve1, reserve2;

    for (int j = samplesAm1.size() / 2; j < samplesAm1.size(); j++)
    {
        reserve1.push_back(samplesAm1[j] / 16.0);
        reserve2.push_back(samplesPh[j]);
    }

    for (int i = 0; i < samplesAm1.size() / 2; i++)
    {
        reserve1.push_back(samplesAm1[i] / 16);
        reserve2.push_back(samplesPh[i]);
    }

    /*for (int k = xleft; k <= xright; k++)
    {
        if (reserve1.at(k) > 90)
            emit(phaseResponsePointsReady(reserve2.at(k), reserve1.at(k)));
    } */

    double amplitude = std::accumulate(reserve1.begin() + xleft, reserve1.begin() + xright, .0) / (xright - (xleft - 1));
    double phase = std::accumulate(reserve2.begin() + xleft, reserve2.begin() + xright, .0) / (xright - (xleft - 1));

    emit(phaseResponsePointsReady(phase, amplitude));
}
