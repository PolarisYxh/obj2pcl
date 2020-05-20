#include "Photographer_sx.h"
#include <string>
#include <vector>
#include<windows.h>
#include<fstream>
#include<osg/LOD>
#include<cmath>
using namespace std;
//#include<opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp>  
//#include <opencv2/highgui/highgui.hpp> 
//using namespace cv;

MyUtils::MyUtils() {}
MyUtils::~MyUtils() {}
//2020.4.7
void  MyUtils::getFile(std::string path, std::string suffix, std::vector<std::string>& model_names)
{
	long long  hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;//�����洢�ļ���Ϣ�Ľṹ��
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)  //��һ�β���
	{
		do
		{
			std::string f_name = fileinfo.name;
			int suffix_begin = f_name.find(".");
			std::string temp = f_name.substr(suffix_begin, f_name.size() - suffix_begin);
			if (temp == suffix)//ʶ��.ive   .txt   .mat   .png.....
			{
				model_names.push_back(f_name);
			}

		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile); //��������
	}
}


NodeVisitor_dyer::NodeVisitor_dyer() :osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
{
	open_transparent = true;
}

NodeVisitor_dyer::NodeVisitor_dyer(osg::Vec4 given_color, bool if_open) : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
{
	color = given_color;
	open_transparent = if_open;
}

NodeVisitor_dyer::~NodeVisitor_dyer() {}

void NodeVisitor_dyer::set_color(osg::Vec4 given_color)
{
	color = given_color;
}

osg::Vec4 NodeVisitor_dyer::get_color()
{
	return color;
}

void NodeVisitor_dyer::apply(osg::Geode& geode)
{
	unsigned int i = 0;
	osg::ref_ptr<osg::Vec4Array> changecolor = new osg::Vec4Array();//����changecolor�д洢��part������Ϊ��geode��ɫ
	changecolor->push_back(color);
	for (i = 0; i < geode.getNumDrawables(); i++)
	{
		osg::ref_ptr<osg::Geometry> geometry = geode.getDrawable(i)->asGeometry();
		osg::ref_ptr<osg::Array> geocolor = geometry->getColorArray();

		geometry->setColorArray(changecolor);
		geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

		osg::ref_ptr< osg::StateSet > state_set = geometry->getOrCreateStateSet();//����geode�Ĳ��ʣ����������൱�����е�geode�Ĳ�����ͬ�����ÿ��part���ʲ�ͬ����Ч�����ã���
		osg::ref_ptr< osg::Material > material = new osg::Material;
		material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
		material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.75f, 0.75f, 0.75f, 0.5f));
		material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.75f, 0.75f, 0.75f, 0.5f));
		material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.75f, 0.75f, 0.75f, 0.5f));
		material->setShininess(osg::Material::FRONT_AND_BACK, 51.2f);
		state_set->setAttributeAndModes(material.get(), osg::StateAttribute::ON);

		if (open_transparent)
		{
			//ֻ��ģ���еĲ������а�͸�����������ڱ���ģ�Ͳ���ʱ��1��������ʾ��ɫalphaͨ��С��1��2�������ں�ģ�ͣ�3��ͬʱ��Ⱦ��ʽ��Ϊ͸����ʽ
			//�����ںϲ���
			state_set->setMode(GL_BLEND, osg::StateAttribute::ON);
			//������Ⱦģʽ
			state_set->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		}
	}
}


Photographer::Photographer() {}
Photographer::~Photographer() {}

bool Photographer::is_dof_by_dynamic_cast(osg::Node* node)
{
	osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(node);
	if (dofnode == nullptr)
		return false;
	else
		return true;
}

void Photographer::add_father_level(osg::Group* child, osg::Group* new_parent)
{
	osg::ref_ptr<osg::Group> old_parent;
	old_parent = child->getParent(0);
	string name = child->getName() + "_trans";
	new_parent->setName(name);
	new_parent->addChild(child);
	old_parent->removeChild(child);
	old_parent->addChild(new_parent);
}

//ͨ���ĸ��������仯��������Ǿ������ĸ��任���������
osg::Matrix Photographer::calculateTransformMatrix(osg::Vec3f a1, osg::Vec3f b1, osg::Vec3f c1, osg::Vec3f d1,
	osg::Vec3f a2, osg::Vec3f b2, osg::Vec3f c2, osg::Vec3f d2)
{
	osg::Matrix mt;
	osg::Matrix m1, m2;
	m1.set(a1.x(), a1.y(), a1.z(), 1.0,
		b1.x(), b1.y(), b1.z(), 1.0,
		c1.x(), c1.y(), c1.z(), 1.0,
		d1.x(), d1.y(), d1.z(), 1.0);
	m2.set(a2.x(), a2.y(), a2.z(), 1.0,
		b2.x(), b2.y(), b2.z(), 1.0,
		c2.x(), c2.y(), c2.z(), 1.0,
		d2.x(), d2.y(), d2.z(), 1.0);
	mt = osg::Matrix::inverse(m1) * m2;
	return mt;
}

void Photographer::getAllPartNodesUnderNode(osg::Node* node, std::vector<osg::Group*>& node_vec)
{
	node_vec.clear();
	std::list<osg::Group*> groupqueue;
	osg::ref_ptr<osg::Group> tempgroup;
	std::string  temp_name;

	int i = 0, num = 0;
	groupqueue.push_back(node->asGroup());
	while (!groupqueue.empty())
	{
		tempgroup = groupqueue.front();
		groupqueue.pop_front();
		num = tempgroup->getNumChildren();
		for (i = 0; i < num; i++)
		{
			//�жϵ�ǰ���ӵĽڵ��ǲ���dof��������
			//temp_name=tempgroup->getChild(i)->getName();
			if (is_dof_by_dynamic_cast(tempgroup->getChild(i)))
				groupqueue.push_back(tempgroup->getChild(i)->asGroup());
			else
				node_vec.push_back(tempgroup->getChild(i)->asGroup());
		}
	}
}

void Photographer::getAllDOFNodesUnderNode(osg::Node* node, std::vector<osg::Group*>& node_vec)
{
	node_vec.clear();
	std::list<osg::Group*> groupqueue;
	osg::ref_ptr<osg::Group> tempgroup;
	std::string  temp_name;

	int i = 0, num = 0;
	groupqueue.push_back(node->asGroup());
	while (!groupqueue.empty())   //�����д����⣡�����������������������������������������пո�
	{
		tempgroup = groupqueue.front();
		groupqueue.pop_front();

		num = tempgroup->getNumChildren();
		for (i = 0; i < num; i++)
		{
			//�жϵ�ǰ���ӵĽڵ��ǲ���dof
			temp_name=tempgroup->getChild(i)->getName();
			if (is_dof_by_dynamic_cast(tempgroup->getChild(i)))
			{
				node_vec.push_back(tempgroup->getChild(i)->asGroup());
				groupqueue.push_back(tempgroup->getChild(i)->asGroup()); //������if������break������������������

			}
			else
				//groupqueue.push_back(tempgroup->getChild(i)->asGroup()); //������if������break������������������
				break;
		}
	}
}

int Photographer::Tool_test_1(std::string ive_path, std::string save_path, std::string dof_name, double ani_angle, double ani_count, double cam_H, double track_R, double photo_count)
{
	//�������ļ���
	std::string sub1 = save_path + "pose";
	std::string sub2 = save_path + "depth";
	std::string sub3 = save_path + "label";
	if (0 != access(sub1.c_str(), 0))
	{ // if this folder not exist, create a new one.
		mkdir(sub1.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
		//���� ::_mkdir  ::_access Ҳ�У���֪��ʲô��˼
	}
	if (0 != access(sub2.c_str(), 0))
	{
		mkdir(sub2.c_str());
	}
	if (0 != access(sub3.c_str(), 0))
	{
		mkdir(sub3.c_str());
	}

	//����iveģ�ͣ�ÿ�˶�һ�Σ�����һ�����պ���
	osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
	osg::ref_ptr<osg::Node> scene = osgDB::readNodeFile(ive_path, a);
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(scene.get());

	//����dof�����ҽڵ����  &&  �Լ��˶���ʽ��������
	int dof_ID, i;
	std::vector<osg::Group*> dofnodes;
	Photographer::getAllDOFNodesUnderNode(scene, dofnodes);
	for (i = 1; i < dofnodes.size(); i++)//0�Ǹ��ڵ�
	{
		std::string node_name = dofnodes[i]->getName();
		if (dofnodes[i]->getName() == dof_name)
		{
			dof_ID = i;
			break;
		}
	}
	std::string ani_type = dof_name.substr(dof_name.length() - 2, 2); //����

	//ģ����ɫ+��������
	NodeVisitor_dyer dyerb(osg::Vec4(0.0, 0.0, 0.0, 1.0), false); //������͸��  
	dofnodes[0]->accept(dyerb); //
	NodeVisitor_dyer dyerr(osg::Vec4(1.0, 0.0, 0., 1.0), false); //������͸��  
	dofnodes[dof_ID]->accept(dyerr); //
	osg::StateSet* state = scene->getOrCreateStateSet();
	state->setMode(GL_CULL_FACE, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);   // ֻ�رձ���ü���������ɱ��治͸����������;
	state->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);//�رչ���
	state->setMode(GL_LIGHT0, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

	//�����������λ��
	osg::ref_ptr<osg::Vec3dArray> eye = new osg::Vec3dArray();
	float thea = 2*osg::PI / photo_count;
	for (i = 0; i < photo_count; i++)
	{
		eye->push_back(osg::Vec3d(track_R * cos(i * thea), track_R * sin(i * thea), cam_H));
	}

	//��ʼ��ÿ��ģ�ͽ���һ������
	osg::ComputeBoundsVisitor boundVisitor;
	osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(dofnodes[dof_ID]);
	dofnode->accept(boundVisitor);
	osg::Vec3 curvec;
	osg::Matrix PutMatrix = dofnode->getPutMatrix();
	osg::Vec3d start = dofnode->getInversePutMatrix().getTrans();

	//����ani_angle�����Ϊ�����˶���Χ��������������޸�
	//float ani_radians = osg::DegreesToRadians(ani_angle/ ani_count); //����

	for (i = 0; i < ani_count + 1; i++)
	{
		if (ani_type == "_r")
		{
			float ani_radians = osg::DegreesToRadians(ani_angle / ani_count);
			curvec.set(0, i * ani_radians, 0);  //����֤
			dofnode->setCurrentHPR(curvec);
		}
		else if (ani_type == "_t")
		{
			float ani_radians = ani_angle / ani_count;
			curvec.set( i * ani_radians, 0, 0);  //����֤
			dofnode->setCurrentTranslate(curvec);
		}

		//��ʼ����ÿ�����
		for (int j = 0; j < photo_count; j++)
		{
			osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
			//viewer->getCamera()->setComputeNearFarMode( osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR );//���ø�ģʽ�� zNear zFar��ֵ����׼ȷ�ģ�������Ĭ���Զ�����Զ�������棬zNear��ʹ������Ҳ��Ч
			viewer->setSceneData(root.get());
			int pixelwidth = 600, pixelheight = 400;
			osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
			traits->x = 0;
			traits->y = 0;
			traits->width = pixelwidth;
			traits->height = pixelheight;
			traits->windowDecoration = true;
			traits->doubleBuffer = true;
			traits->sharedContext = 0;
			traits->samples = 0; //����� ���ز�����
			traits->pbuffer = true; //������Ⱦ
			osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
			viewer->getCamera()->setClearColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));      //         ��ɫ��ɫ 255 255 255
			viewer->getCamera()->setGraphicsContext(gc);
			gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			viewer->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
			double fovy = 30.f, aspectRatio = double(traits->width) / double(traits->height), zNear = 1.0, zFar = 10.0;
			viewer->getCamera()->setProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
			osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();
			unsigned int  width = gc->getTraits()->width, height = gc->getTraits()->height;
			wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
			viewer->realize();
			osg::Vec3d eyes = eye->at(j), center = osg::Vec3d(0.0, 0.0, 0.0), up = osg::Vec3d(0.0, 0.0, 1.0);
			viewer->getCamera()->setViewMatrixAsLookAt(eyes, center, up);
			//����ͼƬ�Ͳ���
			std::string output_path_p = save_path + "pose\\p_ani_" + std::to_string((long double)i) + "_photo_" + std::to_string((long double)j) + ".txt";
			std::ofstream outfile(output_path_p);
			osg::ref_ptr<osg::Image> image_d = new osg::Image();
			std::string output_path_d = save_path + "depth\\d_ani_" + std::to_string((long double)i) + "_photo_" + std::to_string((long double)j) + ".png";
			osg::ref_ptr<osg::Image> image_rgb = new osg::Image();
			std::string output_path_l = save_path + "label\\l_ani_" + std::to_string((long double)i) + "_photo_" + std::to_string((long double)j) + ".png";
			image_d->allocateImage(width, height, 1, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE);
			viewer->getCamera()->attach(osg::Camera::DEPTH_BUFFER, image_d.get());
			image_rgb->allocateImage(width, height, 1, GL_RGB, GL_UNSIGNED_BYTE);
			viewer->getCamera()->attach(osg::Camera::COLOR_BUFFER, image_rgb.get());
			viewer->frame();
			Sleep(1000);
			//viewer->run();
			osg::Matrix V = viewer->getCamera()->getViewMatrix(); // ��ͼ����
			osg::Matrix P = viewer->getCamera()->getProjectionMatrix(); //ͶӰ����
			osg::Matrix W = viewer->getCamera()->getViewport()->computeWindowMatrix(); //���ڱ任����
			osg::Matrix PW = P * W;
			for (int m = 0; m < 4; m++) {
				for (int n = 0; n < 4; n++) {
					outfile << V(m, n) << " ";
				}
				outfile << std::endl;
			}
			for (int m = 0; m < 4; m++) {
				for (int n = 0; n < 4; n++) {
					outfile << P(m, n) << " ";
				}
				outfile << std::endl;
			}
			for (int m = 0; m < 4; m++) {
				for (int n = 0; n < 4; n++) {
					outfile << W(m, n) << " ";
				}
				outfile << std::endl;
			}
			for (int m = 0; m < 4; m++) {
				for (int n = 0; n < 4; n++) {
					outfile << PutMatrix(m, n) << " ";
				}
				outfile << std::endl;
			}
			//outfile << start.x()<<" "<<start.y()<<" "<<start.z()<<" "<<0<< std::endl; //������ ���˶������λ�ñ��桿
			outfile.close();
			bool a = osgDB::writeImageFile(*(image_d.get()), output_path_d);
			bool b = osgDB::writeImageFile(*(image_rgb.get()), output_path_l);
		}

	}
	return 0;
}

int Photographer::Tool_test_2(std::string ive_path, std::string save_path, std::string dof_name, double ani_angle, double ani_count, double cam_H, double track_R, double photo_count, std::string scene_name)
{
	//����iveģ�ͣ�ÿ�˶�һ�Σ�����һ�����պ���
	osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
	osg::ref_ptr<osg::Node> scene = osgDB::readNodeFile(ive_path, a);
	/*osg::ComputeBoundsVisitor boundVisitor;
	scene->accept(boundVisitor);
	osg::BoundingBox boundingBox = boundVisitor.getBoundingBox();
	float xl = boundingBox.xMax() - boundingBox.xMin(), yl = boundingBox.yMax() - boundingBox.yMin(), zl = boundingBox.zMax() - boundingBox.zMin();
	float ss;
	if (xl > yl && xl > zl)ss = xl;
	if (yl > xl && yl > zl)ss = yl;
	if (zl > yl && zl > xl)ss = zl;
	osg::ref_ptr < osg::MatrixTransform> scale = new osg::MatrixTransform;
	osg::Vec3 center = scene->getBound().center();
	scale->setMatrix(osg::Matrix::translate(-center) * osg::Matrix::scale(1/ss, 1 / ss, 1 / ss));
	scale->addChild(scene);*/
	osg::ref_ptr<osg::Group> root = new osg::Group;
	osg::ref_ptr<osg::Group> scenegroup = scene->asGroup()->getChild(0)->asGroup(); //��"root"���
	root->addChild(scene.get());
	int i=0;

	//dofnodes װ����ģ��Ҫ�˶���dof��� ��������ÿ��ģ�͵ĵ�һ��dof��
	std::vector<osg::Group*> dofnodes;
	// ��¼dofnodes��ÿ������PutMatrix�������Լ���flow ,Ҳ��¼���˶��� ,instance_ID��PutMatrix˳���Ӧ
	std::vector<osg::Matrix> PutMatrix;
	// ��¼ÿ��ģ��λ�˵ı任����
	std::vector<osg::Matrix> Trans;
	// ��¼dof��� ��_r _rt _t
	std::vector<int> dof_type;

	//��¼����scene�ı任����
	osg::ref_ptr < osg::MatrixTransform> scenescale = dynamic_cast<osg::MatrixTransform*>(scene->asGroup());
	osg::Matrix scene_M = scenescale->getMatrix();
	//�����ģ�͸��ڵ㣬����������ʵ��Ⱦɫ
	//osg::ref_ptr<osg::Group> scenegroup = scene->asGroup()->getChild(0)->asGroup(); //��"root"���
	for (i = 0; i < scenegroup->getNumChildren(); i++)
	{
		//��ֹ�������Ⱦɫ�ǣ� 0,0,0,1��
		osg::ref_ptr<osg::Node> tempgroup = scenegroup->getChild(i)->asGroup()->getChild(0);
		std::string temp_name = tempgroup->getName();
		int suffix_begin = temp_name.find("_");
		if (temp_name.substr(0, suffix_begin) == "cabinet")
		{   
			// dyerr�ĵ�һ���������������𣬵ڶ����������instanceʵ��ID
			NodeVisitor_dyer dyerr(osg::Vec4(0.01, (dofnodes.size()+1)*0.01, 1.0, 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.01, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			//��һ��dof��root_dof����Ҫ����
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		if (temp_name.substr(0, suffix_begin) == "swivel")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.02, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.02, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		if (temp_name.substr(0, suffix_begin) == "washing")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.03, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.03, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		if (temp_name.substr(0, suffix_begin) == "water")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.04, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.04, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		if (temp_name.substr(0, suffix_begin) == "laptop")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.05, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.05, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		if (temp_name.substr(0, suffix_begin) == "lamp")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.06, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.06, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		if (temp_name.substr(0, suffix_begin) == "bucket")
		{
			NodeVisitor_dyer dyerr(osg::Vec4( 0.07, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.07, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		if (temp_name.substr(0, suffix_begin) == "clock")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.08, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.08, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		if (temp_name.substr(0, suffix_begin) == "oven")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.09, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.09, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		if (temp_name.substr(0, suffix_begin) == "faucet")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.10, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.10, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		/*if (temp_name.substr(0, suffix_begin) == "04379243")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.7, 0.0, 0., 1.0), false); //������͸��  
			osg::ref_ptr<osg::Node> color_parent = tempgroup->asGroup()->getParent(0);
			color_parent->accept(dyerr);
		}
		if (temp_name.substr(0, suffix_begin) == "03211117")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.6, 0.0, 0., 1.0), false); //������͸��  
			osg::ref_ptr<osg::Node> color_parent = tempgroup->asGroup()->getParent(0);
			color_parent->accept(dyerr);
		}
		if (temp_name.substr(0, suffix_begin) == "02818832")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.5, 0.0, 0., 1.0), false); //������͸��  
			osg::ref_ptr<osg::Node> color_parent = tempgroup->asGroup()->getParent(0);
			color_parent->accept(dyerr);
		}
		if (temp_name.substr(0, suffix_begin) == "04256520")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.4, 0.0, 0., 1.0), false); //������͸��  
			osg::ref_ptr<osg::Node> color_parent = tempgroup->asGroup()->getParent(0);
			color_parent->accept(dyerr);
		}
		if (temp_name.substr(0, suffix_begin) == "03467517")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.3, 0.0, 0., 1.0), false); //������͸��  
			osg::ref_ptr<osg::Node> color_parent = tempgroup->asGroup()->getParent(0);
			color_parent->accept(dyerr);
		}
		if (temp_name.substr(0, suffix_begin) == "03991062")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.2, 0.0, 0., 1.0), false); //������͸��  
			osg::ref_ptr<osg::Node> color_parent = tempgroup->asGroup()->getParent(0);
			color_parent->accept(dyerr);
		}
		if (temp_name.substr(0, suffix_begin) == "02773838")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.1, 0.0, 0., 1.0), false); //������͸��  
			osg::ref_ptr<osg::Node> color_parent = tempgroup->asGroup()->getParent(0);
			color_parent->accept(dyerr);
		}*/
	}

	//ģ����ɫ+��������
	osg::StateSet* state = scene->getOrCreateStateSet();
	state->setMode(GL_CULL_FACE, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);   // ֻ�رձ���ü���������ɱ��治͸����������;
	state->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);//�رչ���
	state->setMode(GL_LIGHT0, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

	//�����λ��
	osg::Vec3d eye = osg::Vec3d(0.,1,4);
	// �����˶�����
	ani_count = 4;
	float rotate_step = 20.0;
	float tran_step = 0.05;
	// ���Ի����������ʵ����RGBͼ��depthͼ��matrix��Ϣ����
	for (i = 0; i < ani_count ; i++)
	{
		int type = 0;
		//��ʼ��ÿ��ģ�ͽ���һ������
		osg::ComputeBoundsVisitor boundVisitor;
		for (int j = 0; j < dofnodes.size(); j++)
		{
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(dofnodes[j]->getChild(0)->asGroup()->getChild(0));
			dofnode->accept(boundVisitor);
			osg::Vec3 curvec;
			std::string dof_name_temp = dofnode->getName();
			int suffix_begin = dof_name_temp.rfind("_", dof_name_temp.size()-1);
			std::string ani_type = dof_name_temp.substr(suffix_begin, dof_name_temp.size()- suffix_begin); //����
			if (ani_type == "_r")
			{
				float ani_radians = osg::DegreesToRadians(rotate_step);
				curvec.set(0, (i+1 )* ani_radians, 0);  
				dofnode->setCurrentHPR(curvec);
				type = 1;
			}
			if (ani_type == "_t")
			{
				osg::ref_ptr<osgSim::DOFTransform> dof_next = dynamic_cast<osgSim::DOFTransform*>(dofnodes[j]->getChild(0)->asGroup()->getChild(0)->asGroup()->getChild(0));
				if (dof_next == nullptr) {
					float ani_radians = tran_step;
					curvec.set( (i+1) * ani_radians, 0, 0);  
					dofnode->setCurrentTranslate(curvec);
					type = 2;
				}
				else {//�൱��_rt
					float ani_radians = osg::DegreesToRadians(rotate_step);
					curvec.set(0, (i + 1)* ani_radians, 0);
					dofnode->setCurrentHPR(curvec);
					type = 1;
				}
				
			}
			if (ani_type == "_s")   //rt���ֻ������ת ����
			{
				float ani_radians = osg::DegreesToRadians(rotate_step);
				curvec.set(0, (i + 1)* ani_radians, 0);  
				dofnode->setCurrentHPR(curvec);
				type = 1;
			}
			//����ǵ�һ�α���dof,��¼_r��_rt��_t���Ϊ1 1 2
			if (i == 0) {
				dof_type.push_back(type);
			}

		}
		osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
		//viewer->getCamera()->setComputeNearFarMode( osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR );//���ø�ģʽ�� zNear zFar��ֵ����׼ȷ�ģ�������Ĭ���Զ�����Զ�������棬zNear��ʹ������Ҳ��Ч
		viewer->setSceneData(root.get());
		int pixelwidth = 600, pixelheight = 400;
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		traits->x = 0;
		traits->y = 0;
		traits->width = pixelwidth;
		traits->height = pixelheight;
		traits->windowDecoration = true;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;
		traits->samples = 0; //����� ���ز�����
		traits->pbuffer = true; //������Ⱦ
		osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
		viewer->getCamera()->setClearColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));      //         ��ɫ��ɫ 255 255 255
		viewer->getCamera()->setGraphicsContext(gc);
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		viewer->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
		double fovy = 30.f, aspectRatio = double(traits->width) / double(traits->height), zNear = 1.0, zFar = 10.0;
		viewer->getCamera()->setProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
		osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();
		unsigned int  width = gc->getTraits()->width, height = gc->getTraits()->height;
		wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
		viewer->realize();
		osg::Vec3d eyes = eye, center = osg::Vec3d(0.0, 0.0, 0.0), up = osg::Vec3d(0.0, 0.0, 1.0);
		viewer->getCamera()->setViewMatrixAsLookAt(eyes, center, up);
		//����ͼƬ�Ͳ���
		std::string output_path_p = save_path + "pose\\"+scene_name+"_"+ std::to_string((long double)i)+".txt";
		std::ofstream outfile(output_path_p);
		osg::ref_ptr<osg::Image> image_d = new osg::Image();
		std::string output_path_d = save_path + "depth\\" + scene_name + "_" + std::to_string((long double)i) + ".png";
		osg::ref_ptr<osg::Image> image_rgb = new osg::Image();
		std::string output_path_l = save_path + "label\\" + scene_name + "_" + std::to_string((long double)i) + ".png";
		image_d->allocateImage(width, height, 1, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE);
		viewer->getCamera()->attach(osg::Camera::DEPTH_BUFFER, image_d.get());
		image_rgb->allocateImage(width, height, 1, GL_RGB, GL_UNSIGNED_BYTE);
		viewer->getCamera()->attach(osg::Camera::COLOR_BUFFER, image_rgb.get());
		viewer->frame();
		Sleep(1000);
		//viewer->run();
		osg::Matrix V = viewer->getCamera()->getViewMatrix(); // ��ͼ����
		osg::Matrix P = viewer->getCamera()->getProjectionMatrix(); //ͶӰ����
		osg::Matrix W = viewer->getCamera()->getViewport()->computeWindowMatrix(); //���ڱ任����
		osg::Matrix VPW = V* P * W;
		for (int m = 0; m < 4; m++) {
			for (int n = 0; n < 4; n++) {
				outfile << VPW(m, n) << " ";
			}
			outfile << std::endl;
		}
		for (int m = 0; m < 4; m++) {
			for (int n = 0; n < 4; n++) {
				outfile << scene_M(m, n) << " ";
			}
			outfile << std::endl;
		}
		outfile << PutMatrix.size() << " " <<0<< " "<<0 << " " <<0<< std::endl;
		for (int j = 0; j < dof_type.size(); j++) {
			outfile << dof_type[j] << " " << 0 << " " << 0 << " " << 0 << std::endl;
		}
		for (int j = 0; j < PutMatrix.size(); j++) {
			for (int m = 0; m < 4; m++) {
				for (int n = 0; n < 4; n++) {
					outfile << PutMatrix[j](m, n) << " ";
				}
				outfile << std::endl;
			}
			for (int m = 0; m < 4; m++) {
				for (int n = 0; n < 4; n++) {
					outfile << Trans[j](m, n) << " ";
				}
				outfile << std::endl;
			}
		}
		outfile.close();
		bool a = osgDB::writeImageFile(*(image_d.get()), output_path_d);
		bool b = osgDB::writeImageFile(*(image_rgb.get()), output_path_l);
		
	}

	//��dofpart������ɫ
	/*for (int j = 0; j < dofnodes.size(); j++)
	{
		NodeVisitor_dyer dyerr(osg::Vec4(0.10, (i + 1) * 0.01, 0., 1.0), false); //������͸��  
		tempgroup->accept(dyerr);
		dofnodes[j]->getChild(0)->asGroup()->getChild(0);
	}
	//���Ի��instance�� ��ֹ������dof_part������
	for (i = 0; i < ani_count; i++)
	{
		//��ʼ��ÿ��ģ�ͽ���һ������
		osg::ComputeBoundsVisitor boundVisitor;
		for (int j = 0; j < dofnodes.size(); j++)
		{
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(dofnodes[j]->getChild(0)->asGroup()->getChild(0));
			dofnode->accept(boundVisitor);
			osg::Vec3 curvec;
			std::string dof_name_temp = dofnode->getName();
			int suffix_begin = dof_name_temp.rfind("_", dof_name_temp.size() - 1);
			std::string ani_type = dof_name.substr(suffix_begin, dof_name_temp.size() - suffix_begin); //����
			if (ani_type == "_r")
			{
				float ani_radians = osg::DegreesToRadians(10.0);
				curvec.set(0, (i + 1) * ani_radians, 0);
				dofnode->setCurrentHPR(curvec);
			}
			if (ani_type == "_t")
			{
				float ani_radians = 0.02;
				curvec.set((i + 1) * ani_radians, 0, 0);
				dofnode->setCurrentTranslate(curvec);
			}
			if (ani_type == "_rt")   //rt���ֻ����ѡ�� ����
			{
				float ani_radians = osg::DegreesToRadians(10.0);
				curvec.set(i * ani_radians, 0, 0);
				curvec.set(0, (i + 1) * ani_radians, 0);
				dofnode->setCurrentHPR(curvec);
			}

		}
		osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
		//viewer->getCamera()->setComputeNearFarMode( osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR );//���ø�ģʽ�� zNear zFar��ֵ����׼ȷ�ģ�������Ĭ���Զ�����Զ�������棬zNear��ʹ������Ҳ��Ч
		viewer->setSceneData(root.get());
		int pixelwidth = 600, pixelheight = 400;
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		traits->x = 0;
		traits->y = 0;
		traits->width = pixelwidth;
		traits->height = pixelheight;
		traits->windowDecoration = true;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;
		traits->samples = 0; //����� ���ز�����
		traits->pbuffer = true; //������Ⱦ
		osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
		viewer->getCamera()->setClearColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));      //         ��ɫ��ɫ 255 255 255
		viewer->getCamera()->setGraphicsContext(gc);
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		viewer->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
		double fovy = 30.f, aspectRatio = double(traits->width) / double(traits->height), zNear = 1.0, zFar = 10.0;
		viewer->getCamera()->setProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
		osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();
		unsigned int  width = gc->getTraits()->width, height = gc->getTraits()->height;
		wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
		viewer->realize();
		osg::Vec3d eyes = eye, center = osg::Vec3d(0.0, 0.0, 0.0), up = osg::Vec3d(0.0, 0.0, 1.0);
		viewer->getCamera()->setViewMatrixAsLookAt(eyes, center, up);
		//����ͼƬ�Ͳ���
		osg::ref_ptr<osg::Image> image_rgb = new osg::Image();
		std::string output_path_l = save_path + "dof_label\\" + scene_name + "_" + std::to_string((long double)i) + ".png";
		image_rgb->allocateImage(width, height, 1, GL_RGB, GL_UNSIGNED_BYTE);
		viewer->getCamera()->attach(osg::Camera::COLOR_BUFFER, image_rgb.get());
		viewer->frame();
		Sleep(1000);
		bool b = osgDB::writeImageFile(*(image_rgb.get()), output_path_l);

	}*/
	return 0;
}

int Photographer::Tool_test_3(std::string ive_path, std::string save_path, std::string dof_name, double ani_angle, double ani_count, double cam_H, double track_R, double photo_count, std::string scene_name)
{
	//����iveģ�ͣ�ÿ�˶�һ�Σ�����һ�����պ���
	osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
	osg::ref_ptr<osg::Node> scene = osgDB::readNodeFile(ive_path, a);
	osg::ref_ptr<osg::Group> root = new osg::Group;
	osg::ref_ptr<osg::Group> scenegroup = scene->asGroup()->getChild(0)->asGroup(); //��"root"���
	root->addChild(scene.get());
	int i = 0;

	//dofnodes װ����ģ��Ҫ�˶���dof��� ��������ÿ��ģ�͵ĵ�һ��dof��
	std::vector<osg::Group*> dofnodes;
	// ��¼dofnodes��ÿ������PutMatrix�������Լ���flow ,Ҳ��¼���˶��� ,instance_ID��PutMatrix˳���Ӧ
	std::vector<osg::Matrix> PutMatrix;
	// ��¼ÿ��ģ��λ�˵ı任����
	std::vector<osg::Matrix> Trans;
	// ��¼dof��� ��_r _rt _t
	std::vector<int> dof_type;

	//��¼����scene�ı任����
	osg::ref_ptr < osg::MatrixTransform> scenescale = dynamic_cast<osg::MatrixTransform*>(scene->asGroup());
	osg::Matrix scene_M = scenescale->getMatrix();
	//�����ģ�͸��ڵ㣬����������ʵ��Ⱦɫ
	//osg::ref_ptr<osg::Group> scenegroup = scene->asGroup()->getChild(0)->asGroup(); //��"root"���
	for (i = 0; i < scenegroup->getNumChildren(); i++)
	{
		//��ֹ�������Ⱦɫ�ǣ� 0,0,0,1��
		osg::ref_ptr<osg::Node> tempgroup = scenegroup->getChild(i)->asGroup()->getChild(0);
		std::string temp_name = tempgroup->getName();
		int suffix_begin = temp_name.find("_");
		if (temp_name.substr(0, suffix_begin) == "cabinet")
		{
			// dyerr�ĵ�һ���������������𣬵ڶ����������instanceʵ��ID
			NodeVisitor_dyer dyerr(osg::Vec4(0.01, (dofnodes.size() + 1) * 0.01, 1.0, 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.01, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			//��һ��dof��root_dof����Ҫ����
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());	
			
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
			osg::Matrix tt = model_trans->getMatrix();
			osg::Matrix PutM = dofnode->getInversePutMatrix();
			osg::Vec3d start = dofnode->getInversePutMatrix().getTrans();
			osg::Vec3d ori_axies = osg::Vec3d(1, 0, 0) * PutM- start;
			osg::Vec3d new_start = start*tt * scene_M;
			osg::Vec3d new_axies = ori_axies * tt *scene_M;
			osg::Matrix new_rot = osg::Matrix::rotate(osg::Vec3d(1, 0, 0), ori_axies);
			osg::Matrix new_t = osg::Matrix::translate(new_start);
			osg::Matrix new_inv_pm = new_rot * new_t;
		}
		if (temp_name.substr(0, suffix_begin) == "swivel")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.02, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.02, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		if (temp_name.substr(0, suffix_begin) == "washing")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.03, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.03, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		if (temp_name.substr(0, suffix_begin) == "water")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.04, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.04, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		if (temp_name.substr(0, suffix_begin) == "laptop")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.05, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.05, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		if (temp_name.substr(0, suffix_begin) == "lamp")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.06, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.06, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		if (temp_name.substr(0, suffix_begin) == "bucket")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.07, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.07, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		if (temp_name.substr(0, suffix_begin) == "clock")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.08, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.08, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		if (temp_name.substr(0, suffix_begin) == "oven")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.09, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.09, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		if (temp_name.substr(0, suffix_begin) == "faucet")
		{
			NodeVisitor_dyer dyerr(osg::Vec4(0.10, (dofnodes.size() + 1) * 0.01, 1., 1.0), false); //������͸��  
			tempgroup->accept(dyerr);
			NodeVisitor_dyer dyerdof(osg::Vec4(0.10, (dofnodes.size() + 1) * 0.01, 0.0, 1.0), false);
			tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->accept(dyerdof);
			dofnodes.push_back(tempgroup->asGroup());
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(tempgroup->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup());
			PutMatrix.push_back(dofnode->getPutMatrix());
			osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(tempgroup->asGroup()->getParent(0)->asGroup());
			Trans.push_back(model_trans->getMatrix());
		}
		
	}

	//ģ����ɫ+��������
	osg::StateSet* state = scene->getOrCreateStateSet();
	state->setMode(GL_CULL_FACE, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);   // ֻ�رձ���ü���������ɱ��治͸����������;
	state->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);//�رչ���
	state->setMode(GL_LIGHT0, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

	//�����λ��
	osg::Vec3d eye = osg::Vec3d(0., 1, 4);
	// �����˶�����
	ani_count = 4;
	float rotate_step = 20.0;
	float tran_step = 0.05;
	// ���Ի����������ʵ����RGBͼ��depthͼ��matrix��Ϣ����
	for (i = 0; i < ani_count; i++)
	{
		int type = 0;
		//��ʼ��ÿ��ģ�ͽ���һ������
		osg::ComputeBoundsVisitor boundVisitor;
		for (int j = 0; j < dofnodes.size(); j++)
		{
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(dofnodes[j]->getChild(0)->asGroup()->getChild(0));
			dofnode->accept(boundVisitor);
			osg::Vec3 curvec;
			std::string dof_name_temp = dofnode->getName();
			int suffix_begin = dof_name_temp.rfind("_", dof_name_temp.size() - 1);
			std::string ani_type = dof_name_temp.substr(suffix_begin, dof_name_temp.size() - suffix_begin); //����
			if (ani_type == "_r")
			{
				float ani_radians = osg::DegreesToRadians(rotate_step);
				curvec.set(0, (i + 1) * ani_radians, 0);
				dofnode->setCurrentHPR(curvec);
				type = 1;
			}
			if (ani_type == "_t")
			{
				osg::ref_ptr<osgSim::DOFTransform> dof_next = dynamic_cast<osgSim::DOFTransform*>(dofnodes[j]->getChild(0)->asGroup()->getChild(0)->asGroup()->getChild(0));
				if (dof_next == nullptr) {
					float ani_radians = tran_step;
					curvec.set((i + 1) * ani_radians, 0, 0);
					dofnode->setCurrentTranslate(curvec);
					type = 2;
				}
				else {//�൱��_rt
					float ani_radians = osg::DegreesToRadians(rotate_step);
					curvec.set(0, (i + 1) * ani_radians, 0);
					dofnode->setCurrentHPR(curvec);
					type = 1;
				}

			}
			if (ani_type == "_s")   //rt���ֻ������ת ����
			{
				float ani_radians = osg::DegreesToRadians(rotate_step);
				curvec.set(0, (i + 1) * ani_radians, 0);
				dofnode->setCurrentHPR(curvec);
				type = 1;
			}
			//����ǵ�һ�α���dof,��¼_r��_rt��_t���Ϊ1 1 2
			if (i == 0) {
				dof_type.push_back(type);
			}

		}
		osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
		//viewer->getCamera()->setComputeNearFarMode( osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR );//���ø�ģʽ�� zNear zFar��ֵ����׼ȷ�ģ�������Ĭ���Զ�����Զ�������棬zNear��ʹ������Ҳ��Ч
		viewer->setSceneData(root.get());
		int pixelwidth = 600, pixelheight = 400;
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		traits->x = 0;
		traits->y = 0;
		traits->width = pixelwidth;
		traits->height = pixelheight;
		traits->windowDecoration = true;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;
		traits->samples = 0; //����� ���ز�����
		traits->pbuffer = true; //������Ⱦ
		osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
		viewer->getCamera()->setClearColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));      //         ��ɫ��ɫ 255 255 255
		viewer->getCamera()->setGraphicsContext(gc);
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		viewer->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
		double fovy = 30.f, aspectRatio = double(traits->width) / double(traits->height), zNear = 1.0, zFar = 10.0;
		viewer->getCamera()->setProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
		osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();
		unsigned int  width = gc->getTraits()->width, height = gc->getTraits()->height;
		wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
		viewer->realize();
		osg::Vec3d eyes = eye, center = osg::Vec3d(0.0, 0.0, 0.0), up = osg::Vec3d(0.0, 0.0, 1.0);
		viewer->getCamera()->setViewMatrixAsLookAt(eyes, center, up);
		//����ͼƬ�Ͳ���
		std::string output_path_p = save_path + "pose\\" + scene_name + "_" + std::to_string((long double)i) + ".txt";
		std::ofstream outfile(output_path_p);
		osg::ref_ptr<osg::Image> image_d = new osg::Image();
		std::string output_path_d = save_path + "depth\\" + scene_name + "_" + std::to_string((long double)i) + ".png";
		osg::ref_ptr<osg::Image> image_rgb = new osg::Image();
		std::string output_path_l = save_path + "label\\" + scene_name + "_" + std::to_string((long double)i) + ".png";
		image_d->allocateImage(width, height, 1, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE);
		viewer->getCamera()->attach(osg::Camera::DEPTH_BUFFER, image_d.get());
		image_rgb->allocateImage(width, height, 1, GL_RGB, GL_UNSIGNED_BYTE);
		viewer->getCamera()->attach(osg::Camera::COLOR_BUFFER, image_rgb.get());
		viewer->frame();
		Sleep(1000);
		//viewer->run();
		osg::Matrix V = viewer->getCamera()->getViewMatrix(); // ��ͼ����
		osg::Matrix P = viewer->getCamera()->getProjectionMatrix(); //ͶӰ����
		osg::Matrix W = viewer->getCamera()->getViewport()->computeWindowMatrix(); //���ڱ任����
		osg::Matrix VPW = V * P * W;
		for (int m = 0; m < 4; m++) {
			for (int n = 0; n < 4; n++) {
				outfile << VPW(m, n) << " ";
			}
			outfile << std::endl;
		}
		for (int m = 0; m < 4; m++) {
			for (int n = 0; n < 4; n++) {
				outfile << scene_M(m, n) << " ";
			}
			outfile << std::endl;
		}
		outfile << PutMatrix.size() << " " << 0 << " " << 0 << " " << 0 << std::endl;
		for (int j = 0; j < dof_type.size(); j++) {
			outfile << dof_type[j] << " " << 0 << " " << 0 << " " << 0 << std::endl;
		}
		for (int j = 0; j < PutMatrix.size(); j++) {
			for (int m = 0; m < 4; m++) {
				for (int n = 0; n < 4; n++) {
					outfile << PutMatrix[j](m, n) << " ";
				}
				outfile << std::endl;
			}
			for (int m = 0; m < 4; m++) {
				for (int n = 0; n < 4; n++) {
					outfile << Trans[j](m, n) << " ";
				}
				outfile << std::endl;
			}
		}
		outfile.close();
		bool a = osgDB::writeImageFile(*(image_d.get()), output_path_d);
		bool b = osgDB::writeImageFile(*(image_rgb.get()), output_path_l);

	}

	return 0;
}

int Photographer::new_dataset(std::string ive_path, std::string save_path)
{
	osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
	osg::ref_ptr<osg::Node> scene = osgDB::readNodeFile(ive_path, a);
	

	osg::ref_ptr<osg::Group> scenegroup = scene->asGroup()->getChild(0)->asGroup(); //��"root"���
	osg::ref_ptr<osg::Group> root = new osg::Group; 
	root->addChild(scenegroup);

	osg::ref_ptr < osg::MatrixTransform> scenescale = dynamic_cast<osg::MatrixTransform*>(scene->asGroup());
	osg::Matrix scene_M = scenescale->getMatrix();
	for (int model_id = 0; model_id < scenegroup->getNumChildren(); model_id++)
	{
		osg::ref_ptr<osg::Node> tempgroup = scenegroup->getChild(model_id)->asGroup()->getChild(0); //ÿ��ģ�͵�root ,e.g. cabinet_0006.flt
		std::string temp_name = tempgroup->getName();
		int suffix_begin = temp_name.find("_");
		osg::ref_ptr < osg::MatrixTransform> model_trans = dynamic_cast<osg::MatrixTransform*>(scenegroup->getChild(model_id)->asGroup()); // �任������ trans
		osg::Matrix trans = model_trans->getMatrix();
		osg::Matrix trans_scene_M = trans * scene_M;
		std::string name_suf = temp_name.substr(0, suffix_begin);

		if (name_suf == "swivel" || name_suf == "cabinet" || name_suf == "washing" || name_suf == "water" || name_suf == "laptop" || name_suf == "lamp" || name_suf == "bucket" || name_suf == "clock" || name_suf == "oven" || name_suf == "faucet")
		{
			std::vector<osg::Group*> dofnodes;
			getAllDOFNodesUnderNode(tempgroup, dofnodes);
			unsigned int i = 0;
			for (i = 0; i < dofnodes.size(); i++)
			{
				osg::ref_ptr<osgSim::DOFTransform> dofnode;
				dofnode = dynamic_cast<osgSim::DOFTransform*>(dofnodes[i]);

				dofnode->setCurrentHPR(osg::Vec3(0.0, 0.0, 0.0));
				dofnode->setCurrentTranslate(osg::Vec3(0.0, 0.0, 0.0));
				dofnode->setCurrentScale(osg::Vec3(1.0, 1.0, 1.0));
				dofnode->setMaxScale(osg::Vec3(0.0, 0.0, 0.0));
				if (i == 0)
				{
					dofnode->setInversePutMatrix(osg::Matrix::identity());
					dofnode->setPutMatrix(osg::Matrix::identity());
				}
				else
				{
					osg::Matrix inverseput = dofnode->getInversePutMatrix();
					osg::Vec3 old_center, old_x_axis, old_y_axis, old_z_axis, ox, oy, oz, new_center, new_x_axis, new_y_axis, new_z_axis;
					float axis_length = 1.0;
					old_center = osg::Vec3(0.0, 0.0, 0.0) * inverseput;
					old_x_axis = osg::Vec3(axis_length, 0.0, 0.0) * inverseput;
					old_y_axis = osg::Vec3(0.0, axis_length, 0.0) * inverseput;
					old_z_axis = osg::Vec3(0.0, 0.0, axis_length) * inverseput;

					new_center = old_center * trans_scene_M;

					ox = old_x_axis * trans_scene_M - new_center;
					new_x_axis = new_center + (ox / ox.length() * axis_length);
					oy = old_y_axis * trans_scene_M - new_center;
					new_y_axis = new_center + (oy / oy.length() * axis_length);
					oz = old_z_axis * trans_scene_M - new_center;
					new_z_axis = new_center + (oz / oz.length() * axis_length);

					osg::Matrix new_inverseput = inverseput * calculateTransformMatrix(old_center, old_x_axis, old_y_axis, old_z_axis, new_center, new_x_axis, new_y_axis, new_z_axis);
					osg::Matrix new_put = osg::Matrix::inverse(new_inverseput);
					dofnode->setInversePutMatrix(new_inverseput);
					dofnode->setPutMatrix(new_put);
				}
			}
			//Ϊģ�ͷ���MatrixTransform���������
			std::vector<osg::Group*> partnodes;
			getAllPartNodesUnderNode(tempgroup, partnodes);
			unsigned int j = 0;
			for (j = 0; j < partnodes.size(); j++)
			{
				osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
				mt->setMatrix(trans_scene_M);
				add_father_level(partnodes[j], mt);
			}

			scenegroup->setChild(model_id, tempgroup);

		}
		else 
		{
			//osg::ref_ptr < osg::MatrixTransform> new_trans = trans_merge->clone(osg::CopyOp::DEEP_COPY_ALL);
			//osg::ref_ptr < osg::Node> new_node = dynamic_cast<osg::Node*>(scenegroup->getChild(i)->clone(osg::CopyOp::DEEP_COPY_ALL));
			
			osg::ref_ptr < osg::MatrixTransform> trans_merge = dynamic_cast<osg::MatrixTransform*>(scenegroup->getChild(model_id));
			trans_merge->setMatrix(trans_merge->getMatrix()*scene_M);
			osg::Node* child = trans_merge->getChild(0);
			if (trans_merge->getName() == "floortrans"|| trans_merge->getName() == "walltrans1"|| trans_merge->getName() == "walltrans2"|| trans_merge->getName() == "walltrans3")
			{
				osg::Node* planecp= child->clone(osg::CopyOp::DEEP_COPY_ALL)->asNode();
				trans_merge->asGroup()->setChild(0, planecp);
				if (trans_merge->getName() == "floortrans")
				{
					planecp->setName("floor");
					
				}	
				else if (trans_merge->getName() == "walltrans1")
				{
					planecp->setName("wall_1");
					trans_merge->setName("walltrans_1");
				}
				else if (trans_merge->getName() == "walltrans2")
				{
					planecp->setName("wall_2");
					trans_merge->setName("walltrans_2");
				}
				else if (trans_merge->getName() == "walltrans3")
				{
					planecp->setName("wall_3");
					trans_merge->setName("walltrans_3");
				}
			}	
			else if (trans_merge->getName() == "walltrans4")
			{
				child->setName("wall_4");
				trans_merge->setName("walltrans_4");
			}	
			else
			{
				string na = child->getName();
				int suffix_begin = na.find("_");
				std::string name_suf = na.substr(0, suffix_begin);
				trans_merge->setName( name_suf+ "_trans");
			}
		}
	}
	osgDB::Registry::instance()->writeNode(*root, save_path, osgDB::Registry::instance()->getOptions());
	return 0;
	/*if (name_suf == "cabinet" || name_suf == "washing" || name_suf == "water" || name_suf == "laptop" || name_suf == "lamp" || name_suf == "bucket" || name_suf == "clock" || name_suf == "oven" || name_suf == "faucet")
{
	osg::ref_ptr<osg::Group> dof_root = tempgroup->asGroup()->getChild(0)->asGroup();
	int num = dof_root->getNumChildren();
	for(int j=0;j<num-1;j++)
	{
		// ����dof����
		osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(dof_root->getChild(j));
		osg::Matrix inverseput = dofnode->getInversePutMatrix();
		osg::Vec3 old_center, old_x_axis, old_y_axis, old_z_axis, ox, oy, oz, new_center, new_x_axis, new_y_axis, new_z_axis;
		float axis_length = 1.0;
		old_center = osg::Vec3(0.0, 0.0, 0.0) * inverseput;
		old_x_axis = osg::Vec3(axis_length, 0.0, 0.0) * inverseput;
		old_y_axis = osg::Vec3(0.0, axis_length, 0.0) * inverseput;
		old_z_axis = osg::Vec3(0.0, 0.0, axis_length) * inverseput;

		new_center = old_center * trans_scene_M;

		ox = old_x_axis * trans_scene_M - new_center;
		new_x_axis = new_center + (ox / ox.length() * axis_length);
		oy = old_y_axis * trans_scene_M - new_center;
		new_y_axis = new_center + (oy / oy.length() * axis_length);
		oz = old_z_axis * trans_scene_M - new_center;
		new_z_axis = new_center + (oz / oz.length() * axis_length);

		osg::Matrix new_inverseput = inverseput * calculateTransformMatrix(old_center, old_x_axis, old_y_axis, old_z_axis, new_center, new_x_axis, new_y_axis, new_z_axis);
		osg::Matrix new_put = osg::Matrix::inverse(new_inverseput);
		dofnode->setInversePutMatrix(new_inverseput);
		dofnode->setPutMatrix(new_put);

		// ÿ��dof_root��ÿ���ӽ���� ����һ��trans���
		osg::ref_ptr<osg::Group> dof_child_node = dofnode->getChild(0)->asGroup();
		osg::ref_ptr < osg::MatrixTransform> trans_merge = new osg::MatrixTransform;
		trans_merge->setMatrix(trans_scene_M);
		trans_merge->setName("cabinet_Trans");
		trans_merge->addChild(dof_child_node);
		dofnode->setChild(0, trans_merge);
	}
	//��ֹ����Ҳ����һ��trans���
	osg::ref_ptr<osg::Group> dof_child_node = dof_root->getChild(num-1)->asGroup();
	osg::ref_ptr < osg::MatrixTransform> trans_merge = new osg::MatrixTransform;
	trans_merge->setMatrix(trans_scene_M);
	trans_merge->setName("cabinet_Trans");
	trans_merge->addChild(dof_child_node);
	dof_root->asGroup()->setChild(num - 1, trans_merge);

	scenegroup->setChild(i, tempgroup);
}
else if( name_suf == "swivel" )
{
	osg::ref_ptr<osg::Group> dof_root = tempgroup->asGroup()->getChild(0)->asGroup();
	int num = dof_root->getNumChildren();
	for (int j = 0; j < num - 1; j++)
	{
		// ����dof����
		osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(dof_root->getChild(j));
		osg::Vec3d ori_start = dofnode->getInversePutMatrix().getTrans();
		osg::Matrix inv_PutM = dofnode->getInversePutMatrix();
		osg::Vec3d ori_axies = osg::Vec3d(1, 0, 0) * inv_PutM - ori_start;
		osg::Vec3d new_start = ori_start * trans_scene_M;
		osg::Vec3d new_axies = ori_axies * trans_scene_M;
		osg::Matrix new_rot = osg::Matrix::rotate(osg::Vec3d(1, 0, 0), new_axies);
		osg::Matrix new_t = osg::Matrix::translate(new_start);
		osg::Matrix new_inv_pm = new_rot * new_t;
		dofnode->setInversePutMatrix(new_inv_pm);
		dofnode->setPutMatrix(osg::Matrix::inverse(new_inv_pm));
		// ÿ��dof_root��ÿ���ӽ���� ����һ��trans���
		osg::ref_ptr<osg::Group> dof_child_node = dofnode->getChild(0)->asGroup();
		osg::ref_ptr < osg::MatrixTransform> trans_merge = new osg::MatrixTransform;
		trans_merge->setMatrix(trans_scene_M);
		trans_merge->setName("cabinet_Trans");
		trans_merge->addChild(dof_child_node);
		dofnode->setChild(0, trans_merge);
	}
	//��ֹ����Ҳ����һ��trans���
	osg::ref_ptr<osg::Group> dof_child_node = dof_root->getChild(num - 1)->asGroup();
	osg::ref_ptr < osg::MatrixTransform> trans_merge = new osg::MatrixTransform;
	trans_merge->setMatrix(trans_scene_M);
	trans_merge->setName("cabinet_Trans");
	trans_merge->addChild(dof_child_node);
	dof_root->asGroup()->setChild(num - 1, trans_merge);

	scenegroup->setChild(i, tempgroup);
}*/
}