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
	//��������Ĺ��캯��
    Triangle(osg::Vec3d v1,osg::Vec3d v2,osg::Vec3d v3);
    ~Triangle();
	//���ú���
	void set(osg::Vec3d v1,osg::Vec3d v2,osg::Vec3d v3);
	//���a
	osg::Vec3d get_a();
	//���b
	osg::Vec3d get_b();
	//���c
	osg::Vec3d get_c();
	//�����Ƭ�ĵ�λ�������������������������ab�������ac�ٳ��������ģ�õ���
	osg::Vec3d get_normal_vector();

	//����==����� 
    bool operator==(const Triangle &t) const
	{  
		return (a==t.a)&&(b==t.b)&&(c==t.c); 
	} 
	//����<����� 
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

	//������ķ�ʽ�޸�dof��p1��ԭ�㣬p2�Ǳ�ʾ����ĵ㣬�涨�����˶���r,t,rt�����˶��ᶼ��x��
	static void modify_dof_two_point(osgSim::DOFTransform* dofnode,osg::Vec3d p1,osg::Vec3d p2,Motion_type type);
	//��һ�����һ����ķ�ʽ�޸�dof��p1��ԭ�㣬��ķ����Ƿ���
    static void modify_dof_one_point_one_face(osgSim::DOFTransform* dofnode,osg::Vec3d p1,Triangle f1,Motion_type type);
private:
	//�����˶������޸�dof����
	static void modify_dof_name(osgSim::DOFTransform* dofnode,Motion_type type);
};
