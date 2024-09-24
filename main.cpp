#include <iostream>
#include <chrono>
#include <QApplication>
#include <QMainWindow>
#include <QChart>
#include <QChartView>
#include <QValueAxis>
#include <QSplineSeries>
#include "rns.hpp"


auto compareOperations(int bits, bool isRns)
{
    mp::cpp_int num1{mp::pow(mp::cpp_dec_float_100{2}, bits)};
    mp::cpp_int num2{mp::pow(mp::cpp_dec_float_100{2}, bits)};

    std::chrono::duration<double> duration;
    

    if (isRns)
    {
        RnsInt::initialize(bits * 2, ceil((bits * 2) / 65.0));
        RnsInt rnsNum1{num1};
        RnsInt rnsNum2{num2};

        auto start = std::chrono::high_resolution_clock::now();
        RnsInt myNum3 = std::move(rnsNum1 * rnsNum2);
        auto end = std::chrono::high_resolution_clock::now();
        duration = end - start;
    }
    else 
    {
        auto start = std::chrono::high_resolution_clock::now();
        mp::cpp_int num3 = num1 * num2;
        auto end = std::chrono::high_resolution_clock::now();
        duration = end - start;
    }
    std::cout << duration.count() << "\n";

    return duration;
}

QChartView* createGraph()
{
    QChart* chart = new QChart();
    QValueAxis* axisX = new QValueAxis();
    QValueAxis* axisY = new QValueAxis();
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    QSplineSeries* firstSeries = new QSplineSeries();
    for (int i{0}, degree{512}; i < 5; ++i, degree *= 2)
    {
        firstSeries->append(degree, compareOperations(degree, false).count());
    }
    firstSeries->setName("Boost");

    QSplineSeries* secondSeries = new QSplineSeries();
    for (int i{0}, degree{512}; i < 5; ++i, degree *= 2)
    {
        secondSeries->append(degree, compareOperations(degree, true).count());
    }
    secondSeries->setName("СОК");

    chart->addSeries(firstSeries);
    firstSeries->attachAxis(axisX);
    firstSeries->attachAxis(axisY);
    chart->addSeries(secondSeries);

    secondSeries->attachAxis(axisX);
    secondSeries->attachAxis(axisY);
    chart->setTitle("Test");

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}



int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QMainWindow window;
    window.setCentralWidget(createGraph());
    window.resize(800, 600);
    window.show();

    // RnsInt::initialize(2048, ceil(2048 / 64));

    // mp::cpp_int num1{mp::pow(mp::cpp_dec_float_100{2}, 1024)};
    // mp::cpp_int num2{mp::pow(mp::cpp_dec_float_100{2}, 1024)};

    // RnsInt rnsNum1{num1};
    // RnsInt rnsNum2{num2};

    // RnsInt rnsNum3 = rnsNum1 * rnsNum2;



    return app.exec();
}