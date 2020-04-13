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
void  MyUtils::getFile(std::string path, std::string suffix, std::vector<std::string> & model_names)
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


Photographer::Photographer(){}
Photographer::~Photographer() {}

bool Photographer::is_dof_by_dynamic_cast(osg::Node* node)
{
	osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(node);
	if (dofnode == nullptr)
		return false;
	else
		return true;
}

void Photographer::getAllDOFNodesUnderNode(osg::Node* node, std::vector<osg::Group*>& node_vec)
{
	if (node == NULL)return;
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
			//temp_name=tempgroup->getChild(i)->getName();
			if (is_dof_by_dynamic_cast(tempgroup->getChild(i)))
			{
				node_vec.push_back(tempgroup->getChild(i)->asGroup());
				groupqueue.push_back(tempgroup->getChild(i)->asGroup()); //������if������break������������������

			}

		}
	}
}


int Photographer::Tool_test_1(std::string ive_path, std::string save_path, std::string dof_name, double ani_angle, double ani_count, double cam_H, double track_R, double photo_count)
{
	//�������ļ���
	std::string sub1 = save_path+ "pose";
	std::string sub2 = save_path  + "depth";
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

	//����dof�����ҽڵ����
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
	osg::Vec3 curvec = dofnode->getCurrentHPR();
	float ani_radians = osg::DegreesToRadians(ani_angle);
	for (i = 0; i < ani_count + 1; i++)
	{
		curvec.set(0, i * ani_radians, 0);  //����֤
		dofnode->setCurrentHPR(curvec);

		//��ʼ����ÿ�����
		for (int j = 0; j < photo_count; j++)
		{
			osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
			//viewer->getCamera()->setComputeNearFarMode( osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR );//���ø�ģʽ�� zNear zFar��ֵ����׼ȷ�ģ�������Ĭ���Զ�����Զ�������棬zNear��ʹ������Ҳ��Ч
			viewer->setSceneData(root.get());
			int pixelwidth = 1080, pixelheight = 1040;
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
			//osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();
			unsigned int  width = gc->getTraits()->width, height = gc->getTraits()->height;
			//wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
			viewer->realize();
			osg::Vec3d eyes = eye->at(j), center = osg::Vec3d(0.0, 0.0, 0.0), up = osg::Vec3d(0.0, 0.0, 1.0);
			viewer->getCamera()->setViewMatrixAsLookAt(eyes, center, up);
			//����ͼƬ�Ͳ���
			std::string output_path_p = save_path + "pose\\p_ani_" + std::to_string((long double)i) + "_photo_" + std::to_string((long double)j)+ ".txt";
			std::ofstream outfile(output_path_p);
			osg::ref_ptr<osg::Image> image_d = new osg::Image();
			std::string output_path_d = save_path + "depth\\d_ani_" + std::to_string((long double)i) +  "_photo_" + std::to_string((long double)j) + ".png";
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
			outfile.close();
			osgDB::writeImageFile(*(image_d.get()), output_path_d);
			osgDB::writeImageFile(*(image_rgb.get()), output_path_l);
		}
	}
	return 0;
}
