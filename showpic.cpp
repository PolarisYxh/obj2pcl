#include "showpic.h"
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <QMessageBox>
showpic::showpic(QWidget* parent,QString x) :
	QWidget(parent),
	ui(new Ui::showpic),savepicpath(x)
{
	ui->setupUi(this);

	//ui->leftLayout->addWidget(ViewerWindow);
	//ui->rightLayout->addWidget(ViewerWindow1);
	ui->totalLayout->setStretch(0, 360);
	ui->totalLayout->setStretch(1, 360);
	this->setLayout(ui->totalLayout);
	//showcad();
	showrgb();
}
showpic::~showpic() {
	// ×¢ÊÍ£ºÎö¹¹º¯Êý
	delete ui;
}
void showpic::showrgb()
{
	QImage* img = new QImage;
	if (!(img->load(savepicpath))) //¼ÓÔØÍ¼Ïñ
	{
		QMessageBox::information(this,
			tr("error"),
			tr("open error!"));
		delete img;
		return;
	}
	ui->rgb->setPixmap(QPixmap::fromImage(*img));
}