#include "computer.h"

Computer::Computer()
{
    qDebug() << "Computer Initiated.";
}

void Computer::setC1(const qreal &value)
{
    C1 = value;
}

qreal Computer::length(qreal x1, qreal y1, qreal x2, qreal y2)
{
    qreal lensq = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
    return qSqrt(lensq);
}

/** Euler Method */
struct CompleteSeries Computer::ComputeEuler(qreal from, qreal to, qreal step, qreal y0) // y' = x^3y^4 - y/x
{
    QLineSeries** series = new QLineSeries*[2];
    series[0] = new QLineSeries();
    series[1] = new QLineSeries();
    series[0]->setName("Euler");
    series[1]->setName("Euler");
    series[0]->append(from, y0);
    series[1]->append(from, 0);

    struct CompleteSeries complete;
    complete.totalError = 0;

    qreal yp = y0;
    qreal err;
    for (qreal x = from + step; x < to; x += step) {
        qreal y = yp + step * DE_FUNCTION(x, yp);

        series[0]->append(x, y);
        err = qAbs(ComputePointExact(x).y - y);
        series[1]->append(x, err);
        complete.totalError += err;
        yp = y;
    }

    complete.values = series[0];
    complete.errors = series[1];

    qDebug() << "Computed Euler.";

    return complete;
}

/** Imroved Euler (Heun) Method */
struct CompleteSeries Computer::ComputeImprovedEuler(qreal from, qreal to, qreal step, qreal y0)
{
    QLineSeries** series = new QLineSeries*[2];
    series[0] = new QLineSeries();
    series[0]->setName("Heun");
    series[0]->append(from, y0);
    series[1] = new QLineSeries();
    series[1]->setName("Heun");
    series[1]->append(from, 0);

    struct CompleteSeries complete;
    complete.totalError = 0;

    qreal y = y0, err;
    for (qreal x = from; x < to; x += step) {
        qreal xj = x + step;
        qreal m1 = DE_FUNCTION(x, y);
        qreal m2 = DE_FUNCTION(xj, y + step*m1);
        y = y + step*(m1 + m2)/2;

        series[0]->append(xj, y);
        err = qAbs(ComputePointExact(x).y - y);
        series[1]->append(x, err);
        complete.totalError += err;
    }

    complete.values = series[0];
    complete.errors = series[1];

    qDebug() << "Computed Heun.";

    return complete;
}

/** Runge Kutta Method */
struct CompleteSeries Computer::ComputeRungeKutta(qreal from, qreal to, qreal step, qreal y0)
{
    QLineSeries** series = new QLineSeries*[2];
    series[0] = new QLineSeries();
    series[0]->setName("Runge Kutta");
    series[0]->append(from, y0);
    series[1] = new QLineSeries();
    series[1]->setName("Runge Kutta");
    series[1]->append(from, 0);

    struct CompleteSeries complete;
    complete.totalError = 0;

    qreal y = y0, err;
    for (qreal x = from; x < to; x += step) {
        qreal xj = x + step;
        qreal k1 = step*DE_FUNCTION(x, y);
        qreal k2 = step*DE_FUNCTION(x + (step/2), y + (k1/2));
        qreal k3 = step*DE_FUNCTION(x + (step/2), y + (k2/2));
        qreal k4 = step*DE_FUNCTION(x + step, y + k3);
        y = y + k1/6 + k2/3 + k3/3 + k4/6;


        series[0]->append(xj, y);
        err = qAbs(ComputePointExact(x).y - y);
        series[1]->append(x, err);
        complete.totalError += err;
    }

    complete.values = series[0];
    complete.errors = series[1];

    qDebug() << "Computed Runge-Kutta.";

    return complete;
}

/** Using the exact solution */
QLineSeries* Computer::ComputeExact(qreal from, qreal to, qreal step)
{
    QLineSeries* series = new QLineSeries;
    series = new QLineSeries();
    series->setName("Exact");

    for (qreal x = from; x < to; x += step) {
        struct ExactPoint py = ComputePointExact(x);
        if (py.valid) series->append(x, py.y);
    }

    qDebug() << "Computed Exact.";

    return series;
}

/** Compute just one point using the exact solution */
struct ExactPoint Computer::ComputePointExact(qreal x)
{
    struct ExactPoint p;
    p.valid = false;
    p.y = (2*x + qExp(-2*x) - 1);
    if (!qIsInf(p.y) && !qIsNaN(p.y)) p.valid = true;
    //else p.y = -1;
    return p;
}

