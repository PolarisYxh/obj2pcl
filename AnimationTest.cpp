#include "AnimationTest.h"

AnimationTest::AnimationTest()
{
    motion_type=NONE;
}

AnimationTest::AnimationTest(osgSim::DOFTransform* node)
{
    dofnode=node;
    std::string node_name=dofnode->getName();
    //根据后缀名确定运动类型
    if(node_name.length()>=2&&(node_name.substr(node_name.length()-2,2)=="_r"))
        motion_type=R;
    else if(node_name.length()>=2&&(node_name.substr(node_name.length()-2,2)=="_t"))
        motion_type=T;
    else if(node_name.length()>=3&&(node_name.substr(node_name.length()-3,3)=="_rt"))
        motion_type=RT;
    else
        motion_type=NONE;
}

AnimationTest::~AnimationTest(){}

void AnimationTest::set_dof_node(osgSim::DOFTransform* node)
{
	dofnode=node;
    std::string node_name=dofnode->getName();
    //根据后缀名确定运动类型
    if(node_name.length()>=2&&(node_name.substr(node_name.length()-2,2)=="_r"))
        motion_type=R;
    else if(node_name.length()>=2&&(node_name.substr(node_name.length()-2,2)=="_t"))
        motion_type=T;
    else if(node_name.length()>=3&&(node_name.substr(node_name.length()-3,3)=="_rt"))
        motion_type=RT;
    else
        motion_type=NONE;
}

bool AnimationTest::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* obj, osg::NodeVisitor* nodeVisitor)
{
	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	if (!viewer) 
		return false;

    do_animation();
	return false;
}

void AnimationTest::do_animation()
{
    osg::Vec3 curvec;
	switch (motion_type)
	{
    case R:
        curvec=dofnode->getCurrentHPR();
        curvec.set(curvec.x(),curvec.y()+DEGREE_PER_STEP,curvec.z());
        dofnode->setCurrentHPR(curvec);
		break;
    case T:
        curvec=dofnode->getCurrentTranslate();
        curvec.set(curvec.x()+DISTANCE_PER_STEP,curvec.y(),curvec.z());
        dofnode->setCurrentTranslate(curvec);
		break;
	case RT:
        curvec=dofnode->getCurrentHPR();
        curvec.set(curvec.x(),curvec.y()+DEGREE_PER_STEP,curvec.z());
        dofnode->setCurrentHPR(curvec);
        curvec=dofnode->getCurrentTranslate();
        curvec.set(curvec.x()+DISTANCE_PER_STEP,curvec.y(),curvec.z());
        dofnode->setCurrentTranslate(curvec);
		break;
	default:
		break;
	}
}
