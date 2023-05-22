#include "wave.h"
#include "qstring.h"
#include "qfiledialog.h"
#include "qtextstream.h"
#include "qdebug.h"
#include "qendian.h"

wave::wave(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ch1_axisX = new QValueAxis();
	ch1_axisY = new QValueAxis();
	ch1_lineSeries_A = new QSplineSeries();
	ch1_lineSeries_B = new QSplineSeries();
	ch1_chart = new QChart();


	ch2_axisX = new QValueAxis();
	ch2_axisY = new QValueAxis();
	ch2_lineSeries_A = new QSplineSeries();
	ch2_lineSeries_B = new QSplineSeries();
	ch2_chart = new QChart();

	ch3_axisX = new QValueAxis();
	ch3_axisY = new QValueAxis();
	ch3_lineSeries_A = new QSplineSeries();
	ch3_lineSeries_B = new QSplineSeries();
	ch3_chart = new QChart();
}

wave::~wave()
{}


void wave::on_pbStart_clicked()
{
	QString filePath = QFileDialog::getOpenFileName(this, "Open File", "./", "txt file(*.txt)");
	QFile file(filePath);
	QString readText;
	if (file.open(QIODevice::ReadOnly)) {
		QTextStream in(&file);
		readText = in.readAll();
		qDebug() << readText;
		file.close();
	}
	QStringList strList = readText.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
	//qDebug() << strList.size();
	foreach(const QString & str, strList) {
		//qDebug() << str;
		QByteArray byteData = QByteArray::fromHex(str.toLatin1());
		//qDebug() << byteData.toHex();
		//quint16 hexValue = byteData.toUInt(nullptr, 16);
		//qDebug() << hexValue;
		quint32 hexValue = *(reinterpret_cast<quint32*>(byteData.data()));
	/*	qDebug() << QString("Hex value:0x%1").arg(qToBigEndian(hexValue),0,16);*/
		hexValue = qToBigEndian(hexValue);
		quint8 ch = hexValue >> 24;
		quint32 val = hexValue & 0xffffff;
		if (ch == 1) {
			ch1.append(val);
		}
		else if (ch == 3) {
			ch2.append(val);
		}
		else if (ch == 4) {
			ch3.append(val);
		}
	}

	//qDebug() << ch1.size();

	ch1_chart->addAxis(ch1_axisX, Qt::AlignLeft);
	ch1_chart->addAxis(ch1_axisY, Qt::AlignBottom);
	ch1_chart->addSeries(ch1_lineSeries_A);
	ch1_chart->setAnimationOptions(QChart::SeriesAnimations);

	ch2_chart->addAxis(ch2_axisX, Qt::AlignLeft);
	ch2_chart->addAxis(ch2_axisY, Qt::AlignBottom);
	ch2_chart->addSeries(ch2_lineSeries_A);
	ch2_chart->setAnimationOptions(QChart::SeriesAnimations);

	ch3_chart->addAxis(ch3_axisX, Qt::AlignLeft);
	ch3_chart->addAxis(ch3_axisY, Qt::AlignBottom);
	ch3_chart->addSeries(ch3_lineSeries_A);
	ch3_chart->setAnimationOptions(QChart::SeriesAnimations);


	for (int i = 0; i < ch1.size(); i++)
	{
		QPointF tmpF;
		tmpF.setX(static_cast<qreal>(i));
		tmpF.setY(static_cast<qreal>(ch1[i]));
		ch1_lineSeries_A->append(tmpF);
		//qDebug() << tmpF << endl;
	}

	ch1_lineSeries_A->attachAxis(ch1_axisX);
	ch1_lineSeries_A->attachAxis(ch1_axisY);

	ui.gv_ch1->setChart(ch1_chart);
	ui.gv_ch1->setRenderHint(QPainter::Antialiasing);


	for (int i = 0; i < ch2.size(); i++)
	{
		QPointF tmpF;
		tmpF.setX(static_cast<qreal>(i));
		tmpF.setY(static_cast<qreal>(ch2[i]));
		ch2_lineSeries_A->append(tmpF);
		//qDebug() << tmpF << endl;
	}

	ch2_lineSeries_A->attachAxis(ch2_axisX);
	ch2_lineSeries_A->attachAxis(ch2_axisY);

	ui.gv_ch2->setChart(ch2_chart);
	ui.gv_ch2->setRenderHint(QPainter::Antialiasing);



	for (int i = 0; i < ch3.size(); i++)
	{
		QPointF tmpF;
		tmpF.setX(static_cast<qreal>(i));
		tmpF.setY(static_cast<qreal>(ch3[i]));
		ch3_lineSeries_A->append(tmpF);
		//qDebug() << tmpF << endl;
	}

	ch3_lineSeries_A->attachAxis(ch3_axisX);
	ch3_lineSeries_A->attachAxis(ch3_axisY);

	ui.gv_ch3->setChart(ch3_chart);
	ui.gv_ch3->setRenderHint(QPainter::Antialiasing);


}