#pragma once

#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/MatrixTransform>
#include <osgSim/DOFTransform>
#include <osg/ComputeBoundsVisitor>
#include <osgDB/ReadFile>
#include <osgDB/Registry>
#include <osgDB/WriteFile>
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>
#include <osg/LineWidth>
#include <osg/Geometry>
#include <osg/Geode>
#include <fstream>

#define MOTION_PART_NAME_LENGTH 7
//part_id Node*
typedef std::map<std::string,osg::Node*> Map_PI;
//instance_id Map_PI
typedef std::map<std::string,Map_PI> Map_II;
//system_id Map_II
typedef std::map<std::string,Map_II> Map_SI;

class DOFAdder
{
public:
	DOFAdder();
	~DOFAdder();

	//根据正确的模型中的运动部件标注，生成新的带dof节点的模型(除了窗、门、门把手)
	void generate_dofnode_added_model(std::string input_path,std::string output_path);
	//根据正确的模型中的运动部件标注，生成新的带dof节点的模型(窗、门、门把手专用)
	void generate_dofnode_added_model_WDK(std::string input_path,std::string output_path);

private:
	//在运动部件名称树中加入一个节点
	bool add_node_into_Map_SI(Map_SI &map_si,osg::Node* node);
	//在正确的模型中，根据运动部件名称树为模型添加dof节点(除了窗、门、门把手)
	void add_dofnodes_by_Map_SI(Map_SI map_si,osg::Group* Model);	
	//在正确的模型中，根据运动部件名称树为模型添加dof节点(窗、门、门把手专用)
	void add_dofnodes_by_Map_SI_WDK(Map_SI map_si,osg::Group* Model);	
	//判断一个名称是否符合运动部件的命名格式
	bool check_motion_part_name(std::string name);
	//获得运动部件的所在联动系统号
	std::string get_system_id(std::string name);
	//获得运动部件的部件类别号
	std::string get_part_id(std::string name);
	//获得运动部件的实例号
	std::string get_instance_id(std::string name);
	//获得一个部件的包围盒中心坐标
	osg::Vec3f get_center_of_part(osg::Node* node);
	//错误信息
	std::string error_message;
};