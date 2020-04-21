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
#include <iostream>
#include <string>
#include <vector>
#include <direct.h>

using namespace  std;
namespace Ui {
class osgqt;
}
class NodeVisitor_dyer : public osg::NodeVisitor//用于每帧设置模型的颜色，透明度及光照设置等
{
public:
	NodeVisitor_dyer() :osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{
		open_transparent = true;
	}
	NodeVisitor_dyer(osg::Vec4 given_color, bool if_open) : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{
		color = given_color;
		open_transparent = if_open;
		diffuse = osg::Vec4(0.75f, 0.75f, 0.75f, 0.5f);
		ambient = osg::Vec4(0.75f, 0.75f, 0.75f, 0.5f);
		specular = osg::Vec4(0.75f, 0.75f, 0.75f, 0.5f);
	}
	NodeVisitor_dyer(osg::Vec4 given_color, osg::Vec4 ambientcolor, osg::Vec4 diffusecolor,osg::Vec4 specularcolor, bool if_open) : 
		osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{
		color = given_color;
		open_transparent = if_open;
		ambient = ambientcolor;
		diffuse = diffusecolor;
		specular = specularcolor;
	}
	~NodeVisitor_dyer() {};

	void apply(osg::Geode& geode)
	{
		unsigned int i = 0;
		osg::ref_ptr<osg::Vec4Array> changecolor = new osg::Vec4Array();//根据changecolor中存储的part类型名为此geode上色
		changecolor->push_back(color);
		for (i = 0; i < geode.getNumDrawables(); i++)
		{
			osg::ref_ptr<osg::Geometry> geometry = geode.getDrawable(i)->asGeometry();
			osg::ref_ptr<osg::Array> geocolor = geometry->getColorArray();

			geometry->setColorArray(changecolor);
			geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

			osg::ref_ptr< osg::StateSet > state_set = geometry->getOrCreateStateSet();//设置geode的材质（这样设置相当于所有的geode的材质相同，如果每个part材质不同可能效果更好？）
			osg::ref_ptr< osg::Material > material = new osg::Material;
			material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
			material->setDiffuse(osg::Material::FRONT_AND_BACK,diffuse );
			material->setSpecular(osg::Material::FRONT_AND_BACK,specular);
			material->setAmbient(osg::Material::FRONT_AND_BACK, ambient);
			material->setShininess(osg::Material::FRONT_AND_BACK, 51.2f);
			state_set->setAttributeAndModes(material.get(), osg::StateAttribute::ON);

			if (open_transparent)
			{
				osg::ref_ptr<osg::LightModel> TwoSideLight = new osg::LightModel;
				TwoSideLight->setTwoSided(true);
				//只对模型中的部件进行半透明化，可以在遍历模型部件时，1）设置显示颜色alpha通道小于1，2）开启融合模型，3）同时渲染方式设为透明方式
				//开启融合操作
				state_set->setMode(GL_BLEND, osg::StateAttribute::ON);
				state_set->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);
				//设置渲染模式
				state_set->setRenderingHint(osg::StateSet::TRANSPARENT_BIN); 
				state_set->setMode(GL_CULL_FACE, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);   // 只关闭背面裁剪，造成生成背面不透明，但黑面 ;
				state_set->setAttributeAndModes(TwoSideLight, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);  //再加上双面光照，使背面完全出现！;
			}
		}
	}
	void set_color(osg::Vec4 given_color)
	{
		color = given_color;
	}
	osg::Vec4 get_color()
	{
		return color;
	}

private:
	osg::Vec4 color,ambient,diffuse,specular;
	bool open_transparent;//是否开启半透明模式
};
class modelCallBack1 : public osg::NodeCallback//用于调用颜色、光照等上面的回调类，暂时没用
{
public:
	modelCallBack1() :Color() {}
	modelCallBack1(osg::Vec4 color,bool open):Color(color),open_transparent(open)
	{
	}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		NodeVisitor_dyer dyerb(Color,open_transparent);
		node->accept(dyerb);
		traverse(node, nv);
	}
private:
	osg::Vec4 Color;
	bool open_transparent;
};








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
			osg::Vec3 curvec;
			if (node_name.length() >= 1 && (node_name.substr(node_name.length() - 2, 2) == "_r"))
			{
				double r = 2;
				curvec = dofnode->getCurrentHPR();
				float ani_radians = osg::DegreesToRadians(r);
				float xani_range = osg::DegreesToRadians(range[0]);
				float yani_range = osg::DegreesToRadians(range[1]);
				float zani_range = osg::DegreesToRadians(range[2]);
				float xcur_rad = curvec.x();
				float ycur_rad = curvec.y();
				float zcur_rad = curvec.z();
				if (xcur_rad <xani_range)
				{
					curvec.set(xcur_rad + ani_radians,0,0);
					dofnode->setCurrentHPR(curvec);
				}
				if (ycur_rad < yani_range)
				{
					curvec.set(0, ycur_rad + ani_radians, 0);
					dofnode->setCurrentHPR(curvec);
				}
				if (zcur_rad < zani_range)
				{
					curvec.set(0, 0, zcur_rad + ani_radians);
					dofnode->setCurrentHPR(curvec);
				}
			}
			else if (node_name.length() >= 1 && (node_name.substr(node_name.length() - 2, 2) == "_t"))
			{
				float r = 0.01;
				curvec = dofnode->getCurrentTranslate();
				float xani_range = range[0];
				float yani_range = range[1];
				float zani_range = range[2];
				float xcur_rad = curvec.x();
				float ycur_rad = curvec.y();
				float zcur_rad = curvec.z();
				if (xcur_rad < xani_range)
				{
					curvec.set(xcur_rad + r, 0, 0);
					dofnode->setCurrentTranslate(curvec);
				}
				if (ycur_rad < yani_range)
				{
					curvec.set(0, ycur_rad + r, 0);
					dofnode->setCurrentTranslate(curvec);
				}
				if (zcur_rad < zani_range)
				{
					curvec.set(0, 0, zcur_rad + r);
					dofnode->setCurrentTranslate(curvec);
				}
			}
			else if (node_name.length() >= 2 && (node_name.substr(node_name.length() - 3, 3) == "_rt"))
			{
				/*curvec = dofnode->getCurrentHPR();
				curvec.set(curvec.x(), curvec.y() + r, curvec.z());
				dofnode->setCurrentHPR(curvec);
				curvec = dofnode->getCurrentTranslate();
				curvec.set(curvec.x() + r, curvec.y(), curvec.z());
				dofnode->setCurrentTranslate(curvec);*/
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

public slots:   //申明信号与槽,当树形控件的子选项被改变时执行
    void treeItemChanged(QTreeWidgetItem* item , int column);

private slots:
    void timerUpdate();
    void on_open_clicked();
	void on_remove_clicked();
   // void on_directorycomboBox_currentIndexChanged(int index);
	
    void on_tabWidget_currentChanged(int index);
    void on_treeWidget_itemChanged_child(QTreeWidgetItem *item, int column);
    void SetParentPartiallyChecked(QTreeWidgetItem *itm,int column);
    void on_treeWidget_itemChanged_parent(QTreeWidgetItem *item, int column);
    void update_treewidget();
	void motion_comfirm_clicked();
	void motion_delete_clicked();
	void camera_confirm_clicked();
	void startButton_pressed();
	void generateButton_pressed();
	void motion_play_clicked();
	void showButton_pressed();
	void showpcl_pressed();
    void on_treeWidget_itemSelectionChanged();
    void on_Reset_clicked();
	void on_modeltranslate_clicked();
	void on_modelrotate_clicked();
	//void camera_height_ok();
	//void shooting_times_ok();
	//void shooting_radius_ok();
	void camera_play_clicked();
	void camera_delete_clicked();
	bool isdof(osg::Node& node);
	
private:
	void move(osg::ref_ptr<osg::Node>, double range[4], bool& ismove);
	void changecolor(osg::ref_ptr<osg::Node> node, osg::Vec4 Color, bool open_transparent);
	QTreeWidgetItem* findpar(QTreeWidgetItem* fitem);
	struct motionif
	{
		double motioninfo[4];
	};
    Ui::osgqt *ui;
    osg::ref_ptr<osg::Node>  EditModel;
    
    osg::ref_ptr<osg::Node>  loadedModel2;
    QtOsgView * ViewerWindow;
	QMap<QString, QTreeWidgetItem*> itemMap_; //注释： 树形图每个栏目对应id
	QMap<osg::ref_ptr<osg::MatrixTransform>, double> height;
	void removenode(std::string name);
	bool motionfunc();
	QMap<osg::ref_ptr<osg::Node>, motionif> motion;//保存运动信息，用于拍照和animate
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
	osg::ref_ptr<osg::Node> model;
	osg::ref_ptr<osg::Group> root= new osg::Group;
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
};

#endif // OSG QT_H
