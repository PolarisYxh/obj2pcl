#ifndef MYTREEWIDGET_H
#define MYTREEWIDGET_H

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

class mytreewidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit mytreewidget(QWidget *parent = nullptr);
    std::vector<string> model_components;
    bool error_flag = false;
    std::string error_message;
    std::vector<string> all_node_name;  //注释：父节点信息
    std::map<string,int> node_to_int; //注释：用于确认父节点是第几号结点
	std::vector<std::vector<osg::ref_ptr<osg::Node>>> model_structure_tree;
    std::map<string,bool> child_map; //注释： 用于在attach时防止加入到子节点上
    std::vector<string> final_attach_node_name;
    std::string father_name;
    bool drop = false;
    std::map<osg::ref_ptr<osg::Node>,bool> is_model_componets;
	QMap<QTreeWidgetItem*, osg::ref_ptr<osg::Node> > item_node;
signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
private:
    void performDrag();
    void dfs(string father_name);
    void dfs2(string root_name);
    //QString selectionText() const;

    //QString toHtml(const QString &plainText) const;
    //QString toCsv(const QString &plainText) const;
    //void fromCsv(const QString &csvText);
    QPoint startPos;
};

#endif // MYTREEWIDGET_H
