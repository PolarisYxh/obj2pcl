#pragma once
#include "ui_showpcl.h"
#include <QString>
#include <sstream>
#include <QtWidgets/QMainWindow>
#include <osg/Node>
#include <osg/TriangleFunctor>
#include "Photographer_sx.h"
#include "qtosgview.h"
#include <osg/Point>
#include <QFileInfo>
namespace Ui {
	class showpcl;
}
class showpcl :public QWidget
{
	Q_OBJECT
public:
	explicit showpcl(QWidget* parent=NULL,QString filepath="./",QString savepclpath="./");
	//showpcl( QString filepath = "./", QString savepclpath = "./");
	~showpcl();
private slots:
private:
	Ui::showpcl * ui;
	QtOsgView* ViewerWindow;
	QtOsgView* ViewerWindow1;
	osg::ref_ptr<osgGA::TrackballManipulator> manipulator;
	QString savepclpath;
	QString filepath;
	osg::ref_ptr<osg::Node> model;
	osg::ref_ptr<osg::Group> root = new osg::Group;
	void showcad();
	void showpointcloud();
};