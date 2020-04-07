#include "DOFCalculation.h"

Triangle::Triangle(){}

//用三个点的构造函数
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

//设置函数
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

//获得a
osg::Vec3d Triangle::get_a()
{
	return a;
}

//获得b
osg::Vec3d Triangle::get_b()
{
	return b;
}

//获得c
osg::Vec3d Triangle::get_c()
{
	return c;
}

//获得面片的单位法向量，这个法向量是由向量ab叉乘向量ac再除以自身的模得到的
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

//用两点的方式加dof，p1是原点，p2是表示方向的点，规定所有运动（r,t,rt）的运动轴都是x轴
void DOFCalculation::modify_dof_two_point(osgSim::DOFTransform* dofnode,osg::Vec3d p1,osg::Vec3d p2,Motion_type type)
{
	//dof的x轴的新方向
	osg::Vec3d new_x_axis=p2-p1;
	//计算新的dof放置矩阵，并设置
	osg::Matrix rot,trans,inverse_put,put;
	rot=osg::Matrix::rotate(osg::Vec3d(1.0,0.0,0.0),new_x_axis);
	trans=osg::Matrix::translate(p1);
	inverse_put=rot*trans;
	put=osg::Matrix::inverse(inverse_put);
    dofnode->setInversePutMatrix(inverse_put);
    dofnode->setPutMatrix(put);
	//修改dof的名称
    modify_dof_name(dofnode,type);
}

//用一个点和一个面的方式修改dof，p1是原点，
void DOFCalculation::modify_dof_one_point_one_face(osgSim::DOFTransform* dofnode,osg::Vec3d p1,Triangle f1,Motion_type type)
{
	//面片的法向量
	osg::Vec3d normal_vector=f1.get_normal_vector();
	osg::Vec3d ap1=p1-f1.get_a();
	//dof的x轴的新方向
	osg::Vec3d new_x_axis;
	if(ap1*normal_vector>0)
		new_x_axis=-normal_vector;
	else
		new_x_axis=normal_vector;
	//计算新的dof放置矩阵，并设置
	osg::Matrix rot,trans,inverse_put,put;
	rot=osg::Matrix::rotate(osg::Vec3d(1.0,0.0,0.0),new_x_axis);
	trans=osg::Matrix::translate(p1);
	inverse_put=rot*trans;
	put=osg::Matrix::inverse(inverse_put);
    dofnode->setInversePutMatrix(inverse_put);
    dofnode->setPutMatrix(put);
    //修改dof的名称
    modify_dof_name(dofnode,type);
}

//根据运动类型修改dof名称
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
	//如果已经有后缀名，修改
	if(old_name.length()>=2&&(old_name.substr(old_name.length()-2,2)=="_r"))
		new_name=old_name.substr(0,old_name.length()-2)+suffix;
	else if(old_name.length()>=2&&(old_name.substr(old_name.length()-2,2)=="_t"))
		new_name=old_name.substr(0,old_name.length()-2)+suffix;
	else if(old_name.length()>=3&&(old_name.substr(old_name.length()-3,3)=="_rt"))
		new_name=old_name.substr(0,old_name.length()-3)+suffix;
	//如果没有后缀名，添加
	else
		new_name=old_name+suffix;
	dofnode->setName(new_name);
}
