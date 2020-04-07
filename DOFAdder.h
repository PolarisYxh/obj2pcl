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

	//������ȷ��ģ���е��˶�������ע�������µĴ�dof�ڵ��ģ��(���˴����š��Ű���)
	void generate_dofnode_added_model(std::string input_path,std::string output_path);
	//������ȷ��ģ���е��˶�������ע�������µĴ�dof�ڵ��ģ��(�����š��Ű���ר��)
	void generate_dofnode_added_model_WDK(std::string input_path,std::string output_path);

private:
	//���˶������������м���һ���ڵ�
	bool add_node_into_Map_SI(Map_SI &map_si,osg::Node* node);
	//����ȷ��ģ���У������˶�����������Ϊģ�����dof�ڵ�(���˴����š��Ű���)
	void add_dofnodes_by_Map_SI(Map_SI map_si,osg::Group* Model);	
	//����ȷ��ģ���У������˶�����������Ϊģ�����dof�ڵ�(�����š��Ű���ר��)
	void add_dofnodes_by_Map_SI_WDK(Map_SI map_si,osg::Group* Model);	
	//�ж�һ�������Ƿ�����˶�������������ʽ
	bool check_motion_part_name(std::string name);
	//����˶���������������ϵͳ��
	std::string get_system_id(std::string name);
	//����˶������Ĳ�������
	std::string get_part_id(std::string name);
	//����˶�������ʵ����
	std::string get_instance_id(std::string name);
	//���һ�������İ�Χ����������
	osg::Vec3f get_center_of_part(osg::Node* node);
	//������Ϣ
	std::string error_message;
};