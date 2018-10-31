#ifndef COMPUTER_H
#define COMPUTER_H

#include <QtMath>
#include <QtCharts/QLineSeries>
#include <limits>
#include <QtDebug>
#include <queue>

QT_CHARTS_USE_NAMESPACE

#define DE_FUNCTION(X, Y) ( ((-2)*(Y)) + (4*(X)) )

struct ExactPoint {
    bool valid;
    qreal y;
};

struct CompleteSeries {
    QLineSeries* values;
    QLineSeries* errors;
    uint totalError;
};

class Computer
{
public:
    Computer();

    qreal C1 = 1;
    qreal bound = qPow(std::numeric_limits<qreal>::max(), 0.25);

    struct CompleteSeries ComputeEuler(qreal from, qreal to, qreal step, qreal y0);
    struct CompleteSeries ComputeImprovedEuler(qreal from, qreal to, qreal step, qreal y0);
    struct CompleteSeries ComputeRungeKutta(qreal from, qreal to, qreal step, qreal y0);
    QLineSeries* ComputeExact(qreal from, qreal to, qreal step);
    struct ExactPoint ComputePointExact(qreal x);
    void setC1(const qreal &value);
    qreal length(qreal x1, qreal y1, qreal x2, qreal y2);
};

#endif // COMPUTER_H
