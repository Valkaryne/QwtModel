#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <QThread>

class FrontEnd;

class BackEnd : public QThread
{
    Q_OBJECT

public:
    explicit BackEnd(QObject    *parent = 0);
    ~BackEnd();

private slots:
    void samplesHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                        const QVector<double> samplesPh);
signals:
    /* void samplesReady(const QVector<double> amplitudeOne, const QVector<double> amplitudeTwo,
                      const QVector<double> phaseDiff); */
    void amplitudeSamplesReady(const QVector<double> amplitudeOne, const QVector<double> amplitudeTwo);
    void phaseSamplesReady(const QVector<double> phaseDiff);

private:
    FrontEnd    *frontEnd;
};

#endif // BACKEND_HPP
