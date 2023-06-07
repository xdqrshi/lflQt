#include "Qt5Demo.h"
#include "qnetworkdatagram.h"
#include "qfile.h"
#include "qfiledialog.h"
#include "qendian.h"
#include "qstringlist.h"
#include "qnetworkinterface.h"
#include "qhostaddress.h"
#include "qthread.h"
#include "qmessagebox.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8");
#endif // _MSC_VER >= 1600


Qt5Demo::Qt5Demo(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //connect(this, &ui.pb_SendConfig->clicked(), this, this->on_pbSendConfig());
    ParaInit();

    ui.le_Freq->setEnabled(false);
    ui.le_PhaseOffset->setEnabled(false);
    connect(ui.cb_ch2, &QCheckBox::clicked, this, &Qt5Demo::on_cbCh2_clicked);

    udpSocket = new QUdpSocket(this);
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    m_lineSeries_A = new QSplineSeries();
    m_lineSeries_B = new QSplineSeries();
    m_lineSeries_A->setPen(QPen(Qt::red));
    m_lineSeries_B->setPen(QPen(Qt::blue));
    m_chart = new QChart();
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_chart->addSeries(m_lineSeries_A);
    m_chart->addSeries(m_lineSeries_B);
    m_chart->setAnimationOptions(QChart::NoAnimation);
    //m_chart->setAnimationOptions(QChart::SeriesAnimations);

    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        QHostAddress address = ipAddressesList.at(i);
        if (!address.isNull() && address != QHostAddress::LocalHost &&
            address.protocol() == QAbstractSocket::IPv4Protocol) {
            qDebug() << "IP address:" << address.toString();
        }
    }


    ui.le_ipAddr->setText("192.168.3.75");
    ui.le_ipPort->setText("5001");
    timer = new QTimer();



    // 打印所有 IP 地址




}

Qt5Demo::~Qt5Demo()
{}

void Qt5Demo::on_cbCh2_clicked(bool checked)
{
    if (checked)
    {
        ui.le_Freq->setEnabled(true);
        ui.le_PhaseOffset->setEnabled(true);
    }
    else
    {
        ui.le_Freq->setEnabled(false);
        ui.le_PhaseOffset->setEnabled(false);
    }


}

void Qt5Demo::on_pbSendConfig_clicked()
{


    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);

    m_PhaseDelay = static_cast<quint32>(m_Fclk * (ui.le_PhaseOffset->text().toDouble()) / (ui.le_Freq->text().toDouble()) / 360);
    m_DDSTime = vMifValue.size();

    m_ModeVal = 0x01bbbbbb;

    out.writeRawData(reinterpret_cast<char*>(&m_FramHead),4);
    out.writeRawData(reinterpret_cast<char*>(&m_ModeVal), 4);
    out.writeRawData(reinterpret_cast<char*>(&m_t1), 4);
    out.writeRawData(reinterpret_cast<char*>(&m_t2), 4);
    out.writeRawData(reinterpret_cast<char*>(&m_t3), 4);
    out.writeRawData(reinterpret_cast<char*>(&m_t4), 4);
    out.writeRawData(reinterpret_cast<char*>(&m_t5), 4);
    m_t6 = m_DDSTime - m_t7;
    out.writeRawData(reinterpret_cast<char*>(&m_t6), 4);
    out.writeRawData(reinterpret_cast<char*>(&m_t7), 4);
    out.writeRawData(reinterpret_cast<char*>(&m_t8), 4);
    out.writeRawData(reinterpret_cast<char*>(&m_PhaseDelay), 4);
    out.writeRawData(reinterpret_cast<char*>(&m_CycleTimes), 4);
    out.writeRawData(reinterpret_cast<char*>(&m_CycleDelay), 4);
    out.writeRawData(reinterpret_cast<char*>(&m_PowerOut), 4);
    out.writeRawData(reinterpret_cast<char*>(&m_Fctrl), 4);
    out.writeRawData(reinterpret_cast<char*>(&m_Pctrl), 4);
    quint32 vMifValue_Len = static_cast<quint32>(vMifValue.size());
    out.writeRawData(reinterpret_cast<char*>(&vMifValue_Len), 4);
    for (quint32 i = 0; i < vMifValue.size(); i++)
    {
        out.writeRawData(reinterpret_cast<char*>(&vMifValue[i]), 4);
    }
    out.writeRawData(reinterpret_cast<char*>(&m_FramTail), 4);

    udpSocket->writeDatagram(ba, targetAddress, targetPort);
    qDebug() << "enter sendconfig";
}

void Qt5Demo::on_pbPowerOn_clicked()
{

    if (ui.cb_ch2->isChecked())
    {
        //通道1
        {
            quint32 cmd;
            QByteArray ba;
            QDataStream out(&ba, QIODevice::WriteOnly);
            m_ModeVal = 0x01aaaaaa;
            out.writeRawData(reinterpret_cast<char*>(&m_FramHead), 4);
            out.writeRawData(reinterpret_cast<char*>(&m_ModeVal), 4);
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
            out.writeRawData(reinterpret_cast<char*>(&cmd), 4);
            out.writeRawData(reinterpret_cast<char*>(&m_FramTail), 4);
            udpSocket->writeDatagram(ba, targetAddress, targetPort);
         }
        QThread::msleep(10);
        {
            //通道2
            quint32 cmd;
            QByteArray ba;
            QDataStream out(&ba, QIODevice::WriteOnly);
            m_ModeVal = 0x02aaaaaa;
            out.writeRawData(reinterpret_cast<char*>(&m_FramHead), 4);
            out.writeRawData(reinterpret_cast<char*>(&m_ModeVal), 4);
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
            out.writeRawData(reinterpret_cast<char*>(&cmd), 4);
            out.writeRawData(reinterpret_cast<char*>(&m_FramTail), 4);
            udpSocket->writeDatagram(ba, targetAddress, targetPort);
        }
    }
    else
    {
        /// <summary>
        /// 只使用通道1
        /// </summary>
        quint32 cmd;
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        m_ModeVal = 0x01aaaaaa;
        out.writeRawData(reinterpret_cast<char*>(&m_FramHead), 4);
        out.writeRawData(reinterpret_cast<char*>(&m_ModeVal), 4);
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
        out.writeRawData(reinterpret_cast<char*>(&cmd), 4);
        out.writeRawData(reinterpret_cast<char*>(&m_FramTail), 4);
        udpSocket->writeDatagram(ba, targetAddress, targetPort);


    }

   
}

void Qt5Demo::on_pbStartWork_clicked()
{
    if (ui.cb_ch2->isChecked())
    {
        quint32 cmd;
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        m_ModeVal = 0x02aaaaaa;
        out.writeRawData(reinterpret_cast<char*>(&m_FramHead), 4);
        out.writeRawData(reinterpret_cast<char*>(&m_ModeVal), 4);
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


        out.writeRawData(reinterpret_cast<char*>(&cmd), 4);
        out.writeRawData(reinterpret_cast<char*>(&m_FramTail), 4);
        udpSocket->writeDatagram(ba, targetAddress, targetPort);

    }
    else
    {
        quint32 cmd;
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        m_ModeVal = 0x01aaaaaa;
        out.writeRawData(reinterpret_cast<char*>(&m_FramHead), 4);
        out.writeRawData(reinterpret_cast<char*>(&m_ModeVal), 4);
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

        out.writeRawData(reinterpret_cast<char*>(&cmd), 4);
        out.writeRawData(reinterpret_cast<char*>(&m_FramTail), 4);
        udpSocket->writeDatagram(ba, targetAddress, targetPort);

    }




}

void Qt5Demo::on_pbWavePreview_clicked()
{
    if (ui.cb_ch2->isChecked())
    {
        if (ui.le_Freq->text().isEmpty() || ui.le_PhaseOffset->text().isEmpty())
        {
            QMessageBox::information(this, tr("警告"), tr("请填写频率和相位差"));
            return;
        }

    }
 

    ui.graphicsView->repaint();
    m_lineSeries_A->clear();
    m_lineSeries_B->clear();
    if (ui.cb_ch2->isChecked())
    {
        m_PhaseDelay = static_cast<quint32>(m_Fclk * (ui.le_PhaseOffset->text().toDouble()) / (ui.le_Freq->text().toDouble()) / 360);
    }
    else
    {
        m_PhaseDelay = 0;
    }
   

    QList<QPointF> points_A, points_B;
    quint32 axisY_min = 0;
    quint32 axisY_max = 0;
    for (int i = 0; i < m_PhaseDelay; i++)
    {
        QPointF tmpF;
        tmpF.setX(static_cast<qreal>(i));
        tmpF.setY(static_cast<qreal>(8192));
        points_B.append(tmpF);
        //qDebug() << tmpF << endl;
    }


    for (int i = 0; i < vMifValue.size(); i++)
    {
        QPointF tmpF;
        tmpF.setX(static_cast<qreal>(i));
        tmpF.setY(static_cast<qreal>(vMifValue[i]));
        if (vMifValue[i] > axisY_max) axisY_max = vMifValue[i];
        if (vMifValue[i] < axisY_min) axisY_min = vMifValue[i];
        points_A.append(tmpF);
        tmpF.setX(static_cast<qreal>(i + m_PhaseDelay));
        tmpF.setY(static_cast<qreal>(vMifValue[i]));
        points_B.append(tmpF);

    }
    m_lineSeries_A->replace(points_A);
    if (ui.cb_ch2->isChecked())
    {
        m_lineSeries_B->replace(points_B);
    }
    

    m_axisX->setRange(0, static_cast<qreal>(vMifValue.size() + m_PhaseDelay));
    m_axisY->setRange(static_cast<qreal>(axisY_min), static_cast<qreal>(axisY_max));
    m_lineSeries_A->attachAxis(m_axisX);
    m_lineSeries_A->attachAxis(m_axisY);
    m_lineSeries_B->attachAxis(m_axisX);
    m_lineSeries_B->attachAxis(m_axisY);

    ui.graphicsView->setChart(m_chart);
    ui.graphicsView->chart()->legend()->hide();
    ui.graphicsView->setRenderHint(QPainter::Antialiasing);
    //
    //qDebug() << "enter pbWavePreview";
}

void Qt5Demo::on_Timeout()
{
    qDebug() << "timeout";
    if (udpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        QByteArray recvData = datagram.data();
        quint32 v = *(reinterpret_cast<const quint32*>(recvData.data()));
        qDebug() << "v =" << v;
        ui.le_ADC->setText(QString::number(v));

    }


}

int Qt5Demo::ParseMifFile(QString fileName,QVector<quint32> &vec)
{
    vec.clear();
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
    }
}

void Qt5Demo::on_pbOpenDevice_clicked()
{
    bool ok;
    quint32 cmd;
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    if (ui.pb_OpenDevice->text() == "打开设备")
    {

        connect(timer, SIGNAL(timeout()), this, SLOT(on_Timeout()));
        targetAddress = QHostAddress(ui.le_ipAddr->text());
        targetPort = ui.le_ipPort->text().toUShort(&ok, 10);
        if (ok)
        {
            udpSocket->bind(targetAddress, targetPort);
            timer->start(1000);
            cmd = 0x12345678;
            out.writeRawData(reinterpret_cast<char*>(&cmd), 4);
            udpSocket->writeDatagram(ba, targetAddress, targetPort);
        }
        ui.pb_OpenDevice->setText("关闭设备");
    }
    else if(ui.pb_OpenDevice->text() == "关闭设备")
    {
        ui.pb_OpenDevice->setText("打开设备");
        timer->stop();
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