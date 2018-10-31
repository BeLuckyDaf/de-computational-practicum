#include "deplotter.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->domainFromSpinBox->setValue(0);
    ui->domainToSpinBox->setValue(xFinal + 1);
    ui->yInitialSpinBox->setValue(yInitial);
    ui->xInitialSpinBox->setValue(xInitial);
    ui->xFinalSpinBox->setValue(xFinal);
    ui->gridDensitySpinBox->setValue(DEFAULT_GRID_DENSITY);

    computer = new Computer();

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
    mainChart = new QChart();
    mainChart->legend()->setVisible(true);
    mainChart->legend()->setAlignment(Qt::AlignBottom);
    mainChart->setTitle("Equation");
    mainChart->setAnimationOptions(QChart::NoAnimation);

    errorChart = new QChart();
    errorChart->legend()->setVisible(true);
    errorChart->legend()->setAlignment(Qt::AlignBottom);
    errorChart->setTitle("Local Errors");
    errorChart->setAnimationOptions(QChart::NoAnimation);

    ui->mainLineChart->setChart(mainChart);
    ui->mainLineChart->setRenderHint(QPainter::Antialiasing);
    ui->errorLineChart->setChart(errorChart);
    ui->errorLineChart->setRenderHint(QPainter::Antialiasing);

    mainAxisX = new QValueAxis();
    mainAxisY = new QValueAxis();
    errorAxisX = new QValueAxis();
    errorAxisY = new QValueAxis();
}

/** Draw */
void MainWindow::draw()
{
    /* input parameters */
    qreal xFrom, xTo, yFrom, yTo, x0, y0, X;
    xFrom = ui->domainFromSpinBox->value();
    xTo = ui->domainToSpinBox->value();
    yFrom = ui->rangeFromSpinBox->value();
    yTo = ui->rangeToSpinBox->value();
    x0 = ui->xInitialSpinBox->value();
    y0 = ui->yInitialSpinBox->value();
    X = ui->xFinalSpinBox->value();

    /* cleaning the chart */
    ui->mainLineChart->chart()->removeAllSeries();
    ui->errorLineChart->chart()->removeAllSeries();
    qreal step = ui->stepSpinBox->value();

    qreal C1 = (qPow(y0, 3) * qCos(x0) * qCos(x0) * qSin(x0) + 1)/(qPow(y0, 3) * qCos(xFrom) * qCos(x0) * qCos(x0));
    computer->setC1(C1);
    struct CompleteSeries euler = computer->ComputeEuler(x0, X, step, y0);
    struct CompleteSeries heun = computer->ComputeImprovedEuler(x0, X, step, y0);
    QLineSeries** rungekutta = computer->ComputeRungeKutta(x0, X, step, y0);
    std::queue<QLineSeries*> exact = computer->ComputeExact(x0, X, step);

    while(!euler.values.empty()) {
        mainChart->addSeries(euler.values.front());
        euler.values.pop();
    }

    while(!exact.empty()) {
        mainChart->addSeries(exact.front());
        exact.pop();
    }

    while(!heun.values.empty()) {
        mainChart->addSeries(heun.values.front());
        heun.values.pop();
    }

    /* computing and adding series */
    mainChart->addSeries(rungekutta[0]);
    errorChart->addSeries(euler.errors);
    errorChart->addSeries(heun.errors);
    errorChart->addSeries(rungekutta[1]);

    changeGridDensity(ui->gridDensitySpinBox->value());

    mainChart->axisX()->setRange(xFrom, xTo);
    mainChart->axisY()->setRange(yFrom, yTo);
    errorChart->axisX()->setRange(xFrom, xTo);
    errorChart->axisY()->setRange(yFrom, yTo);
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
    ui->mainLineChart->chart()->axisX()->setRange(xFrom, xTo);
    ui->mainLineChart->chart()->axisY()->setRange(yFrom, yTo);
    ui->errorLineChart->chart()->axisX()->setRange(xFrom, xTo);
    ui->errorLineChart->chart()->axisY()->setRange(yFrom, yTo);
}

/** Change Grid Density */
void MainWindow::changeGridDensity(int density)
{
    mainAxisX = new QValueAxis();
    mainAxisY = new QValueAxis();
    errorAxisX = new QValueAxis();
    errorAxisY = new QValueAxis();

    qreal ratio = mainChart->geometry().width()/mainChart->geometry().height();

    mainAxisY->setTickCount(density);
    mainAxisX->setTickCount(qRound(ratio * density));
    errorAxisY->setTickCount(density);
    errorAxisX->setTickCount(qRound(ratio * density));

    mainChart->setAxisX(mainAxisX);
    mainChart->setAxisY(mainAxisY);
    errorChart->setAxisX(errorAxisX);
    errorChart->setAxisY(errorAxisY);

    for (auto series : mainChart->series()) {
        series->attachedAxes().clear();
        series->attachAxis(mainAxisX);
        series->attachAxis(mainAxisY);
    }

    for (auto series : errorChart->series()) {
        series->attachedAxes().clear();
        series->attachAxis(errorAxisX);
        series->attachAxis(errorAxisY);
    }
}

/** Redraw (Draw button) */
void MainWindow::on_pushButton_clicked()
{
    draw();
}

/** Zoom when clicked */
void MainWindow::on_ZoomButton_clicked()
{
    zoom();
}

/** Domain SpinBox Lower Bound */
void MainWindow::on_domainFromSpinBox_valueChanged(double arg1)
{
    double val = ui->domainToSpinBox->value();
    if (arg1 > val) ui->domainFromSpinBox->setValue(val);
}

/** Domain SpinBox Upper Bound */
void MainWindow::on_domainToSpinBox_valueChanged(double arg1)
{
    double val = ui->domainFromSpinBox->value();
    if (arg1 < val) ui->domainToSpinBox->setValue(val);
}

/** Range SpinBox Lower Bound */
void MainWindow::on_rangeFromSpinBox_valueChanged(double arg1)
{
    double val = ui->rangeToSpinBox->value();
    if (arg1 > val) ui->rangeFromSpinBox->setValue(val);
}

/** Range SpinBox Upper Bound */
void MainWindow::on_rangeToSpinBox_valueChanged(double arg1)
{
    double val = ui->rangeFromSpinBox->value();
    if (arg1 < val) ui->rangeToSpinBox->setValue(val);
}

void MainWindow::on_yInitialSpinBox_valueChanged(double arg1)
{
    yInitial = arg1;
}

void MainWindow::on_xInitialSpinBox_valueChanged(double arg1)
{
    xInitial = arg1;
}

void MainWindow::on_xFinalSpinBox_valueChanged(double arg1)
{
    xFinal = arg1;
}

void MainWindow::on_gridDensitySpinBox_valueChanged(int arg1)
{
    changeGridDensity(arg1);
}
