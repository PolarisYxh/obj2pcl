#pragma once
#include "DOFCalculation.h"

#include <osgDB/ReadFile>
#include <osg/Group>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/ComputeBoundsVisitor>
#include <osgSim/DOFTransform>

#define DEGREE_PER_STEP 0.05f
#define DISTANCE_PER_STEP 0.01f

class AnimationTest :public osgGA::GUIEventHandler
{
public:
	AnimationTest();
    AnimationTest(osgSim::DOFTransform* node);
	~AnimationTest();
    void set_dof_node(osgSim::DOFTransform* node);

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* obj, osg::NodeVisitor* nodeVisitor);

private:
	void do_animation();
    Motion_type motion_type;

    osg::ref_ptr<osgSim::DOFTransform> dofnode;
};
	
