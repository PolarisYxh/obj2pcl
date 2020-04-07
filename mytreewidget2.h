#ifndef MYTREEWIDGET2_H
#define MYTREEWIDGET2_H
#include <QWidget>
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
using namespace std;

class mytreewidget2 : public QTreeWidget
{
public:
    explicit mytreewidget2(QWidget *parent = nullptr);
};

#endif // MYTREEWIDGET2_H
