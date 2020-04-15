#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QFileInfoList>
#include "qtosgview.h"
#include <osg/Point>
#include "qabout.h"
#include "qimageviewer.h"
#include <QHBoxLayout>
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
	explicit MainWindow(QWidget* parent = 0,QString dir="./",int flag=0);//0为展示图片，1为展示点云
private:
    QMenuBar *menuBar;
    QToolBar *toolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;
    QLabel *imageLabel;
    QAbout aboutWidget;
	QString dir;
    /* new class */
    QImageViewer *imageViewer;
	QImageViewer* PCLViewer;
	QHBoxLayout* totalLayout;
	QtOsgView* ViewerWindow = new QtOsgView();;
	osg::ref_ptr<osgGA::TrackballManipulator> manipulator;
	QString savepclpath;
	osg::ref_ptr<osg::Node> model=new osg::Node;

	int flag;

	void showpointcloud();
    void initMainWindow(void);
    void initUiComponent(void);
    void setQImageViewerWidget(void);
    void setWindowComponet(void);
    void initImageResource(void);
    void loadImageResource(void);
	void loadPCLResource(void);
	void setPCLViewerWidget(void);

    QAction *openAction;
    QAction *closeAction;
    QAction *lastAction;
    QAction *nextAction;
    QAction *toLeftAction;
    QAction *toRightAction;
    QAction *toEnlargeAction;
    QAction *toLessenAction;
    QAction *deleteAction;

public slots:
    void openActionTriggered(void);
    void closeActionTriggered(void);
    void lastActionTriggered(void);
    void nextActionTriggered(void);
    void toLeftActionTriggered(void);
    void toRightActionTriggered(void);
    void toEnlargeActionTriggered(void);
    void toLessenActionTriggered(void);
    void deleteActionTriggered(void);
    void aboutTriggered(void);
    void aboutQtTriggered(void);

	void lastActionTriggered1(void);
	void nextActionTriggered1(void);
	void openActionTriggered1(void);
};

#endif // MAINWINDOW_H
