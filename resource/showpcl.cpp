#include "showpcl.h"
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
showpcl::showpcl(QWidget* parent,QString x,QString y) :
	QWidget(parent),
	ui(new Ui::showpcl),filepath(x),savepclpath(y)
{
	ui->setupUi(this);
	ViewerWindow = new QtOsgView(this);

	manipulator = new osgGA::TrackballManipulator;
	ViewerWindow->setCameraManipulator(manipulator);
	ViewerWindow->addEventHandler(new osgViewer::WindowSizeHandler);

	ViewerWindow->getCamera()->setClearColor(osg::Vec4f(255.0f, 255.0f, 255.0f, 1.0f));

	// 环境光
	ViewerWindow->getLight()->setAmbient(osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
	// 漫反射光
	ViewerWindow->getLight()->setDiffuse(osg::Vec4(0.1f, 0.1f, 0.1f, 1.0f));

	QFont font("Microsoft YaHei", 9, 75);

	ui->leftLayout->addWidget(ViewerWindow);
	//ui->rightLayout->addWidget(ViewerWindow1);
	ui->totalLayout->setStretch(0, 360);
	ui->totalLayout->setStretch(1, 360);
	this->setLayout(ui->totalLayout);
	//showcad();
	showpointcloud();
}
showpcl::~showpcl() {
	// 注释：析构函数
	delete ui;
}
void showpcl::showcad()
{
	osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
	std::string x = filepath.toStdString();
	model = osgDB::readNodeFile(x, a);
	model->setName("model");
	osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform;

	//trans->setMatrix(osg::Matrix::scale(scale_all,scale_all,scale_all));
	trans->addChild(model);
	model->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);

	osg::StateSet* state = model->getOrCreateStateSet();
	osg::ref_ptr<osg::LightModel> TwoSideLight = new osg::LightModel;
	TwoSideLight->setTwoSided(true);
	state->setMode(GL_CULL_FACE, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);   // 只关闭背面裁剪，造成生成背面不透明，但黑面 ;
	state->setAttributeAndModes(TwoSideLight, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);  //再加上双面光照，使背面完全出现！;
	trans->setName("trans");
	root->addChild(trans);
	root->setName("root");



	//osg::ref_ptr<BoxPicker> picker = new BoxPicker;
	//ViewerWindow->addEventHandler(picker.get());
	osg::ref_ptr<osg::Geometry> geo = new osg::Geometry;
	geo->setDataVariance(osg::Object::DYNAMIC);
	geo->setUseDisplayList(false);
	osg::Vec3Array* vertex = new osg::Vec3Array(4);
	(*vertex)[0] = osg::Vec3(-100.5, 0.0, -100.5);
	(*vertex)[1] = osg::Vec3(100.5, 0.0, -100.5);
	(*vertex)[2] = osg::Vec3(100.5, 0.0, 100.5);
	(*vertex)[3] = osg::Vec3(-100.5, 0.0, 100.5);
	geo->setVertexArray(vertex);

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0, 1.0, 0.0, 1.0));
	geo->setColorArray(colors);
	geo->setColorBinding(osg::Geometry::BIND_OVERALL);

	osg::Vec3Array* normal = new osg::Vec3Array(1);
	(*normal)[0] = osg::Vec3(0, -1, 0);
	geo->setNormalArray(normal);
	geo->setNormalBinding(osg::Geometry::BIND_OVERALL);

	osg::ref_ptr<osg::DrawArrays> pri = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, 4);

	geo->addPrimitiveSet(pri.get());

	osg::ref_ptr<osg::PolygonMode> polyMode = new osg::PolygonMode;
	polyMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
	geo->getOrCreateStateSet()->setAttributeAndModes(polyMode.get());
	geo->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	//picker->geometry = geo;

	osg::Camera* camera = new osg::Camera;
	camera->setProjectionMatrix(osg::Matrix::ortho2D(-1.0, 1.0, -1.0, 1.0));
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrixAsLookAt(osg::Vec3(0, -1, 0), osg::Vec3(0, 0, 0), osg::Vec3(0, 0, 1));
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	camera->setAllowEventFocus(false);
	camera->setName("camera");
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(geo.get());

	camera->addChild(geode.get());
	root->addChild(camera);
	ViewerWindow->setSceneData(root.get());
}
void showpcl::showpointcloud()
{
		std::fstream fs;
		//std::string pclpath = savepclpath.toStdString() + "/" + ;
		QFileInfo s(savepclpath);
		fs.open("D:\\my\\sunxun\\test1\\pointcloud\\0.txt");
		//新建一个用于存放几何图元的Geode
		osg::ref_ptr<osg::Geode> geode = new osg::Geode();
		//创建保存几何信息的对象
		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
		osg::ref_ptr<osg::Vec3dArray> vertices = new osg::Vec3dArray();
		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
		while (!fs.eof())
		{
			//从txt读取一行中的坐标和proposal类型
			double x, y, z, r;
			int proposal_type;
			fs >> x >> y >> z >> r ;
			//fs>>z>>x>>y;
			vertices->push_back(osg::Vec3d(x, y, z));
			osg::Vec4 color;
			if(r==0)
				color = osg::Vec4(0, 0, 0, 1.0);
			else if(r==1)
				color = osg::Vec4(1.0, 0, 0, 1.0);
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