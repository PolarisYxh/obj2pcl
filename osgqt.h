#ifndef OSGQT_H
#define OSGQT_H

#include <QWidget>
#include <osg/Node>
#include <osg/TriangleFunctor>
#include <qtosgview.h>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHBoxLayout>
#include <QMessagebox>
#include <QQuaternion>
#include <QVector4d>
#include <QMatrix4x4>
#include <sstream>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/LightModel>
#include <osgGA/TrackballManipulator>

#include <osg/Material>

#include <osg/TextureCubeMap>
#include <osg/TexGen>
#include <osg/TexEnvCombine>
#include <osg/ShadeModel>
#include <osgSim/DOFTransform>
#include <osg/Multisample>
#include <osgUtil/ReflectionMapGenerator>
#include <osgUtil/HighlightMapGenerator>
#include <osgUtil/HalfWayMapGenerator>
#include <osg/ComputeBoundsVisitor>
#include <osg/Group>
#include <osg/Node>
#include <osg/Geometry>
#include <osg/PolygonMode>
#include <osg/LineWidth>
#include <osg/PolygonOffset>
#include <osgFX/Outline>
#include <osg/BlendColor>  
#include <osg/BlendFunc> 
#include <osg/Geode>  
#include <osg/Depth>  

#include <io.h>
#include <osg/LightModel>
#include <osg/Group>
#include <osgGA/TrackballManipulator>
#include <osg/Camera>
#include <osg/Image>
#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/TextureCubeMap>
#include <osg/TexGen>
#include <osg/TexEnvCombine>
#include <osg/DrawPixels>
#include <osg/Texture2D>
#include <osgUtil/HighlightMapGenerator>
#include <osgUtil/SceneView>
#include "mainwindow.h"
#include "Tool_test_2.h"
#include <string>
#include <vector>
#include<windows.h>
#include<fstream>
#include<osg/LOD>
#include<cmath>
#include <stdlib.h> 
#include <random>
#include <iostream>
#include <string>
#include <vector>
#include <direct.h>
#include "QtImage2PCL.h"

//#include <omp.h>
using namespace  std;
namespace Ui {
class osgqt;
}

class modelCallBack : public osg::NodeCallback//模型dof运动的回调函数
{
  public:
	     modelCallBack() :range(){}
		 modelCallBack(double x0, double x1, double x2, double x3) 
		 { 
			 range[0] = x0; 
			 range[1] = x1;
			 range[2] = x2;
			 range[3] = x3;
		 }
	
		virtual void operator()(osg::Node * node, osg::NodeVisitor * nv)
		{
			std::string node_name = node->getName();
			osg::ComputeBoundsVisitor boundVisitor;
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(node);
			dofnode->accept(boundVisitor);
			osg::Vec3 curvec,curvec1;
			if (node_name.length() >= 2 && (node_name.substr(node_name.length() - 2, 2) == "_r"))
			{
				double r = range[1]/range[3]/10.0;
				curvec = dofnode->getCurrentHPR();
				float ani_radians = osg::DegreesToRadians(r);
				float xani_range = osg::DegreesToRadians(range[0]);
				float yani_range = osg::DegreesToRadians(range[1]);
				float zani_range = osg::DegreesToRadians(range[2]);
				float xcur_rad = curvec.x();
				float ycur_rad = curvec.y();
				float zcur_rad = curvec.z();
				if (yani_range > 0)
				{
					if (ycur_rad < yani_range)//_r正确的在y上
					{
						curvec.set(0, ycur_rad + ani_radians, 0);
						dofnode->setCurrentHPR(curvec);
					}
				}
				else if (yani_range < 0)
				{
					if (ycur_rad > yani_range)//_r正确的在y上
					{
						curvec.set(0, ycur_rad - ani_radians, 0);
						dofnode->setCurrentHPR(curvec);
					}
				}
				
			}
			else if (node_name.length() >= 2 && (node_name.substr(node_name.length() - 2, 2) == "_t"))
			{
				float r = range[0]/range[3]/10;
				curvec = dofnode->getCurrentTranslate();
				float xani_range = range[0];
				float yani_range = range[1];
				float zani_range = range[2];
				float xcur_rad = curvec.x();
				float ycur_rad = curvec.y();
				float zcur_rad = curvec.z();
				if (xani_range>0)
				{
					if (xcur_rad < xani_range)//_t正确的是在x上
					{
						curvec.set(xcur_rad + r, 0, 0);
						dofnode->setCurrentTranslate(curvec);
					}
				}
				else if (xani_range < 0)
				{
					if (xcur_rad > xani_range)//_t正确的是在x上
					{
						curvec.set(xcur_rad - r, 0, 0);
						dofnode->setCurrentTranslate(curvec);
					}
				}
			}
			else if (node_name.length() >= 2 && (node_name.substr(node_name.length() - 2, 2) == "_s"))
			{
				double r1 = range[1] / range[3] / 10.0;
				curvec = dofnode->getCurrentHPR();
				float ani_radians = osg::DegreesToRadians(r1);
				float yani_range = osg::DegreesToRadians(range[1]);
				float ycur_rad = curvec.y();
				
				if (yani_range > 0)
				{
					if (ycur_rad < yani_range)//_r正确的在y上
					{
						curvec.set(0, ycur_rad + ani_radians, 0);
						dofnode->setCurrentHPR(curvec);
					}
				}
				else if (yani_range < 0)
				{
					if (ycur_rad > yani_range)//_r正确的在y上
					{
						curvec.set(0, ycur_rad - ani_radians, 0);
						dofnode->setCurrentHPR(curvec);
					}
				}
			
				float r = range[0] / range[3] / 10.0;
				curvec1 = dofnode->getCurrentTranslate();
				float xani_range = range[0];
				float xcur_rad = curvec.x();

				if (xani_range > 0)
				{
					if (xcur_rad < xani_range)//_t正确的是在x上
					{
						curvec.set(xcur_rad + r, 0, 0);
						dofnode->setCurrentTranslate(curvec);
					}
				}
				else if (xani_range < 0)
				{
					if (xcur_rad > xani_range)//_t正确的是在x上
					{
						curvec.set(xcur_rad - r, 0, 0);
						dofnode->setCurrentTranslate(curvec);
					}
				}
			}
			traverse(node, nv);
		}
 private:
	 double range[4] = {0};
};

class osgqt : public QWidget
{
    Q_OBJECT

public:
    explicit osgqt(QWidget *parent = 0);
	void osgqt::initwindow();
    void addmodel();
	void viewmotion();
	void drawCircle();
	void placecamera();
    osg::ref_ptr<osg::Vec3Array> getarray(osg::ref_ptr<osg::Node> node,osg::Matrix m);
    ~osgqt();
  
   

private slots:
    void timerUpdate();// 注释：计时器函数，用于更新显示，里面主要进行模型部件右键和树状图点选、ctrl+框选的更新显示
	void on_Reset_clicked();//点击reset camera按钮

    void on_tabWidget_currentChanged(int index);//没什么用，但勿删
    void on_treeWidget_itemChanged_child(QTreeWidgetItem *item, int column);//点击item复选框时，改变子节点的复选框状态
    void SetParentPartiallyChecked(QTreeWidgetItem *itm,int column);
    void on_treeWidget_itemChanged_parent(QTreeWidgetItem *item, int column);//点击item复选框时，改变父节点的复选框状态
	void treeItemChanged(QTreeWidgetItem* item, int column);//当树形控件的子选项被改变时执行，取消 则模型部件消失，勾上则模型部件显示
	void on_treeWidget_itemExpanded(QTreeWidgetItem* item);//点击某项item展开后，自动加载子项
	void on_treeWidget_itemSelectionChanged();//点击了树形图的文字
	//void update_all_treewidget();// 注释：加载模型后用于直接加载所有节点树形结构图,暂时不用，加载全部比较慢
	void update_treewidget();//加载模型后用于直接加载第一和第二层节点树形结构图，异步加载方式

    void on_open_clicked();//add model
	void on_remove_clicked();////删除所选模型的树状图、所属模型的motion信息、lastmotion信息、root下的所属模型
	void on_modeltranslate_clicked();//点击model模块的平移按钮
	void on_modelrotate_clicked();//点击model模块的旋转按钮

	void motion_comfirm_clicked();
	void motion_delete_clicked();
	void motion_play_clicked();

	void camera_confirm_clicked();	
	void camera_play_clicked();//点击camera模块的confirm按钮
	void camera_delete_clicked();//点击camera模块的delete按钮

	void startButton_pressed();//开始拍照
	void generateButton_pressed();//开始生成点云
	
	void showButton_pressed(); //点击show picture
	void showpcl_pressed();//点击show pointcloud

	//自动场景相关
	//void on_pushButton_clicked();//自动场景入口
	//void addmodel1(int i);//用于scan2cadtxt文件添加shapenet模型
	//void addmodel2(int i);//用于文本文件添加shape2motion模型
	//void on_pushButton_2_clicked();//删除所有节点，使整个程序reset
	//void on_pushButton_3_clicked();
private:
	void move(osg::ref_ptr<osg::Node>, double range[4], bool& ismove);//用于拍照过程中运动部件的运动

	void changecolor(osg::ref_ptr<osg::Node> node, osg::Vec4 Color, bool open_transparent);//没用

	//void update_treewidget1();//用于自动搭建场景里更新树形结构图，并且给第一个dof添加运动信息到motion结构体
	//void update_treewidget2();//用于拍摄自动搭建场景rgb图，添加所有dof到motion中
	//void autostartshoot();//自动场景的自动depth+label拍照功能
	//void autorgbshoot();//自动场景的自动rgb拍照功能
	//void autorgbshoot1();//用于场景flt的自动拍照

	QTreeWidgetItem* findpar(QTreeWidgetItem* fitem);//找到树形图节点的最上层父节点
	osg::ref_ptr<osg::Node> findnodepar(osg::ref_ptr<osg::Node> node);//返回模型节点的trans父节点

	bool isdof(osg::Node& node);
	//struct motionif
	//{
	//	double motioninfo[4];//012对应xyz运动范围，3对应次数
	//};
    Ui::osgqt *ui;
    QtOsgView * ViewerWindow;
	QMap<osg::ref_ptr<osg::MatrixTransform>, double> height;
	void removenode(std::string name);
	bool motionfunc();
	//QMap<osg::ref_ptr<osg::Node>, motionif> motion;//保存运动信息，用于拍照和animate
	//QVector<osg::ref_ptr<osg::Node>> lastmotion;//animate中记录上次animate完未reset的部件
	//QString motionname;
	//float motion[2];
	double camera_height;
	double shooting_radius;
	int shooting_times;
	QString savepicpath;
	QString savepclpath;
	QString filepath;
	std::string current_file_name = ""; //注释： 用于在信息框显示当前模型名
	
	//Photographer pg;
	osg::ref_ptr<osg::Node> model=new osg::Node;
	osg::ref_ptr<osg::Group> root= new osg::Group;
	osg::ref_ptr<osg::MatrixTransform> roottrans = new osg::MatrixTransform;
	//表盘的几何节点
	osg::ref_ptr<osg::Geode> clockGeode = new osg::Geode;
	//圆圈
	osg::ref_ptr<osg::Geometry> clockGeometry = new osg::Geometry;
	osg::ref_ptr<osg::Node> kinect;
	osg::ref_ptr<osg::Node> viewport;
	//motion
	std::vector<osg::Group*> dofnodes;
	int dof_ID;
	modelCallBack * cb;
	struct modeldata
	{
		string cate;
		string id;
		osg::Matrix t[4];//t0 平移 t1 旋转 t2 缩放 t3 boundingbox到模型中心的平移转换
		osg::Vec3 bobox;//boundingbox
	}* modelinfo;//scene0700_00.txt中的信息
	osg::Matrix scanM;
	vector<osg::Vec4> VColor;
	QIcon dir;
	QIcon dofp;
};

#endif // OSG QT_H
