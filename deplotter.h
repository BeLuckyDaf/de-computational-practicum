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

QT_CHARTS_USE_NAMESPACE

#define DEFAULT_GRID_DENSITY 10

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

    void on_yInitialSpinBox_valueChanged(double arg1);

    void on_xInitialSpinBox_valueChanged(double arg1);

    void on_xFinalSpinBox_valueChanged(double arg1);

    void on_gridDensitySpinBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;

    QChart *mainChart, *errorChart;
    Computer *computer;

    void setup();
    void draw();
    void zoom();
    void changeGridDensity(int density);

    QLineSeries* computeEuler(qreal from, qreal to, qreal step, qreal y0);
    QLineSeries* computeImprovedEuler(qreal from, qreal to, qreal step, qreal y0);
    QLineSeries* computeRungeKutta(qreal from, qreal to, qreal step, qreal y0);

    QValueAxis* mainAxisX, *errorAxisX;
    QValueAxis* mainAxisY, *errorAxisY;

    qreal xInitial = 0;
    qreal yInitial = 1;
    qreal xFinal = 9.5;

};

#endif // MAINWINDOW_H
