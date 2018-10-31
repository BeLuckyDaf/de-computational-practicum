#include "computer.h"

Computer::Computer()
{

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
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    pen.setBrush(Qt::blue);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    QLineSeries** series = new QLineSeries*[2];
    series[0] = new QLineSeries();
    series[1] = new QLineSeries();
    series[0]->setPen(pen);
    series[1]->setPen(pen);
    series[0]->setName("Euler");
    series[1]->setName("Euler");
    series[0]->append(from, y0);
    series[1]->append(from, 0);

    struct CompleteSeries complete;

    qreal yp = y0;
    for (qreal x = from + step; x < to; x += step) {
        qreal y = yp + step * DE_FUNCTION(x, yp);

        series[0]->append(x, qMin(Y_UPPER_BOUND_DUMMY, y));
        yp = y;

        // handling asymptotes
        qreal err = qAbs(ComputePointExact(x).y - y);
        if (!qIsFinite(y)) {
            x += step;
            //series[0]->append(x, y > 0 ? Y_UPPER_BOUND_DUMMY : -Y_UPPER_BOUND_DUMMY);
            complete.values.push(series[0]);
            series[0] = new QLineSeries;
            series[0]->setPen(pen);

            x += step;
            struct ExactPoint p;
            while(!(p = ComputePointExact(x)).valid && x < to) x += step;
            y = p.y;
            yp = y;
            series[0]->append(x, y);
            series[1]->append(x, 0);
        } else series[1]->append(x, err);
    }

    complete.values.push(series[0]);
    complete.errors = series[1];
    return complete;
}

/** Imroved Euler (Heun) Method */
struct CompleteSeries Computer::ComputeImprovedEuler(qreal from, qreal to, qreal step, qreal y0)
{
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    pen.setBrush(Qt::magenta);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    QLineSeries** series = new QLineSeries*[2];
    series[0] = new QLineSeries();
    series[0]->setName("Heun");
    series[0]->append(from, y0);
    series[0]->setPen(pen);
    series[1] = new QLineSeries();
    series[1]->setName("Heun");
    series[1]->append(from, 0);
    series[1]->setPen(pen);

    struct CompleteSeries complete;

    qreal y = y0;
    for (qreal x = from; x < to; x += step) {
        qreal xj = x + step;
        qreal m1 = DE_FUNCTION(x, y);
        qreal m2 = DE_FUNCTION(xj, y + step*m1);
        y = y + step*(m1 + m2)/2;

        series[0]->append(xj, qMin(Y_UPPER_BOUND_DUMMY, y));

        // handling asymptotes
        qreal err = qAbs(ComputePointExact(x).y - y);
        if (!qIsFinite(m1) || !qIsFinite(m2) || !qIsFinite(y)) {
            x += step;
            //series[0]->append(x, y > 0 ? Y_UPPER_BOUND_DUMMY : -Y_UPPER_BOUND_DUMMY);
            complete.values.push(series[0]);
            series[0] = new QLineSeries;
            series[0]->setPen(pen);

            x += step;
            struct ExactPoint p;
            while(!(p = ComputePointExact(x)).valid && x < to) x += step;
            y = p.y;
            series[0]->append(xj, y);
            series[1]->append(xj, 0);
        } else series[1]->append(xj, err);
    }

    complete.values.push(series[0]);
    complete.errors = series[1];
    return complete;
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

        if (qIsInf(y) || qIsNaN(y) || y > Y_UPPER_BOUND_DUMMY) {
            y = Y_UPPER_BOUND_DUMMY;
            series[0]->append(xj, y);
            return series;
        }

        series[0]->append(xj, y);
        //series[1]->append(xj, ComputePointExact(x)-y);
    }

    return series;
}

std::queue<QLineSeries*> Computer::ComputeExact(qreal from, qreal to, qreal step)
{
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    pen.setBrush(Qt::green);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    QLineSeries* series = new QLineSeries;
    series = new QLineSeries();
    series->setName("Exact");
    series->setPen(pen);

    std::queue<QLineSeries*> queue;

    qreal last = 0;
    for (qreal x = from; x < to; x += step) {
        struct ExactPoint py = ComputePointExact(x);
        qreal y;
        if (py.valid && std::signbit(py.y) == std::signbit(last)) {
            last = y = py.y;
            series->append(x, y);
        } else {
            if (py.valid) last = py.y;
            if (series->count() > 0) {
                queue.push(series);
                series = new QLineSeries;
                series->setPen(pen);
                series->append(x, last > 0 ? Y_UPPER_BOUND_DUMMY : -Y_UPPER_BOUND_DUMMY);
            }
        }
    }

    if (series->count() > 0) queue.push(series);
    return queue;
}

struct ExactPoint Computer::ComputePointExact(qreal x)
{
    struct ExactPoint p;
    p.valid = false;
    p.y = (2*x + qExp(-2*x) - 1);
    if (!qIsInf(p.y) && !qIsNaN(p.y)) p.valid = true;
    //else p.y = -1;
    return p;
}

