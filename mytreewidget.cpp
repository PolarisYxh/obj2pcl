#include "mytreewidget.h"

#include <QMainWindow>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osg/PrimitiveSet>
#include <osg/PrimitiveRestartIndex>
#include <qtosgview.h>
#include <osgGA/TrackballManipulator>
#include <osgFX/Scribe>
#include <osgUtil/LineSegmentIntersector>
#include <osg/MatrixTransform>
#include <structures.h>
#include <vector>
#include <osgManipulator/TabBoxDragger>
#include <osgManipulator/CommandManager>
#include <osgManipulator/Selection>
#include <osg/Matrix>
#include <osg/Geode>
#include <osg/Matrixd>
#include <osg/PositionAttitudeTransform>
#include <QTimer>
#include <QDir>
#include <QFileDialog>
#include <string>
#include <osg/CopyOp>
#include <fstream>
#include <osg/ShapeDrawable>
#include <osg/LineWidth>
#include <osg/LightModel>
#include <osgUtil/Optimizer>
#include <osgUtil/Simplifier>
#include <osg/PolygonMode>
#include <osg/Shape>
#include <osg/Geometry>
#include <osg/Node>
#include <iostream>
#include <QString>
#include <osg/ComputeBoundsVisitor>
#include <QMouseEvent>
#include <QPainter>
#include <QMimeData>
#include <QDrag>
#include <QDebug>
#include <QHeaderView>
#include <QPainter>
#include <QApplication>
#include <QStyleFactory>


mytreewidget::mytreewidget(QWidget *parent) : QTreeWidget(parent){
    //tree = new QTreeWidget(this);
    this->setHeaderLabel("3D Model Components List");
    //this->resize(211,831);
    this->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->header()->setStretchLastSection(false);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setItemsExpandable(true);
    this->setStyle(QStyleFactory::create("windowsxp"));
    //tree->setDragEnabled(true);
    this->setAcceptDrops(true);
    //this->setDropIndicatorShown(true);
    //tree->setDragDropMode(QAbstractItemView::InternalMove);
}


void mytreewidget::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton)
        startPos = event->pos();
    QTreeWidget::mousePressEvent(event);
}

void mytreewidget::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton) {
        int distance = (event->pos() - startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance()) {
            performDrag();
        }
    }
}

void mytreewidget::performDrag(){
    QList<QTreeWidgetItem*> selectedItemList = this->selectedItems();
    if(selectedItemList.size()==0)
        return;
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("tree/item","move");
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->exec(Qt::MoveAction);
}

void mytreewidget::dragEnterEvent(QDragEnterEvent *event)
{
    mytreewidget *source =
            qobject_cast<mytreewidget *>(event->source());
    if (source && source == this) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void mytreewidget::dragMoveEvent(QDragMoveEvent *event)
{
    mytreewidget *source =
            qobject_cast<mytreewidget *>(event->source());
    if (source && source == this) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}


void mytreewidget::dfs(string father_name){
    //注释： 用于判断是否向子节点attach
    int father_idx = this->node_to_int[father_name];
    this->child_map[father_name] = true;
    for(int i = 0;i<this->model_structure_tree[father_idx].size();i++){
        dfs(this->model_structure_tree[father_idx].at(i));
    }
}

void mytreewidget::dfs2(string root_name){
    //注释： 用于添加attach的结点
    if(this->child_map[root_name]){
        this->final_attach_node_name.push_back(root_name);
        return;
    }
    int father_idx = this->node_to_int[root_name];
    for(int i = 0;i<this->model_structure_tree[father_idx].size();i++){
        dfs2(this->model_structure_tree[father_idx].at(i));
    }
}

void mytreewidget::dropEvent(QDropEvent *event)
{
    QTreeWidgetItem* itemOver = itemAt(event->pos());
    if(itemOver == NULL){
        return;
    }
    if (event->mimeData()->hasFormat("tree/item")) {


        QString Dropname = itemOver->text(0);
        if(QString::compare(Dropname,"3D Object")==0){
            this->error_flag = true;
            this->error_message = "This is invalid chooose!";
            return;
        }

        bool flag = this->is_model_componets[Dropname.toStdString()];
        if(flag){
            this->error_flag = true;
            this->error_message = "This is not a group,it's a componet!";
            return;
        }


        QList<QTreeWidgetItem*> selectedItemList = this->selectedItems();

        //attach 操作


        std::vector<string> attach_node_name;
        attach_node_name.clear();

        this->child_map.clear();
        this->node_to_int.clear();
        for(int i = 0;i<this->all_node_name.size();i++){
            this->node_to_int[this->all_node_name.at(i)] = i;
        }


        bool is_3dobject = false;
        for ( int i=0; i!=selectedItemList.size(); ++i ){
            attach_node_name.push_back(selectedItemList.at(i)->text(0).toStdString());
            if(QString::compare(selectedItemList.at(i)->text(0),"3D Object")==0||QString::compare(selectedItemList.at(i)->text(0),"db")==0){
                is_3dobject = true;
                break;
            }
        }
        if(is_3dobject){
            this->error_flag = true;
            this->error_message = "You can not move \"3D Object\" or \"db\"!";
            return;
        }


        for(int i = 0;i<attach_node_name.size();i++){
            dfs(attach_node_name.at(i));
        }

        bool is_choose = false;

        map<string,bool>::iterator it;
        it = this->child_map.begin();
        while(it != this->child_map.end())
        {
            qDebug()<<QString::fromStdString(it->first)<<" "<<it->second;
            if((it->first==Dropname.toStdString())&&it->second){
                is_choose = true;
                break;
            }
            it++;
        }
        if(is_choose){
            this->error_flag = true;
            this->error_message = "You can not move to the group you has been chosen or its child";
            return;
        }

        this->final_attach_node_name.clear();
        this->father_name = Dropname.toStdString();
        this->drop = true;
        dfs2("db");

        QByteArray csvData = event->mimeData()->data("tree/item");
        QString csvText = QString::fromUtf8(csvData);
        event->acceptProposedAction();
    }
}
