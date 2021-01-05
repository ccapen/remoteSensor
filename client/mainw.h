#ifndef MAINW_H
#define MAINW_H

#include <QMainWindow>
#include <QQueue>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QLayout>

#define chartbuflen 61

QT_BEGIN_NAMESPACE
namespace Ui { class MainW; }
QT_END_NAMESPACE

struct pht{
    double pressure,humidity,temperature;
};

class MainW : public QMainWindow
{
    Q_OBJECT

public:
    MainW(QWidget *parent = nullptr);
    ~MainW();
    QQueue<pht> dataqueue;
    void drawchart();
    QtCharts::QLineSeries *pseries,*hseries,*tseries;
    QtCharts::QChartView *chartView;
    QVBoxLayout *layout;
    bool inited=false;
    QtCharts::QChart *chart;


private slots:
    void on_serialportbutton_clicked();

    void ReadData();
private:
    Ui::MainW *ui;
    QSerialPort *serial;
};
#endif // MAINW_H
