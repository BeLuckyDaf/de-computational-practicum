#ifndef COMPUTER_H
#define COMPUTER_H

#include <QtMath>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

#define DE_FUNCTION(X, Y) ( (qPow((Y), 4)*qCos((X))) + ((Y) * qTan((X))) )

class Computer
{
public:
    Computer();

    QLineSeries* ComputeEuler(qreal from, qreal to, qreal step, qreal y0);
    QLineSeries* ComputeImprovedEuler(qreal from, qreal to, qreal step, qreal y0);
    QLineSeries* ComputeRungeKutta(qreal from, qreal to, qreal step, qreal y0);
};

#endif // COMPUTER_H
