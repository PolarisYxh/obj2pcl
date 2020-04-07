#include "mytreewidget2.h"

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


mytreewidget2::mytreewidget2(QWidget *parent) : QTreeWidget(parent){
    //tree = new QTreeWidget(this);
    this->setHeaderLabel("3D Model Components List");
    this->resize(211,631);
    this->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->header()->setStretchLastSection(false);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setItemsExpandable(true);
    this->setStyle(QStyleFactory::create("windowsxp"));
    //tree->setDragEnabled(true);
    //this->setAcceptDrops(true);
    //this->setDropIndicatorShown(true);
    //tree->setDragDropMode(QAbstractItemView::InternalMove);
}
