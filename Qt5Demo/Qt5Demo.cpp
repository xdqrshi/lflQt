#include "Qt5Demo.h"
#include "qnetworkdatagram.h"
#include "qfile.h"
#include "qfiledialog.h"
#include "qendian.h"
#include "qstringlist.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8");
#endif // _MSC_VER >= 1600


Qt5Demo::Qt5Demo(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //connect(this, &ui.pb_SendConfig->clicked(), this, this->on_pbSendConfig());
    ParaInit();
    udpSocket = new QUdpSocket(this);
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    m_lineSeries_A = new QSplineSeries();
    m_lineSeries_B = new QSplineSeries();
    m_chart = new QChart();
    m_Fclk = 100000;

    //targetAddress = QHostAddress("192.168.3.75");
    //targetPort = 5001;
    ui.le_ipAddr->setText("192.168.1.10");
    //ui.le_ipAddr->setText("127.0.0.1");
    ui.le_ipPort->setText("5001");
    udpSocket->bind(targetAddress, targetPort);
    cnt = 0;
    timer = new QTimer();
    timer->start(5000);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_Timeout()));


}

Qt5Demo::~Qt5Demo()
{}


void Qt5Demo::on_pbSendConfig_clicked()
{

    m_ModeVal = 0;
    m_t1 = 50;
    m_t2 = 70;
    m_t3 = 50;
    m_t4 = 50;
    m_t5 = 300;
    m_t6 = 0;
    m_t7 = 20;
    m_t8 = 30;
    m_PhaseDelay = 0;
    m_CycleTimes = 5;
    m_CycleDelay = 10;
    m_PowerOut = 10;
    m_Fctrl = 0;
    m_Pctrl = 0;
    m_Fclk = 100000;
    m_DDSTime = 0;
    m_FramHead = 0x5a5a5a5a;
    m_FramTail = 0xa5a5a5a5;




    QByteArray ba;
    //ba.clear();
    QDataStream out(&ba, QIODevice::WriteOnly);

    m_PhaseDelay = static_cast<quint32>(m_Fclk * (ui.le_PhaseOffset->text().toDouble()) / (ui.le_Freq->text().toDouble()) / 360);
    m_DDSTime = vMifValue.size();

    m_ModeVal = 0x01bbbbbb;
    //out << qToBigEndian(m_FramHead);
    //out << qToBigEndian(m_ModeVal);
    //out << qToBigEndian(m_t1);
    //out << qToBigEndian(m_t2);
    //out << qToBigEndian(m_t3);
    //out << qToBigEndian(m_t4);
    //out << qToBigEndian(m_t5);
    //m_t6 = m_DDSTime - m_t7;
    //out << qToBigEndian(m_t6);
    //out << qToBigEndian(m_t7);
    //out << qToBigEndian(m_t8);
    //out << qToBigEndian(m_PhaseDelay);
    //out << qToBigEndian(m_CycleTimes);
    //out << qToBigEndian(m_CycleDelay);
    //out << qToBigEndian(m_PowerOut);
    //out << qToBigEndian(m_Fctrl);
    //out << qToBigEndian(m_Pctrl);
    //out << qToBigEndian(static_cast<quint32>(vMifValue.size()));
    //for (quint32 i = 0; i < vMifValue.size(); i++)
    //{
    //    out << qToBigEndian(vMifValue[i]);
    //    //out << qToBigEndian(i);
    //}
    //out << qToBigEndian(m_FramTail);

    //m_FramHead = qToBigEndian(m_FramHead);
    out.writeRawData(reinterpret_cast<char*>(&m_FramHead),4);
    //m_ModeVal = qToBigEndian(m_ModeVal);
    out.writeRawData(reinterpret_cast<char*>(&m_ModeVal), 4);
    //m_t1 = qToBigEndian(m_t1);
    out.writeRawData(reinterpret_cast<char*>(&m_t1), 4);
    //m_t2 = qToBigEndian(m_t2);
    out.writeRawData(reinterpret_cast<char*>(&m_t2), 4);
    //m_t3 = qToBigEndian(m_t3);
    out.writeRawData(reinterpret_cast<char*>(&m_t3), 4);
    //m_t4 = qToBigEndian(m_t4);
    out.writeRawData(reinterpret_cast<char*>(&m_t4), 4);
    //m_t5 = qToBigEndian(m_t5);
    out.writeRawData(reinterpret_cast<char*>(&m_t5), 4);
    m_t6 = m_DDSTime - m_t7;
    //m_t6 = qToBigEndian(m_t6);
    out.writeRawData(reinterpret_cast<char*>(&m_t6), 4);
    //m_t7 = qToBigEndian(m_t7);
    out.writeRawData(reinterpret_cast<char*>(&m_t7), 4);
    //m_t8 = qToBigEndian(m_t8);
    out.writeRawData(reinterpret_cast<char*>(&m_t8), 4);
    //m_PhaseDelay = qToBigEndian(m_PhaseDelay);
    out.writeRawData(reinterpret_cast<char*>(&m_PhaseDelay), 4);
    //m_CycleTimes = qToBigEndian(m_CycleTimes);
    out.writeRawData(reinterpret_cast<char*>(&m_CycleTimes), 4);
    //m_CycleDelay = qToBigEndian(m_CycleDelay);
    out.writeRawData(reinterpret_cast<char*>(&m_CycleDelay), 4);
    //m_PowerOut = qToBigEndian(m_PowerOut);
    out.writeRawData(reinterpret_cast<char*>(&m_PowerOut), 4);
    //m_Fctrl = qToBigEndian(m_Fctrl);
    out.writeRawData(reinterpret_cast<char*>(&m_Fctrl), 4);
    //m_Pctrl = qToBigEndian(m_Pctrl);
    out.writeRawData(reinterpret_cast<char*>(&m_Pctrl), 4);
    quint32 vMifValue_Len = static_cast<quint32>(vMifValue.size());
    out.writeRawData(reinterpret_cast<char*>(&vMifValue_Len), 4);
    //out.writeRawData(reinterpret_cast<char*>(&vMifValue_Len), 4);
    for (quint32 i = 0; i < vMifValue.size(); i++)
    {
        //vMifValue[i] = qToBigEndian(vMifValue[i]);
        out.writeRawData(reinterpret_cast<char*>(&vMifValue[i]), 4);
        //out << qToBigEndian(i);
    }
    //m_FramTail = qToBigEndian(m_FramTail);
    out.writeRawData(reinterpret_cast<char*>(&m_FramTail), 4);



    udpSocket->writeDatagram(ba, targetAddress, targetPort);
    qDebug() << "enter sendconfig";
}

void Qt5Demo::on_pbPowerOn_clicked()
{
    quint32 cmd;
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    m_ModeVal = 0x01aaaaaa;
    out << qToBigEndian(m_FramHead);
    out << qToBigEndian(m_ModeVal);
    if (ui.pb_PowerOn->text() == "打开电源")
    {
        cmd = (0xeeee << 16) | (0x01 & 0xffff);
        ui.pb_PowerOn->setText("关闭电源");
    }
    else if (ui.pb_PowerOn->text() == "关闭电源")
    {
        cmd = (0xeeee << 16) | (0x00 & 0xffff);
        ui.pb_PowerOn->setText("打开电源");
    }
    
    out << qToBigEndian(cmd);
    out << qToBigEndian(m_FramTail);
    udpSocket->writeDatagram(ba, targetAddress, targetPort);

    timer->stop();
    qDebug() << "enter pbPowerOn";
}

void Qt5Demo::on_pbStartWork_clicked()
{
    quint32 cmd;
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    m_ModeVal = 0x02aaaaaa;
    out << qToBigEndian(m_FramHead);
    out << qToBigEndian(m_ModeVal);
    if (ui.pb_StartWork->text() == "开始工作")
    {
        cmd = (0xdddd << 16) | (0x01 & 0xffff);
        ui.pb_StartWork->setText("停止工作");
    }
    else if (ui.pb_StartWork->text() == "停止工作")
    {
        cmd = (0xdddd << 16) | (0x00 & 0xffff);
        ui.pb_StartWork->setText("开始工作");
    }

    out << qToBigEndian(cmd);
    out << qToBigEndian(m_FramTail);
    udpSocket->writeDatagram(ba, targetAddress, targetPort);


    qDebug() << "enter pbStartWork";
}

void Qt5Demo::on_pbWavePreview_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", "./", "txt File (*.txt)");
    QFile file(filePath);
    bool flag = false;
    bool ok;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream textStream(&file);
        QString str;
        str = textStream.readAll();
        QStringList numList = str.split(" ", QString::SkipEmptyParts);
        QString outputstr;

        for (size_t i = 0; i < numList.count(); i+=4) 
        {
            QString lineStr;
            for (size_t j = i; j < i+4; j++)
            {
                lineStr += numList[j] + " ";
            }
            outputstr += lineStr.trimmed() + "\n";
        }
        QFile ofile("pp.txt");
        if (!ofile.open(QIODevice::WriteOnly | QIODevice::Text)) 
        {
            qDebug() << "open pp.txt error";
        }
        QTextStream out(&ofile);
        out << outputstr;



        file.close();
        ofile.close();
    }
    else
    {
        qDebug() << "Open failed." << file.errorString();
    }



    //m_chart->addAxis(m_axisX, Qt::AlignLeft);
    //m_chart->addAxis(m_axisY, Qt::AlignBottom);
    //m_chart->addSeries(m_lineSeries_A);
    //m_chart->setAnimationOptions(QChart::SeriesAnimations);

    //for (int i = 0; i < vMifValue.size(); i++)
    //{
    //    QPointF tmpF;
    //    tmpF.setX(static_cast<qreal>(i));
    //    tmpF.setY(static_cast<qreal>(vMifValue[i]));
    //    m_lineSeries_A->append(tmpF);
    //    qDebug() << tmpF << endl;
    //}

    //m_lineSeries_A->attachAxis(m_axisX);
    //m_lineSeries_A->attachAxis(m_axisY);

    //ui.graphicsView->setChart(m_chart);
    //ui.graphicsView->setRenderHint(QPainter::Antialiasing);
    //
    //qDebug() << "enter pbWavePreview";
}

void Qt5Demo::on_Timeout()
{
    if (udpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        QByteArray recvData = datagram.data();
        qDebug() << "recvData:" << recvData;

    }

    QString str_cnt = QString::number(cnt);
    ui.le_ADC->setText(str_cnt);
    cnt++;
    qDebug() << "cnt=" << cnt;
}

int Qt5Demo::ParseMifFile(QString fileName,QVector<quint32> &vec)
{
 
    QFile file(filePath);
    bool flag = false;
    bool ok;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream textStream(&file);
        QString str;
        while (!textStream.atEnd())
        {
            str = textStream.readLine();
            //qDebug() << str;
            if ((str.contains("BEGIN")==true)||(str.contains("begin")==true))
            {
                flag = true;
                continue;
            }
            if ((str.contains("END") == true) || (str.contains("end") == true))
            {
                flag = false;
                continue;
            }
            if (flag == true)
            {
                str = str.replace("\n", "");
                str = str.replace(";", "");
                QStringList strList = str.split(":");
                unsigned short s = strList[1].toUShort(&ok);
                if (ok)
                {
                    quint32 i = static_cast<quint32>(s);
                    vec.push_back(i);
                }
            }

        }
        file.close();
    }
    else
    {
        qDebug() << "Open failed." << file.errorString();
    }
    return 1;
}

void Qt5Demo::on_pbMifOpen_clicked()
{
    filePath = QFileDialog::getOpenFileName(this, "Open File", "./", "mif File (*.mif)");
    ui.le_MifFile->setText(filePath);
    if (!filePath.isEmpty())
    {
        ParseMifFile(filePath,vMifValue);
        //for (int i = 0; i < vMifValue.size(); i++)
        //{
        //    qDebug() << i<<":"<<vMifValue[i] << endl;
        //}
    }
}

void Qt5Demo::on_pbOpenDevice_clicked()
{
    bool ok;
    quint32 cmd;
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    //targetAddress = QHostAddress("192.168.1.10");
    //targetPort = 5001;
    QString tmp = "打开设备";
    QString tmp2 = ui.pb_OpenDevice->text();
    if (ui.pb_OpenDevice->text() == "打开设备")
    {
        targetAddress = QHostAddress(ui.le_ipAddr->text());
        targetPort = ui.le_ipPort->text().toUShort(&ok, 10);
        if (ok)
        {
            udpSocket->bind(targetAddress, targetPort);
            cmd = qToBigEndian(0x12345678);
            out << cmd;
            udpSocket->writeDatagram(ba, targetAddress, targetPort);
        }
        ui.pb_OpenDevice->setText("关闭设备");
    }
    else if(ui.pb_OpenDevice->text() == "关闭设备")
    {
        ui.pb_OpenDevice->setText("打开设备");
    }

    

}

void Qt5Demo::ParaInit()
{
    m_ModeVal = 0;
    m_t1 = 50;
    m_t2 = 70;
    m_t3 = 50;
    m_t4 = 50;
    m_t5 = 300;
    m_t6 = 0;
    m_t7 = 20;
    m_t8 = 30;
    m_PhaseDelay = 0;
    m_CycleTimes = 5;
    m_CycleDelay = 10;
    m_PowerOut = 10;
    m_Fctrl = 0;
    m_Pctrl = 0;
    m_Fclk = 100000;
    m_DDSTime = 0;
    m_FramHead = 0x5a5a5a5a;
    m_FramTail = 0xa5a5a5a5;
}