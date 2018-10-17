#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "computer.h"

#include <QtWidgets/QMainWindow>
#include <QApplication>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QDebug>
#include <QtMath>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_ZoomButton_clicked();

    void on_domainFromSpinBox_valueChanged(double arg1);

    void on_domainToSpinBox_valueChanged(double arg1);

    void on_rangeFromSpinBox_valueChanged(double arg1);

    void on_rangeToSpinBox_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    Computer *computer;

    void setup();
    void draw();
    void zoom();

    QLineSeries* ComputeEuler(qreal from, qreal to, qreal step, qreal y0);
    QLineSeries* ComputeImprovedEuler(qreal from, qreal to, qreal step, qreal y0);
    QLineSeries* ComputeRungeKutta(qreal from, qreal to, qreal step, qreal y0);

    const qreal xInitial = 0;
    const qreal yInitial = 1;
    const qreal xFinal = 9.5;
};

#endif // MAINWINDOW_H
