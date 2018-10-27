#include "computer.h"

Computer::Computer()
{

}

void Computer::setC1(const qreal &value)
{
    C1 = value;
}

/** Euler Method */
QLineSeries **Computer::ComputeEuler(qreal from, qreal to, qreal step, qreal y0) // y' = x^3y^4 - y/x
{
    QLineSeries** series = new QLineSeries*[2];
    series[0] = new QLineSeries();
    series[1] = new QLineSeries();
    series[0]->setName("Euler");
    series[1]->setName("Euler");
    series[0]->append(from, y0);
    series[1]->append(from, 0);

    qreal yp = y0;
    for (qreal x = from + step; x < to; x += step) {
        qreal y = yp + step * DE_FUNCTION(x, yp);
        yp = y;

        if (qIsInf(y) || qIsNaN(y)) return series;

        series[0]->append(x, y);
        series[1]->append(x, ComputePointExact(x) - y);
    }

    return series;
}

/** Imroved Euler (Heun) Method */
QLineSeries **Computer::ComputeImprovedEuler(qreal from, qreal to, qreal step, qreal y0)
{
    QLineSeries** series = new QLineSeries*[2];
    series[0] = new QLineSeries();
    series[0]->setName("Heun");
    series[0]->append(from, y0);
    series[1] = new QLineSeries();
    series[1]->setName("Heun");
    series[1]->append(from, 0);

    qreal y = y0;
    for (qreal x = from; x < to; x += step) {
        qreal xj = x + step;
        qreal m1 = DE_FUNCTION(x, y);
        qreal m2 = DE_FUNCTION(xj, y + step*m1);
        y = y + step*(m1 + m2)/2;

        if (qIsInf(y) || qIsNaN(y)) return series;

        series[0]->append(xj, y);
        series[1]->append(xj, ComputePointExact(x)-y);
    }

    return series;
}

/** Runge Kutta Method */
QLineSeries **Computer::ComputeRungeKutta(qreal from, qreal to, qreal step, qreal y0)
{
    QLineSeries** series = new QLineSeries*[2];
    series[0] = new QLineSeries();
    series[0]->setName("Runge Kutta");
    series[0]->append(from, y0);
    series[1] = new QLineSeries();
    series[1]->setName("Runge Kutta");
    series[1]->append(from, 0);

    qreal y = y0;
    for (qreal x = from; x < to; x += step) {
        qreal xj = x + step;
        qreal k1 = step*DE_FUNCTION(x, y);
        qreal k2 = step*DE_FUNCTION(x + (step/2), y + (k1/2));
        qreal k3 = step*DE_FUNCTION(x + (step/2), y + (k2/2));
        qreal k4 = step*DE_FUNCTION(x + step, y + k3);
        y = y + k1/6 + k2/3 + k3/3 + k4/6;
        if (qIsInf(y) || qIsNaN(y)) {
            qDebug() << "Asymptote.";
            return series;
        }

        if (qIsInf(y) || qIsNaN(y)) return series;

        series[0]->append(xj, y);
        series[1]->append(xj, ComputePointExact(x)-y);
    }

    return series;
}

QLineSeries *Computer::ComputeExact(qreal from, qreal to, qreal step)
{
    QLineSeries* series = new QLineSeries();
    series = new QLineSeries();
    series->setName("Exact");

    for (qreal x = from; x < to; x += step) {
        qreal y = ComputePointExact(x);
        if (qIsInf(y) || qIsNaN(y)) y = MAX_Y;
        series->append(x, y);
    }

    return series;
}

qreal Computer::ComputePointExact(qreal x)
{
    qreal y = 1.0/qPow(qCos(x)*qCos(x)*(C1*qCos(x) - 3*qSin(x)), 1.0/3.0);
    return y;
}

