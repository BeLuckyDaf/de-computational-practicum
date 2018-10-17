#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->domainFromSpinBox->setValue(0);
    ui->domainToSpinBox->setValue(xFinal + 1);
    ui->rangeFromSpinBox->setValue(0);

    setup();
    draw();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/** Initial chart setup */
void MainWindow::setup()
{
    QChart *chart = new QChart();
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setTitle("Equation");
    chart->setAnimationOptions(QChart::AllAnimations);

    ui->lineChartView->setChart(chart);
    ui->lineChartView->setRenderHint(QPainter::Antialiasing);
}

/** Draw */
void MainWindow::draw()
{
    /* input parameters */
    qreal xFrom, xTo, yFrom, yTo;
    xFrom = ui->domainFromSpinBox->value();
    xTo = ui->domainToSpinBox->value();
    yFrom = ui->rangeFromSpinBox->value();
    yTo = ui->rangeToSpinBox->value();

    /* cleaning the chart */
    ui->lineChartView->chart()->removeAllSeries();
    qreal step = ui->stepSpinBox->value();

    /* computing and adding series */
    ui->lineChartView->chart()->addSeries(ComputeEuler(xInitial, xFinal, step, yInitial));
    ui->lineChartView->chart()->addSeries(ComputeImprovedEuler(xInitial, xFinal, step, yInitial));
    ui->lineChartView->chart()->addSeries(ComputeRungeKutta(xInitial, xFinal, step, yInitial));

    /* setting up the chart */
    ui->lineChartView->chart()->createDefaultAxes();
    ui->lineChartView->chart()->axisX()->setRange(xFrom, xTo);
    ui->lineChartView->chart()->axisY()->setRange(yFrom, yTo);
}

/** Zoom */
void MainWindow::zoom() {
    /* input parameters */
    qreal xFrom, xTo, yFrom, yTo;
    xFrom = ui->domainFromSpinBox->value();
    xTo = ui->domainToSpinBox->value();
    yFrom = ui->rangeFromSpinBox->value();
    yTo = ui->rangeToSpinBox->value();

    /* setting up the chart */
    ui->lineChartView->chart()->axisX()->setRange(xFrom, xTo);
    ui->lineChartView->chart()->axisY()->setRange(yFrom, yTo);
}

/** Euler Method */
QLineSeries *MainWindow::ComputeEuler(qreal from, qreal to, qreal step, qreal y0) // y' = x^3y^4 - y/x
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
QLineSeries *MainWindow::ComputeImprovedEuler(qreal from, qreal to, qreal step, qreal y0)
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
QLineSeries *MainWindow::ComputeRungeKutta(qreal from, qreal to, qreal step, qreal y0)
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

/** Redraw */
void MainWindow::on_pushButton_clicked()
{
    draw();
}

/** Zoom when clicked */
void MainWindow::on_ZoomButton_clicked()
{
    zoom();
}

/** SpinBox Constraints */
void MainWindow::on_domainFromSpinBox_valueChanged(double arg1)
{
    double val = ui->domainToSpinBox->value();
    if (arg1 > val) ui->domainFromSpinBox->setValue(val);
}

/** SpinBox Constraints */
void MainWindow::on_domainToSpinBox_valueChanged(double arg1)
{
    double val = ui->domainFromSpinBox->value();
    if (arg1 < val) ui->domainToSpinBox->setValue(val);
}

/** SpinBox Constraints */
void MainWindow::on_rangeFromSpinBox_valueChanged(double arg1)
{
    double val = ui->rangeToSpinBox->value();
    if (arg1 > val) ui->rangeFromSpinBox->setValue(val);
}

/** SpinBox Constraints */
void MainWindow::on_rangeToSpinBox_valueChanged(double arg1)
{
    double val = ui->rangeFromSpinBox->value();
    if (arg1 < val) ui->rangeToSpinBox->setValue(val);
}
