#include "wave.h"
#include "qstring.h"
#include "qfiledialog.h"
#include "qtextstream.h"
#include "qdebug.h"

wave::wave(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

wave::~wave()
{}


void wave::on_pb_Start_clicked()
{
	//QString filePath = QFileDialog::getOpenFileName(this, "Open File", "./", "txt file(*.txt)");
	qDebug() << "hello";
	//QFile file("adc.txt");
	//if (file.open(QIODevice::ReadOnly)) {
	//	QTextStream in(&file);
	//	QString readText = in.readLine();
	//	qDebug() << readText;
	//	file.close();
	//}

}