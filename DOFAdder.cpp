#include "DOFAdder.h"

//���캯��
DOFAdder::DOFAdder()
{}

//��������
DOFAdder::~DOFAdder()
{}

//������ȷ��ģ���е��˶�������ע�������µĴ�dof�ڵ��ģ��(���˴����š��Ű���)
void DOFAdder::generate_dofnode_added_model(std::string input_path,std::string output_path)
{
	//��ģ��
	osg::ref_ptr<osg::Node> Model=osgDB::readNodeFile(input_path);
	//�����˶�����������
	Map_SI map_si;
	unsigned int i=0;
	for(i=0;i<Model->asGroup()->getNumChildren();i++)
	{
		add_node_into_Map_SI(map_si,Model->asGroup()->getChild(i));
	}
	//�����˶�����������Ϊģ�����dof�ڵ�
	add_dofnodes_by_Map_SI(map_si,Model->asGroup());
	//������ģ��
	osgDB::Registry::instance()->writeNode(*Model,output_path,osgDB::Registry::instance()->getOptions());
}

//������ȷ��ģ���е��˶�������ע�������µĴ�dof�ڵ��ģ��(�����š��Ű���ר��)
void DOFAdder::generate_dofnode_added_model_WDK(std::string input_path,std::string output_path)
{
	//��ģ��
	osg::ref_ptr<osg::Node> Model=osgDB::readNodeFile(input_path);
	//�����˶�����������
	Map_SI map_si;
	unsigned int i=0;
	for(i=0;i<Model->asGroup()->getNumChildren();i++)
	{
		add_node_into_Map_SI(map_si,Model->asGroup()->getChild(i));
	}
	//�����˶�����������Ϊģ�����dof�ڵ�
	add_dofnodes_by_Map_SI_WDK(map_si,Model->asGroup());
	//������ģ��
	osgDB::Registry::instance()->writeNode(*Model,output_path,osgDB::Registry::instance()->getOptions());
}

//���һ�������İ�Χ����������
osg::Vec3f DOFAdder::get_center_of_part(osg::Node* node)
{
	osg::ComputeBoundsVisitor boundVisitor;
	node->accept(boundVisitor);
	osg::BoundingBox boundingBox = boundVisitor.getBoundingBox();
	return boundingBox.center();
}

//���˶������������м���һ���ڵ�
bool DOFAdder::add_node_into_Map_SI(Map_SI &map_si,osg::Node* node)
{
	std::string name=node->getName();
	std::string si=get_system_id(name);
	std::string ii=get_instance_id(name);
	std::string pi=get_part_id(name);
	Map_SI::iterator it_si;
	it_si=map_si.find(si);
	//���map_si��û�����si,����
	if(it_si==map_si.end())
	{
		Map_PI map_pi;
		map_pi.insert(std::pair<std::string,osg::Node*>(pi,node));
		Map_II map_ii;
		map_ii.insert(std::pair<std::string,Map_PI>(ii,map_pi));
		map_si.insert(std::pair<std::string,Map_II>(si,map_ii));
	}
	//���map_si�����si��
	else
	{
		Map_II::iterator it_ii;
		it_ii=it_si->second.find(ii);
		//���map_ii��û�����ii������
		if(it_ii==it_si->second.end())
		{
			Map_PI map_pi;
			map_pi.insert(std::pair<std::string,osg::Node*>(pi,node));
			it_si->second.insert(std::pair<std::string,Map_PI>(ii,map_pi));
		}
		//���map_ii�������ii:
		else
		{
			Map_PI::iterator it_pi;
			it_pi=it_ii->second.find(pi);
			//���map_pi��û�����pi�����
			if(it_pi==it_ii->second.end())
			{
				it_ii->second.insert(std::pair<std::string,osg::Node*>(pi,node));
			}
			//���map_pi�������pi��˵�����������˶�����������
			else
			{
				std::cout<<"�˶����������������"<<std::endl;
				error_message="�˶����������������\n";
				return false;
			}
		}
	}
	return true;
}

//����ȷ��ģ���У������˶�����������Ϊģ�����dof�ڵ�(���˴����š��Ű���)
void DOFAdder::add_dofnodes_by_Map_SI(Map_SI map_si,osg::Group* Model)
{
	//������dof�Ĳ�����Ϊ����û��ȷ���ȴ���
	Map_SI::iterator it_si;
	//����Map_SI
	for(it_si=map_si.begin();it_si!=map_si.end();it_si++)
	{
		//����Ƿ��˶�����(si=10)�Ļ�������
		if(it_si->first=="10")
			continue;
		Map_II::iterator it_ii;
		//����Map_SI��ÿ��MAP_II
		for(it_ii=it_si->second.begin();it_ii!=it_si->second.end();it_ii++)
		{
			Map_PI::iterator it_pi;
			//����Map_II��ÿ��MAP_PI
			osg::ref_ptr<osgSim::DOFTransform> last_dofnode;
			for(it_pi=it_ii->second.begin();it_pi!=it_ii->second.end();it_pi++)
			{
				//��һ���˶�����
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
					//���dof�ڵ㣬ֻ����һ�������ڵ�
					osg::ref_ptr<osg::Group> old_parent=it_pi->second->getParent(0);
					dofnode->addChild(it_pi->second);
					old_parent->removeChild(it_pi->second);
					old_parent->addChild(dofnode);
					last_dofnode=dofnode;
				}
				//�ڶ��ڼ��Ժ���˶�����
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
					//���dof�ڵ㣬����һ�������ڵ��һ��dof�ڵ�
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

//����ȷ��ģ���У������˶�����������Ϊģ�����dof�ڵ�(�����š��Ű���ר��)
void DOFAdder::add_dofnodes_by_Map_SI_WDK(Map_SI map_si,osg::Group* Model)
{
	//������dof�Ĳ�����Ϊ����û��ȷ���ȴ���
	Map_SI::iterator it_si;
	//����Map_SI
	for(it_si=map_si.begin();it_si!=map_si.end();it_si++)
	{
		//����Ƿ��˶�����(si=10)�Ļ�������
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
			//���Ų����ӵ�dof���棬ֻ����һ�������ڵ�
			osg::ref_ptr<osg::Group> old_parent=it_pi->second->getParent(0);
			door_dofnode->addChild(it_pi->second);
			old_parent->removeChild(it_pi->second);
			old_parent->addChild(door_dofnode);
		}
		//����Map_SI��ÿ��MAP_II
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
			//���dof�ڵ㣬ֻ����һ�������ڵ�
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

//�ж�һ�������Ƿ�����˶�������������ʽ
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

//����˶���������������ϵͳ��
std::string DOFAdder::get_system_id(std::string name)
{
	if(check_motion_part_name(name))
		return name.substr(0,2);
	else
		return "";
}

//����˶������Ĳ�������
std::string DOFAdder::get_part_id(std::string name)
{
	if(check_motion_part_name(name))
		return name.substr(0,3);
	else
		return "";
}

//����˶�������ʵ����
std::string DOFAdder::get_instance_id(std::string name)
{
	if(check_motion_part_name(name))
		return name.substr(4,3);
	else
		return "";
}
