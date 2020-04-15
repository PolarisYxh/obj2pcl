#pragma once
#include "ui_showpic.h"
#include <QString>
#include <osg/Node>
#include <osg/TriangleFunctor>
#include "Photographer_sx.h"
#include <osg/Point>
namespace Ui {
	class showpic;
}
class showpic :public QWidget
{
	Q_OBJECT
public:
	explicit showpic(QWidget* parent=NULL,QString savepicpath="./");
	//showpcl( QString filepath = "./", QString savepclpath = "./");
	~showpic();
private slots:
private:
	Ui::showpic * ui;
	QString savepicpath;

	void showrgb();
	void showdep();
};