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
    ui->lineChartView->chart()->addSeries(computer->ComputeEuler(xInitial, xFinal, step, yInitial));
    ui->lineChartView->chart()->addSeries(computer->ComputeImprovedEuler(xInitial, xFinal, step, yInitial));
    ui->lineChartView->chart()->addSeries(computer->ComputeRungeKutta(xInitial, xFinal, step, yInitial));

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
