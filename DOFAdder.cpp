#include "DOFAdder.h"

//构造函数
DOFAdder::DOFAdder()
{}

//析构函数
DOFAdder::~DOFAdder()
{}

//根据正确的模型中的运动部件标注，生成新的带dof节点的模型(除了窗、门、门把手)
void DOFAdder::generate_dofnode_added_model(std::string input_path,std::string output_path)
{
	//打开模型
	osg::ref_ptr<osg::Node> Model=osgDB::readNodeFile(input_path);
	//构建运动部件名称树
	Map_SI map_si;
	unsigned int i=0;
	for(i=0;i<Model->asGroup()->getNumChildren();i++)
	{
		add_node_into_Map_SI(map_si,Model->asGroup()->getChild(i));
	}
	//根据运动部件名称树为模型添加dof节点
	add_dofnodes_by_Map_SI(map_si,Model->asGroup());
	//保存新模型
	osgDB::Registry::instance()->writeNode(*Model,output_path,osgDB::Registry::instance()->getOptions());
}

//根据正确的模型中的运动部件标注，生成新的带dof节点的模型(窗、门、门把手专用)
void DOFAdder::generate_dofnode_added_model_WDK(std::string input_path,std::string output_path)
{
	//打开模型
	osg::ref_ptr<osg::Node> Model=osgDB::readNodeFile(input_path);
	//构建运动部件名称树
	Map_SI map_si;
	unsigned int i=0;
	for(i=0;i<Model->asGroup()->getNumChildren();i++)
	{
		add_node_into_Map_SI(map_si,Model->asGroup()->getChild(i));
	}
	//根据运动部件名称树为模型添加dof节点
	add_dofnodes_by_Map_SI_WDK(map_si,Model->asGroup());
	//保存新模型
	osgDB::Registry::instance()->writeNode(*Model,output_path,osgDB::Registry::instance()->getOptions());
}

//获得一个部件的包围盒中心坐标
osg::Vec3f DOFAdder::get_center_of_part(osg::Node* node)
{
	osg::ComputeBoundsVisitor boundVisitor;
	node->accept(boundVisitor);
	osg::BoundingBox boundingBox = boundVisitor.getBoundingBox();
	return boundingBox.center();
}

//在运动部件名称树中加入一个节点
bool DOFAdder::add_node_into_Map_SI(Map_SI &map_si,osg::Node* node)
{
	std::string name=node->getName();
	std::string si=get_system_id(name);
	std::string ii=get_instance_id(name);
	std::string pi=get_part_id(name);
	Map_SI::iterator it_si;
	it_si=map_si.find(si);
	//如果map_si中没有这个si,创建
	if(it_si==map_si.end())
	{
		Map_PI map_pi;
		map_pi.insert(std::pair<std::string,osg::Node*>(pi,node));
		Map_II map_ii;
		map_ii.insert(std::pair<std::string,Map_PI>(ii,map_pi));
		map_si.insert(std::pair<std::string,Map_II>(si,map_ii));
	}
	//如果map_si中这个si：
	else
	{
		Map_II::iterator it_ii;
		it_ii=it_si->second.find(ii);
		//如果map_ii中没有这个ii，创建
		if(it_ii==it_si->second.end())
		{
			Map_PI map_pi;
			map_pi.insert(std::pair<std::string,osg::Node*>(pi,node));
			it_si->second.insert(std::pair<std::string,Map_PI>(ii,map_pi));
		}
		//如果map_ii中有这个ii:
		else
		{
			Map_PI::iterator it_pi;
			it_pi=it_ii->second.find(pi);
			//如果map_pi中没有这个pi，添加
			if(it_pi==it_ii->second.end())
			{
				it_ii->second.insert(std::pair<std::string,osg::Node*>(pi,node));
			}
			//如果map_pi中有这个pi，说明有重名的运动部件，错误
			else
			{
				std::cout<<"运动部件有重名的情况"<<std::endl;
				error_message="运动部件有重名的情况\n";
				return false;
			}
		}
	}
	return true;
}

//在正确的模型中，根据运动部件名称树为模型添加dof节点(除了窗、门、门把手)
void DOFAdder::add_dofnodes_by_Map_SI(Map_SI map_si,osg::Group* Model)
{
	//增加总dof的部分因为名称没有确定先待定
	Map_SI::iterator it_si;
	//遍历Map_SI
	for(it_si=map_si.begin();it_si!=map_si.end();it_si++)
	{
		//如果是非运动部件(si=10)的话，不动
		if(it_si->first=="10")
			continue;
		Map_II::iterator it_ii;
		//遍历Map_SI的每个MAP_II
		for(it_ii=it_si->second.begin();it_ii!=it_si->second.end();it_ii++)
		{
			Map_PI::iterator it_pi;
			//遍历Map_II的每个MAP_PI
			osg::ref_ptr<osgSim::DOFTransform> last_dofnode;
			for(it_pi=it_ii->second.begin();it_pi!=it_ii->second.end();it_pi++)
			{
				//第一节运动部件
				if(it_pi==it_ii->second.begin())
				{
					osg::ref_ptr<osgSim::DOFTransform> dofnode=new osgSim::DOFTransform();
					dofnode->setName("dof_"+it_pi->second->getName());
					dofnode->setCurrentScale(osg::Vec3(1.0,1.0,1.0));
					osg::Matrix inverse_put,put;
					inverse_put.setTrans(get_center_of_part(it_pi->second));
					put=osg::Matrix::inverse(inverse_put);
					dofnode->setInversePutMatrix(inverse_put);
					dofnode->setPutMatrix(put);
					//添加dof节点，只带上一个部件节点
					osg::ref_ptr<osg::Group> old_parent=it_pi->second->getParent(0);
					dofnode->addChild(it_pi->second);
					old_parent->removeChild(it_pi->second);
					old_parent->addChild(dofnode);
					last_dofnode=dofnode;
				}
				//第二节及以后的运动部件
				else
				{
					osg::ref_ptr<osgSim::DOFTransform> dofnode=new osgSim::DOFTransform();
					dofnode->setName("dof_"+it_pi->second->getName());
					dofnode->setCurrentScale(osg::Vec3(1.0,1.0,1.0));
					osg::Matrix inverse_put,put;
					inverse_put.setTrans(get_center_of_part(it_pi->second));
					put=osg::Matrix::inverse(inverse_put);
					dofnode->setInversePutMatrix(inverse_put);
					dofnode->setPutMatrix(put);
					//添加dof节点，带上一个部件节点和一个dof节点
					osg::ref_ptr<osg::Group> old_parent=it_pi->second->getParent(0);
					dofnode->addChild(it_pi->second);
					dofnode->addChild(last_dofnode);
					old_parent->removeChild(it_pi->second);
					old_parent->removeChild(last_dofnode);
					old_parent->addChild(dofnode);
					last_dofnode=dofnode;
				}
			}
		}
	}
}

//在正确的模型中，根据运动部件名称树为模型添加dof节点(窗、门、门把手专用)
void DOFAdder::add_dofnodes_by_Map_SI_WDK(Map_SI map_si,osg::Group* Model)
{
	//增加总dof的部分因为名称没有确定先待定
	Map_SI::iterator it_si;
	//遍历Map_SI
	for(it_si=map_si.begin();it_si!=map_si.end();it_si++)
	{
		//如果是非运动部件(si=10)的话，不动
		if(it_si->first=="10")
			continue;
		osg::ref_ptr<osgSim::DOFTransform> door_dofnode;
		Map_II::iterator it_ii;
		it_ii=it_si->second.begin();
		bool has_door=it_ii->second.size()==2;
		if(has_door)
		{
			Map_PI::iterator it_pi;
			it_pi=it_ii->second.begin();
			it_pi++;
			door_dofnode=new osgSim::DOFTransform();
			door_dofnode->setName("dof_"+it_pi->second->getName());
			door_dofnode->setCurrentScale(osg::Vec3(1.0,1.0,1.0));
			osg::Matrix inverse_put,put;
			inverse_put.setTrans(get_center_of_part(it_pi->second));
			put=osg::Matrix::inverse(inverse_put);
			door_dofnode->setInversePutMatrix(inverse_put);
			door_dofnode->setPutMatrix(put);
			//把门部件加到dof下面，只带上一个部件节点
			osg::ref_ptr<osg::Group> old_parent=it_pi->second->getParent(0);
			door_dofnode->addChild(it_pi->second);
			old_parent->removeChild(it_pi->second);
			old_parent->addChild(door_dofnode);
		}
		//遍历Map_SI的每个MAP_II
		for(it_ii=it_si->second.begin();it_ii!=it_si->second.end();it_ii++)
		{
			Map_PI::iterator it_pi;
			it_pi=it_ii->second.begin();
			osg::ref_ptr<osgSim::DOFTransform> knob_dofnode=new osgSim::DOFTransform();
			knob_dofnode->setName("dof_"+it_pi->second->getName());
			knob_dofnode->setCurrentScale(osg::Vec3(1.0,1.0,1.0));
			osg::Matrix inverse_put,put;
			inverse_put.setTrans(get_center_of_part(it_pi->second));
			put=osg::Matrix::inverse(inverse_put);
			knob_dofnode->setInversePutMatrix(inverse_put);
			knob_dofnode->setPutMatrix(put);
			//添加dof节点，只带上一个部件节点
			osg::ref_ptr<osg::Group> old_parent=it_pi->second->getParent(0);
			knob_dofnode->addChild(it_pi->second);
			old_parent->removeChild(it_pi->second);
			if(has_door)
				door_dofnode->addChild(knob_dofnode);
			else
				old_parent->addChild(knob_dofnode);
		}
	}
}

//判断一个名称是否符合运动部件的命名格式
bool DOFAdder::check_motion_part_name(std::string name)
{
	if(name.size()!=MOTION_PART_NAME_LENGTH)
		return false;
	else
	{
		int i=0;
		for(i=0;i<MOTION_PART_NAME_LENGTH;i++)
		{
			if(i==3&&name[i]!='_')
				return false;
			if(i!=3&&!isdigit(name[i]))
				return false;
		}
		return true;
	}
}

//获得运动部件的所在联动系统号
std::string DOFAdder::get_system_id(std::string name)
{
	if(check_motion_part_name(name))
		return name.substr(0,2);
	else
		return "";
}

//获得运动部件的部件类别号
std::string DOFAdder::get_part_id(std::string name)
{
	if(check_motion_part_name(name))
		return name.substr(0,3);
	else
		return "";
}

//获得运动部件的实例号
std::string DOFAdder::get_instance_id(std::string name)
{
	if(check_motion_part_name(name))
		return name.substr(4,3);
	else
		return "";
}
