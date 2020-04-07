#pragma once

#include <osg/NodeVisitor>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Material>
#include <osgSim/DOFTransform>

enum Motion_type
{
    NONE,
	R,
	T,
	RT
};

class Triangle
{
public:
    Triangle();
	//用三个点的构造函数
    Triangle(osg::Vec3d v1,osg::Vec3d v2,osg::Vec3d v3);
    ~Triangle();
	//设置函数
	void set(osg::Vec3d v1,osg::Vec3d v2,osg::Vec3d v3);
	//获得a
	osg::Vec3d get_a();
	//获得b
	osg::Vec3d get_b();
	//获得c
	osg::Vec3d get_c();
	//获得面片的单位法向量，这个法向量是由向量ab叉乘向量ac再除以自身的模得到的
	osg::Vec3d get_normal_vector();

	//重载==运算符 
    bool operator==(const Triangle &t) const
	{  
		return (a==t.a)&&(b==t.b)&&(c==t.c); 
	} 
	//重载<运算符 
    bool operator<(const Triangle &t) const
	{  
		return (a<t.a)||(a==t.a&&b<t.b)||(a==t.a&&b==t.b&&c<t.c); 
	}

private:
	osg::Vec3d a;
	osg::Vec3d b;
	osg::Vec3d c;
};

class DOFCalculation
{
public:
	DOFCalculation();
	~DOFCalculation();

	//用两点的方式修改dof，p1是原点，p2是表示方向的点，规定所有运动（r,t,rt）的运动轴都是x轴
	static void modify_dof_two_point(osgSim::DOFTransform* dofnode,osg::Vec3d p1,osg::Vec3d p2,Motion_type type);
	//用一个点和一个面的方式修改dof，p1是原点，面的法线是方向
    static void modify_dof_one_point_one_face(osgSim::DOFTransform* dofnode,osg::Vec3d p1,Triangle f1,Motion_type type);
private:
	//根据运动类型修改dof名称
	static void modify_dof_name(osgSim::DOFTransform* dofnode,Motion_type type);
};
