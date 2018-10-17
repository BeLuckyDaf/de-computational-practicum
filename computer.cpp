#include "computer.h"

Computer::Computer()
{

}

/** Euler Method */
QLineSeries *Computer::ComputeEuler(qreal from, qreal to, qreal step, qreal y0) // y' = x^3y^4 - y/x
{
    QLineSeries* series = new QLineSeries();
    series->setName("Euler");
    series->append(from, y0);

    qreal yp = y0;
    for (qreal x = from + step; x < to; x += step) {
        qreal y = yp + step * DE_FUNCTION(x, yp);
        series->append(x, y);
    }

    return series;
}

/** Imroved Euler (Heun) Method */
QLineSeries *Computer::ComputeImprovedEuler(qreal from, qreal to, qreal step, qreal y0)
{
    QLineSeries* series = new QLineSeries();
    series->setName("Heun");
    series->append(from, y0);

    qreal y = y0;
    for (qreal x = from; x < to; x += step) {
        qreal xj = x + step;
        qreal m1 = DE_FUNCTION(x, y);
        qreal m2 = DE_FUNCTION(xj, y + step*m1);
        qreal yj = y + step*(m1 + m2)/2;
        series->append(xj, yj);
    }

    return series;
}

/** Runge Kutta Method */
QLineSeries *Computer::ComputeRungeKutta(qreal from, qreal to, qreal step, qreal y0)
{
    QLineSeries* series = new QLineSeries();
    series->setName("Runge Kutta");
    series->append(from, y0);

    qreal y = y0;
    for (qreal x = from; x < to; x += step) {
        qreal xj = x + step;
        qreal k1 = step*DE_FUNCTION(x, y);
        qreal k2 = step*DE_FUNCTION(x + step/2, y + k1/2);
        qreal k3 = step*DE_FUNCTION(x + step/2, y + k2/2);
        qreal k4 = step*DE_FUNCTION(x + step, y + k3);
        qreal yj = y + k1/6 + k2/3 + k3/3 + k4/6;
        series->append(xj, yj);
    }

    return series;
}
