#include "DOFCalculation.h"

Triangle::Triangle(){}

//��������Ĺ��캯��
Triangle::Triangle(osg::Vec3d v1,osg::Vec3d v2,osg::Vec3d v3)
{
	if(v1<v2)
	{
		if(v2<v3)
		{
			a=v1;
			b=v2;
			c=v3;
		}
		else if(v1<v3)
		{
			a=v1;
			b=v3;
			c=v2;
		}
		else
		{
			a=v3;
			b=v1;
			c=v2;
		}
	}
	else
	{
		if(v3<v2)
		{
			a=v3;
			b=v2;
			c=v1;
		}
		else if(v1<v3)
		{
			a=v2;
			b=v1;
			c=v3;
		}
		else
		{
			a=v2;
			b=v3;
			c=v1;
		}
	}
}

Triangle::~Triangle(){}

//���ú���
void Triangle::set(osg::Vec3d v1,osg::Vec3d v2,osg::Vec3d v3)
{
	if(v1<v2)
	{
		if(v2<v3)
		{
			a=v1;
			b=v2;
			c=v3;
		}
		else if(v1<v3)
		{
			a=v1;
			b=v3;
			c=v2;
		}
		else
		{
			a=v3;
			b=v1;
			c=v2;
		}
	}
	else
	{
		if(v3<v2)
		{
			a=v3;
			b=v2;
			c=v1;
		}
		else if(v1<v3)
		{
			a=v2;
			b=v1;
			c=v3;
		}
		else
		{
			a=v2;
			b=v3;
			c=v1;
		}
	}
}

//���a
osg::Vec3d Triangle::get_a()
{
	return a;
}

//���b
osg::Vec3d Triangle::get_b()
{
	return b;
}

//���c
osg::Vec3d Triangle::get_c()
{
	return c;
}

//�����Ƭ�ĵ�λ�������������������������ab�������ac�ٳ��������ģ�õ���
osg::Vec3d Triangle::get_normal_vector()
{
	osg::Vec3d ab=b-a;
	osg::Vec3d ac=c-a;
	osg::Vec3d normal_vector;
	normal_vector=ab^ac;
	normal_vector=normal_vector/normal_vector.length();
	return normal_vector;
}

DOFCalculation::DOFCalculation(){}

DOFCalculation::~DOFCalculation(){}

//������ķ�ʽ��dof��p1��ԭ�㣬p2�Ǳ�ʾ����ĵ㣬�涨�����˶���r,t,rt�����˶��ᶼ��x��
void DOFCalculation::modify_dof_two_point(osgSim::DOFTransform* dofnode,osg::Vec3d p1,osg::Vec3d p2,Motion_type type)
{
	//dof��x����·���
	osg::Vec3d new_x_axis=p2-p1;
	//�����µ�dof���þ��󣬲�����
	osg::Matrix rot,trans,inverse_put,put;
	rot=osg::Matrix::rotate(osg::Vec3d(1.0,0.0,0.0),new_x_axis);
	trans=osg::Matrix::translate(p1);
	inverse_put=rot*trans;
	put=osg::Matrix::inverse(inverse_put);
    dofnode->setInversePutMatrix(inverse_put);
    dofnode->setPutMatrix(put);
	//�޸�dof������
    modify_dof_name(dofnode,type);
}

//��һ�����һ����ķ�ʽ�޸�dof��p1��ԭ�㣬
void DOFCalculation::modify_dof_one_point_one_face(osgSim::DOFTransform* dofnode,osg::Vec3d p1,Triangle f1,Motion_type type)
{
	//��Ƭ�ķ�����
	osg::Vec3d normal_vector=f1.get_normal_vector();
	osg::Vec3d ap1=p1-f1.get_a();
	//dof��x����·���
	osg::Vec3d new_x_axis;
	if(ap1*normal_vector>0)
		new_x_axis=-normal_vector;
	else
		new_x_axis=normal_vector;
	//�����µ�dof���þ��󣬲�����
	osg::Matrix rot,trans,inverse_put,put;
	rot=osg::Matrix::rotate(osg::Vec3d(1.0,0.0,0.0),new_x_axis);
	trans=osg::Matrix::translate(p1);
	inverse_put=rot*trans;
	put=osg::Matrix::inverse(inverse_put);
    dofnode->setInversePutMatrix(inverse_put);
    dofnode->setPutMatrix(put);
    //�޸�dof������
    modify_dof_name(dofnode,type);
}

//�����˶������޸�dof����
void DOFCalculation::modify_dof_name(osgSim::DOFTransform* dofnode,Motion_type type)
{
	std::string suffix,old_name,new_name;
	switch (type)
	{
	case R:
        suffix="_r";
		break;
	case T:
        suffix="_t";
		break;
    case RT:
        suffix="_rt";
        break;
	default:
        suffix="";
		break;
	}
	old_name=dofnode->getName();
	//����Ѿ��к�׺�����޸�
	if(old_name.length()>=2&&(old_name.substr(old_name.length()-2,2)=="_r"))
		new_name=old_name.substr(0,old_name.length()-2)+suffix;
	else if(old_name.length()>=2&&(old_name.substr(old_name.length()-2,2)=="_t"))
		new_name=old_name.substr(0,old_name.length()-2)+suffix;
	else if(old_name.length()>=3&&(old_name.substr(old_name.length()-3,3)=="_rt"))
		new_name=old_name.substr(0,old_name.length()-3)+suffix;
	//���û�к�׺�������
	else
		new_name=old_name+suffix;
	dofnode->setName(new_name);
}
