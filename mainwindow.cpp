#include "mainwindow.h"

#include <QWidget>
#include <QImage>
#include <QFileDialog>
#include <QPixmap>
#include <QAction>
#include <QMessageBox>
#include <QDebug>
#include <QScrollArea>
#include <QGridLayout>
#include <QErrorMessage>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    /* init main window */
    initMainWindow();

    /* init ui */
    initUiComponent();

    /* init resource */
    initImageResource();

    /* create imageViewer */
    imageViewer = new QImageViewer();
	PCLViewer = new QImageViewer();
}
MainWindow::MainWindow(QWidget* parent,QString x,int y) : QMainWindow(parent),
	dir(x),flag(y)
{		
	/* init main window */
	initMainWindow();

	
	if (flag == 0)
	{
		/* init ui */
		initUiComponent();
		/* init resource */
		initImageResource();

		/* create imageViewer */
		imageViewer = new QImageViewer(this);
		int ret = imageViewer->openImageFile(tr("Select image:"),
			dir,
			tr("Images (*.jpg *.jpeg *.png *.bmp *.gif)"));
		if (ret) {
			QMessageBox::information(this, tr("Error"), tr("Open a file failed!"));
			return;
		}
		loadImageResource();
	}
	else if (flag == 1)//进入pcl处理函数
	{
		setWindowComponet();
		setPCLViewerWidget();
		/*QString filename=QFileDialog::getOpenFileName(this, tr("Select point cloud file:"), dir, "*.txt");
		if (filename.isEmpty()) {
			return;
		}
		QDir dir1 = QDir(dir);
		QFileInfoList infoList = dir1.entryInfoList(QDir::Files);
		QFileInfo info;
		for (int i = 0; i < infoList.count(); i++) {
			info = infoList.at(i);
			QString suffix = info.suffix();

			if (suffix == "txt") {
				fileInfoList.append(info);
			}
		}
		if (ret) {
			QMessageBox::information(this, tr("Error"), tr("Open a file failed!"));
			return;
		}*/
		PCLViewer = new QImageViewer(this);
		int ret = PCLViewer->openPCLFile(tr("Select PCL:"),
			dir,
			tr("Point Cloud (*.txt)"));
		if (ret) {
			QMessageBox::information(this, tr("Error"), tr("Open a file failed!"));
			return;
		}
		loadPCLResource();
	}
}

void MainWindow::initImageResource(void)
{
    imageLabel->clear();
    imageLabel->resize(QSize(200, 100));
    setWindowTitle(tr("QImageViewer"));
}

void MainWindow::loadImageResource(void)
{
    imageLabel->setPixmap(imageViewer->pixmap);
    imageLabel->resize(imageViewer->size);
    setWindowTitle(QFileInfo(imageViewer->filename).fileName() + tr(" - QImageViewer"));
}
void MainWindow::loadPCLResource(void)
{
	manipulator = new osgGA::TrackballManipulator;
	ViewerWindow->setCameraManipulator(manipulator);
	ViewerWindow->addEventHandler(new osgViewer::WindowSizeHandler);

	ViewerWindow->getCamera()->setClearColor(osg::Vec4f(255.0f, 255.0f, 255.0f, 1.0f));

	// 环境光
	ViewerWindow->getLight()->setAmbient(osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
	// 漫反射光
	ViewerWindow->getLight()->setDiffuse(osg::Vec4(0.1f, 0.1f, 0.1f, 1.0f));
	
	setWindowTitle(QFileInfo(PCLViewer->filename).fileName());
	showpointcloud();
}
void MainWindow::showpointcloud()
{
	osg::ref_ptr<osg::Group> root = new osg::Group;
	std::fstream fs;
	std::string file=PCLViewer->filename.toStdString();
	fs.open(file);
	//新建一个用于存放几何图元的Geode
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	//创建保存几何信息的对象
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
	osg::ref_ptr<osg::Vec3dArray> vertices = new osg::Vec3dArray();
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
	QMessageBox msgBox;
	msgBox.setText("wait....");
	msgBox.show();
	qApp->processEvents();
	while (!fs.eof())
	{
		//从txt读取一行中的坐标和proposal类型
		double x, y, z, r,g,b;
		int proposal_type;
		fs >> x >> y >> z >> r>>g>>b;
		//fs>>z>>x>>y;
		vertices->push_back(osg::Vec3d(x, y, z));
		osg::Vec4 color;
		color = osg::Vec4(r/255.0,g/255.0,b/255.0, 1.0);
		//color=osg::Vec4(0.7,0.7,0.7,1.0);
		colors->push_back(color);
	}
	geom->setVertexArray(vertices.get());
	geom->setColorArray(colors.get());
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, vertices->size()));
	geode->addDrawable(geom);

	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	osg::StateSet* stateSet = geode->getOrCreateStateSet();
	osg::Point* pointSize = new osg::Point;
	//pointSize->setSize(14.0);
	pointSize->setSize(7.0);
	stateSet->setAttribute(pointSize);
	//挂在root下
	root->addChild(geode);
	ViewerWindow->setSceneData(root.get());
}
void MainWindow::openActionTriggered(void)
{
    int ret = imageViewer->openImageFile(tr("Select image:"),
                                         ".\\",
                                         tr("Images (*.jpg *.jpeg *.png *.bmp *.gif)"));
    if (ret) {
        QMessageBox::information(this, tr("Error"), tr("Open a file failed!"));
        return ;
    }

    loadImageResource();
}
void MainWindow::openActionTriggered1(void)
{
	int ret = PCLViewer->openPCLFile(tr("Select PCL:"),
		".\\",
		tr("PCL (*.txt)"));
	if (ret) {
		QMessageBox::information(this, tr("Error"), tr("Open a file failed!"));
		return;
	}

	loadPCLResource();
}
void MainWindow::closeActionTriggered(void)
{
    initImageResource();
    imageViewer->closeImageFile();
}
void MainWindow::lastActionTriggered(void)
{
    int ret = imageViewer->last();
    if (ret) {
        QMessageBox::information(this,
                                 tr("Error"),
                                 tr("Open a image, please!"));
        return ;
    }

    loadImageResource();
}
void MainWindow::lastActionTriggered1(void)
{
	int ret = PCLViewer->last(1);
	if (ret) {
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a PCL, please!"));
		return;
	}

	loadPCLResource();
}
void MainWindow::nextActionTriggered(void)
{
    int ret = imageViewer->next();
    if (ret) {
        QMessageBox::information(this,
                                 tr("Error"),
                                 tr("Open a image, please!"));
        return ;
    }

    loadImageResource();
}
void MainWindow::nextActionTriggered1(void)
{
	int ret = PCLViewer->next(1);
	if (ret) {
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a PCL, please!"));
		return;
	}

	loadPCLResource();
}
void MainWindow::toLeftActionTriggered(void)
{
    int ret = imageViewer->spinToLeft();
    if (ret) {
        QMessageBox::information(this,
                                 tr("Error"),
                                 tr("Open a image, please!"));
        return ;
    }

    loadImageResource();
}

void MainWindow::toRightActionTriggered(void)
{
    int ret = imageViewer->spinToRight();
    if (ret) {
        QMessageBox::information(this,
                                 tr("Error"),
                                 tr("Open a image, please!"));
        return ;
    }

    loadImageResource();
}

void MainWindow::toEnlargeActionTriggered(void)
{
    int ret = imageViewer->zoomIn();
    if (ret) {
        QMessageBox::information(this,
                                 tr("Error"),
                                 tr("Open a image, please!"));
        return ;
    }

    loadImageResource();
}

void MainWindow::toLessenActionTriggered(void)
{
    int ret = imageViewer->zoomOut();
    if (ret) {
        QMessageBox::information(this,
                                 tr("Error"),
                                 tr("Open a image, please!"));
        return ;
    }

    loadImageResource();
}

void MainWindow::deleteActionTriggered(void)
{
    if (!QFile(imageViewer->filename).exists()) {
        QMessageBox::information(this,
                                 tr("Error"),
                                 tr("Open a image, please!"));
        return ;
    }

    QMessageBox message(QMessageBox::Warning,
                        tr("Warning"),
                        tr("Do you want to delete this image?"),
                        QMessageBox::Yes|QMessageBox::No,
                        NULL);
    if (message.exec() == QMessageBox::No) {
        return ;
    }

    int ret = imageViewer->delImageFile();
    if (ret) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("Delete a image failed!"));
        return ;
    }

    initImageResource();
}

void MainWindow::setQImageViewerWidget(void)
{
    /* label show image */
    imageLabel = new QLabel();

    QScrollArea *imageScrollArea = new QScrollArea();
    imageScrollArea->setAlignment(Qt::AlignCenter);
    imageScrollArea->setFrameShape(QFrame::NoFrame);
    imageScrollArea->setWidget(imageLabel);

    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addWidget(imageScrollArea, 0, 0);
    centralWidget->setLayout(mainLayout);
}
void MainWindow::setPCLViewerWidget(void)
{
	//QGridLayout* mainLayout = new QGridLayout();
	//QWidget* tab = new QWidget();
	totalLayout=new QHBoxLayout();
	totalLayout->addWidget(ViewerWindow);
	
	centralWidget->setLayout(totalLayout);
}
void MainWindow::setWindowComponet(void)
{
    openAction = new QAction(tr("Open"), this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open a image."));
    openAction->setIcon(QIcon("./images/open.png"));

    closeAction = new QAction(tr("Close"), this);
    closeAction->setShortcut(QKeySequence::Close);
    closeAction->setStatusTip(tr("Close a image."));
    closeAction->setIcon(QIcon("./images/close.png"));

    lastAction = new QAction(tr("Last"), this);
    lastAction->setStatusTip(tr("Last image."));
    lastAction->setIcon(QIcon("./images/left.png"));

    nextAction = new QAction(tr("Next"), this);
    nextAction->setStatusTip(tr("Next image"));
    nextAction->setIcon(QIcon("./images/right.png"));

    toLeftAction = new QAction(tr("LeftSpin"), this);
    toLeftAction->setStatusTip(tr("To Left."));
    toLeftAction->setIcon(QIcon("./images/toLeft.png"));

    toRightAction = new QAction(tr("RightSpin"), this);
    toRightAction->setStatusTip(tr("To Right."));
    toRightAction->setIcon(QIcon("./images/toRight.png"));

    toEnlargeAction = new QAction(tr("Enlarge"), this);
    toEnlargeAction->setStatusTip(tr("To Enlarge."));
    toEnlargeAction->setIcon(QIcon("./images/large.png"));

    toLessenAction = new QAction(tr("Lessen"), this);
    toLessenAction->setStatusTip(tr("To Lessen."));
    toLessenAction->setIcon(QIcon("./images/small.png"));

    deleteAction = new QAction(tr("Delete"), this);
    deleteAction->setStatusTip(tr("Delete a image"));
    deleteAction->setIcon(QIcon("./images/clear.png"));
    deleteAction->setShortcut(QKeySequence::Delete);

    QAction *exitAction = new QAction(tr("Exit"), this);
    exitAction->setStatusTip(tr("Exit"));
    exitAction->setIcon(QIcon("./images/quit.png"));
    exitAction->setShortcut(QKeySequence::Quit);

    QAction *aboutQt = new QAction(tr("About Qt"), this);
    aboutQt->setStatusTip(tr("About Qt"));
    aboutQt->setIcon(QIcon("./images/Qt.png"));

    QAction *about = new QAction(tr("About QImageViewer"), this);
    about->setStatusTip(tr("About QImageViewer"));
    about->setIcon(QIcon("./images/help.png"));

    QMenu *fileMenu = menuBar->addMenu(tr("File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(closeAction);
    fileMenu->addSeparator();
    fileMenu->addAction(deleteAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    QMenu *operationMenu = menuBar->addMenu(tr("Operate"));
    operationMenu->addAction(lastAction);
    operationMenu->addAction(nextAction);
    operationMenu->addSeparator();
    operationMenu->addAction(toLeftAction);
    operationMenu->addAction(toRightAction);
    operationMenu->addSeparator();
    operationMenu->addAction(toEnlargeAction);
    operationMenu->addAction(toLessenAction);

    QMenu *helpMenu = menuBar->addMenu(tr("Help"));
    helpMenu->addAction(aboutQt);
    helpMenu->addAction(about);

    toolBar->addAction(openAction);
    toolBar->addAction(closeAction);
    toolBar->addAction(lastAction);
    toolBar->addAction(nextAction);
    toolBar->addAction(toLeftAction);
    toolBar->addAction(toRightAction);
    toolBar->addAction(toEnlargeAction);
    toolBar->addAction(toLessenAction);
    toolBar->addAction(deleteAction);
    toolBar->addAction(about);
	if (flag == 0)
	{
		connect(openAction, SIGNAL(triggered(bool)), this, SLOT(openActionTriggered()));
		connect(closeAction, SIGNAL(triggered(bool)), this, SLOT(closeActionTriggered()));
		connect(lastAction, SIGNAL(triggered(bool)), this, SLOT(lastActionTriggered()));
		connect(nextAction, SIGNAL(triggered(bool)), this, SLOT(nextActionTriggered()));
		connect(toLeftAction, SIGNAL(triggered(bool)), this, SLOT(toLeftActionTriggered()));
		connect(toRightAction, SIGNAL(triggered(bool)), this, SLOT(toRightActionTriggered()));
		connect(toEnlargeAction, SIGNAL(triggered(bool)), this, SLOT(toEnlargeActionTriggered()));
		connect(toLessenAction, SIGNAL(triggered(bool)), this, SLOT(toLessenActionTriggered()));
		connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(deleteActionTriggered()));

		connect(about, SIGNAL(triggered(bool)), this, SLOT(aboutTriggered()));
		connect(aboutQt, SIGNAL(triggered(bool)), this, SLOT(aboutQtTriggered()));
		connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
	}
	else if (flag == 1)
	{
		connect(openAction, SIGNAL(triggered(bool)), this, SLOT(openActionTriggered1()));
		connect(lastAction, SIGNAL(triggered(bool)), this, SLOT(lastActionTriggered1()));
		connect(nextAction, SIGNAL(triggered(bool)), this, SLOT(nextActionTriggered1()));
		//connect(toLeftAction, SIGNAL(triggered(bool)), this, SLOT(toLeftActionTriggered()));
		//connect(toRightAction, SIGNAL(triggered(bool)), this, SLOT(toRightActionTriggered()));
		//connect(toEnlargeAction, SIGNAL(triggered(bool)), this, SLOT(toEnlargeActionTriggered()));
		//connect(toLessenAction, SIGNAL(triggered(bool)), this, SLOT(toLessenActionTriggered()));
		//connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(deleteActionTriggered()));

		connect(about, SIGNAL(triggered(bool)), this, SLOT(aboutTriggered()));
		connect(aboutQt, SIGNAL(triggered(bool)), this, SLOT(aboutQtTriggered()));
		connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
	}
}

void MainWindow::aboutQtTriggered(void)
{
    qApp->aboutQt();
}

void MainWindow::aboutTriggered(void)
{
    aboutWidget.setWindowTitle("Help Infomation");
    aboutWidget.setWindowIcon(QIcon("./images/help.png"));
    aboutWidget.setFixedSize(QABOUT_WIDGET_WIDTH, QABOUT_WIDGET_HEIGHT);

    aboutWidget.show();
}

void MainWindow::initUiComponent(void)
{
    /* set menu and toolbar */
    setWindowComponet();

    /* image show widget */
    setQImageViewerWidget();
}

void MainWindow::initMainWindow(void)
{
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    toolBar = new QToolBar(this);
    addToolBar(toolBar);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
}