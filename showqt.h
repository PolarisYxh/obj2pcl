#ifndef SHOWQT_H
#define SHOWQT_H

#include <QWidget>
#include <osg/Node>
#include <qtosgview.h>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTreeWidgetItem>
//#include "engine.h"
namespace Ui {
class showqt;
}

class showqt : public QWidget
{
    Q_OBJECT

public:
    explicit showqt(QWidget *parent = 0);
    void addmodel(osg::ref_ptr<osg::Node> model);
    void addmodel(osg::ref_ptr<osg::Group> model);
    osg::ref_ptr<osg::Vec3Array> getarray(osg::ref_ptr<osg::Node> node,osg::Matrix m);
    std::string getcurrent_box_name();
    void control();
    ~showqt();
    void StartMatlab();
//    Engine *ep;
    void initTree();
    void updateParentItem(QTreeWidgetItem* item);

public slots:   //申明信号与槽,当树形控件的子选项被改变时执行
    void treeItemChanged(QTreeWidgetItem* item , int column);

private slots:

    void timerUpdate();

    void on_open_clicked();

    void on_directorycomboBox_currentIndexChanged(int index);


private:
    Ui::showqt *ui;
    osg::ref_ptr<osg::Node>  loadedModel;
    osg::ref_ptr<osg::Group>  loadedModelGroup;
    QtOsgView * ViewerWindow;
};



#endif // SHOWQT_H
