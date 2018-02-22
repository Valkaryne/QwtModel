#ifndef PSEUDOUDPCHANNEL_HPP
#define PSEUDOUDPCHANNEL_HPP

#include <QTimer>

constexpr double BAND = 255;
constexpr double BAND2 = 253;

class PseudoUdpChannel : public QObject
{
    Q_OBJECT
public:
    explicit PseudoUdpChannel(int timerInterval, QObject *parent = 0);

signals:
    void pseudoSamplesReceived(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                         const QVector<double> samplesPh);

public slots:
    void readPseudoDatagram();

private:
    QTimer  timer;
};

#endif // PSEUDOUDPCHANNEL_HPP
