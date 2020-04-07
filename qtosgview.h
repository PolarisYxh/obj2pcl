#ifndef QTOSGVIEW_H
#define QTOSGVIEW_H

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <QString>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLWidget>
#include <QMainWindow>
#include <iostream>

class AdapterWidget :public QOpenGLWidget{
public:
    AdapterWidget(QWidget *parent = 0, const char* name = 0, const QOpenGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);

    virtual ~AdapterWidget(){}

    osgViewer::GraphicsWindow* getGraphicsWindow()    {
        return mGw.get();
    }

    const osgViewer::GraphicsWindow* getGraphicsWidow()const{
        return mGw.get();
    }
protected:
    virtual void resizeGL(int width, int height);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> mGw;
};

class QtOsgView : public osgViewer::Viewer, public AdapterWidget{
public:
    QtOsgView(QWidget * parent = 0, const char * name = 0, const QOpenGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);

    ~QtOsgView(){}
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *event);
    virtual void paintGL();
    bool ctrl_is_picked = false;
    bool ctrl_is_release = false;
    bool ctrl_is_picked2 = false;
protected:
    osg::Timer mTimer;			//定时器，控制帧速
    double mStrTime;			//开始tick时间
    double mEndTime;			//结束tick时间

    double mSleepTime;			//需要睡眠的时间
    int num;
};



#endif // QTOSGWIDGET_H
