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


//mytreewidget::mytreewidget(QMap<QTreeWidgetItem*, osg::ref_ptr<osg::Node> >& item_n, QWidget *parent) : 
//	QTreeWidget(parent),item_node(item_n)
//{
//    //tree = new QTreeWidget(this);
//    this->setHeaderLabel("3D Model Components List");
//    //this->resize(211,831);
//    this->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    this->header()->setStretchLastSection(false);
//    this->setSelectionMode(QAbstractItemView::ExtendedSelection);//可以多项选择
//    this->setItemsExpandable(true);
//    this->setStyle(QStyleFactory::create("windowsxp"));
//    this->setDragEnabled(true);
//    this->setAcceptDrops(true);
//	//设置显示将要被放置的位置
//	this->setDropIndicatorShown(true);
//	//    //设置拖放模式为移动项目，否则为复制项目
//	this->setDragDropMode(QAbstractItemView::InternalMove);
//	//this->setAttribute(Qt::Paint);
//	//this->setDefaultDropAction(Qt::MoveAction);
//	//this->setDragDropMode(QAbstractItemView::DragDrop);
//    //this->setDropIndicatorShown(true);
//    //tree->setDragDropMode(QAbstractItemView::InternalMove);
//}
//
//
//void mytreewidget::mousePressEvent(QMouseEvent *event){
//    if (event->button() == Qt::LeftButton)
//        startPos = event->pos();
//    QTreeWidget::mousePressEvent(event);
//}
//
//void mytreewidget::mouseMoveEvent(QMouseEvent *event){
//    if (event->buttons() & Qt::LeftButton) {
//        int distance = (event->pos() - startPos).manhattanLength();
//        if (distance >= QApplication::startDragDistance()) {
//            performDrag();
//        }
//    }
//}
//
//void mytreewidget::performDrag(){
//    QList<QTreeWidgetItem*> selectedItemList = this->selectedItems();
//    if(selectedItemList.size()==0)
//        return;
//    QMimeData *mimeData = new QMimeData;
//    mimeData->setData("tree/item","move");
//    QDrag *drag = new QDrag(this);
//    drag->setMimeData(mimeData);
//    drag->exec(Qt::MoveAction);
//}
//
//void mytreewidget::dragEnterEvent(QDragEnterEvent *event)
//{
//    mytreewidget *source =
//            qobject_cast<mytreewidget *>(event->source());
//    if (source && source == this) {
//        event->setDropAction(Qt::MoveAction);
//        event->accept();
//    }
//}
//
//void mytreewidget::dragMoveEvent(QDragMoveEvent *event)
//{
//	if (event->mimeData()->hasFormat("tree/item")) {
//		QTreeWidgetItem* hHitTest = itemAt(event->pos());
//		event->setDropAction(Qt::MoveAction);
//		if (hHitTest)
//		{
//			hHitTest->setSelected(true);
//		}
//		event->accept();
//	}
//	else
//		event->ignore();
//}
//
//
//void mytreewidget::dfs(string father_name){
//    //注释： 用于判断是否向子节点attach
//    int father_idx = this->node_to_int[father_name];
//    this->child_map[father_name] = true;
//    for(int i = 0;i<this->model_structure_tree[father_idx].size();i++){
//        dfs(this->model_structure_tree[father_idx].at(i)->getName());
//    }
//}
//
//void mytreewidget::dfs2(string root_name){
//    //注释： 用于添加attach的结点
//    if(this->child_map[root_name]){
//        this->final_attach_node_name.push_back(root_name);
//        return;
//    }
//    int father_idx = this->node_to_int[root_name];
//    for(int i = 0;i<this->model_structure_tree[father_idx].size();i++){
//        dfs2(this->model_structure_tree[father_idx].at(i)->getName());
//    }
//}
//
//void mytreewidget::dropEvent(QDropEvent *event)
//{
//    QTreeWidgetItem* itemOver = itemAt(event->pos());
//    if(itemOver == NULL){
//        return;
//    }
//    if (event->mimeData()->hasFormat("tree/item")) {
//
//        QString Dropname = itemOver->text(0);
//
//		osg::Group * g=dynamic_cast<osg::Group *>(item_node[itemOver].get());
//		if (!g)
//		{
//        //bool flag = this->is_model_componets[item_node[itemOver]];
//        //if(flag){
//            this->error_flag = true;
//            this->error_message = "This is not a group,it's a component!";
//            return;
//        }
//
//        QList<QTreeWidgetItem*> selectedItemList = this->selectedItems();
//		for (int i = 0; i < selectedItemList.size(); i++)
//		{
//			osg::Node* selectednode = item_node[selectedItemList.at(i)];
//			g->addChild(selectednode);
//			selectednode->getParent(0)->removeChild(selectednode);
//			QTreeWidgetItem* item = selectedItemList.at(i);
//			if (item->parent())
//			{
//				item->parent()->removeChild(item);
//			}
//			else
//			{
//				takeTopLevelItem(indexOfTopLevelItem(item));
//			}
//		}
//		//移除
//		
//		
//		//插入
//		if (pItem && m_child)
//		{
//			pItem->insertChild(0, item);
//		}
//		else if (pItem)
//		{
//			QTreeWidgetItem* parentItem = pItem->parent();
//			if (parentItem)
//				parentItem->insertChild(parentItem->indexOfChild(pItem) + 1, item);
//			else
//				insertTopLevelItem(indexOfTopLevelItem(pItem) + 1, item);
//		}
//		////////////////////////////////////////////
//
//		event->setDropAction(Qt::MoveAction);
//		event->accept();
//        //attach 操作
//
//
//        //std::vector<string> attach_node_name;
//        //attach_node_name.clear();
//
//        //this->child_map.clear();
//        //this->node_to_int.clear();
//        //for(int i = 0;i<this->all_node_name.size();i++){
//        //    this->node_to_int[this->all_node_name.at(i)] = i;
//        //}
//
//
//        //bool is_3dobject = false;
//        //for ( int i=0; i!=selectedItemList.size(); ++i ){
//        //    attach_node_name.push_back(selectedItemList.at(i)->text(0).toStdString());
//        //    if(QString::compare(selectedItemList.at(i)->text(0),"3D Object")==0||QString::compare(selectedItemList.at(i)->text(0),"db")==0){
//        //        is_3dobject = true;
//        //        break;
//        //    }
//        //}
//        //if(is_3dobject){
//        //    this->error_flag = true;
//        //    this->error_message = "You can not move \"3D Object\" or \"db\"!";
//        //    return;
//        //}
//
//
//        //for(int i = 0;i<attach_node_name.size();i++){
//        //    dfs(attach_node_name.at(i));
//        //}
//
//        //bool is_choose = false;
//
//        //map<string,bool>::iterator it;
//        //it = this->child_map.begin();
//        //while(it != this->child_map.end())
//        //{
//        //    qDebug()<<QString::fromStdString(it->first)<<" "<<it->second;
//        //    if((it->first==Dropname.toStdString())&&it->second){
//        //        is_choose = true;
//        //        break;
//        //    }
//        //    it++;
//        //}
//        //if(is_choose){
//        //    this->error_flag = true;
//        //    this->error_message = "You can not move to the group you has been chosen or its child";
//        //    return;
//        //}
//
//        //this->final_attach_node_name.clear();
//        //this->father_name = Dropname.toStdString();
//        //this->drop = true;
//        //dfs2("db");
//
//        QByteArray csvData = event->mimeData()->data("tree/item");
//        QString csvText = QString::fromUtf8(csvData);
//        event->acceptProposedAction();
//    }
//}
#define BM_ICON_WIDTH 22
mytreewidget::mytreewidget(QMap<QTreeWidgetItem*, osg::ref_ptr<osg::Node> >& item_n, QWidget* parent)
	:item_node(item_n), QTreeWidget(parent)
{
	this->setHeaderLabel("3D Model Components List");
	this->setSelectionMode(QAbstractItemView::SingleSelection);
	//启用拖放
	this->setDragEnabled(true);
	//设置拖放
	setAcceptDrops(true);
	//    this->viewport()->setAcceptDrops(true);
		//设置显示将要被放置的位置
	this->setDropIndicatorShown(true);
	//    //设置拖放模式为移动项目，否则为复制项目
	this->setDragDropMode(QAbstractItemView::InternalMove);
	//this->setAttribute(Qt::WA_PaintOutsidePaintEvent);
	
	//    //this->resize(211,831);
	    this->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	    this->header()->setStretchLastSection(false);
	//    this->setSelectionMode(QAbstractItemView::ExtendedSelection);//可以多项选择
	//    this->setItemsExpandable(true);
	    this->setStyle(QStyleFactory::create("windowsxp"));
		update();
		paint = false;
		painter = new QPainter;
		m_newLine.setPoints(QPoint(0, 0), QPoint(0, 0));
	//this->setAttribute(Qt::WA_PaintOnScreen);
}

mytreewidget::~mytreewidget()
{
}
void mytreewidget::mousePressEvent(QMouseEvent* event) {
	    if (event->button() == Qt::LeftButton)
	        startPos = event->pos();
	    QTreeWidget::mousePressEvent(event);
	}
	
void mytreewidget::mouseMoveEvent(QMouseEvent *event){
	if (event->buttons() & Qt::LeftButton) {
	    int distance = (event->pos() - startPos).manhattanLength();
	    if (distance >= QApplication::startDragDistance()) {
	        //performDrag();
			startDrag();
	    }
	}
}
void mytreewidget::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasFormat("tree/item"))
		event->accept();
	else
		event->ignore();
}

void mytreewidget::dragMoveEvent(QDragMoveEvent* event)
{
	if (event->mimeData()->hasFormat("tree/item")) {
		QTreeWidgetItem* hHitTest = itemAt(event->pos());
		event->setDropAction(Qt::MoveAction);
		if (hHitTest)
		{
			QPoint pos = event->pos();
			QRect rect = visualItemRect(hHitTest);
			m_oldLine = m_newLine;
			m_newLine = QLine(rect.bottomLeft(), rect.bottomRight());
			int left = rect.left() + BM_ICON_WIDTH + 3;
			if (pos.x() > left)
			{
				m_newLine.setP1(QPoint(left, m_newLine.y1()));
				m_child = true;
			}
			else
				m_child = false;

			//QByteArray itemData = event->mimeData()->data("tree/item");
			//QTreeWidgetItem* dragItem = (QTreeWidgetItem*)((void*)QVariant(itemData));
			//QTreeWidgetItem* dragItem = (QTreeWidgetItem*)QVariant(itemData).value<void*>();
			QTreeWidgetItem* dragItem = currentItem();
			//绘线
			qDebug() << "drag move";
			//paintInsertPos(dragItem, hHitTest);
			paint = true;
			//update();//qt下次处理时调用
			this->repaint();//即时调用
			qApp->processEvents();
			/*QPaintEvent* a;
			paintEvent(a);*/
		}
		event->accept();
	}
	else
		event->ignore();
}

void mytreewidget::dropEvent(QDropEvent* event)
{
	if (event->mimeData()->hasFormat("tree/item"))
	{
		QByteArray itemData = event->mimeData()->data("tree/item");
		//QTreeWidgetItem* item=(QTreeWidgetItem*)QVariant(itemData).value<void*>();
		//QTreeWidgetItem* item = (QTreeWidgetItem*)QVariant(itemData).toInt();
		QTreeWidgetItem* item = currentItem();
		QTreeWidgetItem* pItem = itemAt(event->pos());
		if (pItem == item || pItem == NULL||item==NULL)
		{
			event->ignore();
			return;
		}
		osg::Group* g = dynamic_cast<osg::Group*>(item_node[pItem].get());
		if (g)//判断移动到达的是group节点
		{
			osg::Node* selectednode = item_node[item]->getParent(0);//getparent之后去掉
			qDebug() << QString::fromStdString(g->getName())<<" "<<QString::fromStdString(selectednode->getName());
			g->addChild(selectednode);
			selectednode->getParent(0)->removeChild(selectednode);
			//移除
			if (item->parent())
			{
				item->parent()->removeChild(item);
			}
			else
			{
				takeTopLevelItem(indexOfTopLevelItem(item));
			}
			//插入
			if (pItem && m_child)
			{
				pItem->insertChild(0, item);
			}
			else if (pItem)
			{
				QTreeWidgetItem* parentItem = pItem->parent();
				if (parentItem)
					parentItem->insertChild(parentItem->indexOfChild(pItem) + 1, item);
				else
					insertTopLevelItem(indexOfTopLevelItem(pItem) + 1, item);
			}
		}
		////////////////////////////////////////////

		event->setDropAction(Qt::MoveAction);
		event->accept();
		qDebug() << "2";
	}
	else
		event->ignore();
	QTreeWidget::dropEvent(event);
	paint = false;
}

void mytreewidget::startDrag()
{
	//Q_UNUSED(supportedActions);//消除warning
	QTreeWidgetItem* item = currentItem();
	if (item == NULL)
		return;

	//QString pValue = QString::number(int((void*)item));
	QByteArray itemData;
	itemData = QVariant::fromValue((void*)item).toByteArray();
	
	QMimeData* mimeData = new QMimeData;
	mimeData->setData("tree/item", itemData);//把拖动数据保存到mimeData中

	QDrag* drag = new QDrag(this);
	drag->setMimeData(mimeData);
	//设置鼠标移动时，拖动的图片数据
	QPixmap pPixmap("./Resourcesicon/Drag.png");
	pPixmap = pPixmap.scaledToWidth(2);
	drag->setPixmap(pPixmap);
	drag->setHotSpot(QPoint(drag->pixmap().width() / 2, drag->pixmap().height() / 2));

	if (drag->exec(Qt::MoveAction) == Qt::MoveAction)//启动拖放，在mousePressEvent()或mouseMoveEvent()函数中调用
	{
	}
}

void mytreewidget::paintInsertPos(QTreeWidgetItem* dragItem, QTreeWidgetItem* hitItem)
{
	if (dragItem == hitItem)
	{
		m_newLine = QLine();
		return;
	}

	//painter->end();
}
void mytreewidget::paintEvent(QPaintEvent* p)//painter必须在虚函数event下才能运行
{
	QTreeView::paintEvent(p);//加上这个才能绘制item
	if (!paint)return;
	painter->begin(viewport());//加上这个painter才能用
	painter->save();
	/***********擦除旧线***********/
	//直线
	//qDebug() << "old" << m_oldLine.center();
	QPen pen(palette().base().color());
	//painter->setPen(pen);
	//painter->drawLine(m_oldLine);
	////三角
	//paintTriangle(m_oldLine.p1(), painter);


	/***********绘制新线***********/
	//直线
	qDebug() << "new" << m_newLine.center();
	pen.setColor(Qt::black);
	pen.setWidth(1);
	painter->setPen(pen);
	painter->drawLine(m_newLine);
	painter->restore();
	//三角
	paintTriangle(m_newLine.p1(), painter);
	painter->end();
}
void mytreewidget::paintTriangle(const QPoint& right, QPainter* painter)
{
	QPoint top = QPoint(right.x() - 4, right.y() - 3);
	QPoint bottom = QPoint(right.x() - 4, right.y() + 3);
	painter->drawLine(right, top);
	painter->drawLine(top, bottom);
	painter->drawLine(bottom, right);
}