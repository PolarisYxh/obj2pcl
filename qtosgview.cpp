
#include "qtosgview.h"

AdapterWidget::AdapterWidget(QWidget *parent, const char* name, const QOpenGLWidget * shareWidget, Qt::WindowFlags f)
{
    mGw = new osgViewer::GraphicsWindowEmbedded(0, 0, width(), height());
    setFocusPolicy(Qt::ClickFocus);
}

void AdapterWidget::resizeGL(int width, int height)
{
    mGw->getEventQueue()->windowResize(0, 0, width, height);
    mGw->resized(0, 0, width, height);
}
void AdapterWidget::keyPressEvent(QKeyEvent* event)
{
    mGw->getEventQueue()->keyPress((osgGA::GUIEventAdapter::KeySymbol) *(event->text().toUtf8().data()));
}

void AdapterWidget::keyReleaseEvent(QKeyEvent* event)
{
    mGw->getEventQueue()->keyRelease((osgGA::GUIEventAdapter::KeySymbol)*(event->text().toUtf8().data()));
}


void AdapterWidget::mousePressEvent(QMouseEvent* event)
{
    int button = 0;
    switch (event->button())
    {
    case(Qt::LeftButton) :
        button = 1;
        break;
    case (Qt::MidButton) :
        button = 2;
        break;
    case (Qt::RightButton) :
        button = 3;
        break;
    case (Qt::NoButton) :
        button = 0;
        break;
    default:
        button = 0;
        break;
    }

    mGw->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
}

void AdapterWidget::mouseReleaseEvent(QMouseEvent* event)
{
    int button = 0;
    switch (event->button())
    {
    case(Qt::LeftButton) :
        button = 1;
        break;
    case(Qt::MidButton) :
        button = 2;
        break;
    case(Qt::RightButton) :
        button = 3;
        break;
    case(Qt::NoButton) :
        button = 0;
        break;
    default:
        button = 0;
        break;
    }
    mGw->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
}


void  AdapterWidget::mouseMoveEvent(QMouseEvent* event)
{
    mGw->getEventQueue()->mouseMotion(event->x(), event->y());
}

void AdapterWidget::wheelEvent(QWheelEvent* event)
{
    mGw->getEventQueue()->mouseScroll(event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN);
}


QtOsgView::QtOsgView(QWidget * parent, const char * name, const QOpenGLWidget * shareWidget, Qt::WindowFlags f) :AdapterWidget(parent, name, shareWidget, f)
{
    getCamera()->setViewport(new osg::Viewport(0, 0, width(), height()));
    getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width()) / static_cast<double>(height()), 1.0f, 10000.0f);
    getCamera()->setGraphicsContext(getGraphicsWindow());
    setThreadingModel(osgViewer::Viewer::SingleThreaded);
    //this->grabKeyboard();
    mStrTime = 0.0;
    mEndTime = 0.0;
    mSleepTime = 0.0;

    num = 0;
}

void QtOsgView::paintGL()
{
    mStrTime = mTimer.tick();
    frame();
    num++;
    mEndTime = mTimer.tick();

    //计算需要睡眠的时间
    mSleepTime = 1.0 / 60.0 - mTimer.delta_s(mStrTime, mEndTime);

    if (mSleepTime < 0)
    {
        mSleepTime = 0.0;
    }

    //睡眠
    OpenThreads::Thread::microSleep(mSleepTime * 1000000);		//微秒
    //Sleep(mSleepTime * 1000);										//毫秒

    double mTime = mTimer.tick();
    //std::cout << "帧数：" << mTimer.delta_s(mStrTime, mTime) << std::endl;

    //递归调用
    update();
}


void QtOsgView::keyPressEvent(QKeyEvent *event)
{
    if ((event->modifiers() == Qt::ControlModifier))
    {
        ctrl_is_picked = true;
        ctrl_is_picked2 = true;
    }
}
void QtOsgView::keyReleaseEvent(QKeyEvent *event)
{
    if ((event->modifiers() == Qt::NoModifier))
    {
        ctrl_is_release = true;
        ctrl_is_picked2 = false;
    }
}
