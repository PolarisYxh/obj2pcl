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
#include "Photographer_sx.h"
#include "mainwindow.h"
#include "Tool_test_2.h"
#include "showpcl.h"
#include "showpic.h"
//#include "engine.h"
using namespace  std;
namespace Ui {
class osgqt;
}

class modelCallBack : public osg::NodeCallback
{
  public:
	     modelCallBack() :r(0.0),range(0.0){}
		 modelCallBack(float x1, float x2) :r(x1), range(x2) {}
	
		virtual void operator()(osg::Node * node, osg::NodeVisitor * nv)
		{
				//osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(node);
				osg::ComputeBoundsVisitor boundVisitor;
				osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(node);
				dofnode->accept(boundVisitor);
				osg::Vec3 curvec = dofnode->getCurrentHPR();
				float ani_radians = osg::DegreesToRadians(r);
				float ani_range = osg::DegreesToRadians(range);
				float cur_rad = curvec.y();
				if (cur_rad < ani_range)
				{
					curvec.set(0, cur_rad+ani_radians, 0);
					dofnode->setCurrentHPR(curvec);
				}
			    traverse(node, nv);
		}
 private:
    float r,range;
};

class osgqt : public QWidget
{
    Q_OBJECT

public:
    explicit osgqt(QWidget *parent = 0);
    void addmodel(osg::ref_ptr<osg::Node> model);
	//void viewmotion();
	void drawCircle();
    osg::ref_ptr<osg::Vec3Array> getarray(osg::ref_ptr<osg::Node> node,osg::Matrix m);
    ~osgqt();

public slots:   //申明信号与槽,当树形控件的子选项被改变时执行
    void treeItemChanged(QTreeWidgetItem* item , int column);

private slots:
    void timerUpdate();
    void on_open_clicked();
    void on_directorycomboBox_currentIndexChanged(int index);
	
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
	//void camera_height_ok();
	//void shooting_times_ok();
	//void shooting_radius_ok();
	void camera_play_clicked();
	void camera_delete_clicked();
private:
    Ui::osgqt *ui;
    osg::ref_ptr<osg::Node>  EditModel;
    osg::ref_ptr<osg::Node>  loadedModel;
    osg::ref_ptr<osg::Node>  loadedModel2;
    QtOsgView * ViewerWindow;
	/*typedef struct motion
	{
		QString motion_part;
		int motion_amp;
		int motion_fre;
	}motiondef;*/
	void removenode(std::string name);
	bool motionfunc();
	//QMap<QString, QVector2D> motion;//保存运动信息
	QString motionname;
	float motion[2];
	float camera_height;
	float shooting_radius;
	int shooting_times;
	QString savepicpath;
	QString savepclpath;
	QString filepath;
	Photographer pg;
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
