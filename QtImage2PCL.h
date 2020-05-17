#pragma once
#include <QApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <osg/Matrix>
#include <osg/Geode>
#include <osg/Matrixd>
#include <QFileDialog>
#include <QDebug>
#include <fstream>
#include <QString>
#include <QDir>
#include <QMessagebox>
#include "qprogressbar.h"
#include "qprogressdialog.h"
class createPCL //: public QWidget
{
	//Q_OBJECT
public:
	//createPCL(QWidget* parent = 0);
	int createSinglePCL(QString rootpath1);
private:
	struct dofpart
	{
		osg::Vec4d motioninfo;
		osg::Vec4d color;
		osg::Matrix PutMatrix;
		osg::Matrix ModelMatrix;
	};

};

	//int createSinglePCL(QString rootpath1, QWidget* parent);
	//struct dofpart
	//{
	//	osg::Vec4d motioninfo;
	//	osg::Vec4d color;
	//	osg::Matrix PutMatrix;
	//	osg::Matrix ModelMatrix;
	//};

