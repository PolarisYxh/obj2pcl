#pragma once
#include <Windows.h>
#include<string>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/LightModel>
#include <osgGA/TrackballManipulator>

#include <osg/Material>

#include <osg/TextureCubeMap>
#include <osg/TexGen>
#include <osg/TexEnvCombine>
#include <osg/ShadeModel>
#include <osgSim/DOFTransform>
#include <osg/Multisample>
#include <osgUtil/ReflectionMapGenerator>
#include <osgUtil/HighlightMapGenerator>
#include <osgUtil/HalfWayMapGenerator>
#include <osg/ComputeBoundsVisitor>
#include <osg/Group>
#include <osg/Node>
#include <osg/Geometry>
#include <osg/PolygonMode>
#include <osg/LineWidth>
#include <osg/PolygonOffset>
#include <osgFX/Outline>
#include <osg/BlendColor>  
#include <osg/BlendFunc> 
#include <osg/Geode>  
#include <osg/Depth>  

#include <io.h>
#include <osg/LightModel>
#include <osg/Group>
#include <osgGA/TrackballManipulator>
#include <osg/Camera>
#include <osg/Image>
#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/TextureCubeMap>
#include <osg/TexGen>
#include <osg/TexEnvCombine>
#include <osg/DrawPixels>
#include <osg/Texture2D>
#include <osgUtil/HighlightMapGenerator>
#include <osgUtil/SceneView>
#include <stdlib.h> 
#include <iostream>
#include <string>
#include <vector>
#include <direct.h>
#include <random>

class Photographer
{
public:
	Photographer();
	~Photographer();

	//2020 4.4  工具接口函数
	int Tool_test_1(std::string ive_path, std::string save_path, std::string dof_name, double ani_angle, double ani_count, double cam_H, double track_R, double photo_count);
	int Tool_test_2(std::string ive_path, std::string save_path, std::string dof_name, double ani_angle, double ani_count, double cam_H, double track_R, double photo_count, std::string scene_name);
	int Tool_test_3(std::string ive_path, std::string save_path, std::string dof_name, double ani_angle, double ani_count, double cam_H, double track_R, double photo_count, std::string scene_name);
	int new_dataset(std::string ive_path, std::string save_path);
	bool is_dof_by_dynamic_cast(osg::Node* node);
	void getAllDOFNodesUnderNode(osg::Node* node, std::vector<osg::Group*>& node_vec);

	void getAllPartNodesUnderNode(osg::Node* node, std::vector<osg::Group*>& node_vec);
 
	osg::Matrix calculateTransformMatrix(osg::Vec3f a1, osg::Vec3f b1, osg::Vec3f c1, osg::Vec3f d1,
		osg::Vec3f a2, osg::Vec3f b2, osg::Vec3f c2, osg::Vec3f d2);
	void add_father_level(osg::Group* child, osg::Group* new_parent);
};

class NodeVisitor_dyer : public osg::NodeVisitor
{
public:
	NodeVisitor_dyer();
	NodeVisitor_dyer(osg::Vec4, bool);
	~NodeVisitor_dyer();

	virtual void apply(osg::Geode& geode);
	void set_color(osg::Vec4);
	osg::Vec4 get_color();

private:
	osg::Vec4 color;
	bool open_transparent;//是否开启半透明模式
};

class MyUtils
{
public:
	MyUtils();
	~MyUtils();

	//2020.4.7 返回一个文件夹中所有的文件
	static void  getFile(std::string path, std::string suffix, std::vector<std::string>& model_names);
};

