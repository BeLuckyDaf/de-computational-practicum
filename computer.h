#ifndef COMPUTER_H
#define COMPUTER_H

#include <QtMath>
#include <QtCharts/QLineSeries>
#include <limits>
#include <QtDebug>

QT_CHARTS_USE_NAMESPACE

#define DE_FUNCTION(X, Y) ( ((Y)*(Y)*(Y)*(Y)*qCos((X))) + ((Y) * qTan((X))) )
#define MAX_Y 50000.0

class Computer
{
public:
    Computer();

    qreal C1 = 1;

    QLineSeries** ComputeEuler(qreal from, qreal to, qreal step, qreal y0);
    QLineSeries** ComputeImprovedEuler(qreal from, qreal to, qreal step, qreal y0);
    QLineSeries** ComputeRungeKutta(qreal from, qreal to, qreal step, qreal y0);
    QLineSeries* ComputeExact(qreal from, qreal to, qreal step);
    qreal ComputePointExact(qreal x);
    void setC1(const qreal &value);
};

#endif // COMPUTER_H
