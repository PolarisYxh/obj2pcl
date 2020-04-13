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
#include "Tool_test_2.h"
//#include "engine.h"
using namespace  std;
namespace Ui {
class osgqt;
}

class osgqt : public QWidget
{
    Q_OBJECT

public:
    explicit osgqt(QWidget *parent = 0);
    void addmodel(osg::ref_ptr<osg::Node> model);
	void viewmotion();
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
	void showButton_1_pressed();
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
	osg::ref_ptr<osg::Group> root= new osg::Group;
	//表盘的几何节点
	osg::ref_ptr<osg::Geode> clockGeode = new osg::Geode;
	//圆圈
	osg::ref_ptr<osg::Geometry> clockGeometry = new osg::Geometry;
	osg::ref_ptr<osg::Node> kinect;
	
};

#endif // OSG QT_H
