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
#include <sstream>
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
    void addmodel2(osg::ref_ptr<osg::Node> model);
    void ShowEditModel(osg::ref_ptr<osg::Node> model, int model_idx);
    osg::ref_ptr<osg::Vec3Array> getarray(osg::ref_ptr<osg::Node> node,osg::Matrix m);
    std::string getcurrent_box_name();
    ~osgqt();
    void initTree();
    osg::ref_ptr<osg::Node> PaintEditBoxes(int model_idx);

public slots:   //申明信号与槽,当树形控件的子选项被改变时执行
    void treeItemChanged(QTreeWidgetItem* item , int column);

private slots:
    void timerUpdate();
    void on_open_clicked();
    void on_directorycomboBox_currentIndexChanged(int index);
    void on_toolButton_pressed();
    void on_tabWidget_currentChanged(int index);
    void on_group_button_clicked();
    void on_treeWidget_itemChanged_child(QTreeWidgetItem *item, int column);
    void SetParentPartiallyChecked(QTreeWidgetItem *itm,int column);
    void on_treeWidget_itemChanged_parent(QTreeWidgetItem *item, int column);
    void update_treewidget();
    void on_rename_clicked();
    void on_attach_clicked();
    void on_select_clicked();
    void on_unselect_clicked();
    void on_Delete_node_clicked();
    void on_Ungroup_clicked();
    void on_treeWidget_itemSelectionChanged();
    void on_Reset_clicked();
    // dof adder 函数
    void on_open2_clicked();
    void update_treewidget_2();
    void on_treeWidget_itemSelectionChanged2();
    void on_toolButton_2_clicked();

    void treeItemChanged2(QTreeWidgetItem* item , int column);
    void on_comboBox_2_currentIndexChanged(int index);

    void on_Animation_test_clicked();

    void on_translation_clicked();

    void on_rotation_clicked();

    void on_spiral_clicked();


    void on_tp_clicked();

    void on_opof_clicked();

    void on_ok_clicked();

    void on_cancel_clicked();

    void on_tp_1_clicked();

    void on_tp_2_clicked();

    void on_opof_1_clicked();

    void on_opof_2_clicked();


    void on_reset2_clicked();



    void on_resetanimation_clicked();


private:
    Ui::osgqt *ui;
    osg::ref_ptr<osg::Node>  EditModel;
    osg::ref_ptr<osg::Node>  loadedModel;
    osg::ref_ptr<osg::Node>  loadedModel2;
    QtOsgView * ViewerWindow;
    QtOsgView * ViewerWindow2;
    void dfs(string father_name);
    void dfs2(string root_name);

};

#endif // OSGQT_H
