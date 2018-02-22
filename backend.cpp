#include "backend.hpp"
#include <QDebug>
#include <QMetaType>
#include <QVector>

BackEnd::BackEnd(QObject *parent)
    : QThread(parent)
{
    qRegisterMetaType<QVector<double>>("QVector<double>");
}

BackEnd::~BackEnd()
{
    qDebug() << "BackEnd destoyed";
}

void BackEnd::samplesHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                        const QVector<double> samplesPh)
{
    //QThread::msleep(150);
    emit(amplitudeSamplesReady(samplesAm1, samplesAm2));
    emit(phaseSamplesReady(samplesPh));
}
