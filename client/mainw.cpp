#include "mainw.h"
#include "ui_mainw.h"

MainW::MainW(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainW)
{
    ui->setupUi(this);
    pht ini={0,0,0};

    for(int i=0;i<chartbuflen;i++){
        dataqueue.enqueue(ini);
    }
    //查找可用的串口
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->serialportn->addItem(serial.portName());
            serial.close();
        }
    }
    setWindowTitle("Client");
    drawchart();
}

MainW::~MainW()
{
    delete ui;
}


void MainW::on_serialportbutton_clicked()
{
    if(ui->serialportbutton->text() == QString("打开串口"))
    {
        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->serialportn->currentText());
        //打开串口
        serial->open(QIODevice::ReadWrite);
        //设置波特率
        serial->setBaudRate(QSerialPort::Baud19200);//设置波特率为115200
        //设置数据位数
        serial->setDataBits(QSerialPort::Data8);//设置数据位8
        //设置校验位
        serial->setParity(QSerialPort::NoParity);
        //设置停止位
        serial->setStopBits(QSerialPort::OneStop);//停止位设置为1
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制
        //关闭设置菜单使能
        ui->serialportn->setEnabled(false);
        ui->serialportbutton->setText(QString("关闭串口"));
        //连接信号槽
        QObject::connect(serial,&QSerialPort::readyRead,this,&MainW::ReadData);
    }
    else
    {
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();
        //恢复设置使能
        ui->serialportn->setEnabled(true);
        ui->serialportbutton->setText(QString("打开串口"));
    }
}

//读取接收到的信息
struct messagepht{
    uint pressure;
    int humidity;
    int temperature;
};
void MainW::ReadData()
{
    QByteArray buf,tmp;
    buf = serial->readAll();
    if(!buf.isEmpty())
    {
        //QString str = ui->textEdit->toPlainText();
        //str+=tr(buf);
        //ui->textEdit->clear();
        //for(int i=0;i<4;i++) tmp[i]=buf.at(3-i);
        //int pres;
        messagepht dat;
        memcpy(&dat,buf.data(),sizeof(messagepht));
        pht data={dat.pressure/25600.0,
                 dat.humidity/10.0,
                 dat.temperature/10.0};
        dataqueue.dequeue();
        dataqueue.enqueue(data);
        ui->lineEditpres->setText(QString::number(data.pressure)+"hPa");
        ui->lineEdithumi->setText(QString::number(data.humidity)+"%");
        ui->lineEditTemp->setText(QString::number(data.temperature)+"°C");
        //ui->textEdit->append(QString::number(dat.pressure>>8)+'\t'+QString::number(dat.humidity)+'\t'+QString::number(dat.temperature));
        //ui->textEdit->append(QString::number(sizeof(pht)));
        drawchart();
    }
    buf.clear();
}

void MainW::drawchart()
{
    pseries = new QtCharts::QLineSeries();
    hseries = new QtCharts::QLineSeries();
    tseries = new QtCharts::QLineSeries();
//![1]

//![2]
    pseries->clear();
    hseries->clear();
    tseries->clear();
    /*series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);*/
    for(int i=0;i<chartbuflen;i++){
        pseries->append(i,dataqueue[i].pressure/10);
        hseries->append(i,dataqueue[i].humidity);
        tseries->append(i,dataqueue[i].temperature);
    }
    pseries->setName("气压/kPa");
    pseries->setColor(QColor(255,255,0));
    hseries->setName("湿度/%");
    hseries->setColor(QColor(255,0,255));
    tseries->setName("温度/°C");
    tseries->setColor(QColor(0,255,255));
//![2]

//![3]
    if(!inited) inited=true;
    else{
        //delete chart;
        //delete chartView;
        delete layout;
    }

    chart = new QtCharts::QChart();
    //chart->legend()->hide();
    chart->addSeries(pseries);
    chart->addSeries(hseries);
    chart->addSeries(tseries);
    chart->createDefaultAxes();
    //chart->setTitle("Simple line chart example");
//![3]

//![4]
    chartView= new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    //chartView->show();

    layout=new QVBoxLayout();
    layout->addWidget(chartView);
//![4]


//![5]
    //QMainWindow window;
   // window.setCentralWidget(chartView);
    //window.resize(400, 300);
    //window.show();
//![5]
    //ui->widget-
    ui->widget->setLayout(layout);
    //ui->widget->
}
