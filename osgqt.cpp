#include "osgqt.h"
#include "ui_osgqt.h"
#include <QMainWindow>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osg/PrimitiveSet>
#include <osg/PrimitiveRestartIndex>
#include <qtosgview.h>
#include <osgGA/TrackballManipulator>
#include <osgFX/Scribe>
#include <osgUtil/LineSegmentIntersector>
#include <osg/MatrixTransform>
#include <structures.h>
#include <vector>
#include <osgManipulator/TabBoxDragger>
#include <osgManipulator/CommandManager>
#include <osgManipulator/Selection>
#include <osg/Matrix>
#include <osg/Geode>
#include <osg/Matrixd>
#include <osg/PositionAttitudeTransform>
#include <QTimer>
#include <QDir>
#include <QFileDialog>
#include <string>
#include <osg/CopyOp>
#include <fstream>
#include <osg/ShapeDrawable>
#include <osg/LineWidth>
#include <osg/LightModel>
#include <osgUtil/Optimizer>
#include <osgUtil/Simplifier>
#include <osg/PolygonMode>
#include <osg/Shape>
#include <osg/Geometry>
#include <osg/Node>
#include <iostream>
#include <QString>
#include <osg/ComputeBoundsVisitor>
#include <QMouseEvent>
#include <QPainter>
#include <QMimeData>
#include <QDrag>
#include <QDebug>
#include "mytreewidget.h"
#include "mytreewidget2.h"
#include <osgSim/DOFTransform>
#include "DOFCalculation.h"
#include "AnimationTest.h"
#include "DOFAdder.h"

using namespace std;

//注释：motion_part_标注采用的全局变量
std::vector<osg::ref_ptr<osg::Node>> all_node(5000);  //注释：model及其子节点的名字
struct motionif
{
	double motioninfo[4];//012对应xyz运动范围，3对应次数
};
QMap<osg::ref_ptr<osg::Node>, motionif> motion;

std::map<string,int> node_to_int; //注释：用于确认父节点是第几号结点
std::vector<std::vector<osg::ref_ptr<osg::Node>>> model_structure_tree(10000); //注释：树形图记录模型下各节点到根节点model的层数，model的下一层节点存在0索引位置
QTreeWidgetItem* root_tree; //注释：树形图widget

bool flag_open = false; //注释： 打开文件初始化是否完成
std::vector<string> model_components; //注释：是否是可绘制Geode

std::map<osg::ref_ptr<osg::Node>,bool> is_model_componets; //注释：是否是可绘制Geode,用于加载模型
//QMap<osg::ref_ptr<osg::Node>, bool> is_dof_node;//注释：是否是可绘制Geode，用于右键点选部件
const float scale_all = 4; //注释： 由于显示问题，所有模型加载时统一放大四倍，结构为root->trans(放大4倍）->model
std::string current_tree_item_name =""; //注释： 用于在树形图中高亮当前点选的部件
osg::ref_ptr<osg::StateSet> init_state_set; //注释： 初始的渲染stateset
osg::ref_ptr<osg::StateSet> model_state_set; //注释： model原本的stateset
//std::map<string,bool> map_to_choose; //注释： 用于标记每个部件是否被点选

QMap<QTreeWidgetItem*, osg::ref_ptr<osg::Node> > item_node;//树形图每个栏目对应的root下节点
QMap<osg::ref_ptr<osg::Geometry>, osg::ref_ptr<osg::Array>> geo_color;//记录改变叶节点颜色前的颜色
QMap<osg::ref_ptr<osg::Geometry>, osg::ref_ptr<osg::StateSet>> geo_state;//记录改变叶节点geometry前的状态
QList<QTreeWidgetItem*> selectedItemList;
QVector<osg::ref_ptr<osg::Node>> lastselectedNode;

bool mouse_is_picked = false;//注释： 用于判断鼠标是否在被点击
bool mouse_is_picked_empty = false; //注释： 用于判断鼠标是否点击空白出
bool tree_widget_is_picked = false; //注释： 用于判断是否点击了tree_widget
bool tree_widget_is_picked_empyt = false;//注释： 用于判断是否点击了tree_widget 空白
std::string box_infomation = ""; //注释： 用于显示文件信息
int tab_idx = 0; //注释： 确定该tab的值
std::map<string,bool> child_map; //注释： 用于在attach时防止加入到子节点上
std::vector<string> final_attach_node_name;
//std::map<string,bool> is_dof_node1;//如果是运动节点，则存入



//注释： 新加功能所需全局变量
mytreewidget *treewidget; //注释： 树形图类
osg::ref_ptr<osgGA::TrackballManipulator> manipulator; //注释：视图控制器
bool box_is_picked = false; //注释：判断是否box_pick
bool ctrl_is_picked2 = false;//注释： 用于控制不按下ctrl时box——pick不运行
//std::map<string,int> map_node_to_int; //用于查找node
//std::vector<osg::ref_ptr<osg::Node>> all_node_vector; //所有node的集合



////dof add 所需全局变量
//mytreewidget2 *treewidget2;
//std::vector<string> all_node_name_2;
//std::vector<std::vector<string>> model_structure_tree_2(30000);
//std::map<string,bool> is_model_componets_2;
//std::map<string,bool> map_to_choose_2;
//osg::ref_ptr<osg::StateSet> init_state_set_2; //注释： 初始的渲染stateset
//osg::ref_ptr<osg::StateSet> model_state_set_2; //注释： model原本的stateset
//QTreeWidgetItem* root_tree_2; //注释：树形图widget
//bool tree_widget_is_picked_empyt_2 = false;//注释： 用于判断是否点击了tree_widget 空白
//std::string current_file_name2 = "";
//int dof_type = 0;
//int choose_dof_mode = 0;
//int dof_pick2_mode = 0;
//osg::Vec3d Two_point_coor_1(0,0,0);
//osg::Vec3d Two_point_coor_2(0,0,0);
//osg::Vec3d one_point_coor(0,0,0);
//bool Two_point_is_picked_1 = false;
//bool Two_point_is_picked_2 = false;
//bool one_point_is_picked_1 = false;
//bool one_point_is_picked_2 = false;
//osg::ref_ptr<osg::Vec3Array> facepointarray;
//std::vector<string> dof_done_name;
//bool ok_is_clicked = false;
//std::map<string,osg::Vec3> dof_HPR;
//std::map<string,osg::Vec3> dof_trans;
//std::map<string,bool> is_dof_node2;




void Transform_Point(double out[4],const double m[16], const double in[4]){
#define M(row,col) m[col*4+row]
    out[0] =M(0,0)*in[0] + M(0,1)*in[1] + M(0,2)*in[2] + M(0,3) * in[3];
    out[1] =M(1,0)*in[0] + M(1,1)*in[1] + M(1,2)*in[2] + M(1,3) * in[3];
    out[2] =M(2,0)*in[0] + M(2,1)*in[1] + M(2,2)*in[2] + M(2,3) * in[3];
    out[3] =M(3,0)*in[0] + M(3,1)*in[1] + M(3,2)*in[2] + M(3,3) * in[3];
#undef M
}


osg::Vec3 WorldToScreen(osgViewer::View *view,osg::Vec3 worldpoint){
    double in[4],out[4];
    in[0] = worldpoint._v[0];
    in[1] = worldpoint._v[1];
    in[2] = worldpoint._v[2];
    in[3] = 1.0;
    osg::Matrix projectMatrix = view->getCamera()->getProjectionMatrix();
    osg::Matrix viewprojectMatrix = view->getCamera()->getViewMatrix();
    double modelViewMatrix[16];
    memcpy(modelViewMatrix,viewprojectMatrix.ptr(),sizeof(GLdouble)*16);
    Transform_Point(out,modelViewMatrix,in);
    double myprojectMatrix[16];
    memcpy(myprojectMatrix,projectMatrix.ptr(),sizeof(GLdouble)*16);
    Transform_Point(in,myprojectMatrix,out);
    if(in[3] == 0.0)
        return osg::Vec3d(0,0,0);
    in[0]/=in[3];
    in[1]/=in[3];
    in[2]/=in[3];

    int viewPort[4];
    osg::Viewport* myviewPort = view->getCamera()->getViewport();
    viewPort[0] = myviewPort->x();
    viewPort[1] = myviewPort->y();
    viewPort[2] = myviewPort->width();
    viewPort[3] = myviewPort->height();
    osg::Vec3d sceenPoint;
    sceenPoint._v[0] = (int)(viewPort[0]+(1+in[0])*viewPort[2]/2+0.5);
    sceenPoint._v[1] = (int)(viewPort[1]+(1+in[1])*viewPort[3]/2+0.5);
    sceenPoint._v[2] = 0;
    return sceenPoint;
}




class BoxPicker : public osgGA::GUIEventHandler//用于左键控制旋转，中键控制移动,ctrl+左键框选
{
public:
    BoxPicker()
    {
        x = 0.0f;
        y = 0.0f;
        OK = false;
    }

    osg::observer_ptr<osg::Geometry> geometry;

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
    {

        //if(ea.getButton()!=ea.LEFT_MOUSE_BUTTON)
        //     return false;
       bool doit = false;

        osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
        if (!viewer)
        {
            return false;
        }
        //int aaaa = ea.getButton();
        if (ea.getEventType() == osgGA::GUIEventAdapter::PUSH)
        {

            if(ea.getButton()==ea.LEFT_MOUSE_BUTTON&&ctrl_is_picked2){
                x = ea.getXnormalized();
                y = ea.getYnormalized();

                x_pick = ea.getX();
                y_pick = ea.getY();

                OK = true;
            }

        }

        if (ea.getEventType() == osgGA::GUIEventAdapter::DRAG)
        {
            //if(left_button_picked){
            if (OK)
            {
                float end_x = ea.getXnormalized();
                float end_y = ea.getYnormalized();

                if (geometry.valid())
                {
                    osg::Vec3Array* vertex = new osg::Vec3Array(4);
                    (*vertex)[0] = osg::Vec3(x, 0, y);
                    (*vertex)[1] = osg::Vec3(x, 0, end_y);
                    (*vertex)[2] = osg::Vec3(end_x, 0, end_y);
                    (*vertex)[3] = osg::Vec3(end_x, 0, y);
                    geometry->setVertexArray(vertex);
                    geometry->dirtyDisplayList();

                    //std::cout<<x<<" "<<y<<" "<<end_x<<" "<<end_y<<std::endl;
                }
                if (ea.getModKeyMask()&osgGA::GUIEventAdapter::MODKEY_LEFT_SHIFT)
                {
                    doit = true;
                }
            }
            //}
        }

        if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE)//ctrl+左键框选触发
        {
            if(ea.getButton()==ea.LEFT_MOUSE_BUTTON&&ctrl_is_picked2){

                std::map<string,bool> flag;
                flag.clear();
                for(int i = 0;i<model_components.size();i++){
                    flag[model_components.at(i)] = false;
                }



                OK = false;
                float pick_x = ea.getX();
                float pick_y = ea.getY();

                float xMin, xMax, yMin, yMax;

                xMin = osg::minimum(x_pick, pick_x);
                xMax = osg::maximum(x_pick, pick_x);
                yMin = osg::minimum(y_pick, pick_y);
                yMax = osg::maximum(y_pick, pick_y);

                osg::ref_ptr<osgUtil::PolytopeIntersector> intersector =
                        new osgUtil::PolytopeIntersector(osgUtil::Intersector::WINDOW, xMin, yMin, xMax, yMax);

                osgUtil::IntersectionVisitor iv(intersector.get());
                viewer->getCamera()->accept(iv);
                osg::Matrix VPM = viewer->getCamera()->getViewMatrix() *
                        viewer->getCamera()->getProjectionMatrix() *
                        viewer->getCamera()->getViewport()->computeWindowMatrix();
                if (intersector->containsIntersections())
                {

                    //std::cout << "OK" << std::endl;
                    for(osgUtil::PolytopeIntersector::Intersections::iterator
                        hitr = intersector->getIntersections().begin();
                        hitr != intersector->getIntersections().end();
                        ++hitr)
                    {

                        osg::NodePath np = hitr->nodePath;
                        //std::cout<<"*************************************************"<<endl;
                        for (osg::NodePath::iterator itr = np.end()-2; itr != np.begin(); itr--)
                        {
                            if ((*itr)->getName().empty()){
                                break;
                            }
                            //{
                            string temp_name = (*itr)->getName();
                            if(!flag[temp_name]&&is_model_componets[*itr]){
                                flag[temp_name] = true;
                                osg::Vec3 bbb = (*itr)->getBound().center()* osg::computeLocalToWorld((*itr)->getParentalNodePaths()[0]) * VPM;
                                //osg::Vec3d ccc = WorldToScreen(viewer,(*itr)->getBound().center() * osg::computeLocalToWorld((*itr)->getParentalNodePaths()[0]));
                                //if(ccc.x()>xMin&&ccc.x()<xMax&&ccc.y()>yMin&&ccc.y()<yMax)
								if (bbb.x() > xMin&& bbb.x() < xMax&& bbb.y() > yMin&& bbb.y() < yMax)
								{
									QMap<QTreeWidgetItem*, osg::ref_ptr<osg::Node>>::iterator iter = item_node.begin();
									for (; iter != item_node.end(); iter++)
									{
										if (iter.value() == *itr)
										{
											selectedItemList.append(iter.key());
										}
									}
								}
                                    //map_to_choose[temp_name] = true;
                                break;
                            }
                            //}
                        }
                    }
                }

                if (geometry.valid())
                {
                    osg::Vec3Array* vertex = new osg::Vec3Array(4);
                    (*vertex)[0] = osg::Vec3(0, 0, 0);
                    (*vertex)[1] = osg::Vec3(0, 0, 0);
                    (*vertex)[2] = osg::Vec3(0, 0, 0);
                    (*vertex)[3] = osg::Vec3(0, 0, 0);
                    geometry->setVertexArray(vertex);
                    geometry->dirtyDisplayList();
                }

                box_is_picked = true;
            }
        }

		return doit;
    }

    float x, y;
    float x_pick, y_pick;

    bool OK;
};




/*class Open_all_node_mask1 : public osg::NodeVisitor{
    /* 注释：class InfoVisitor 主要用于更新树形结构图以及获取可绘制的Geode
   infovisitor -> accept(model) model为加载的模型
   更新全局变量all_node_name，model_structure_tree，model_components
*
public:
    Open_all_node_mask1(): osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), _indent(0){}	//向子节点遍历

    virtual void apply( osg::Node &node){									//获取访问节点对象，并执行自定义节点操作
        //当访问到默认的Node节点时，使用该函数打印节点的名称
        //并根据节点层次适当使用_indent缩进
        //        for ( int i = 0; i < _indent; ++i)
        //        {
        //            std::cout<< " ";
        //        }
        //        std::cout<< "["<< _indent + 1<< "]"<< node.libraryName()<< "::"<< node.className()<< std::endl;
        node.setNodeMask(1);
        osg::ref_ptr<osg::StateSet> now_state = new osg::StateSet;
        now_state = model_state_set.get();
        node.setStateSet(now_state);
        //cout<<"_indent in: "<<_indent<<endl;
        _indent++;
        traverse( node);													//向下一个需要的节点推进
        _indent--;
        //cout<<"_indent out: "<<_indent<<endl;
    }

    //访问Geode节点
    virtual void apply( osg::Geode& node){
        //        for ( int i = 0; i < _indent; ++i)
        //        {
        //            std::cout<< " ";
        //        }
        //        std::cout<< "["<< _indent + 1<<"]"<< node.libraryName()<< "::"<< node.className()<< "::"<<node.getName()<< std::endl;
        node.setNodeMask(1);


        //        osg::ref_ptr<osgFX::Scribe> scribe1 = new osgFX::Scribe;
        //        node.getParent(0)->getParent(0)->addChild(scribe1);
        //        node.getParent(0)->getParent(0)->removeChild(0, 1);

        //        for (unsigned int n = 0; n < node.getNumDrawables(); ++n)
        //        {
        //            osg::Drawable* drawable = node.getDrawable(n);

        //            if(!drawable)	continue;
        //        }

        _indent ++;
        traverse( node);
        _indent --;
    }

protected:
    int _indent;
};*/






/*class Open_all_node_mask : public osg::NodeVisitor{
    /* 注释：class InfoVisitor 主要用于更新树形结构图以及获取可绘制的Geode
   infovisitor -> accept(model) model为加载的模型
   更新全局变量all_node_name，model_structure_tree，model_components
*
public:
    Open_all_node_mask(): osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), _indent(0){}	//向子节点遍历

    virtual void apply( osg::Node &node){									//获取访问节点对象，并执行自定义节点操作
        //当访问到默认的Node节点时，使用该函数打印节点的名称
        //并根据节点层次适当使用_indent缩进
        //        for ( int i = 0; i < _indent; ++i)
        //        {
        //            std::cout<< " ";
        //        }
        //        std::cout<< "["<< _indent + 1<< "]"<< node.libraryName()<< "::"<< node.className()<< std::endl;
        osg::ref_ptr<osgSim::DOFTransform> dof = dynamic_cast<osgSim::DOFTransform*>(&node);
        if(dof!=NULL){
            osg::Vec3 hpr = dof_HPR[node.getName()];
            osg::Vec3 trans = dof_trans[node.getName()];
            dof->setCurrentHPR(hpr);
            dof->setCurrentTranslate(trans);
        }

        node.setNodeMask(1);
        osg::ref_ptr<osg::StateSet> now_state = new osg::StateSet;
        now_state = model_state_set.get();
        node.setStateSet(now_state);
        //cout<<"_indent in: "<<_indent<<endl;
        _indent++;
        traverse( node);													//向下一个需要的节点推进
        _indent--;
        //cout<<"_indent out: "<<_indent<<endl;
    }

    //访问Geode节点
    virtual void apply( osg::Geode& node){
        //        for ( int i = 0; i < _indent; ++i)
        //        {
        //            std::cout<< " ";
        //        }
        //        std::cout<< "["<< _indent + 1<<"]"<< node.libraryName()<< "::"<< node.className()<< "::"<<node.getName()<< std::endl;
        node.setNodeMask(1);


        //        osg::ref_ptr<osgFX::Scribe> scribe1 = new osgFX::Scribe;
        //        node.getParent(0)->getParent(0)->addChild(scribe1);
        //        node.getParent(0)->getParent(0)->removeChild(0, 1);

        //        for (unsigned int n = 0; n < node.getNumDrawables(); ++n)
        //        {
        //            osg::Drawable* drawable = node.getDrawable(n);

        //            if(!drawable)	continue;
        //        }

        _indent ++;
        traverse( node);
        _indent --;
    }

protected:
    int _indent;
};*/

class InfoVisitor : public osg::NodeVisitor{//自定义model节点访问器
    /* 注释：class InfoVisitor 主要用于更新树形结构图以及获取可绘制的Geode
   model -> accept(infovisitor) model为加载的模型
   更新全局变量all_node_name，model_structure_tree，model_components,is_dof_node1
*/
public:
    InfoVisitor(): osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), _indent(0){}	//向子节点遍历

    virtual void apply( osg::Node &node){									//获取访问节点对象，并执行自定义节点操作
        //当访问到默认的Node节点时，使用该函数打印节点的名称
        //并根据节点层次适当使用_indent缩进
        //        for ( int i = 0; i < _indent; ++i)
        //        {
        //            std::cout<< " ";
        //        }
        //        std::cout<< "["<< _indent + 1<< "]"<< node.libraryName()<< "::"<< node.className()<< std::endl;
        if(node.getName()!="")
		{
			if (all_node.size() > 4900&&!reserve1)
			{
				all_node.reserve(10000); 
				model_structure_tree.resize(10000);
				reserve1 = true;
			}
			if (all_node.size() >= 8000 && !reserve2)
			{
				all_node.reserve(15000);
				model_structure_tree.resize(15000);
				reserve2 = true;
			}
            all_node.push_back(&node);
            //all_node_vector.push_back(&node);
            //map_node_to_int[node.getName()] = all_node_vector.size()-1;
            //cout<<"print current_node_name: "<<node.getName()<<endl;
            if(_indent!=0){
                string father_name = node.getParent(0)->getName();
                //cout<<"father_name: "<<father_name<<endl;
                //cout<<"print all_node_name_size: "<<all_node_name.size()<<endl;
                //for(int i=0 ;i<all_node_name.size();i++)
                //    cout<<all_node_name[i]<<" ";
                //cout<<"---------------"<<endl;

                vector<osg::ref_ptr<osg::Node>>::iterator ite1 = find(all_node.begin(), all_node.end(), node.getParent(0)->asNode());
                int index1 = std::distance(std::begin(all_node), ite1);
                model_structure_tree[index1].push_back(&node);
                //cout<<"*********************"<<endl;
            }
        }
        //cout<<"_indent in: "<<_indent<<endl;
        _indent++;
        traverse( node);													//向下一个需要的节点推进
        _indent--;
        //cout<<"_indent out: "<<_indent<<endl;
    }

    //访问Geode节点
    virtual void apply( osg::Geode& node)
	{
        is_model_componets[&node] = true;
        _indent ++;
        traverse( node);
        _indent --;
    }

protected:
    int _indent;
	bool reserve1 = false;
	bool reserve2 = false;
};


//class InfoVisitor2 : public osg::NodeVisitor{
//    /* 注释：class InfoVisitor 主要用于dof adder更新树形结构图以及获取可绘制的Geode
//   model -> accept(infovisitor) model为加载的模型
//   更新全局变量all_node_name，model_structure_tree，model_components
//*/
//public:
//    InfoVisitor2(): osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), _indent(0){}	//向子节点遍历
//
//    virtual void apply( osg::Node &node){									//获取访问节点对象，并执行自定义节点操作
//        //当访问到默认的Node节点时，使用该函数打印节点的名称
//        //并根据节点层次适当使用_indent缩进
//        //        for ( int i = 0; i < _indent; ++i)
//        //        {
//        //            std::cout<< " ";
//        //        }
//        //        std::cout<< "["<< _indent + 1<< "]"<< node.libraryName()<< "::"<< node.className()<< std::endl;
//        if(node.getName()!=""){
//            all_node_name_2.push_back(node.getName());
//            osg::ref_ptr<osgSim::DOFTransform> dof = dynamic_cast<osgSim::DOFTransform*>(&node);
//            if(dof!=NULL){
//                osg::Vec3 hpr = dof->getCurrentHPR();
//                osg::Vec3 trans = dof->getCurrentTranslate();
//                dof_HPR[node.getName()] = hpr;
//                dof_trans[node.getName()] = trans;
//                is_dof_node2[node.getName()] = true;
//            }
//            //all_node_vector.push_back(&node);
//            //map_node_to_int[node.getName()] = all_node_vector.size()-1;
//            //cout<<"print current_node_name: "<<node.getName()<<endl;
//            if(_indent!=0){
//                string father_name = node.getParent(0)->getName();
//                //cout<<"father_name: "<<father_name<<endl;
//                //cout<<"print all_node_name_size: "<<all_node_name.size()<<endl;
//                //for(int i=0 ;i<all_node_name.size();i++)
//                //    cout<<all_node_name[i]<<" ";
//                //cout<<"---------------"<<endl;
//                vector<string>::iterator ite1 = find(all_node_name_2.begin(), all_node_name_2.end(), father_name);
//                int index1 = std::distance(std::begin(all_node_name_2), ite1);
//                model_structure_tree_2[index1].push_back(node.getName());
//                //cout<<"*********************"<<endl;
//            }
//        }
//        //cout<<"_indent in: "<<_indent<<endl;
//        _indent++;
//        traverse( node);													//向下一个需要的节点推进
//        _indent--;
//        //cout<<"_indent out: "<<_indent<<endl;
//    }
//
//    //访问Geode节点
//    virtual void apply( osg::Geode& node){
//        //        for ( int i = 0; i < _indent; ++i)
//        //        {
//        //            std::cout<< " ";
//        //        }
//        //        std::cout<< "["<< _indent + 1<<"]"<< node.libraryName()<< "::"<< node.className()<< "::"<<node.getName()<< std::endl;
//        vector<string>::iterator iter;
//
//        string geode_name =node.getParent(0)->getName();
//        is_model_componets_2[geode_name] = true;
//
//
//        //        osg::ref_ptr<osgFX::Scribe> scribe1 = new osgFX::Scribe;
//        //        node.getParent(0)->getParent(0)->addChild(scribe1);
//        //        node.getParent(0)->getParent(0)->removeChild(0, 1);
//
//        //        for (unsigned int n = 0; n < node.getNumDrawables(); ++n)
//        //        {
//        //            osg::Drawable* drawable = node.getDrawable(n);
//
//        //            if(!drawable)	continue;
//        //        }
//
//        _indent ++;
//        traverse( node);
//        _indent --;
//    }
//
//protected:
//    int _indent;
//};





class findGeoNamedNode :public osg::NodeVisitor{
    /* 注释：class findGeoNamedNode 主要用于查找指定名字的结点
 * 返回具有指定名字的结点
 */
public:
    findGeoNamedNode(const std::string name) : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN){
        resultNode = NULL;
        this->name = name;
    }

    virtual void apply(osg::Node &searchNode){
        if (searchNode.getName() == name){
            osg::Node* dynamicTry = dynamic_cast<osg::Node*>(&searchNode);
            if (dynamicTry){
                resultNode = dynamicTry;
            }
        }
        traverse(searchNode);
    }

    osg::Node* getNode(){
        return resultNode;
    }

private:
    osg::Node* resultNode;
    std::string name;
};
class findDofNamedNode :public osg::NodeVisitor {
	/* 注释：class findGeoNamedNode 主要用于查找指定名字的结点
 * 返回具有指定名字的结点
 */
public:
	findDofNamedNode(const std::string name) : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN) {
		resultNode = NULL;
		this->name = name;
		mode = 0;
	}
	findDofNamedNode() :osg::NodeVisitor(TRAVERSE_ALL_CHILDREN)
	{
		resultNode = NULL;
		mode = 1;
	}
	virtual void apply(osg::Node& searchNode) {
		if (mode == 0)
		{
			if (searchNode.getName() == name) {
				osgSim::DOFTransform* dynamicTry = dynamic_cast<osgSim::DOFTransform*>(&searchNode);
				if (dynamicTry) {
					resultNode = dynamicTry;
				}
			}
			traverse(searchNode);
		}
		else if (mode == 1)
		{
			osgSim::DOFTransform* dynamicTry = dynamic_cast<osgSim::DOFTransform*>(&searchNode);
			std::string childname = searchNode.getName();
			if (dynamicTry)
			{
				if (childname.length() >= 2 && (childname.substr(childname.length() - 2, 2) == "_r"))//添加旋转运动信息
				{
					motionif x;
					x.motioninfo[0] = 0;
					x.motioninfo[1] = 90;
					x.motioninfo[2] = 0;
					x.motioninfo[3] = 5;
					motion.insert(&searchNode, x);
				}
				else if (childname.length() >= 2 && (childname.substr(childname.length() - 2, 2) == "_t"))
				{
					motionif x;
					x.motioninfo[0] = 0.1;
					x.motioninfo[1] = 0;
					x.motioninfo[2] = 0;
					x.motioninfo[3] = 5;
					motion.insert(&searchNode, x);
				}
				else if (childname.length() >= 3 && (childname.substr(childname.length() - 3, 3) == "_rt"))
				{
					motionif x;
					x.motioninfo[0] = 0.1;
					x.motioninfo[1] = 90;
					x.motioninfo[2] = 0;
					x.motioninfo[3] = 5;
					motion.insert(&searchNode, x);
				}
			}
			traverse(searchNode);
		}
	}

	osgSim::DOFTransform* getNode() {
		return resultNode;
	}
private:
	osgSim::DOFTransform* resultNode;
	std::string name;
	int mode;
};

class NodeVisitor_dyer : public osg::NodeVisitor//用于每帧设置模型的颜色，透明度及光照设置等
{
public:
	NodeVisitor_dyer() :osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{
		open_transparent = true;
	}
	NodeVisitor_dyer(osg::Vec4 given_color, bool if_open) : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)//用于拍depth，调整颜色
	{
		color = given_color;
		open_transparent = if_open;
		diffuse = osg::Vec4(0.75f, 0.75f, 0.75f, 0.5f);
		ambient = osg::Vec4(0.75f, 0.75f, 0.75f, 0.5f);
		specular = osg::Vec4(0.75f, 0.75f, 0.75f, 0.5f);
		mode = 2;
	}
	NodeVisitor_dyer( osg::Vec4 ambientcolor, osg::Vec4 diffusecolor, osg::Vec4 specularcolor, bool if_open) ://用于右键点选透明，只调整光照
		osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{
		ambient = ambientcolor;
		diffuse = diffusecolor;
		specular = specularcolor;
		mode = 0;
	}
	NodeVisitor_dyer(osg::ref_ptr<osg::StateSet> init_state) :osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),initstate(init_state)//用于恢复到初始状态
	{
		mode = 1;
	}
	NodeVisitor_dyer(osg::Vec4 given_color,int m): osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{
		color = given_color;
		mode = m;
	}
	~NodeVisitor_dyer() {};

	void apply(osg::Node& geode)
	{
		if (mode == 0)//右键点选部件透明
		{
			unsigned int i = 0;
			osg::ref_ptr<osg::Material> material = new osg::Material;
			material->setAmbient(osg::Material::FRONT_AND_BACK, ambient);
			material->setDiffuse(osg::Material::FRONT_AND_BACK, diffuse);
			//material->setSpecular(osg::Material::FRONT_AND_BACK, specular);
			osg::ref_ptr<osg::StateSet> now_state = new osg::StateSet;
			now_state->setMode(GL_BLEND, osg::StateAttribute::ON);
			now_state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
			//设置网格模型
			//osg::ref_ptr<osg::PolygonMode> polyMode=new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);
			now_state->setAttributeAndModes(material.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
			geode.setStateSet(now_state);
		}
		else if (mode == 2)
		{
			osg::ref_ptr<osg::Geode> ge = geode.asGeode();
			if (ge)//找到叶节点
			{
				osg::ref_ptr<osg::Vec4Array> changecolor = new osg::Vec4Array();//根据changecolor中存储的part类型名为此geode上色
				changecolor->push_back(color);
				for (int i = 0; i < ge->getNumDrawables(); i++)
				{
					osg::ref_ptr<osg::Geometry> geometry = ge->getDrawable(i)->asGeometry();
					osg::ref_ptr<osg::Array> lastgeocolor = geometry->getColorArray();
					geo_color.insert(geometry, lastgeocolor);//记录下修改前的颜色
					//osg::ref_ptr<osg::StateSet> initgeostate=geometry->getStateSet();
					geometry->setColorArray(changecolor);
					geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

					osg::ref_ptr< osg::StateSet > state_set = geometry->getOrCreateStateSet();
					geo_state.insert(geometry, state_set);
					//设置geode的材质（这样设置相当于所有的geode的材质相同，如果每个part材质不同可能效果更好？）
					osg::ref_ptr< osg::Material > material = new osg::Material;
					material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
					material->setDiffuse(osg::Material::FRONT_AND_BACK, diffuse);
					material->setSpecular(osg::Material::FRONT_AND_BACK, specular);
					material->setAmbient(osg::Material::FRONT_AND_BACK, ambient);
					material->setShininess(osg::Material::FRONT_AND_BACK, 51.2f);
					state_set->setAttributeAndModes(material.get(), osg::StateAttribute::ON);
					state_set->setMode(GL_CULL_FACE, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);   // 只关闭背面裁剪，造成生成背面不透明，但黑面;
					state_set->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);//关闭光照
					state_set->setMode(GL_LIGHT0, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
					if (open_transparent)
					{
						osg::ref_ptr<osg::LightModel> TwoSideLight = new osg::LightModel;
						TwoSideLight->setTwoSided(true);
						//只对模型中的部件进行半透明化，可以在遍历模型部件时，1）设置显示颜色alpha通道小于1，2）开启融合模型，3）同时渲染方式设为透明方式
						//开启融合操作
						state_set->setMode(GL_BLEND, osg::StateAttribute::ON);
						state_set->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);
						//设置渲染模式
						state_set->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
						state_set->setMode(GL_CULL_FACE, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);   // 只关闭背面裁剪，造成生成背面不透明，但黑面 ;
						state_set->setAttributeAndModes(TwoSideLight, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);  //再加上双面光照，使背面完全出现！;
					}
				}
			}
			traverse(geode);//从节点开始遍历
		}
		else if (mode == 3)//用于rgb拍照
		{
			osg::ref_ptr<osg::Geode> ge = geode.asGeode();
			if (ge)//找到叶节点
			{
				unsigned int i = 0;
				osg::ref_ptr<osg::Vec4Array> changecolor = new osg::Vec4Array();//根据changecolor中存储的part类型名为此geode上色
				changecolor->push_back(color);
				for (i = 0; i < ge->getNumDrawables(); i++)
				{
					osg::ref_ptr<osg::Geometry> geometry = ge->getDrawable(i)->asGeometry();
					osg::ref_ptr<osg::Array> geocolor = geometry->getColorArray();

					geometry->setColorArray(changecolor);
					geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

					osg::ref_ptr< osg::StateSet > state_set = geometry->getOrCreateStateSet();//设置geode的材质（这样设置相当于所有的geode的材质相同，如果每个part材质不同可能效果更好？）
					osg::ref_ptr< osg::Material > material = new osg::Material;
					material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
					material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.75f, 0.75f, 0.75f, 0.5f));
					material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.75f, 0.75f, 0.75f, 0.5f));
					material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.75f, 0.75f, 0.75f, 0.5f));
					material->setShininess(osg::Material::FRONT_AND_BACK, 51.2f);
					state_set->setAttributeAndModes(material.get(), osg::StateAttribute::ON);
					osg::Texture2D* const tex2D = new osg::Texture2D;
					state_set->setAttributeAndModes(tex2D, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);//去除导入模型的纹理
					//只对模型中的部件进行半透明化，可以在遍历模型部件时，1）设置显示颜色alpha通道小于1，2）开启融合模型，3）同时渲染方式设为透明方式
					//开启融合操作
					state_set->setMode(GL_BLEND, osg::StateAttribute::ON);
					//设置渲染模式
					state_set->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
				}
			}
			traverse(geode);//从节点开始遍历
		}
		else if (mode == 1)//返回到模型初始设置
		{
			geode.setStateSet(init_state_set.get());
			QMap<osg::ref_ptr<osg::Geometry>, osg::ref_ptr<osg::Array>>::iterator arrayitr = geo_color.begin();
			QMap<osg::ref_ptr<osg::Geometry>, osg::ref_ptr<osg::StateSet>>::iterator stateitr = geo_state.begin();
			for (; arrayitr != geo_color.end(); arrayitr++)
			{
				arrayitr.key()->setColorArray(arrayitr.value());
				arrayitr.key()->setColorBinding(osg::Geometry::BIND_OVERALL);
			}
			for (; stateitr != geo_state.end(); stateitr++)
			{
				stateitr.key()->setStateSet(stateitr.value().get());
			}
			geo_color.clear();
			geo_state.clear();
		}
	}
	void set_color(osg::Vec4 given_color)
	{
		color = given_color;
	}
	osg::Vec4 get_color()
	{
		return color;
	}

private:
	osg::Vec4 color, ambient, diffuse, specular;
	bool open_transparent=false;//是否开启半透明模式
	osg::ref_ptr<osg::StateSet> initstate;
	int mode;
};
class modelCallBack1 : public osg::NodeCallback//用于调用颜色、光照等上面的回调类，暂时没用
{
public:
	modelCallBack1() :Color() {}
	modelCallBack1(osg::Vec4 color, bool open) :Color(color), open_transparent(open)
	{
	}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		NodeVisitor_dyer dyerb(Color, open_transparent);
		node->accept(dyerb);
		traverse(node, nv);
	}
private:
	osg::Vec4 Color;
	bool open_transparent;
};



 /*class KeyboardHandler :public osgGA::GUIEventHandler{
    注释：class keyboardHandler 用于处理键盘人机交互事件处理器
 * 按D键打印all_node_name,model_structure_tree,model_components 用于调试

public:
    //重构父类GUIEventHandler.handle，事件处理函数，自定义交互操作，
    //参数1:当前传入此处理器的事件，只可以被获取，不能被修改
    //参数2：反馈动作，动作适配器，可以获取也可以修改的参数，大部分时候这个传入值表示当前所用的视图对象View，可以用它来获取
    //        或控制视景器的状态变化。如：osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
    //参数3：对象指针obj，保存该处理器的对象，可能是当前事件回调所在的Node节点指针，也可能是Drawable指针
    //参数4：传递该事件的访问器（EventVisitor），nv通常为当前时间访问器指针。
    virtual bool handle(const osgGA::GUIEventAdapter& ea,
                        osgGA::GUIActionAdapter& aa,
                        osg::Object* obj, osg::NodeVisitor* nv)
    {
        osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
        if (!viewer)return false;
        switch (ea.getEventType())//判断事件，做出响应
        {
        case osgGA::GUIEventAdapter::KEYDOWN://ea.getEventType()获取到的如果是键盘事件
            if(ea.getKey() == osgGA::GUIEventAdapter::KEY_D){
                qDebug()<<"Press D";
                cout<<"press D"<<endl;
                cout<<"---------------"<<endl;
                cout<<"print all_node_name"<<endl;
                for(int i=0 ;i<all_node_name.size();i++)
                    cout<<all_node_name[i]<<" ";
                cout<<""<<endl;
                cout<<"---------------"<<endl;
                cout<<"print model_structure_tree"<<endl;
                for(int i = 0; i < all_node_name.size(); i++)
                {
                    cout<<"father_name: "<<all_node_name[i]<<" child_num: "<<model_structure_tree[i].size()<<endl;
                    for (int j = 0; j < model_structure_tree[i].size();j++)
                    {
                        cout<<model_structure_tree[i][j]<<" ";
                    }
                    cout<<" "<<endl;
                }
                cout<<""<<endl;
                cout<<"---------------"<<endl;
                cout<<"print model_components"<<endl;
                map<string,bool>::iterator itr;
                itr = map_to_choose.begin();
                while(itr != map_to_choose.end())
                {
                    cout<<(itr->first)<<endl;
                    itr++;
                }
                cout<<""<<endl;
                cout<<"---------------"<<endl;
                map<string,bool>::iterator it;
                cout<<"print map_to_choose"<<endl;
                it = map_to_choose.begin();
                while(it != map_to_choose.end())
                {
                    cout<<it->first<<" : "<<it->second<<endl;
                    it++;
                }
            }
            break;
        default:
            break;
        }
        return false;
    }

}; */

class Pick :public osgGA::GUIEventHandler {//右键点选变透明,
    /* 注释: class Pick 主要用于鼠标人机交互事件
 * Pick1左键点击部件获取点击部件的名字，Pick3右键点选部件用于生成包围盒
 */
    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
        osgViewer::View *viewer = dynamic_cast<osgViewer::View*>(&aa);
        if(!viewer) return false;
        switch (ea.getEventType()) {
        case(osgGA::GUIEventAdapter::PUSH):
            //if (ea.getButton() == 1) {
            //    pick(ea.getX(), ea.getY(),viewer);
            //}
            if (ea.getButton() == ea.RIGHT_MOUSE_BUTTON){
                mouse_is_picked = true;
                pick(ea.getX(), ea.getY(),viewer);
            }
            break;
        case(osgGA::GUIEventAdapter::RELEASE):
            if (ea.getButton() == ea.RIGHT_MOUSE_BUTTON){
                mouse_is_picked = false;
            }
            break;
        default:
            break;
        }
        return false;
    }
    /*
    void pick3(float x, float y,osgViewer::View *viewer) { //注释： 点击部件生成包围盒
        osgUtil::LineSegmentIntersector::Intersections intersections;
        if (viewer->computeIntersections(x,y,intersections)) {
            osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
            osg::Vec3 vec = intersections.begin()->getWorldIntersectPoint();
            std::cout<< vec.x() <<" " << vec.y() << " " <<vec.z() << std::endl;
            if (!hitr->nodePath.empty()) {
                const osg::NodePath& np = hitr->nodePath;
                //                std::cout << np.size() << std::endl;

                //cout<<"tab_idx = "<<tab_idx<<endl;
                if(tab_idx==0){
                    select_box = false;
                    bool flag_find = false;
                    for (int i = 0; i <np.size(); i++) {

                        osg::Node *node = dynamic_cast<osg::Node *>(np[np.size()-1-i]);
                        if (node!=NULL){
                            bool isbox = false;
                            if(node->getName()!=""){
                                current_tree_item_name = node->getName();
                                auto aaaaa = node->getName();
                                for(int ii = 0; ii<all_node_name.size(); ii++){
                                    std::string this_name = all_node_name.at(ii);
                                    if(this_name.compare(current_tree_item_name.c_str())==0){
                                        cout <<"current_tree_item_name: "<< current_tree_item_name <<endl;
                                        marked_tree_idx = ii;
                                        isbox = true;
                                        flag_find = true;
                                        osg::ComputeBoundsVisitor boundVisitor;
                                        node->accept(boundVisitor);
                                        osg::BoundingBox bs = boundVisitor.getBoundingBox();

                                        float xmax = bs.xMax();
                                        float ymax = bs.yMax();
                                        float zmax = bs.zMax();

                                        float xmin = bs.xMin();
                                        float ymin = bs.yMin();
                                        float zmin = bs.zMin();

                                        for(int j = 0;j<8;j++){
                                            float x_coor = 0;
                                            float y_coor = 0;
                                            float z_coor = 0;
                                            if(j&1){
                                                x_coor = xmax*scale_all;
                                            }else{
                                                x_coor = xmin*scale_all;
                                            }
                                            if(j&2){
                                                y_coor = ymax*scale_all;
                                            }else{
                                                y_coor = ymin*scale_all;
                                            }
                                            if(j&4){
                                                z_coor = zmax*scale_all;
                                            }else{
                                                z_coor = zmin*scale_all;
                                            }
                                            std::ostringstream ss1;
                                            ss1 << x_coor <<" " << y_coor << " " <<z_coor;
                                            cout <<"ss1: "<<endl;
                                            std::string s1(ss1.str());
                                            std::string mat_command1 = "box_point = [box_point;"+s1+"];";
                                            std::cout<<s1<<endl;
                                            engEvalString(ep, mat_command1.c_str());
                                        }

                                        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
                                        geode->setName("red_dot");
                                        osg::TessellationHints* hints = new osg::TessellationHints;
                                        hints->setDetailRatio(0.1f);
                                        osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(vec,0.1);
                                        osg::ref_ptr<osg::ShapeDrawable> shapedrawable = new osg::ShapeDrawable(sphere,hints);
                                        osg::Vec4 color(1.0f,0.0f,0.0f,1.0f);
                                        shapedrawable->setColor(color);
                                        geode->addDrawable(shapedrawable);
                                        viewer->getSceneData()->asGroup()->addChild(geode);
                                    }

                                }
                                if(Boxes_components.size()>0&&(!isbox)){

                                    for(int ii = 0; ii<Boxes_components.size(); ii++){
                                        std::string this_name = Boxes_components.at(ii)->getName();
                                        if(this_name.compare(current_tree_item_name.c_str())==0){
                                            select_box = true;
                                            box_sel_idx = ii;
                                        }

                                    }
                                }
                            }
                        }
                        if(flag_find)
                            break;
                    }
                }
                else if(tab_idx==1){
                }
            }else{
                current_tree_item_name = "";
            }
        }
    }
    void pick2(float x,float y,osgViewer::View *viewer) {
        osgUtil::LineSegmentIntersector::Intersections intersections;
        if (viewer->computeIntersections(x,y,intersections)) {
            osg::Vec3 vec = intersections.begin()->getWorldIntersectPoint();
            std::cout<< vec.x() <<" " << vec.y() << " " <<vec.z() << std::endl;

            std::ostringstream ss;
            ss << vec.x() <<" " << vec.y() << " " <<vec.z();
            std::string s(ss.str());
            std::string mat_command = "box_point = [box_point;"+s+"];";
            std::cout<<s<<endl;
            engEvalString(ep, mat_command.c_str());

            osg::ref_ptr<osg::Geode> geode = new osg::Geode;
            geode->setName("red_dot");
            osg::TessellationHints* hints = new osg::TessellationHints;
            hints->setDetailRatio(0.1f);
            osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(vec,0.01);
            osg::ref_ptr<osg::ShapeDrawable> shapedrawable = new osg::ShapeDrawable(sphere,hints);
            osg::Vec4 color(1.0f,0.0f,0.0f,1.0f);
            shapedrawable->setColor(color);
            geode->addDrawable(shapedrawable);
            viewer->getSceneData()->asGroup()->addChild(geode);

        }
    }
    */
    void pick(float x, float y,osgViewer::View *viewer) 
	{
        osgUtil::LineSegmentIntersector::Intersections intersections;
        if (viewer->computeIntersections(x,y,intersections)) {
            osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
            if (!hitr->nodePath.empty()) {
                const osg::NodePath& np = hitr->nodePath;
                //                std::cout << np.size() << std::endl;

                //cout<<"tab_idx = "<<tab_idx<<endl;
                if(tab_idx==0){
                    bool flag_find = false;
                    for (int i = np.size()-1; i >=0; i--) {

                        osg::Node *node = dynamic_cast<osg::Node *>(np[i]);
                        if (node!=NULL){
                            if(node->getName()!=""){
                                //current_box_name = node->getName();
                                current_tree_item_name = node->getName();
                                ///*
                                //auto iter = is_model_componets.find(current_tree_item_name);
                                //if(iter!=is_model_componets.end())
                                //{
                                //if(is_model_componets[current_tree_item_name]){
								osg::ref_ptr<osgSim::DOFTransform> dof = dynamic_cast<osgSim::DOFTransform*>(node);
								if(dof){
									QMap<QTreeWidgetItem*, osg::ref_ptr<osg::Node>>::iterator iter = item_node.begin();
									for (; iter != item_node.end(); iter++)
									{
										if (iter.value() == node)
										{
											selectedItemList.append(iter.key());
											iter.key()->setExpanded(true);
											iter.key()->setSelected(true);
										}
									}
                                    flag_find = true;
                                }
                                //}
                            }

                        }
                        if(flag_find)
                            break;
                    }
                }
            }
        }
		else{
            mouse_is_picked_empty = true;
			selectedItemList.empty();
            current_tree_item_name = "";
        }
    }
};


/*class Pick2 :public osgGA::GUIEventHandler {
    /* 注释: class Pick 主要用于鼠标人机交互时间
 // Pick1左键点击部件获取点击部件的名字，Pick3右键点选部件用于生成包围盒
 



    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
        osgViewer::View *viewer = dynamic_cast<osgViewer::View*>(&aa);
        if(!viewer) return false;
        switch (ea.getEventType()) {
        case(osgGA::GUIEventAdapter::PUSH):
            //if (ea.getButton() == 1) {
            //    pick(ea.getX(), ea.getY(),viewer);
            //}
            if (ea.getButton() == ea.RIGHT_MOUSE_BUTTON){
                pick(ea.getX(), ea.getY(),viewer);
            }
            break;
        case(osgGA::GUIEventAdapter::RELEASE):
            if (ea.getButton() == ea.RIGHT_MOUSE_BUTTON){
            }
            break;
        default:
            break;
        }
        return false;
    }
    /*
    void pick3(float x, float y,osgViewer::View *viewer) { //注释： 点击部件生成包围盒
        osgUtil::LineSegmentIntersector::Intersections intersections;
        if (viewer->computeIntersections(x,y,intersections)) {
            osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
            osg::Vec3 vec = intersections.begin()->getWorldIntersectPoint();
            std::cout<< vec.x() <<" " << vec.y() << " " <<vec.z() << std::endl;
            if (!hitr->nodePath.empty()) {
                const osg::NodePath& np = hitr->nodePath;
                //                std::cout << np.size() << std::endl;

                //cout<<"tab_idx = "<<tab_idx<<endl;
                if(tab_idx==0){
                    select_box = false;
                    bool flag_find = false;
                    for (int i = 0; i <np.size(); i++) {

                        osg::Node *node = dynamic_cast<osg::Node *>(np[np.size()-1-i]);
                        if (node!=NULL){
                            bool isbox = false;
                            if(node->getName()!=""){
                                current_tree_item_name = node->getName();
                                auto aaaaa = node->getName();
                                for(int ii = 0; ii<all_node_name.size(); ii++){
                                    std::string this_name = all_node_name.at(ii);
                                    if(this_name.compare(current_tree_item_name.c_str())==0){
                                        cout <<"current_tree_item_name: "<< current_tree_item_name <<endl;
                                        marked_tree_idx = ii;
                                        isbox = true;
                                        flag_find = true;
                                        osg::ComputeBoundsVisitor boundVisitor;
                                        node->accept(boundVisitor);
                                        osg::BoundingBox bs = boundVisitor.getBoundingBox();

                                        float xmax = bs.xMax();
                                        float ymax = bs.yMax();
                                        float zmax = bs.zMax();

                                        float xmin = bs.xMin();
                                        float ymin = bs.yMin();
                                        float zmin = bs.zMin();

                                        for(int j = 0;j<8;j++){
                                            float x_coor = 0;
                                            float y_coor = 0;
                                            float z_coor = 0;
                                            if(j&1){
                                                x_coor = xmax*scale_all;
                                            }else{
                                                x_coor = xmin*scale_all;
                                            }
                                            if(j&2){
                                                y_coor = ymax*scale_all;
                                            }else{
                                                y_coor = ymin*scale_all;
                                            }
                                            if(j&4){
                                                z_coor = zmax*scale_all;
                                            }else{
                                                z_coor = zmin*scale_all;
                                            }
                                            std::ostringstream ss1;
                                            ss1 << x_coor <<" " << y_coor << " " <<z_coor;
                                            cout <<"ss1: "<<endl;
                                            std::string s1(ss1.str());
                                            std::string mat_command1 = "box_point = [box_point;"+s1+"];";
                                            std::cout<<s1<<endl;
                                            engEvalString(ep, mat_command1.c_str());
                                        }

                                        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
                                        geode->setName("red_dot");
                                        osg::TessellationHints* hints = new osg::TessellationHints;
                                        hints->setDetailRatio(0.1f);
                                        osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(vec,0.1);
                                        osg::ref_ptr<osg::ShapeDrawable> shapedrawable = new osg::ShapeDrawable(sphere,hints);
                                        osg::Vec4 color(1.0f,0.0f,0.0f,1.0f);
                                        shapedrawable->setColor(color);
                                        geode->addDrawable(shapedrawable);
                                        viewer->getSceneData()->asGroup()->addChild(geode);
                                    }

                                }
                                if(Boxes_components.size()>0&&(!isbox)){

                                    for(int ii = 0; ii<Boxes_components.size(); ii++){
                                        std::string this_name = Boxes_components.at(ii)->getName();
                                        if(this_name.compare(current_tree_item_name.c_str())==0){
                                            select_box = true;
                                            box_sel_idx = ii;
                                        }

                                    }
                                }
                            }
                        }
                        if(flag_find)
                            break;
                    }
                }
                else if(tab_idx==1){
                }
            }else{
                current_tree_item_name = "";
            }
        }
    }

  
    void pick2(float x,float y,osgViewer::View *viewer) {
        osgUtil::LineSegmentIntersector::Intersections intersections;
        if (viewer->computeIntersections(x,y,intersections)) {
            osg::Vec3 vec = intersections.begin()->getWorldIntersectPoint();
            std::cout<< vec.x() <<" " << vec.y() << " " <<vec.z() << std::endl;

            std::ostringstream ss;
            ss << vec.x() <<" " << vec.y() << " " <<vec.z();
            std::string s(ss.str());
            std::string mat_command = "box_point = [box_point;"+s+"];";
            std::cout<<s<<endl;
            engEvalString(ep, mat_command.c_str());

            osg::ref_ptr<osg::Geode> geode = new osg::Geode;
            geode->setName("red_dot");
            osg::TessellationHints* hints = new osg::TessellationHints;
            hints->setDetailRatio(0.1f);
            osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(vec,0.01);
            osg::ref_ptr<osg::ShapeDrawable> shapedrawable = new osg::ShapeDrawable(sphere,hints);
            osg::Vec4 color(1.0f,0.0f,0.0f,1.0f);
            shapedrawable->setColor(color);
            geode->addDrawable(shapedrawable);
            viewer->getSceneData()->asGroup()->addChild(geode);

        }
    }
    */
    /*void pick(float x, float y,osgViewer::View *viewer) {
        osgUtil::LineSegmentIntersector::Intersections intersections;
        if (viewer->computeIntersections(x,y,intersections)) {
            osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
            osg::Vec3 vec = intersections.begin()->getWorldIntersectPoint();
            unsigned int primitiveindex = intersections.begin()->primitiveIndex;
            std::vector<unsigned int> indexlist = intersections.begin()->indexList;
            //std::cout<< vec.x() <<" " << vec.y() << " " <<vec.z() << std::endl;
            qDebug()<< vec.x() <<" " << vec.y() << " " <<vec.z();
            if(dof_pick2_mode == 1){

                findGeoNamedNode* visitor1 = new findGeoNamedNode("lineeeeeeeee");
                viewer->getSceneData()->asGroup()->accept(*visitor1);
                osg::ref_ptr<osg::Node> getnode1 = visitor1->getNode();
                if(getnode1!=NULL){
                    getnode1->getParent(0)->removeChild(getnode1);
                }



                findGeoNamedNode* visitor = new findGeoNamedNode("red_dot_1");
                viewer->getSceneData()->asGroup()->accept(*visitor);
                osg::ref_ptr<osg::Node> getnode = visitor->getNode();
                if(getnode!=NULL){
                    getnode->getParent(0)->removeChild(getnode);
                }
                osg::ref_ptr<osg::Geode> geode = new osg::Geode;
                geode->setName("red_dot_1");
                osg::TessellationHints* hints = new osg::TessellationHints;
                hints->setDetailRatio(0.5f);
                osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(vec,0.003);
                osg::ref_ptr<osg::ShapeDrawable> shapedrawable = new osg::ShapeDrawable(sphere,hints);
                osg::Vec4 color(0.0f,0.0f,1.0f,1.0f);
                shapedrawable->setColor(color);
                geode->addDrawable(shapedrawable);
                viewer->getSceneData()->asGroup()->addChild(geode);
                Two_point_is_picked_1 = true;
                Two_point_coor_1.x() = vec.x();
                Two_point_coor_1.y() = vec.y();
                Two_point_coor_1.z() = vec.z();

                findGeoNamedNode* visitor2 = new findGeoNamedNode("green_dot_1");
                viewer->getSceneData()->asGroup()->accept(*visitor2);
                osg::ref_ptr<osg::Node> getnode2 = visitor2->getNode();
                if(getnode2!=NULL){
                    osg::ref_ptr<osg::LineWidth> width = new osg::LineWidth;
                    osg::Geode *geodeline = new osg::Geode();
                    geodeline->setName("lineeeeeeeee");
                    osg::ref_ptr<osg::Vec3Array> vecarryline = new osg::Vec3Array();
                    osg::ref_ptr<osg::Geometry> geometryline = new osg::Geometry();
                    osg::Vec4 color(0.0,0.0,0.0,1.0);
                    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
                    c->push_back(color);
                    c->push_back(color);
                    vecarryline->push_back(Two_point_coor_1);
                    vecarryline->push_back(Two_point_coor_2);
                    geometryline->setVertexArray(vecarryline.get());
                    geometryline->setColorArray(c.get());
                    geometryline->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
                    geometryline->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, 2));
                    width->setWidth(5.0);
                    geometryline->getOrCreateStateSet()->setAttributeAndModes(width);
                    geodeline->addDrawable(geometryline.get());
                    viewer->getSceneData()->asGroup()->addChild(geodeline);
                }

            }else if(dof_pick2_mode == 2){
                findGeoNamedNode* visitor1 = new findGeoNamedNode("lineeeeeeeee");
                viewer->getSceneData()->asGroup()->accept(*visitor1);
                osg::ref_ptr<osg::Node> getnode1 = visitor1->getNode();
                if(getnode1!=NULL){
                    getnode1->getParent(0)->removeChild(getnode1);
                }

                findGeoNamedNode* visitor = new findGeoNamedNode("green_dot_1");
                viewer->getSceneData()->asGroup()->accept(*visitor);
                osg::ref_ptr<osg::Node> getnode = visitor->getNode();
                if(getnode!=NULL){
                    getnode->getParent(0)->removeChild(getnode);
                }
                osg::ref_ptr<osg::Geode> geode = new osg::Geode;
                geode->setName("green_dot_1");
                osg::TessellationHints* hints = new osg::TessellationHints;
                hints->setDetailRatio(0.5f);
                osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(vec,0.003);
                osg::ref_ptr<osg::ShapeDrawable> shapedrawable = new osg::ShapeDrawable(sphere,hints);
                osg::Vec4 color(1.0f,1.0f,0.0f,1.0f);
                shapedrawable->setColor(color);
                geode->addDrawable(shapedrawable);
                viewer->getSceneData()->asGroup()->addChild(geode);
                Two_point_is_picked_2 = true;
                Two_point_coor_2.x() = vec.x();
                Two_point_coor_2.y() = vec.y();
                Two_point_coor_2.z() = vec.z();



                findGeoNamedNode* visitor2 = new findGeoNamedNode("red_dot_1");
                viewer->getSceneData()->asGroup()->accept(*visitor2);
                osg::ref_ptr<osg::Node> getnode2 = visitor2->getNode();
                if(getnode2!=NULL){
                    osg::ref_ptr<osg::LineWidth> width = new osg::LineWidth;
                    osg::Geode *geodeline = new osg::Geode();
                    geodeline->setName("lineeeeeeeee");
                    osg::ref_ptr<osg::Vec3Array> vecarryline = new osg::Vec3Array();
                    osg::ref_ptr<osg::Geometry> geometryline = new osg::Geometry();
                    osg::Vec4 color(0.0,0.0,0.0,1.0);
                    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
                    c->push_back(color);
                    c->push_back(color);
                    vecarryline->push_back(Two_point_coor_1);
                    vecarryline->push_back(Two_point_coor_2);
                    geometryline->setVertexArray(vecarryline.get());
                    geometryline->setColorArray(c.get());
                    geometryline->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
                    geometryline->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, 2));
                    width->setWidth(5.0);
                    geometryline->getOrCreateStateSet()->setAttributeAndModes(width);
                    geodeline->addDrawable(geometryline.get());
                    viewer->getSceneData()->asGroup()->addChild(geodeline);
                }


            }else if(dof_pick2_mode == 3){
                findGeoNamedNode* visitor = new findGeoNamedNode("blue_dot_1");
                viewer->getSceneData()->asGroup()->accept(*visitor);
                osg::ref_ptr<osg::Node> getnode = visitor->getNode();
                if(getnode!=NULL){
                    getnode->getParent(0)->removeChild(getnode);
                }
                osg::ref_ptr<osg::Geode> geode = new osg::Geode;
                geode->setName("blue_dot_1");
                osg::TessellationHints* hints = new osg::TessellationHints;
                hints->setDetailRatio(0.5f);
                osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(vec,0.003);
                osg::ref_ptr<osg::ShapeDrawable> shapedrawable = new osg::ShapeDrawable(sphere,hints);
                osg::Vec4 color(0.0f,0.0f,1.0f,1.0f);
                shapedrawable->setColor(color);
                geode->addDrawable(shapedrawable);
                viewer->getSceneData()->asGroup()->addChild(geode);
                one_point_is_picked_1 = true;
                one_point_coor.x() = vec.x();
                one_point_coor.y() = vec.y();
                one_point_coor.z() = vec.z();
            }else if(dof_pick2_mode == 4){
                if (!hitr->nodePath.empty()) {
                    const osg::NodePath& np = hitr->nodePath;
                    osg::Node *node = dynamic_cast<osg::Node *>(np[np.size()-1]);
                    if (node!=NULL){
                        osg::Matrix m = osg::computeLocalToWorld(node->getParentalNodePaths()[0]);
                        facepointarray = new osg::Vec3Array;
                        osg::ref_ptr<osg::Geode> p=node->asGeode();
                        unsigned int count = p->getNumDrawables();

                        osg::Geometry *geometry = p->getDrawable(0)->asGeometry();
                        osg::Vec3Array* vertices=dynamic_cast<osg::Vec3Array*>(geometry->getVertexArray());
                        facepointarray->push_back(vertices->at(indexlist.at(0))*m);
                        facepointarray->push_back(vertices->at(indexlist.at(1))*m);
                        facepointarray->push_back(vertices->at(indexlist.at(2))*m);




                        findGeoNamedNode* visitor = new findGeoNamedNode("faceeeeeeee");
                        viewer->getSceneData()->asGroup()->accept(*visitor);
                        osg::ref_ptr<osg::Node> getnode = visitor->getNode();
                        if(getnode!=NULL){
                            getnode->getParent(0)->removeChild(getnode);
                        }

                        osg::Geode* Part = new osg::Geode();
                        osg::Vec4 color(0.0,1.0,0.0,1.0);
                        osg::ref_ptr<osg::Geometry> part_i = new osg::Geometry;
                        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
                        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
                        part_i->setVertexArray(v.get());
                        part_i->setColorArray(c.get());
                        part_i->setColorBinding( osg::Geometry::BIND_PER_VERTEX );


                        osg::Vec3d ab=facepointarray->at(1)-facepointarray->at(0);
                        osg::Vec3d ac=facepointarray->at(2)-facepointarray->at(0);
                        osg::Vec3d normal_vector;
                        normal_vector=ab^ac;
                        normal_vector=normal_vector/normal_vector.length();
                        v->push_back(facepointarray->at(0)-normal_vector*0.0001);
                        v->push_back(facepointarray->at(1)-normal_vector*0.0001);
                        v->push_back(facepointarray->at(2)-normal_vector*0.0001);
                        v->push_back(facepointarray->at(0)+normal_vector*0.0001);
                        v->push_back(facepointarray->at(1)+normal_vector*0.0001);
                        v->push_back(facepointarray->at(2)+normal_vector*0.0001);
                        c->push_back(color);
                        c->push_back(color);
                        c->push_back(color);
                        c->push_back(color);
                        c->push_back(color);
                        c->push_back(color);
                        part_i->addPrimitiveSet(new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, 6));
                        Part->addDrawable(part_i);
                        Part->setName("faceeeeeeee");
                        viewer->getSceneData()->asGroup()->addChild(Part);



                    }


                }
                one_point_is_picked_2 = true;
            }


            /*
            if (!hitr->nodePath.empty()) {
                const osg::NodePath& np = hitr->nodePath;
                //                std::cout << np.size() << std::endl;

                //cout<<"tab_idx = "<<tab_idx<<endl;
                if(tab_idx==0){
                    bool flag_find = false;
                    for (int i = np.size()-1; i >=0; i--) {

                        osg::Node *node = dynamic_cast<osg::Node *>(np[i]);
                        if (node!=NULL){
                            if(node->getName()!=""){
                                //current_box_name = node->getName();
                                current_tree_item_name = node->getName();
                                ///*
                                //auto iter = is_model_componets.find(current_tree_item_name);
                                //if(iter!=is_model_componets.end())
                                //{
                                if(is_model_componets[current_tree_item_name]){
                                    bool node_is_picked = map_to_choose[current_tree_item_name];
                                    map_to_choose[current_tree_item_name] = !node_is_picked;
                                    flag_find = true;
                                }
                                //}

                            }
                        }
                        if(flag_find)
                            break;
                    }
                }
            }
            */
       /* }
    }
};*/



osgqt::osgqt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::osgqt)
{
    // 注释：构造函数，用于一些初始化
    ui->setupUi(this);
	//auto widget = new QWidget;
   // QMainWindow* mw = new QMainWindow(this);
   // mw->setWindowFlags(Qt::Widget);
    ViewerWindow = new QtOsgView(this);
    manipulator = new osgGA::TrackballManipulator;
    ViewerWindow->setCameraManipulator(manipulator);
    ViewerWindow->addEventHandler(new osgViewer::WindowSizeHandler);
    ViewerWindow->addEventHandler(new Pick());
    //ViewerWindow->addEventHandler(new KeyboardHandler());
    ViewerWindow->getCamera()->setClearColor(osg::Vec4f(255.0f, 255.0f, 255.0f, 1.0f));

    // 环境光
    ViewerWindow->getLight()->setAmbient(osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
    // 漫反射光
    ViewerWindow->getLight()->setDiffuse(osg::Vec4(0.1f, 0.1f, 0.1f, 1.0f));
	ViewerWindow->getLight()->setPosition(osg::Vec4(0.0f, 0.0f, 1.f, 1.0f));
    
    //ViewerWindow->grabKeyboard();
    QFont font ( "Microsoft YaHei", 9, 75);
    ui->copyright1->setFont(font);
	//ui->rangecombo->set
    //ui->done_name_info->setReadOnly(true);
    //ui->dof_add_info->setReadOnly(true);

    //mw->setCentralWidget(ViewerWindow);
	//mw->setStyleSheet("background:blue;border:2px solid red;");
    ui->middleLayout->addWidget(ViewerWindow);
    treewidget = new mytreewidget();
    //treewidget->resize(211,831);
	ui->leftLayout->addWidget(treewidget);
	ui->totalLayout->setStretch(0, 180);
	ui->totalLayout->setStretch(1, 720);
	ui->totalLayout->setStretch(2, 200);
	ui->tab->setLayout(ui->totalLayout);//设置布局随窗口动
	//ui->rangecombo->addItem("x");
	//ui->rangecombo->addItem("y");
	//ui->rangecombo->addItem("z");
	osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
	kinect = osgDB::readNodeFile("./resource/kinect.obj", a);
	viewport= osgDB::readNodeFile("./resource/viewport2.obj", a);
	//osg::ref_ptr<osg::PolygonMode> polyMode = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
	//osg::StateSet* ss1 = viewport->getOrCreateStateSet();  //线框模式
	//osg::LineWidth* line=new osg::LineWidth(2.f); 
	//ss1->setAttribute(polyMode);
	//ss1->setAttribute(line);
	/*osg::Vec4 temp_color(255.0, 0, 0, 1.0);//设置透明
	NodeVisitor_dyer dyer(temp_color, true);
	viewport->asGroup()->accept(dyer);*/
	//ViewerWindow->setSceneData(viewport.get());
	osg::ref_ptr<osg::StateSet> now_state = new osg::StateSet;//设置viewport为黄色透明
	//if(it->second){
	osg::ref_ptr<osg::Material> material = new osg::Material;
	osg::Vec4 redColor(255, 191, 0, 0.5f);
	material->setAmbient(osg::Material::FRONT_AND_BACK, redColor);
	material->setDiffuse(osg::Material::FRONT_AND_BACK, redColor);
	now_state->setMode(GL_BLEND, osg::StateAttribute::ON);
	now_state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	//设置网格模型
	//osg::ref_ptr<osg::PolygonMode> polyMode=new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);
	now_state->setAttributeAndModes(material.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	viewport->setStateSet(now_state);
	initwindow();
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    connect(treewidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));
    connect(treewidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(on_treeWidget_itemChanged_child(QTreeWidgetItem*,int)));
    connect(treewidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(on_treeWidget_itemChanged_parent(QTreeWidgetItem*,int)));
    connect(treewidget,SIGNAL(itemSelectionChanged()),this,SLOT(on_treeWidget_itemSelectionChanged()));


	connect(ui->motion_confirm, SIGNAL(pressed()), this, SLOT(motion_comfirm_clicked()));
	connect(ui->motion_delete, SIGNAL(pressed()), this, SLOT(motion_delete_clicked()));
	connect(ui->motion_play, SIGNAL(pressed()), this, SLOT(motion_play_clicked()));

	connect(ui->camera_confirm, SIGNAL(pressed()), this, SLOT(camera_confirm_clicked()));
	connect(ui->camera_delete, SIGNAL(pressed()), this, SLOT(camera_delete_clicked()));
	connect(ui->camera_play, SIGNAL(pressed()), this, SLOT(camera_play_clicked()));

	connect(ui->startButton, SIGNAL(pressed()), this, SLOT(startButton_pressed()));
	connect(ui->generateButton, SIGNAL(pressed()), this, SLOT(generateButton_pressed()));
	connect(ui->showButton, SIGNAL(pressed()), this, SLOT(showButton_pressed()));
	connect(ui->showButton_1, SIGNAL(clicked()), this, SLOT(showpcl_pressed()));

	//connect(ui->input_camera_height, SIGNAL(editingFinished()), this, SLOT(camera_height_ok()));
	//connect(ui->input_shooting_radius, SIGNAL(editingFinished()), this, SLOT(shooting_radius_ok()));
	//connect(ui->input_shooting_times, SIGNAL(editingFinished()), this, SLOT(shooting_times_ok()));

    timer->start(1);
}
void osgqt::initwindow()
{
	osg::ref_ptr<BoxPicker> picker = new BoxPicker;
	ViewerWindow->addEventHandler(picker.get());
	osg::ref_ptr<osg::Geometry> geo = new osg::Geometry;
	geo->setDataVariance(osg::Object::DYNAMIC);
	geo->setUseDisplayList(false);
	osg::Vec3Array* vertex = new osg::Vec3Array(4);
	(*vertex)[0] = osg::Vec3(-100.5, 0.0, -100.5);
	(*vertex)[1] = osg::Vec3(100.5, 0.0, -100.5);
	(*vertex)[2] = osg::Vec3(100.5, 0.0, 100.5);
	(*vertex)[3] = osg::Vec3(-100.5, 0.0, 100.5);
	geo->setVertexArray(vertex);

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0, 1.0, 0.0, 1.0));
	geo->setColorArray(colors);
	geo->setColorBinding(osg::Geometry::BIND_OVERALL);

	osg::Vec3Array* normal = new osg::Vec3Array(1);
	(*normal)[0] = osg::Vec3(0, -1, 0);
	geo->setNormalArray(normal);
	geo->setNormalBinding(osg::Geometry::BIND_OVERALL);

	osg::ref_ptr<osg::DrawArrays> pri = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, 4);

	geo->addPrimitiveSet(pri.get());

	osg::ref_ptr<osg::PolygonMode> polyMode = new osg::PolygonMode;
	polyMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
	geo->getOrCreateStateSet()->setAttributeAndModes(polyMode.get());
	geo->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	picker->geometry = geo;

	osg::Camera* camera = new osg::Camera;
	camera->setProjectionMatrix(osg::Matrix::ortho2D(-1.0, 1.0, -1.0, 1.0));
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrixAsLookAt(osg::Vec3(0, -1, 0), osg::Vec3(0, 0, 0), osg::Vec3(0, 0, 1));
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	camera->setAllowEventFocus(false);
	camera->setName("camera");
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(geo.get());

	camera->addChild(geode.get());
	root->addChild(camera);
	osg::Vec4 color;
	int num = 0;
	while (num<=214)
	{
		if (num <= 5)//每一个通道6个状态，相当于6进制算法，总共能得到216种颜色，除去黑色（模型）、全白（背景），有214种颜色
			color = osg::Vec4(0.2 * num, 0.0, 0., 1.0);
		else if (num <= 35)
			color = osg::Vec4(0.2 * (num % 6), 0.2 * (num / 6), 0, 1.0);
		else if (num <= 214)
			color = osg::Vec4(0.2 * (num % 6), 0.2 * (num / 6 % 6), 0.2 * (num / 36), 1.0);
		VColor.push_back(color);
		num++;
	}
	
}
osgqt::~osgqt(){
    // 注释：析构函数
    delete ui;
}
bool osgqt::isdof(osg::Node& node)
{
	osg::ref_ptr<osgSim::DOFTransform> dof = dynamic_cast<osgSim::DOFTransform*>(&node);
	if (dof != NULL) {
		return true;
	}
	return false;
}
void osgqt::viewmotion()
{
	QMap<osg::ref_ptr<osg::Node>,motionif>::iterator it;
	it = motion.begin();
	ui->setting_message->clear();
	ui->setting_message->append("motion part:amplitude,frequency;");
	while (it != motion.end())
	{
		ui->setting_message->append(QString("%1:%2,%3,%4,%5;")
			.arg(QString::fromStdString(it.key()->getName())).arg(it.value().motioninfo[0]).arg(it.value().motioninfo[1])
			.arg(it.value().motioninfo[2]).arg(it.value().motioninfo[3]));
		it++;
	}
}
void osgqt::motion_comfirm_clicked() //点击add按钮
{
	if(!motionfunc())return;
	ui->motion_amp->clear();
	ui->motion_amp_2->clear();
	ui->input_motion_name->clear();
	ui->input_motion_fre->clear();
	ui->input_motion_fre->setEnabled(false);
	ui->motion_amp_2->setEnabled(false);
	ui->motion_amp->setEnabled(false);
}
void osgqt::motion_play_clicked()
{
	if (ui->motion_play->text() == "animate")
	{
		if (motion.empty())
			return;
		for (QMap<osg::ref_ptr<osg::Node>,motionif>::iterator itr = motion.begin(); itr != motion.end(); itr++)
		{
			cb = new modelCallBack(itr.value().motioninfo[0], itr.value().motioninfo[1], itr.value().motioninfo[2], itr.value().motioninfo[3]);
			itr.key()->setUpdateCallback(cb);
			//lastmotion.append(itr.key());
		}
		ui->motion_play->setText("reset");
	}
	else if (ui->motion_play->text() == "reset")
	{
		for (QMap<osg::ref_ptr<osg::Node>, motionif>::iterator itr = motion.begin(); itr != motion.end(); itr++)
		{
			cb = new modelCallBack(0,0,0,0);
			itr.key()->setUpdateCallback(cb);
			osg::Node* x = itr.key();
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(x);
			osg::Vec3 reset = osg::Vec3(0, 0, 0);
			dofnode->setCurrentTranslate(reset);
			dofnode->setCurrentHPR(reset);
		}
		ui->motion_play->setText("animate");
		//lastmotion.clear();
	}
	viewmotion();
}
bool osgqt::motionfunc()
{
	QString fre = ui->input_motion_fre->text();
	double rtem=0,ttem=0;
	if(ui->motion_amp->isEnabled())
		rtem= ui->motion_amp->text().toDouble();
	if (ui->motion_amp_2->isEnabled())
		ttem = ui->motion_amp_2->text().toDouble();
	int fretem = fre.toInt();
	if (rtem<0 ||ttem<0|| fretem <= 0)
	{
		QMessageBox msgBox;
		msgBox.setText("Please input data or data must be positive!");
		msgBox.exec();
		//ui->setting_message->append("please input motion amplitude or motion frequency!");
		return false;
	}
	
	if (selectedItemList.empty())
	{
		QMessageBox msgBox;
		msgBox.setText("Please click the dof in tree widget to add a motion part!");
		msgBox.exec();
		return false;
	}
	
	for (int i = 0; i < selectedItemList.size(); ++i)
	{
		osg::ref_ptr<osg::Node> getnode = item_node.find(selectedItemList.at(i)).value();
		bool f = false;
		if (isdof(*getnode))
		{
			if (getnode->getName().length() >= 2 && (getnode->getName().substr(getnode->getName().length() - 2, 2) == "_r"))
			{
				motionif x;
				x.motioninfo[0] = 0;
				x.motioninfo[1] = rtem;
				x.motioninfo[2] = 0;
				x.motioninfo[3] = fretem;
				motion.insert(getnode, x);
			}
			else if (getnode->getName().length() >= 2 && (getnode->getName().substr(getnode->getName().length() - 2, 2) == "_t"))
			{
				motionif x;
				x.motioninfo[0] = ttem;
				x.motioninfo[1] = 0;
				x.motioninfo[2] = 0;
				x.motioninfo[3] = fretem;
				motion.insert(getnode, x);
			}
			else if (getnode->getName().length() >= 3 && (getnode->getName().substr(getnode->getName().length() - 3, 3) == "_rt"))
			{
				motionif x;
				x.motioninfo[0] = ttem;
				x.motioninfo[1] = rtem;
				x.motioninfo[2] = 0;
				x.motioninfo[3] = fretem;
				motion.insert(getnode, x);
			}
			else
			{
				QString ques = QString("%1 is not a dof part\nskip").arg(QString::fromStdString(getnode->getName()));
				QMessageBox messageBox(QMessageBox::NoIcon,
					"critical", ques);
				messageBox.exec();
			}
		}
		else
		{
			QString ques = QString("%1 is not a dof part\nskip").arg(QString::fromStdString(getnode->getName()));
			QMessageBox messageBox(QMessageBox::NoIcon,
				"critical", ques);
			messageBox.exec();
		}
	}
	viewmotion();
	return true;
}
void osgqt::motion_delete_clicked()
{
	//dofnodes[dof_ID]->removeUpdateCallback(cb);
   // osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(dofnodes[dof_ID]);
	if (selectedItemList.empty())
	{
		QMessageBox msgBox;
		msgBox.setText("Please click the dof in tree widget to delete a motion part!");
		msgBox.exec();
		return;
	}
	if (ui->motion_play->text() == "reset")//delete motion信息前reset一下animate
	{
		for (QMap<osg::ref_ptr<osg::Node>, motionif>::iterator itr = motion.begin(); itr != motion.end(); itr++)
		{
			cb = new modelCallBack(0, 0, 0, 0);
			itr.key()->setUpdateCallback(cb);
			osg::Node* x = itr.key();
			osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(x);
			osg::Vec3 reset = osg::Vec3(0, 0, 0);
			dofnode->setCurrentHPR(reset);
		}
		ui->motion_play->setText("animate");
		//lastmotion.clear();
	}
	for (int i = 0; i < selectedItemList.size(); ++i)
	{
		osg::ref_ptr<osg::Node> getnode = item_node.find(selectedItemList.at(i)).value();
		motion.remove(getnode);
	}
	viewmotion();
	treewidget->clearSelection();//注意：清除树形图item的文字点选，清除后文字点选添加的selectedItemList也会变空
}
void osgqt::camera_confirm_clicked() 
{
	QString h=ui->input_camera_height->text();
	QString r = ui->input_shooting_radius->text();
	QString n = ui->input_shooting_times->text();
	if (h == "" || r == "" || n == "")
	{
		QMessageBox msgBox;
		msgBox.setText("Please input camera's parameter!");
		msgBox.exec();
		return;
	}
	camera_height = h.toFloat();
	shooting_radius = r.toFloat();
	shooting_times = n.toFloat();
	ui->input_camera_height->setDisabled(true);
	ui->input_shooting_radius->setDisabled(true);
	ui->input_shooting_times->setDisabled(true);
}
void osgqt::camera_delete_clicked()
{
	removenode("kinectMT");
	removenode("viewportMT");
	removenode("circle");
	ui->input_camera_height->clear();
	ui->input_shooting_radius->clear();
	ui->input_shooting_times->clear();
	camera_height = 0;
	shooting_radius = 0;
	shooting_times = 0;
	ui->camera_play->setText("animate");
	ui->input_camera_height->setDisabled(false);
	ui->input_shooting_radius->setDisabled(false);
	ui->input_shooting_times->setDisabled(false);
}
void osgqt::removenode(std::string name)//用于删除camera和circle
{
	for (int i = 0; i < root->getNumChildren(); i++)
	{
		std::string x = root->getChild(i)->getName();
		if (x == name)
		{
			root->removeChild(i);
			i--;
		}
	}
}

void osgqt::camera_play_clicked()
{
	if (ui->camera_play->text() == "animate")
	{
		QString h = ui->input_camera_height->text();
		QString r = ui->input_shooting_radius->text();
		QString n = ui->input_shooting_times->text();

		if (h == "" || r == "" || n == "")
		{
			QMessageBox msgBox;
			msgBox.setText("Please input all camera's parameter!");
			msgBox.exec();
			return;
		}
		shooting_times = n.toInt();
		camera_height = ui->input_camera_height->text().toDouble();
		shooting_radius = ui->input_shooting_radius->text().toDouble();
		//MT->removeChild(kinect);
		removenode("kinectMT");
		removenode("viewportMT");
		removenode("circle");
		placecamera();
		ui->camera_play->setText("clear");
	}
	else if (ui->camera_play->text() == "clear")
	{
		removenode("kinectMT");
		removenode("viewportMT");
		removenode("circle");
		ui->camera_play->setText("animate");
	}
}
void osgqt::placecamera()
{
	float thea = 2 * osg::PI / shooting_times;
	for (int i = 0; i < shooting_times; i++)
	{
		osg::Vec3d eyes = osg::Vec3d(shooting_radius * cos(i * thea), shooting_radius * sin(i * thea), camera_height);
		osg::Vec3d	center = osg::Vec3d(0.0, 0.0, 0.0);
		osg::Vec3d up = osg::Vec3d(0.0, 0.0, -1.0);
		ViewerWindow->getCamera()->setViewMatrixAsLookAt(eyes, center, up);
		osg::Matrix V = ViewerWindow->getCamera()->getViewMatrix(); // 视图矩阵
		osg::Matrix Vni = osg::Matrix::inverse(V);
		osg::Matrix Vtrans = osg::Matrix
			(1, 0, 0, 0,
			0, -1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
		osg::Matrix Vf = Vni * Vtrans;
		osg::Matrix m = osg::Matrix::scale(1, 1, 1) * Vf;
		//osg::Matrix m = osg::Matrix::rotate(osg::inDegrees(90.0f),0.0f, 1.0f, 0.0f)* osg::Matrix::rotate(osg::inDegrees(-90.0f), 1.0f, 0.0f, 0.0f)
			//* osg::Matrix::scale(1, 1, 1)
			//* osg::Matrix::translate(0.5f, 0, 0);
		osg::MatrixTransform* MT = new osg::MatrixTransform;
		osg::MatrixTransform* MT1 = new osg::MatrixTransform;
		MT->setName("kinectMT");
		MT->setMatrix(m);
		MT->addChild(kinect);
		MT1->setName("viewportMT");
		m = osg::Matrix::scale(0.1, 0.1, 0.1) * m;
		MT1->setMatrix(m);
		MT1->addChild(viewport);
		root->addChild(MT);
		root->addChild(MT1);
		ViewerWindow->setSceneData(root.get());
	}
	drawCircle();
}
void osgqt::drawCircle()
{
	//设置线宽
	osg::ref_ptr<osg::LineWidth> lineSize = new osg::LineWidth;
	lineSize->setWidth(1.0);

	//获取根节点
	//root = new osg::Group;
	osg::ref_ptr<osg::StateSet> stateSet = root->getOrCreateStateSet();
	//打开线宽属性
	stateSet->setAttributeAndModes(lineSize, osg::StateAttribute::ON);

	//存放所有圆盘上的点，把这些点连接成直线画成圆盘
	osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array;
	
	
	//标牌数字
	//osg::ref_ptr<osg::Geometry> numPoint = new osg::Geometry;
	//clockGeode->removeChild(clockGeometry);
	clockGeode->addChild(clockGeometry);
	//clockGeode->addChild(numPoint);
	//root->removeChild(clockGeode.get());
	clockGeode->setName("circle");
	root->addChild(clockGeode);
	//数字节点顶点
	//osg::ref_ptr<osg::Vec3Array> num = new osg::Vec3Array;

	//表盘颜色
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	//表针颜色
	//osg::ref_ptr<osg::Vec4Array> colors2 = new osg::Vec4Array;
	//加入颜色得到十二个数字的顶点位置
	//for (double i = 0; i < 6.28; i += 0.52) {
	//	num->push_back(osg::Vec3(45 * sin(i), -0.0, 45 * cos(i)));
	//}

	//得到半径为50的钟表的314个点，这些点形成线坐标表盘
	for (double i = 0.0; i < 6.28; i += 0.02) {
		colors->push_back(osg::Vec4f(1, 0, 0, 1.0));
		allPoints->push_back(osg::Vec3f(shooting_radius * sin(i), shooting_radius * cos(i), camera_height));
	}

	//设置顶点
	clockGeometry->setVertexArray(allPoints);

	//画线
	clockGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, allPoints->size()));


	//clockGeometry->addPrimitiveSet(new osg::DrawElementsUInt(osg::PrimitiveSet::LINE_LOOP, 0));
	clockGeometry->setColorArray(colors);
	clockGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	//osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	//viewer->addEventHandler(new osgViewer::WindowSizeHandler());
	//root->addChild(osgDB::readNodeFile("cow.osg"));
	ViewerWindow->setSceneData(root.get());
}
void osgqt::changecolor(osg::ref_ptr<osg::Node> node, osg::Vec4 Color,bool open_transparent)//没用到
{
	osg::ref_ptr<osg::StateSet> now_state = new osg::StateSet;

	osg::ref_ptr<osg::Material> material = new osg::Material;
	material->setAmbient(osg::Material::FRONT_AND_BACK, Color);
	material->setDiffuse(osg::Material::FRONT_AND_BACK, Color);

	now_state->setMode(GL_BLEND, osg::StateAttribute::ON);
	now_state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	//设置网格模型
	//osg::ref_ptr<osg::PolygonMode> polyMode=new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);
	now_state->setAttributeAndModes(material.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	node->setStateSet(now_state);

	/*osg::ref_ptr<osg::Geode> geo = node->asGeode();
	
	osg::ref_ptr<osg::Vec4Array> chcolor = new osg::Vec4Array();//根据changecolor中存储的part类型名为此geode上色
	chcolor->push_back(Color);
	for (int i = 0; i < geo->getNumDrawables(); i++)
	{
		osg::ref_ptr<osg::Geometry> geometry = geo->getDrawable(i)->asGeometry();
		geometry->setColorArray(chcolor);
		geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

		osg::ref_ptr< osg::StateSet > state_set = geometry->getOrCreateStateSet();//设置geode的材质（这样设置相当于所有的geode的材质相同，如果每个part材质不同可能效果更好？）
		osg::ref_ptr< osg::Material > material = new osg::Material;
		material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
		material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.75f, 0.75f, 0.75f, 0.5f));
		material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.75f, 0.75f, 0.75f, 0.5f));
		material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.75f, 0.75f, 0.75f, 0.5f));
		material->setShininess(osg::Material::FRONT_AND_BACK, 51.2f);
		state_set->setAttributeAndModes(material.get(), osg::StateAttribute::ON);*/

		/*if (open_transparent)
		{
			//只对模型中的部件进行半透明化，可以在遍历模型部件时，1）设置显示颜色alpha通道小于1，2）开启融合模型，3）同时渲染方式设为透明方式
			//开启融合操作
			state_set->setMode(GL_BLEND, osg::StateAttribute::ON);
			//设置渲染模式
			state_set->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		}
	}*/
}
void osgqt::startButton_pressed() //开始拍照
{
	QString directory;
	//QString current_path = QDir::currentPath();
	if(savepicpath!="")
		directory = QFileDialog::getExistingDirectory(this, tr("Choose Save Path"), savepicpath);
	else if(savepicpath != "")
		directory = QFileDialog::getExistingDirectory(this, tr("Choose Save Path"), savepclpath);
	else directory = QFileDialog::getExistingDirectory(this, tr("Choose Save Path"), "./");
	savepicpath = directory;
	if (savepicpath == "")
		return;
	if (ui->input_camera_height->isEnabled()||motion.empty())
	{
		QMessageBox msgBox;
		msgBox.setText("Please confirm the camera or add motion information!");
		msgBox.exec();
		return;
	}
	QString ques = "motion part:amplitude,frequency;\n";
	
	for (QMap<osg::ref_ptr<osg::Node>, motionif>::iterator it=motion.begin();it != motion.end();it++)
	{	
		ques += QString("%1:%2,%3,%4,%5;\n").arg(QString::fromStdString(it.key()->getName())).arg(it->motioninfo[0]).arg(it->motioninfo[1])
			.arg(it->motioninfo[2]).arg(it->motioninfo[3]);
	}
	ques += QString("camera height:%1,camera radius:%2,camera times:%3;\n\n").arg(camera_height).arg(shooting_radius).arg(shooting_times);
	ques += QString("Continue shooting?");
	QMessageBox messageBox(QMessageBox::NoIcon,
		"critical", ques,
		QMessageBox::Yes | QMessageBox::No, NULL);
	int result = messageBox.exec();
	if (result == QMessageBox::No)
	{
		return;
	}
	savepicpath += "/";
	std::string savepicpath1 = savepicpath.toStdString();
	std::string sub1 = savepicpath1 + "pose";
	std::string sub2 = savepicpath1 + "depth";
	std::string sub3 = savepicpath1 + "label";
	if (0 != access(sub1.c_str(), 0))
	{ // if this folder not exist, create a new one.
		mkdir(sub1.c_str());   // 返回 0 表示创建成功，-1 表示失败
		//换成 ::_mkdir  ::_access 也行，不知道什么意思
	}
	if (0 != access(sub2.c_str(), 0))
	{
		mkdir(sub2.c_str());
	}
	if (0 != access(sub3.c_str(), 0))
	{
		mkdir(sub3.c_str());
	}
	/*pg.Tool_test_1(filepath.toStdString(), savepicpath.toStdString(), motionname.toStdString(), 
		motion[0], motion[1], camera_height, shooting_radius, shooting_times);*/
		//模型和motion部件颜色+光照设置
	removenode("kinectMT");
	removenode("camera");
	removenode("circle");
	removenode("viewportMT");
	ui->camera_play->setText("animate");
	ui->motion_play->setText("reset");//拍照之前先得将模型复位
	motion_play_clicked();
	for (int i = 0; i < root->getNumChildren(); i++)
	{	
		osg::ref_ptr<osg::Node> tran = root->getChild(i);
		if (tran->getName() == "trans")
		{
			
			//changecolor(tran->asGroup()->getChild(0), Color,false);
			NodeVisitor_dyer dyerb(osg::Vec4(0.0, 0.0, 0.0, 1.0), false); //不开启透明  
			tran->asGroup()->getChild(0)->accept(dyerb); //给所有model设置整体为黑色
			/*osg::Vec4 Color(0,0,0,1.f);
			osg::Callback * cb1=new modelCallBack1(Color, false);
			tran->asGroup()->getChild(0)->setUpdateCallback(cb1);*/
			osg::StateSet* state = tran->asGroup()->getChild(0)->getOrCreateStateSet();
			state->setMode(GL_CULL_FACE, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);   // 只关闭背面裁剪，造成生成背面不透明，但黑面;
			state->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);//关闭光照
			state->setMode(GL_LIGHT0, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
		}
	}
	std::string output_path_p = savepicpath1 + "pose\\ani_color_matrix_map"+ ".txt";
	std::ofstream outfile(output_path_p);
	int num = 1;
	for (QMap<osg::ref_ptr<osg::Node>, motionif>::iterator it = motion.begin(); it != motion.end(); it++,num++)
	//为不同运动部件设置不同颜色，并且输出颜色和相应运动部件的转换矩阵到txt文件
	{
		
		osg::Vec4 color;
		if (num <= 5)//每一个通道6个状态，相当于6进制算法，总共能得到216种颜色，除去黑色（模型）、全白（背景），有214种颜色
			color = osg::Vec4(0.2 * num, 0.0, 0., 1.0);
		else if(num <= 35)
			color = osg::Vec4(0.2*(num %6),0.2*(num /6),0,1.0);
		else if(num <=214)
			color = osg::Vec4(0.2 * (num % 6), 0.2 * (num /6%6), 0.2*(num /36), 1.0);
		NodeVisitor_dyer dyerr(color, false); //不开启透明
		it.key()->accept(dyerr); //运动部件设置颜色
		osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>((osg::Node *)it.key());
		osg::Matrix PutMatrix = dofnode->getPutMatrix();
		//osg::Vec3d start = dofnode->getInversePutMatrix().getTrans();
		if (it.key()->getName().length() >= 2 && (it.key()->getName().substr(it.key()->getName().length() - 2, 2) == "_r"))
		{
			outfile << "1" <<" "<<it->motioninfo[1] << " " << it->motioninfo[3] << " " << "0" << endl;
		}
		else if (it.key()->getName().length() >= 2 && (it.key()->getName().substr(it.key()->getName().length() - 2, 2) == "_t"))
		{
			outfile << "2" << " " << it->motioninfo[0] << " " << it->motioninfo[3] << " " << "0" << endl;
		}
		else if (it.key()->getName().length() >= 3 && (it.key()->getName().substr(it.key()->getName().length() -3, 3) == "_rt"))
		{
			outfile << "3" << " " << it->motioninfo[0] << " " << it->motioninfo[1] << " " << it->motioninfo[3] << endl;
		}
		for (int m = 0; m < 4; m++)
		{
			outfile << color[m] << " ";
		}
		outfile << endl;
		for (int m = 0; m < 4; m++) {
			for (int n = 0; n < 4; n++) {
				outfile << PutMatrix(m, n) << " ";
			}
			outfile << std::endl;
		}
	}
	osg::ref_ptr<osg::Vec3dArray> eye = new osg::Vec3dArray();
	float thea = 2 * osg::PI / shooting_times;		
	for (int i = 0; i < shooting_times; i++)
	{
		eye->push_back(osg::Vec3d(shooting_radius * cos(i * thea), shooting_radius * sin(i * thea), camera_height));
	}
	bool ismove = true;//若部件还在运动，那么flag会被置为true,先假设为true，先拍一张静态
	double i = 0.000001;//记录拍了i组照；
	while (ismove)
	{
		
		for (int j = 0; j < shooting_times; j++)
		{
			//开始布置每个相机
			osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
			viewer->setSceneData(root.get());//or roottrans
			int pixelwidth = 1080, pixelheight = 1040;
			osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
			traits->x = 0;
			traits->y = 0;
			traits->width = pixelwidth;
			traits->height = pixelheight;
			traits->windowDecoration = true;
			traits->doubleBuffer = true;
			traits->sharedContext = 0;
			traits->samples = 0; //抗锯齿 像素采样率
			traits->pbuffer = true; //离屏渲染
			osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
			viewer->getCamera()->setClearColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));      //         底色白色 255 255 255
			viewer->getCamera()->setGraphicsContext(gc);
			gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			viewer->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
			double fovy = 30.f, aspectRatio = double(traits->width) / double(traits->height), zNear = 1.0, zFar = 10.0;
			viewer->getCamera()->setProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
			//osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();
			unsigned int  width = gc->getTraits()->width, height = gc->getTraits()->height;
			//wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
			viewer->realize();
			osg::Vec3d eyes = eye->at(j), center = osg::Vec3d(0.0, 0.0, 0.0), up = osg::Vec3d(0.0, 0.0, 1.0);
			viewer->getCamera()->setViewMatrixAsLookAt(eyes, center, up);
			//保存图片和参数
			std::string output_path_p = savepicpath1 + "pose\\p_ani_" + std::to_string((long double)i) + "_photo_" + std::to_string((long double)j) + ".txt";
			std::ofstream outfile(output_path_p);
			osg::ref_ptr<osg::Image> image_d = new osg::Image();
			std::string output_path_d = savepicpath1 + "depth\\d_ani_" + std::to_string((long double)i) + "_photo_" + std::to_string((long double)j) + ".png";
			osg::ref_ptr<osg::Image> image_rgb = new osg::Image();
			std::string output_path_l = savepicpath1 + "label\\l_ani_" + std::to_string((long double)i) + "_photo_" + std::to_string((long double)j) + ".png";
			image_d->allocateImage(width, height, 1, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE);
			viewer->getCamera()->attach(osg::Camera::DEPTH_BUFFER, image_d.get());
			image_rgb->allocateImage(width, height, 1, GL_RGB, GL_UNSIGNED_BYTE);
			viewer->getCamera()->attach(osg::Camera::COLOR_BUFFER, image_rgb.get());
			viewer->frame();
			Sleep(2000);
			//viewer->run();
			osg::Matrix V = viewer->getCamera()->getViewMatrix(); // 视图矩阵
			osg::Matrix P = viewer->getCamera()->getProjectionMatrix(); //投影矩阵
			osg::Matrix W = viewer->getCamera()->getViewport()->computeWindowMatrix(); //窗口变换矩阵
			osg::Matrix PW = P * W;
			for (int m = 0; m < 4; m++) {
				for (int n = 0; n < 4; n++) {
					outfile << V(m, n) << " ";
				}
				outfile << std::endl;
			}
			for (int m = 0; m < 4; m++) {
				for (int n = 0; n < 4; n++) {
					outfile << P(m, n) << " ";
				}
				outfile << std::endl;
			}
			for (int m = 0; m < 4; m++) {
				for (int n = 0; n < 4; n++) {
					outfile << W(m, n) << " ";
				}
				outfile << std::endl;
			}
			outfile.close();
			osgDB::writeImageFile(*(image_d.get()), output_path_d);
			osgDB::writeImageFile(*(image_rgb.get()), output_path_l);
			i+=0.000001;
		}
		ismove = false;
		for (QMap<osg::ref_ptr<osg::Node>, motionif>::iterator itr = motion.begin(); itr != motion.end(); itr++)//遍历运动部件并且运动
		{
			move(itr.key(), itr->motioninfo,ismove);
		}
	}
	//模型和motion部件颜色+光照设置
	for (int i = 0; i < root->getNumChildren(); i++)
	{
		osg::ref_ptr<osg::Node> tran = root->getChild(i);
		if (tran->getName() == "trans")
		{
			NodeVisitor_dyer dyerb(init_state_set);
			tran->asGroup()->getChild(0)->accept(dyerb);
			//tran->asGroup()->getChild(0)->setStateSet(init_state_set);
		}
	}
	ui->motion_play->setText("reset");
	on_Reset_clicked();
	motion_play_clicked();
	QMessageBox msgBox;
	msgBox.setText("output finished!");
	msgBox.exec();

}
void osgqt::autostartshoot() //开始拍照
{
	savepicpath += "/";
	std::string savepicpath1 = savepicpath.toStdString();
	std::string sub1 = savepicpath1 + "pose";
	std::string sub2 = savepicpath1 + "depth";
	std::string sub3 = savepicpath1 + "label";
	if (0 != access(sub1.c_str(), 0))
	{ // if this folder not exist, create a new one.
		mkdir(sub1.c_str());   // 返回 0 表示创建成功，-1 表示失败
		//换成 ::_mkdir  ::_access 也行，不知道什么意思
	}
	if (0 != access(sub2.c_str(), 0))
	{
		mkdir(sub2.c_str());
	}
	if (0 != access(sub3.c_str(), 0))
	{
		mkdir(sub3.c_str());
	}
	/*pg.Tool_test_1(filepath.toStdString(), savepicpath.toStdString(), motionname.toStdString(),
		motion[0], motion[1], camera_height, shooting_radius, shooting_times);*/
		//模型和motion部件颜色+光照设置
	removenode("kinectMT");
	removenode("camera");
	removenode("circle");
	removenode("viewportMT");
	ui->camera_play->setText("animate");
	ui->motion_play->setText("reset");//拍照之前先得将模型复位
	motion_play_clicked();
	shooting_times = 4;
	shooting_radius = 8;
	camera_height = 2;
	for (int i = 0; i < root->getNumChildren(); i++)
	{
		osg::ref_ptr<osg::Node> tran = root->getChild(i);
		if (tran->getName() == "trans")
		{

			//changecolor(tran->asGroup()->getChild(0), Color,false);
			NodeVisitor_dyer dyerb(osg::Vec4(0.0, 0.0, 0.0, 1.0), false); //不开启透明  
			tran->asGroup()->getChild(0)->accept(dyerb); //给所有model设置整体为黑色
			/*osg::Vec4 Color(0,0,0,1.f);
			osg::Callback * cb1=new modelCallBack1(Color, false);
			tran->asGroup()->getChild(0)->setUpdateCallback(cb1);*/
			osg::StateSet* state = tran->asGroup()->getChild(0)->getOrCreateStateSet();
			state->setMode(GL_CULL_FACE, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);   // 只关闭背面裁剪，造成生成背面不透明，但黑面;
			state->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);//关闭光照
			state->setMode(GL_LIGHT0, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
		}
	}
	std::string output_path_p = savepicpath1 + "pose\\ani_color_matrix_map" + ".txt";
	std::ofstream outfile(output_path_p);
	int num = 1;
	for (QMap<osg::ref_ptr<osg::Node>, motionif>::iterator it = motion.begin(); it != motion.end(); it++, num++)
		//为不同运动部件设置不同颜色，并且输出颜色和相应运动部件的转换矩阵到txt文件
	{

		osg::Vec4 color;
		if (num <= 5)//每一个通道6个状态，相当于6进制算法，总共能得到216种颜色，除去黑色（模型）、全白（背景），有214种颜色
			color = osg::Vec4(0.2 * num, 0.0, 0., 1.0);
		else if (num <= 35)
			color = osg::Vec4(0.2 * (num % 6), 0.2 * (num / 6), 0, 1.0);
		else if (num <= 214)
			color = osg::Vec4(0.2 * (num % 6), 0.2 * (num / 6 % 6), 0.2 * (num / 36), 1.0);
		NodeVisitor_dyer dyerr(color, false); //不开启透明
		it.key()->accept(dyerr); //运动部件设置颜色
		osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>((osg::Node*)it.key());
		osg::Matrix PutMatrix = dofnode->getPutMatrix();
		osg::ref_ptr <osg::Node> nod=findnodepar(dofnode);
		osg::ref_ptr<osg::MatrixTransform> tran = dynamic_cast<osg::MatrixTransform *>((osg::Node*)nod);
		osg::Matrix modelMatrix = tran->getMatrix();
		//osg::Vec3d start = dofnode->getInversePutMatrix().getTrans();
		if (it.key()->getName().length() >= 2 && (it.key()->getName().substr(it.key()->getName().length() - 2, 2) == "_r"))
		{
			outfile << "1" << " " << it->motioninfo[1] << " " << it->motioninfo[3] << " " << "0" << endl;
		}
		else if (it.key()->getName().length() >= 2 && (it.key()->getName().substr(it.key()->getName().length() - 2, 2) == "_t"))
		{
			outfile << "2" << " " << it->motioninfo[0] << " " << it->motioninfo[3] << " " << "0" << endl;
		}
		else if (it.key()->getName().length() >= 3 && (it.key()->getName().substr(it.key()->getName().length() - 3, 3) == "_rt"))
		{
			outfile << "3" << " " << it->motioninfo[0] << " " << it->motioninfo[1] << " " << it->motioninfo[3] << endl;
		}
		for (int m = 0; m < 4; m++)
		{
			outfile << color[m] << " ";
		}
		outfile << endl;
		for (int m = 0; m < 4; m++) {//输出dof的变换矩阵
			for (int n = 0; n < 4; n++) {
				outfile << PutMatrix(m, n) << " ";
			}
			outfile << std::endl;
		}
		for (int m = 0; m < 4; m++) {//输出模型的变换矩阵
			for (int n = 0; n < 4; n++) {
				outfile << modelMatrix(m, n) << " ";
			}
			outfile << std::endl;
		}
	}
	osg::ref_ptr<osg::Vec3dArray> eye = new osg::Vec3dArray();
	float thea = 2 * osg::PI / shooting_times;
	for (int i = 0; i < shooting_times; i++)
	{
		eye->push_back(osg::Vec3d(shooting_radius * cos(i * thea), shooting_radius * sin(i * thea), camera_height));
	}
	bool ismove = true;//若部件还在运动，那么flag会被置为true,先假设为true，先拍一张静态
	double i = 0.000001;//记录拍了i组照；
	
	while (ismove)
	{	//开始布置每个相机
		
		for (int j = 0; j < shooting_times; j++)
		{
			osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
			viewer->setSceneData(root.get());//or roottrans
			int pixelwidth = 1080, pixelheight = 1040;
			osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
			traits->x = 0;
			traits->y = 0;
			traits->width = pixelwidth;
			traits->height = pixelheight;
			traits->windowDecoration = true;
			traits->doubleBuffer = true;
			traits->sharedContext = 0;
			traits->samples = 0; //抗锯齿 像素采样率
			traits->pbuffer = true; //离屏渲染
			osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
			viewer->getCamera()->setClearColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));      //         底色白色 255 255 255
			viewer->getCamera()->setGraphicsContext(gc);
			gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			viewer->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
			double fovy = 30.f, aspectRatio = double(traits->width) / double(traits->height), zNear = 1.0, zFar = 10.0;
			viewer->getCamera()->setProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
			//osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();
			unsigned int  width = gc->getTraits()->width, height = gc->getTraits()->height;
			//wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
			viewer->realize();
			osg::Vec3d eyes = eye->at(j), center = osg::Vec3d(0.0, 0.0, 0.0), up = osg::Vec3d(0.0, 0.0, 1.0);
			viewer->getCamera()->setViewMatrixAsLookAt(eyes, center, up);
			//保存图片和参数
			std::string output_path_p = savepicpath1 + "pose\\p_ani_" + std::to_string((long double)i) + "_photo_" + std::to_string((long double)j) + ".txt";
			std::ofstream outfile(output_path_p);
			osg::ref_ptr<osg::Image> image_d = new osg::Image();
			std::string output_path_d = savepicpath1 + "depth\\d_ani_" + std::to_string((long double)i) + "_photo_" + std::to_string((long double)j) + ".png";
			osg::ref_ptr<osg::Image> image_rgb = new osg::Image();
			std::string output_path_l = savepicpath1 + "label\\l_ani_" + std::to_string((long double)i) + "_photo_" + std::to_string((long double)j) + ".png";
			image_d->allocateImage(width, height, 1, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE);
			viewer->getCamera()->attach(osg::Camera::DEPTH_BUFFER, image_d.get());
			image_rgb->allocateImage(width, height, 1, GL_RGB, GL_UNSIGNED_BYTE);
			viewer->getCamera()->attach(osg::Camera::COLOR_BUFFER, image_rgb.get());
			viewer->frame();
			Sleep(1500);
			//viewer->run();
			osg::Matrix V = viewer->getCamera()->getViewMatrix(); // 视图矩阵
			osg::Matrix P = viewer->getCamera()->getProjectionMatrix(); //投影矩阵
			osg::Matrix W = viewer->getCamera()->getViewport()->computeWindowMatrix(); //窗口变换矩阵
			osg::Matrix PW = P * W;
			for (int m = 0; m < 4; m++) {
				for (int n = 0; n < 4; n++) {
					outfile << V(m, n) << " ";
				}
				outfile << std::endl;
			}
			for (int m = 0; m < 4; m++) {
				for (int n = 0; n < 4; n++) {
					outfile << P(m, n) << " ";
				}
				outfile << std::endl;
			}
			for (int m = 0; m < 4; m++) {
				for (int n = 0; n < 4; n++) {
					outfile << W(m, n) << " ";
				}
				outfile << std::endl;
			}
			outfile.close();
			osgDB::writeImageFile(*(image_d.get()), output_path_d);
			osgDB::writeImageFile(*(image_rgb.get()), output_path_l);
			i += 0.000001;
		}
		ismove = false;
		for (QMap<osg::ref_ptr<osg::Node>, motionif>::iterator itr = motion.begin(); itr != motion.end(); itr++)//遍历运动部件并且运动
		{
			move(itr.key(), itr->motioninfo, ismove);
		}
	}
	//模型和motion部件颜色+光照设置
	for (int i = 0; i < root->getNumChildren(); i++)
	{
		osg::ref_ptr<osg::Node> tran = root->getChild(i);
		if (tran->getName() == "trans")
		{
			NodeVisitor_dyer dyerb(init_state_set);
			tran->asGroup()->getChild(0)->accept(dyerb);
			//tran->asGroup()->getChild(0)->setStateSet(init_state_set);
		}
	}
	ui->motion_play->setText("reset");
	motion_play_clicked();
	on_Reset_clicked();
	/*QMessageBox msgBox;
	msgBox.setText("output finished!");
	msgBox.exec();*/
}
void osgqt::autorgbshoot()
{
	savepicpath += "/";
	std::string savepicpath1 = savepicpath.toStdString();
	std::string sub1 = savepicpath1 + "rgb";
	if (0 != access(sub1.c_str(), 0))
	{ // if this folder not exist, create a new one.
		mkdir(sub1.c_str());   // 返回 0 表示创建成功，-1 表示失败
		//换成 ::_mkdir  ::_access 也行，不知道什么意思
	}
	removenode("kinectMT");
	removenode("camera");
	removenode("circle");
	removenode("viewportMT");
	ui->camera_play->setText("animate");
	ui->motion_play->setText("reset");//拍照之前先得将模型复位
	motion_play_clicked();
	shooting_times = 2;
	shooting_radius = 6;
	camera_height = 3;
	int num = 1;
	qDebug() << "ranse gray 1";
	for (int i = 0; i < root->getNumChildren(); i++)
	{
		osg::ref_ptr<osg::Node> tran = root->getChild(i);
		if (tran->getName() == "trans")
		{
			//双面光照（打开）+背面裁剪(关闭状态机)----解决CAD设计的模型中面片法向不同而造成的透明现象----perfect!!!;
			osg::StateSet* state = tran->asGroup()->getChild(0)->getOrCreateStateSet();
			osg::ref_ptr<osg::LightModel> TwoSideLight = new osg::LightModel;
			TwoSideLight->setTwoSided(true);
			state->setMode(GL_CULL_FACE, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);   // 只关闭背面裁剪，造成生成背面不透明，但黑面;
			state->setAttributeAndModes(TwoSideLight, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);  //再加上双面光照，使背面完全出现;

			state->setMode(GL_LIGHTING, osg::StateAttribute::ON);
			state->setMode(GL_LIGHT0, osg::StateAttribute::ON);
			state->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
			//changecolor(tran->asGroup()->getChild(0), Color,false);
			NodeVisitor_dyer dyerb(osg::Vec4(0.7, 0.7, 0.7, 1.0),3); //不开启透明  
			tran->asGroup()->getChild(0)->accept(dyerb); //给所有model设置整体为黑色
			/*osg::Vec4 Color(0,0,0,1.f);
			osg::Callback * cb1=new modelCallBack1(Color, false);
			tran->asGroup()->getChild(0)->setUpdateCallback(cb1);*/
			/*osg::StateSet* state = tran->asGroup()->getChild(0)->getOrCreateStateSet();
			state->setMode(GL_CULL_FACE, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);   // 只关闭背面裁剪，造成生成背面不透明，但黑面;
			state->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);//关闭光照
			state->setMode(GL_LIGHT0, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);*/
		}
	}
	qDebug() << "ranse rgb";
	for (QMap<osg::ref_ptr<osg::Node>, motionif>::iterator it = motion.begin(); it != motion.end(); it++, num++)
		//为不同运动部件设置不同颜色
	{
		
		NodeVisitor_dyer dyerr(VColor[num],3); //不开启透明
		it.key()->accept(dyerr); //运动部件设置颜色
	}
	osg::ref_ptr<osg::Vec3dArray> eye = new osg::Vec3dArray();
	float thea = 2 * osg::PI / shooting_times;
	for (int i = 0; i < shooting_times; i++)
	{
		eye->push_back(osg::Vec3d(shooting_radius * cos(i * thea), shooting_radius * sin(i * thea), camera_height));
	}
	double i = 0.000001;//记录拍了i组照；
	qDebug() << "start shoot 1";
	//开始布置每个相机
	for (int j = 0; j < shooting_times; j++)
	{
		osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
		//viewer->getCamera()->setComputeNearFarMode( osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR );//启用该模式后 zNear zFar的值才是准确的，否则是默认自动计算远近剪裁面，zNear即使设置了也无效
		viewer->setSceneData(roottrans.get());
		int pixelwidth = 1080, pixelheight = 1040;
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		traits->x = 0;
		traits->y = 0;
		traits->width = pixelwidth;
		traits->height = pixelheight;
		traits->windowDecoration = true;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;
		traits->samples = 16; //抗锯齿 像素采样率
		traits->pbuffer = true; //离屏渲染
		osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
		viewer->getCamera()->setClearColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));      //         底色白色 255 255 255
		viewer->getCamera()->setGraphicsContext(gc);
		gc->setClearMask(GL_COLOR_BUFFER_BIT);
		viewer->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
		double fovy = 30.f, aspectRatio = double(traits->width) / double(traits->height), zNear = 1.0, zFar = 10.0;
		viewer->getCamera()->setProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
		//osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();
		unsigned int  width = gc->getTraits()->width, height = gc->getTraits()->height;
		//wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
		viewer->realize();
		osg::Vec3d eyes = eye->at(j), center = osg::Vec3d(0.0, 0.0, 0.0), up = osg::Vec3d(0.0, 0.0, 1.0);
		viewer->getCamera()->setViewMatrixAsLookAt(eyes, center, up);

		osg::ref_ptr<osg::Image> image_rgb = new osg::Image();
		std::string output_path_l = savepicpath.toStdString() + "rgb\\l_ani_" + std::to_string((long double)i) + "_photo_" + std::to_string((long double)j) + ".png";
		image_rgb->allocateImage(width, height, 1, GL_RGB, GL_UNSIGNED_BYTE);
		viewer->getCamera()->attach(osg::Camera::COLOR_BUFFER, image_rgb.get());
		viewer->frame();
		viewer->renderingTraversals();
		viewer->renderingTraversals();
		Sleep(2000);
		osgDB::writeImageFile(*(image_rgb.get()), output_path_l);
		i += 0.000001;
	}
	ui->motion_play->setText("reset");
	motion_play_clicked();
	on_Reset_clicked();
}
void osgqt::autorgbshoot1()
{
	savepicpath += "/";
	std::string savepicpath1 = savepicpath.toStdString();
	std::string sub1 = savepicpath1 + "rgb";
	if (0 != access(sub1.c_str(), 0))
	{ // if this folder not exist, create a new one.
		mkdir(sub1.c_str());   // 返回 0 表示创建成功，-1 表示失败
		//换成 ::_mkdir  ::_access 也行，不知道什么意思
	}
	removenode("kinectMT");
	removenode("camera");
	removenode("circle");
	removenode("viewportMT");
	ui->camera_play->setText("animate");
	ui->motion_play->setText("reset");//拍照之前先得将模型复位
	motion_play_clicked();
	shooting_times = 2;
	shooting_radius = 6;
	camera_height = 3;
	int num = 1;
	qDebug() << "open light";
			//双面光照（打开）+背面裁剪(关闭状态机)----解决CAD设计的模型中面片法向不同而造成的透明现象----perfect!!!;
			osg::StateSet* state = model->getOrCreateStateSet();
			osg::ref_ptr<osg::LightModel> TwoSideLight = new osg::LightModel;
			TwoSideLight->setTwoSided(true);
			state->setMode(GL_CULL_FACE, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);   // 只关闭背面裁剪，造成生成背面不透明，但黑面;
			state->setAttributeAndModes(TwoSideLight, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);  //再加上双面光照，使背面完全出现;

			state->setMode(GL_LIGHTING, osg::StateAttribute::ON);
			state->setMode(GL_LIGHT0, osg::StateAttribute::ON);
			state->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
			//changecolor(tran->asGroup()->getChild(0), Color,false);
			qDebug() << "ranse gray 2";
			NodeVisitor_dyer dyerb(osg::Vec4(0.7, 0.7, 0.7, 1.0), 3); //不开启透明  
			model->accept(dyerb); //给所有model设置整体为黑色
			qDebug() << "ranse rgb";
	for (QMap<osg::ref_ptr<osg::Node>, motionif>::iterator it = motion.begin(); it != motion.end(); it++, num++)
		//为不同运动部件设置不同颜色
	{
		
		NodeVisitor_dyer dyerr(VColor[num], 3); //不开启透明
		it.key()->accept(dyerr); //运动部件设置颜色
	}
	osg::ref_ptr<osg::Vec3dArray> eye = new osg::Vec3dArray();
	float thea = 2 * osg::PI / shooting_times;
	for (int i = 0; i < shooting_times; i++)
	{
		eye->push_back(osg::Vec3d(shooting_radius * cos(i * thea), shooting_radius * sin(i * thea), camera_height));
	}
	double i = 0.000001;//记录拍了i组照；
	qDebug() << "shoot2";
	//开始布置每个相机
	for (int j = 0; j < shooting_times; j++)
	{
		osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
		//viewer->getCamera()->setComputeNearFarMode( osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR );//启用该模式后 zNear zFar的值才是准确的，否则是默认自动计算远近剪裁面，zNear即使设置了也无效
		viewer->setSceneData(roottrans.get());
		int pixelwidth = 1080, pixelheight = 1040;
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		traits->x = 0;
		traits->y = 0;
		traits->width = pixelwidth;
		traits->height = pixelheight;
		traits->windowDecoration = true;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;
		traits->samples = 16; //抗锯齿 像素采样率
		traits->pbuffer = true; //离屏渲染
		osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
		viewer->getCamera()->setClearColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));      //         底色白色 255 255 255
		viewer->getCamera()->setGraphicsContext(gc);
		gc->setClearMask(GL_COLOR_BUFFER_BIT);
		viewer->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
		double fovy = 30.f, aspectRatio = double(traits->width) / double(traits->height), zNear = 1.0, zFar = 10.0;
		viewer->getCamera()->setProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
		//osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();
		unsigned int  width = gc->getTraits()->width, height = gc->getTraits()->height;
		//wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
		viewer->realize();
		osg::Vec3d eyes = eye->at(j), center = osg::Vec3d(0.0, 0.0, 0.0), up = osg::Vec3d(0.0, 0.0, 1.0);
		viewer->getCamera()->setViewMatrixAsLookAt(eyes, center, up);

		osg::ref_ptr<osg::Image> image_rgb = new osg::Image();
		std::string output_path_l = savepicpath.toStdString() + "rgb\\l_ani_" + std::to_string((long double)i) + "_photo_" + std::to_string((long double)j) + ".png";
		image_rgb->allocateImage(width, height, 1, GL_RGB, GL_UNSIGNED_BYTE);
		viewer->getCamera()->attach(osg::Camera::COLOR_BUFFER, image_rgb.get());
		viewer->frame();
		viewer->renderingTraversals();
		viewer->renderingTraversals();
		Sleep(2000);
		osgDB::writeImageFile(*(image_rgb.get()), output_path_l);
		i += 0.000001;
	}

	ui->motion_play->setText("reset");
	motion_play_clicked();
	on_Reset_clicked();
}
osg::ref_ptr<osg::Node> osgqt::findnodepar(osg::ref_ptr<osg::Node> node)//返回模型节点的trans父节点
{
	if (node->getName() == "root")return NULL;
	while (node)
	{
		if (node->getName()=="trans")
			break;
		node = node->getParent(0);
	} 
	return node;
}
void osgqt::move(osg::ref_ptr<osg::Node> node,double range[4], bool &ismove)
{
	std::string node_name = node->getName();
	osg::Node* node1 = node;
	osg::ComputeBoundsVisitor boundVisitor;
	osg::ref_ptr<osgSim::DOFTransform> dofnode = dynamic_cast<osgSim::DOFTransform*>(node1);
	dofnode->accept(boundVisitor);
	osg::Vec3 curvec;
	double r;
	for (int i = 0; i < 3; i++)
	{
		if (range[i] != 0)
		{
			r = range[i] / range[3];
			break;
		}
	}
	if (node_name.length() >= 2 && (node_name.substr(node_name.length() - 2, 2) == "_r"))
	{
		
		curvec = dofnode->getCurrentHPR();
		float ani_radians = osg::DegreesToRadians(r);
		float xani_range = osg::DegreesToRadians(range[0]);
		float yani_range = osg::DegreesToRadians(range[1]);
		float zani_range = osg::DegreesToRadians(range[2]);
		float xcur_rad = curvec.x();
		float ycur_rad = curvec.y();
		float zcur_rad = curvec.z();
		if(xcur_rad < xani_range)
		{
			curvec.set(xcur_rad + ani_radians, 0, 0);
			dofnode->setCurrentHPR(curvec);
			ismove = true;
		}
		if (ycur_rad < yani_range)
		{
			curvec.set(0, ycur_rad + ani_radians, 0);
			dofnode->setCurrentHPR(curvec);
			ismove = true;
		}
		if (zcur_rad < zani_range)
		{
			curvec.set(0, 0, zcur_rad + ani_radians);
			dofnode->setCurrentHPR(curvec);
			ismove = true;
		}
	}
	else if (node_name.length() >= 2 && (node_name.substr(node_name.length() - 2, 2) == "_t"))
	{
		curvec = dofnode->getCurrentTranslate();
		float xani_range = range[0];
		float yani_range = range[1];
		float zani_range = range[2];
		float xcur_rad = curvec.x();
		float ycur_rad = curvec.y();
		float zcur_rad = curvec.z();
		if (xcur_rad < xani_range)
		{
			curvec.set(xcur_rad + r, 0, 0);
			dofnode->setCurrentTranslate(curvec);
			ismove = true;
		}
		if (ycur_rad < yani_range)
		{
			curvec.set(0, ycur_rad + r, 0);
			dofnode->setCurrentTranslate(curvec);
			ismove = true;
		}
		if (zcur_rad < zani_range)
		{
			curvec.set(0, 0, zcur_rad + r);
			dofnode->setCurrentTranslate(curvec);
			ismove = true;
		}
	}
	else if (node_name.length() >= 3 && (node_name.substr(node_name.length() - 3, 3) == "_rt"))
	{
		curvec = dofnode->getCurrentHPR();
		curvec.set(0, curvec.y() + r, 0);
		dofnode->setCurrentHPR(curvec);
		curvec = dofnode->getCurrentTranslate();
		curvec.set(curvec.x() + r, 0,0);
		dofnode->setCurrentTranslate(curvec);
	}
}
void osgqt::generateButton_pressed() 
{
	/*QString directory;
	if (savepicpath == "")
	{
		directory = QFileDialog::getExistingDirectory(this, tr("Choose rgb and depth directory"), "./");
		savepicpath = directory;
	}
	else if (savepicpath != "")
		directory = QFileDialog::getExistingDirectory(this, tr("Choose directory to Save pointcloud data"), savepicpath);
	savepclpath = directory;
	if (savepclpath == "")
		return;
	// initialize matlab lib，这里必须做初始化！
	if (!Tool_test_2Initialize())
	{
		std::cout << "Could not initialize Tool_test_2!" << std::endl;
		return;
	}
	// 为变量分配内存空间，可以查帮助mwArray
	std::string path= savepicpath.toStdString()+"/";
	const char * str = path.c_str();
	//const char* str = "D:\\my\\sunxun\\test1";
	double anitimes = motion[1];
	double ani_count_plus1 = anitimes + 1;
	mwArray root_path(str);
	mwArray ani_count_mat(1, 1, mxDOUBLE_CLASS);// 1，1表示矩阵的大小（所有maltab只有一种变量，就是矩阵，为了和Cpp变量接轨，设置成1*1的矩阵，mxDOUBLE_CLASS表示变量的精度）
	mwArray photo_count_mat(1, 1, mxDOUBLE_CLASS);

	//调用类里面的SetData函数给类赋值
	ani_count_mat.SetData(&ani_count_plus1, 1);
	photo_count_mat.SetData(&shooting_times, 1);

	Tool_test_2(root_path, ani_count_mat, photo_count_mat);

	// 后面是一些终止调用的程序
	//Tool_test_2Terminate();
	// terminate MCR
	//mclTerminateApplication();*/
	
}
void osgqt::showButton_pressed() 
{
	/*showpic* picwindow = new showpic(NULL, savepicpath);
	picwindow->show();*/
	MainWindow* mainWindow;
	if(savepicpath=="")
		mainWindow =new MainWindow(NULL,"D:\\my\\sunxun\\test1",0);
	else
		mainWindow = new MainWindow(NULL, savepicpath,0);//flag=0表示显示图片
	mainWindow->resize(1000,800);
	mainWindow->setWindowIcon(QIcon("./images/logo.png"));
	mainWindow->show();
}
void osgqt::showpcl_pressed() 
{
	MainWindow* mainWindow;
	if (savepicpath == "")
		mainWindow = new MainWindow(NULL, "D:\\my\\sunxun\\test1",1);
	else
		mainWindow = new MainWindow(NULL, savepicpath,1);
	mainWindow->resize(1000, 800);
	mainWindow->setWindowIcon(QIcon("./images/logo.png"));
	mainWindow->show();
}


void osgqt::on_treeWidget_itemChanged_child(QTreeWidgetItem *item, int column)
{
    // 注释：用于树形图修改子节点勾选状态
    QString itemText = item->text(column);
    current_tree_item_name = itemText.toStdString();

    if(QString::compare(itemText,"3D Objects")==0)
        return;
    auto state_now = item->checkState(column);
    if(state_now==Qt::CheckState::PartiallyChecked)
        return;
    int childCount = item->childCount();
    if(childCount > 0)
    {
        for(int i = 0; i < childCount; i++)
        {
            item->child(i)->setCheckState(0,item->checkState(column));
            if(item->child(i)->childCount() > 0)
                on_treeWidget_itemChanged_child(item->child(i),column);
        }
    }
}

void osgqt::on_treeWidget_itemChanged_parent(QTreeWidgetItem *itm, int column){

    // 注释：用于树形图修改父节点勾选状态
    //QString itemText= itm->text(column);
    //auto state_son = itm->checkState(column);
	if (itm== NULL)return;
    QTreeWidgetItem *parent = itm->parent();


    if(parent)
    {
        QString itemText_parent = parent->text(column);
        if(QString::compare(itemText_parent,"3D Objects")==0)
            return;
        int checkedCount = 0;
        int patiallyCount = 0;
        int itemCount = parent->childCount();
        for(int i = 0; i < itemCount; i++)
        {
            QTreeWidgetItem *item = parent->child(i);
            if(Qt::Checked == item->checkState(column))
                checkedCount++;
            else if(Qt::PartiallyChecked == item->checkState(column))
                patiallyCount++;
        }

        if(checkedCount <= 0 )
        {
            if(patiallyCount > 0)
                goto Partially;
            else
                parent->setCheckState(column,Qt::Unchecked);
        }
        else if(checkedCount > 0 && checkedCount < itemCount  )
        {
Partially:
            parent->setCheckState(column,Qt::PartiallyChecked);
            SetParentPartiallyChecked(parent,column);
            return;
        }
        else if(checkedCount > 0 && checkedCount == itemCount)
        {
            parent->setCheckState(column,Qt::Checked);
        }
        // Recursively iterate up
        on_treeWidget_itemChanged_parent(parent,column);
    }
}


void osgqt::SetParentPartiallyChecked(QTreeWidgetItem *itm,int column)
{
    // 注释：用于树形图递归修改父节点勾选状态
    QTreeWidgetItem *parent = itm->parent();
    if(parent)
    {
		QString itemText_parent = parent->text(column);
        parent->setCheckState(0,Qt::PartiallyChecked);
        SetParentPartiallyChecked(parent,column);
    }
}

void osgqt::treeItemChanged(QTreeWidgetItem *item, int column){
    //auto aaa = ui->treeWidget;
    // 注释：用于勾选时隐藏或显示结点
    QString node_name = item->text(0);
    if(item){
        findGeoNamedNode* visitor = new findGeoNamedNode(node_name.toStdString());
        visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
        root->asGroup()->accept(*visitor);
        osg::ref_ptr<osg::Node> getnode = visitor->getNode();
        //osg::ref_ptr<osg::Node> getnode = all_node_vector[map_node_to_int[node_name.toStdString()]];
        if(getnode!=NULL){
            if(item->checkState(0)== Qt::Checked||item->checkState(0) == Qt::PartiallyChecked)
                getnode->setNodeMask(1);
            else
                getnode->setNodeMask(0);
        }
    }
}

void osgqt::timerUpdate(){
    // 注释：计时器函数，用于更新显示
    if(tab_idx==0&&ViewerWindow->ctrl_is_picked){
        ViewerWindow->ctrl_is_picked = false;
        ViewerWindow->setCameraManipulator(NULL);
        ctrl_is_picked2 = ViewerWindow->ctrl_is_picked2;
    }
    if(tab_idx==0&&ViewerWindow->ctrl_is_release){
        ViewerWindow->setCameraManipulator(manipulator,false);
        ViewerWindow->ctrl_is_release = false;
        ctrl_is_picked2 = ViewerWindow->ctrl_is_picked2;
    }

    /*if(tab_idx==1&&ok_is_clicked){
        string temp_done_name_info = "";
        for(int i = 0;i<dof_done_name.size();i++){
            temp_done_name_info = temp_done_name_info + dof_done_name.at(i)+": done!\n";
        }
        ui->done_name_info->setText(QString::fromStdString(temp_done_name_info));
        ok_is_clicked = false;
    }*/
    if(tab_idx==0&&treewidget->error_flag){
       // ui->group_error_message->setText(QString::fromStdString(treewidget->error_message));
        treewidget->error_flag = false;
    }

    if(tab_idx==0&&treewidget->drop){
        treewidget->drop = false;
        string drop_name = treewidget->father_name;
        vector<string> drop_node = treewidget->final_attach_node_name;

        std::vector<osg::ref_ptr<osg::Node>> attach_node;
        attach_node.clear();

        for(int i = 0;i<drop_node.size();i++){
            findGeoNamedNode* visitor = new findGeoNamedNode(drop_node[i]);
            visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
            root->accept(*visitor);

            osg::ref_ptr<osg::Node> getnode = visitor->getNode();
            attach_node.push_back(getnode);
            qDebug()<<QString::fromStdString(getnode->getName());
        }

        findGeoNamedNode* visitor = new findGeoNamedNode(drop_name);
        visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
       root->accept(*visitor);

        osg::ref_ptr<osg::Node> attach_node_father = visitor->getNode();

        for(int i = 0;i<attach_node.size();i++){
            osg::ref_ptr<osg::Node> child = attach_node[i];
            attach_node_father->asGroup()->addChild(child);
            attach_node[i]->getParent(0)->asGroup()->removeChild(attach_node[i]);
        }

        update_treewidget();

    }


    if(tab_idx==0&&current_tree_item_name!=""){
        //findGeoNamedNode* visitor = new findGeoNamedNode(current_box_name);

        //ViewerWindow->getSceneData()->asGroup()->accept(*visitor);

        //osg::ref_ptr<osg::Node> getnode = visitor->getNode();
        std::string print = "File_name: "+current_file_name + "\n" + "Last object you picked up: "+current_tree_item_name+"\n";
        //        osg::Matrix m = osg::computeLocalToWorld(getnode->getParentalNodePaths()[0]);
        //        osg::ref_ptr<osg::Vec3Array> pointarray = getarray(getnode,m);

        QString s = QString::fromStdString(print);
        //if(current_box_name[0]!='o'){
        //ui->textBrowser->setText(s);//}
    }

    if(tab_idx==0&&box_is_picked)//ctrl+左键框选到了某一部件触发，先触发BoxPicker类下
	{
		for (int i = 0; i < lastselectedNode.size(); i++)
		{
			NodeVisitor_dyer dyerb(init_state_set);
			lastselectedNode[i]->accept(dyerb);
			//lastselectedNode[i]->setStateSet(init_state_set.get());
		}
		lastselectedNode.clear();
		for (int i = 0; i < selectedItemList.size(); ++i)
		{
			/*osg::ref_ptr<osg::Node> getnode = item_node.find(selectedItemList.at(i)).value();
			lastselectedNode.append(getnode);
			osg::ref_ptr<osg::StateSet> now_state = new osg::StateSet;
			//if(it->second){
			osg::ref_ptr<osg::Material> material = new osg::Material;
			osg::Vec4 redColor(0, 191, 255, 0.5f);
			material->setAmbient(osg::Material::FRONT_AND_BACK, redColor);
			material->setDiffuse(osg::Material::FRONT_AND_BACK, redColor);
			//material->setSpecular(osg::Material::FRONT_AND_BACK,redColor);
			//material->setSpecular(osg::Material::FRONT_AND_BACK,redColor);
			//material->setShininess(osg::Material::FRONT_AND_BACK,90.0f);
			//material->setColorMode(osg::Material::DIFFUSE);

			now_state->setMode(GL_BLEND, osg::StateAttribute::ON);
			now_state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

			//设置网格模型
			//osg::ref_ptr<osg::PolygonMode> polyMode=new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);
			now_state->setAttributeAndModes(material.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
			getnode->setStateSet(now_state);*/
			osg::ref_ptr<osg::Node> getnode = item_node.find(selectedItemList.at(i)).value();
			lastselectedNode.append(getnode);
			osg::Vec4 redColor(0, 191, 255, 0.5f);
			NodeVisitor_dyer dyerr(redColor, redColor, osg::Vec4(0.75f, 0.75f, 0.75f, 0.5f), true);
			getnode->accept(dyerr);
		}
        box_is_picked = false;
    }

    /*if(tab_idx==1&&tree_widget_is_picked_empyt_2){
        tree_widget_is_picked_empyt_2 = false;
        map<string,bool>::iterator it;
        it = map_to_choose_2.begin();
        while(it != map_to_choose_2.end())
        {

            QMap<QString,QTreeWidgetItem*>::iterator iter_item = itemMap_.find(QString::fromStdString(it->first));
            if (iter_item != itemMap_.end()){
                iter_item.value()->setSelected(it->second);
            }


            findGeoNamedNode* visitor = new findGeoNamedNode(it->first);
            visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
            ViewerWindow2->getSceneData()->asGroup()->accept(*visitor);

            osg::ref_ptr<osg::Node> getnode = visitor->getNode();
            osg::ref_ptr<osg::StateSet> now_state = new osg::StateSet;
            if(it->second){
                osg::ref_ptr<osg::Material> material = new osg::Material;
                osg::Vec4 redColor(1.0,0.0,0.0, 1.0f);
                material->setAmbient(osg::Material::FRONT_AND_BACK, redColor);
                material->setDiffuse(osg::Material::FRONT_AND_BACK,redColor);
                //material->setSpecular(osg::Material::FRONT_AND_BACK,redColor);
                //material->setSpecular(osg::Material::FRONT_AND_BACK,redColor);
                //material->setShininess(osg::Material::FRONT_AND_BACK,90.0f);
                //material->setColorMode(osg::Material::DIFFUSE);

                now_state->setMode(GL_BLEND,osg::StateAttribute::ON);
                now_state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

                //设置网格模型
                //osg::ref_ptr<osg::PolygonMode> polyMode=new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);
                osg::ref_ptr<osg::PolygonMode> polyMode = new osg::PolygonMode;
                polyMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
                now_state->setAttributeAndModes(polyMode.get());

                now_state->setAttributeAndModes(material.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
                getnode->setStateSet(now_state);
            }else{
                now_state = init_state_set.get();
                getnode->setStateSet(now_state);
            }
            it ++;
        }
    }*/

    if(tab_idx==0&&(mouse_is_picked||tree_widget_is_picked||tree_widget_is_picked_empyt))//点击了树性图文字、显示视图中右键点击触发
	{
        tree_widget_is_picked = false;
        tree_widget_is_picked_empyt = false;
		for (int i = 0; i < lastselectedNode.size(); i++)
		{
			NodeVisitor_dyer dyerb(init_state_set);
			lastselectedNode[i]->accept(dyerb);
			//lastselectedNode[i]->setStateSet(init_state_set.get());	
		}
		lastselectedNode.clear();
		for (int i = 0; i < selectedItemList.size(); ++i) 
		{
			/*osg::ref_ptr<osg::Node> getnode = item_node.find(selectedItemList.at(i)).value();
			lastselectedNode.append(getnode);
            osg::ref_ptr<osg::StateSet> now_state = new osg::StateSet;

            osg::ref_ptr<osg::Material> material = new osg::Material;
            osg::Vec4 redColor(0,191,255, 0.5f);
            material->setAmbient(osg::Material::FRONT_AND_BACK, redColor);
            material->setDiffuse(osg::Material::FRONT_AND_BACK,redColor);

            now_state->setMode(GL_BLEND,osg::StateAttribute::ON);
            now_state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

            //设置网格模型
            //osg::ref_ptr<osg::PolygonMode> polyMode=new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);
            now_state->setAttributeAndModes(material.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
            getnode->setStateSet(now_state);*/
			osg::ref_ptr<osg::Node> getnode = item_node.find(selectedItemList.at(i)).value();
			lastselectedNode.append(getnode);
			osg::Vec4 redColor(0, 191, 255, 0.5f);
			NodeVisitor_dyer dyerr(redColor, redColor, osg::Vec4(0.75f, 0.75f, 0.75f, 0.5f), true);
			getnode->accept(dyerr);
        }
		mouse_is_picked = false;
    }
    if(tab_idx==0&&mouse_is_picked_empty){   //右键空白处复位
		for (int i = 0; i < lastselectedNode.size(); i++)
		{
			NodeVisitor_dyer dyerb(init_state_set);
			lastselectedNode[i]->accept(dyerb);
			//lastselectedNode[i]->setStateSet(init_state_set.get());
		}
		lastselectedNode.clear();

        mouse_is_picked_empty = false;
        QTreeWidgetItemIterator item(treewidget);
        while (*item) {
            (*item)->setSelected(false);
            ++item;
        }
    }
}

void osgqt::addmodel()//用于点击open按钮添加模型
{
    //root = new osg::Group;
	//removenode("trans");
	//int num = root->getNumChildren();
	//root->removeChild(0,num);//删除root下所有节点
    osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform;
	//将模型移动到地面上，z=0平面
	osg::Node* model1 = model;
	osg::Node* node = dynamic_cast<osg::Node*>(model1);
	osg::ComputeBoundsVisitor boundVisitor;
	node->accept(boundVisitor);
	osg::BoundingBox boundingBox = boundVisitor.getBoundingBox();
	float z = 0 - boundingBox.zMin();
	height.insert(trans,z);
	osg::Matrix m = osg::Matrix::translate(0, 0, z);
	trans->setMatrix(m);

    //trans->setMatrix(osg::Matrix::scale(scale_all,scale_all,scale_all));
    trans->addChild(model);
    model->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);
    osg::StateSet* state = model->getOrCreateStateSet();
    osg::ref_ptr<osg::LightModel> TwoSideLight = new osg::LightModel;
    TwoSideLight->setTwoSided(true);
    state->setMode(GL_CULL_FACE, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);   // 只关闭背面裁剪，造成生成背面不透明，但黑面 ;
    state->setAttributeAndModes(TwoSideLight, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON | osg	::StateAttribute::PROTECTED);  //再加上双面光照，使背面完全出现！;
    trans->setName("trans");
    root->addChild(trans);
    root->setName("root");
    ViewerWindow->setSceneData(root.get());
}
void osgqt::addmodel1(int i)//用于scan2cadtxt文件添加shapenet模型
{
	//root = new osg::Group;
	//removenode("trans");
	//int num = root->getNumChildren();
	//root->removeChild(0,num);//删除root下所有节点
	osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform;
	//将模型移动到地面上，z=0平面
	osg::Node* model1 = model;
	osg::Node* node = dynamic_cast<osg::Node*>(model1);
	osg::ComputeBoundsVisitor boundVisitor;
	node->accept(boundVisitor);
	osg::BoundingBox boundingBox = boundVisitor.getBoundingBox();
	/*float z = 0 - boundingBox.zMin();
	height.insert(trans, z);
	osg::Matrix m = osg::Matrix::translate(0, 0, z);*/
	osg::Matrix m=osg::Matrix::identity();
	osg::Matrix n=osg::Matrix(1, 0, 0,0,
							0,  0, 1,0,
							0, -1, 0,0,
							0,  0, 0,1);//右乘列变,将位移y，z互换
	osg::Matrix n1 = osg::Matrix(1, 0, 0, 0,
								0, 0, -1, 0,
								0, 1, 0, 0,
								0, 0, 0, 1);//左乘行变用于平移变换

	m = modelinfo[i].t[1];
	m = osg::Matrixf(m(0, 0), m(1, 0), m(2, 0), m(3, 0), 
		m(0, 1), m(1, 1), m(2, 1), m(3, 1), 
		m(0, 2), m(1, 2), m(2, 2), m(3, 2),
		m(0, 3), m(1, 3), m(2, 3), m(3, 3));
	m = modelinfo[i].t[2]*m* modelinfo[i].t[0]*n;//左边乘上模型，所以先缩放后旋转后平移，右边*n以后就可以换一个平面
	trans->setMatrix(m);

	//trans->setMatrix(osg::Matrix::scale(scale_all,scale_all,scale_all));
	trans->addChild(model);
	model->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);
	osg::StateSet* state = model->getOrCreateStateSet();
	osg::ref_ptr<osg::LightModel> TwoSideLight = new osg::LightModel;
	TwoSideLight->setTwoSided(true);
	state->setMode(GL_CULL_FACE, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);   // 只关闭背面裁剪，造成生成背面不透明，但黑面 ;
	state->setAttributeAndModes(TwoSideLight, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);  //再加上双面光照，使背面完全出现！;
	trans->setName("trans");
	root->addChild(trans);
	root->setName("root");
}
void osgqt::addmodel2(int i)//用于文本文件添加shape2motion模型
{
	//root = new osg::Group;
	//removenode("trans");
	//int num = root->getNumChildren();
	//root->removeChild(0,num);//删除root下所有节点
	osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform;
	//将模型移动到地面上，bouding box放缩
	osg::Node* model1 = model;
	osg::Node* node = dynamic_cast<osg::Node*>(model1);
	osg::ComputeBoundsVisitor boundVisitor;
	node->accept(boundVisitor);
	osg::BoundingBox boundingBox = boundVisitor.getBoundingBox();
	//表示shape2motion模型到shapenet模型的缩放
	osg::Matrix s1 = osg::Matrix::scale(modelinfo[i].bobox.z()/boundingBox.xMax(), modelinfo[i].bobox.x() / boundingBox.yMax(), modelinfo[i].bobox.y() / boundingBox.zMax());
	//float z = 0 - boundingBox.zMin();
	//height.insert(trans, z);
	//osg::Matrix m = osg::Matrix::translate(0, 0, z);
	osg::Matrix m = osg::Matrix::identity();//表示旋转
	osg::Matrix n = osg::Matrix(1, 0, 0, 0,
								0, 0, 1, 0,
								0, -1, 0, 0,
								0, 0, 0, 1);
	osg::Matrix n1 = osg::Matrix(1, 0, 0, 0,
								0, 0, -1, 0,
								0, 1, 0, 0,
								0, 0, 0, 1);//左乘行变用于平移变换
	osg::Matrix n2 = osg::Matrix(0, 0, -1, 0, -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1);//表示shape2motion模型到shapenet模型的旋转
	m = modelinfo[i].t[1];
	m = osg::Matrixf(m(0, 0), m(1, 0), m(2, 0), m(3, 0),
		m(0, 1), m(1, 1), m(2, 1), m(3, 1),
		m(0, 2), m(1, 2), m(2, 2), m(3, 2),
		m(0, 3), m(1, 3), m(2, 3), m(3, 3));
	m =  s1 *n2 * modelinfo[i].t[3]* modelinfo[i].t[2]*m* modelinfo[i].t[0]*n;//左边乘上模型，所以先缩放后旋转后平移，右边*n以后就可以换一个平面
	//m = n2;
	trans->setMatrix(m);

	//trans->setMatrix(osg::Matrix::scale(scale_all,scale_all,scale_all));
	trans->addChild(model);
	model->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);
	osg::StateSet* state = model->getOrCreateStateSet();
	osg::ref_ptr<osg::LightModel> TwoSideLight = new osg::LightModel;
	TwoSideLight->setTwoSided(true);
	state->setMode(GL_CULL_FACE, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);   // 只关闭背面裁剪，造成生成背面不透明，但黑面 ;
	state->setAttributeAndModes(TwoSideLight, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);  //再加上双面光照，使背面完全出现！;
	trans->setName("trans");
	root->addChild(trans);
	root->setName("root");
}
void osgqt::on_modeltranslate_clicked()
{
	if (selectedItemList.empty())
	{
		QMessageBox msgBox;
		msgBox.setText("Please click the model in tree widget to delete a model!");
		msgBox.exec();
		return;
	}
	double xcoord = ui->xcoord->text().toDouble();
	double ycoord = ui->ycoord->text().toDouble();
	double zcoord = ui->zcoord->text().toDouble();
	for (int i = 0; i < selectedItemList.size(); ++i)
	{
		osg::ref_ptr<osg::Node> getnode = item_node.find(selectedItemList.at(i)).value();
		osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform; 
		trans = dynamic_cast<osg::MatrixTransform*>(getnode->getParent(0));
		if (trans!=NULL)
		{
			osg::Matrix m;
			if (height.find(trans) != height.end())
				m = osg::Matrix::translate(xcoord, ycoord, zcoord) * osg::Matrix::translate(0, 0, height.find(trans).value()) * osg::Matrix::rotate(trans->getMatrix().getRotate());
			else m =trans->getMatrix()*  osg::Matrix::translate(xcoord, ycoord, zcoord) ;
			trans->setMatrix(m);
			ui->xcoord->clear();
			ui->ycoord->clear();
			ui->zcoord->clear();
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText(QString("%1 is a part.skip").arg(QString::fromStdString(getnode->getName())));
			msgBox.exec();
		}
	}
	//treewidget->clearSelection();
}
void osgqt::on_modelrotate_clicked()
{
	if (selectedItemList.empty())
	{
		QMessageBox msgBox;
		msgBox.setText("Please click the model in tree widget to delete a model!");
		msgBox.exec();
		return;
	}
	double xcoord = ui->xcoord->text().toDouble();
	double ycoord = ui->ycoord->text().toDouble();
	double zcoord = ui->zcoord->text().toDouble();
	xcoord = osg::DegreesToRadians(xcoord);
	ycoord = osg::DegreesToRadians(ycoord);
	zcoord = osg::DegreesToRadians(zcoord);
	for (int i = 0; i < selectedItemList.size(); ++i)
	{
		osg::ref_ptr<osg::Node> getnode = item_node.find(selectedItemList.at(i)).value();
		osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform;
		trans = dynamic_cast<osg::MatrixTransform*>(getnode->getParent(0));
		if (trans != NULL)
		{
			osg::Matrix m = osg::Matrix::rotate(zcoord, 0,0, 1.0f) 
												* osg::Matrix::rotate(ycoord, 0, 1, 0.0f)
												*osg::Matrix::rotate(xcoord, 1, 0,0.0f) 
												* osg::Matrix::translate(trans->getMatrix().getTrans());
			trans->setMatrix(m);
			ui->xcoord->clear();
			ui->ycoord->clear();
			ui->zcoord->clear();
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText(QString("%1 is a part.skip").arg(QString::fromStdString(getnode->getName())));
			msgBox.exec();
		}
	}
}
/*void osgqt::on_pushButton_3_clicked()//用于复制图片到另一文件夹
{
	QDir dir;
	//QString  directory= QFileDialog::getExistingDirectory(this, tr("Choose txt file Path"), "./");
	QString  directory = "D:/my/sunxun/Json2txt_only_ani";//autocreatescene
	dir = QDir(directory);
	dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	QFileInfoList list = dir.entryInfoList();
	ofstream f1("G:/pic/log.txt");
	for (int filenum = 1; filenum < list.size(); filenum++)//依次打开场景txt文件
	{
		QFileInfo fileInfo = list.at(filenum);
		QString name = fileInfo.fileName();
		QDir dir1;
		dir1 = QDir(directory+"/"+name+"/rgb");
		dir1.setFilter(QDir::Files);
		QFileInfoList list1 = dir1.entryInfoList();
		for (int pngnum = 0; pngnum < list1.size(); pngnum++)
		{
			QFileInfo tem = list1.at(pngnum);
			if (!QFile::copy(tem.absoluteFilePath(), "G:/pic/" + name + "-" + QString("%1."+tem.suffix()).arg(pngnum)))
			{
				qDebug() << name;
				f1 << name.toStdString() << "-" << pngnum << endl;
			}
		}
	}
}*/
void osgqt::on_pushButton_clicked()//自动场景入口
{
	QDir dir;
	//QString  directory= QFileDialog::getExistingDirectory(this, tr("Choose txt file Path"), "./");
	QString  directory = "D:/my/sunxun/Json2txt_only_ani";//autocreatescene
	dir = QDir(directory);
	dir.setFilter(QDir::Files);
	//QString  shapenetdir = QFileDialog::getExistingDirectory(this, tr("Choose shapenet Path"), "./");
	
	//QString  shape2motiondir = QFileDialog::getExistingDirectory(this, tr("Choose shape2motion Path"), "./");
	QString  shapenetdir = "E:/Motion Dataset v0"; 
	QString  shape2motiondir = "E:/shapenet_part";
	
	QFileInfoList list = dir.entryInfoList();//获取文件信息列表
	ifstream fin1(dir.absolutePath().toStdString() + "/" + "shapenet_shape2motion_corres.txt");	
	struct shapenet_motion
	{
		std::string shapecate, motioncate, motionid;
	}tem;
	std::string shapecate, motioncate, motionid;
	vector<shapenet_motion> shape_motion;
	while (fin1 >> tem.shapecate >> tem.motioncate >> tem.motionid)
	{
		shape_motion.push_back(tem);
	}fin1.close();
	std::string savemodelpath = "E:/model";
	if (0 != access(savemodelpath.c_str(), 0))
	{ // if this folder not exist, create a new one.
		mkdir(savemodelpath.c_str());   // 返回 0 表示创建成功，-1 表示失败
		//换成 ::_mkdir  ::_access 也行，不知道什么意思
	}
	std::string savetxtpath = dir.absolutePath().toStdString() + "/path";
	if (0 != access(savetxtpath.c_str(), 0))
	{ // if this folder not exist, create a new one.
		mkdir(savetxtpath.c_str());   // 返回 0 表示创建成功，-1 表示失败
		//换成 ::_mkdir  ::_access 也行，不知道什么意思
	}
	
	for (int filenum = 1371; filenum < list.size(); filenum++)//依次打开场景txt文件
	{
		std::ofstream f1;
		f1.open("./log.txt",ios::app);
		QFileInfo fileInfo = list.at(filenum);
		if (fileInfo.fileName() == "shapenet_shape2motion_corres.txt")
			continue;
		if(fileInfo.baseName().mid(0, 5)!="scene")continue;
		if (fileInfo.suffix() != "txt")continue;
		savepicpath = directory + "/" + fileInfo.baseName();
		savepclpath = savepicpath;
		QDir dir1(savepicpath);//创建文件夹
		bool isdone = false;
		if (!dir1.exists()) {
			bool ismkdir = dir.mkdir(savepicpath);
			if (!ismkdir)
				qDebug() << "Create path fail" << endl;
			else
				qDebug() << "Create fullpath success" << endl;
		}
		else {																//判断生成的rgb图片是否有问题
			QDir dir2(savepicpath + "/rgb");
			dir2.setFilter(QDir::Files);
			QFileInfoList list = dir2.entryInfoList();	
			if (list.size() < 2)
				isdone = false;
			else if (list[0].size() < 10000 | list[1].size() < 10000)
				isdone = false;
			else isdone = true;
		}
		ifstream fout(savetxtpath + "/path" + fileInfo.baseName().toStdString() + ".txt", ios::in);
		std::string modelpath = savemodelpath + "/" + fileInfo.baseName().toStdString() + ".flt";
		QFileInfo modelflt(QString::fromStdString(modelpath));
		if (isdone&&modelflt.exists() && modelflt.size() > 10000)
		{	
			continue;
		}
		//std::string tem = fileInfo.baseName().toStdString();
		//f1 << tem<< endl;
		//f1.close();
		qDebug() << filenum << " "<<fileInfo.absoluteFilePath() << endl;
		ifstream fin(fileInfo.absoluteFilePath().toStdString());
		int num;
		fin >> num;
		modelinfo = new modeldata[num];
		for (int i = 0; i < num; i++)
		{
			double x, y, z, h;
			fin >> modelinfo[i].cate >> modelinfo[i].id;
			fin >> x >> y >> z;
			modelinfo[i].t[0] = osg::Matrix::translate(x, y, z);
			double mat[16];
			for (int j = 0; j < 16; j++)
			{
				fin >> mat[j];
			}

			//QQuaternion n1(QVector4D(x,y,z,h));
			//QMatrix4x4 m = QMatrix4x4(n1.toRotationMatrix());
			//float * n2=QMatrix4x4(n1.toRotationMatrix()).data();
			//my[i].t[1] = osg::Matrix::rotate(x, y, z,h);
			//osg::Quat quaternion(x, y, z, h);
			//osg::Matrixf m;
			//m.setRotate(quaternion);
			modelinfo[i].t[1] = osg::Matrix(mat);
			fin >> x >> y >> z;
			modelinfo[i].t[2] = osg::Matrix::scale(x, y, z);
			fin >> x >> y >> z;
			modelinfo[i].bobox = osg::Vec3(x, y, z);
			fin >> x >> y >> z;
			modelinfo[i].t[3] = osg::Matrix::translate(x, y, z);
		}
		fin.close();
		
		if (modelflt.exists()&&modelflt.size()>10000)
		{
			osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
			model = osgDB::readNodeFile(modelpath, a);
			if (model == NULL)
			{
				f1 << modelpath << endl;
				//f1.close();
				QFile modelp(QString::fromStdString(modelpath));
				modelp.remove();
				filenum--;
				continue;
			}
			update_treewidget2();
			osg::Node* node = dynamic_cast<osg::Node*>((osg::Node*)model);
			osg::ComputeBoundsVisitor boundVisitor;
			node->accept(boundVisitor);
			osg::BoundingBox boundingBox = boundVisitor.getBoundingBox();
			double xrange = boundingBox.xMax() - boundingBox.xMin();
			double yrange = boundingBox.yMax() - boundingBox.yMin();
			double zrange = boundingBox.zMax() - boundingBox.zMin();
			osg::Vec3 center = boundingBox.center();
			//osg::Matrix m = osg::Matrix::identity();
			osg::Matrix m = osg::Matrix::scale(2.0 / xrange, 2.0 / xrange, 2.0 / xrange) * osg::Matrix::translate(-boundingBox.center().x(), -boundingBox.center().y(), -boundingBox.center().z());
			roottrans->setMatrix(m);
			roottrans->setName("scenescale");
			roottrans->addChild(model);
			//ViewerWindow->setSceneData(roottrans);
			if (!isdone)
			{
				autorgbshoot1();//自动拍rgb照
			}
			roottrans->removeChild(model);
			motion.clear();
		}
		else
		{
			int i = -1; string cate, id; bool modelnull = false;
			while (fout >> i >> cate)//事先生成的路径文件
			{
				string cad_file;
				if (cate != "none")
				{
					fout >> id;
					cad_file = shapenetdir.toStdString() + "/" + cate + "/" + id + "/flt/" + id + ".flt";
					current_file_name = cate + "/" + id;
				}
				else//没有匹配motion 模型的时候，
				{
					cad_file = shape2motiondir.toStdString() + "/" + modelinfo[i].cate + "/" + modelinfo[i].id + "/models/model_normalized.obj";
					current_file_name = modelinfo[i].cate + "/" + modelinfo[i].id;
				}
				osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
				model = new osg::Node();

				model->setName(modelinfo[i].cate);
				//auto aaaa = model->asGroup();

				model_state_set = model->getStateSet();
				if (cate != "none")
				{
					model = osgDB::readNodeFile(cad_file, a);
					addmodel2(i);//加载shape2motion模型

					init_state_set = model->getStateSet();
					box_infomation = "";

					//update_treewidget1();//给模型的第一个dof 自动添加运动信息
					update_treewidget2();//给模型的所有dof都添加运动信息，用于rgb拍照
					flag_open = true;
					selectedItemList.clear();
					mouse_is_picked_empty = false;
					mouse_is_picked = false;
					tree_widget_is_picked = false;
					tree_widget_is_picked_empyt = false;
					box_is_picked = false;
					flag_open = true;
				}
				else
				{
					osgDB::Options * options = new osgDB::Options("noRotation | noTriStripPolygons");
					qDebug() << QString::fromStdString(cad_file);
					model = osgDB::readNodeFile(cad_file, options);
					if (model == NULL)
					{
						f1 << cad_file << endl;
						//f1.close();
						modelnull = true;
						break;
					}
					addmodel1(i);
					init_state_set = model->getStateSet();
					box_infomation = "";
					//update_treewidget2();
					flag_open = true;
					selectedItemList.clear();
					mouse_is_picked_empty = false;
					mouse_is_picked = false;
					tree_widget_is_picked = false;
					tree_widget_is_picked_empyt = false;
					box_is_picked = false;
					flag_open = true;
				}
			}
			fout.close();
			i++;
			ofstream fout1(savetxtpath + "/path" + fileInfo.baseName().toStdString() + ".txt", ios::out | ios::app);
			for (; i < num; i++)//替换或加载shapenet模型，生成场景及树状图
			{
				for (int j = 0; j < shape_motion.size(); j++)
				{
					if (modelinfo[i].cate == shape_motion[j].shapecate)
					{
						string cad_file;
						if (shape_motion[j].motioncate != "none")
						{
							cad_file = shapenetdir.toStdString() + "/" + shape_motion[j].motioncate + "/" + shape_motion[j].motionid + "/flt/" + shape_motion[j].motionid + ".flt";
							current_file_name = shape_motion[j].motioncate + "/" + shape_motion[j].motionid;
							fout1 << i << " " << shape_motion[j].motioncate << " " << shape_motion[j].motionid << endl;
						}
						else//没有匹配motion 模型的时候，
						{
							cad_file = shape2motiondir.toStdString() + "/" + modelinfo[i].cate + "/" + modelinfo[i].id + "/models/model_normalized.obj";
							current_file_name = modelinfo[i].cate + "/" + modelinfo[i].id;
							fout1 << i << " " << "none" << endl;
						}
						flag_open = false;
						current_tree_item_name = "";
						// 注释： 用于更换模型
						flag_open = false;
						//selectedItemList.clear();
						mouse_is_picked_empty = false;
						mouse_is_picked = false;
						tree_widget_is_picked = false;
						tree_widget_is_picked_empyt = false;
						box_is_picked = false;
						osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
						model = new osg::Node();

						model->setName(modelinfo[i].cate);
						//auto aaaa = model->asGroup();

						model_state_set = model->getStateSet();
						if (shape_motion[j].motioncate != "none")
						{
							model = osgDB::readNodeFile(cad_file, a);
							addmodel2(i);//加载shape2motion模型

							init_state_set = model->getStateSet();
							box_infomation = "";

							//update_treewidget1();//给模型的第一个dof 自动添加运动信息
							update_treewidget2();//给模型的所有dof都添加运动信息，用于rgb拍照
							flag_open = true;
							selectedItemList.clear();
							mouse_is_picked_empty = false;
							mouse_is_picked = false;
							tree_widget_is_picked = false;
							tree_widget_is_picked_empyt = false;
							box_is_picked = false;
							flag_open = true;
						}
						else
						{
							osg::ref_ptr<osgDB::Options> options = new osgDB::Options("noRotation | noTriStripPolygons");
							model = osgDB::readNodeFile(cad_file, options);
							if (model == NULL)
							{
								f1 << cad_file << endl;
								
								modelnull = true;
								break;
							}
							addmodel1(i);
							init_state_set = model->getStateSet();
							box_infomation = "";
							//update_treewidget2();
							flag_open = true;
							selectedItemList.clear();
							mouse_is_picked_empty = false;
							mouse_is_picked = false;
							tree_widget_is_picked = false;
							tree_widget_is_picked_empyt = false;
							box_is_picked = false;
							flag_open = true;
						}
					}
				}
			}
			f1.close();
			fout1.close();
			if (modelnull)continue;
			osg::Node* node = dynamic_cast<osg::Node*>((osg::Node*)root);
			osg::ComputeBoundsVisitor boundVisitor;
			node->accept(boundVisitor);
			osg::BoundingBox boundingBox = boundVisitor.getBoundingBox();
			double xrange = boundingBox.xMax() - boundingBox.xMin();
			double yrange = boundingBox.yMax() - boundingBox.yMin();
			double zrange = boundingBox.zMax() - boundingBox.zMin();
			osg::Vec3 center = boundingBox.center();
			//osg::Matrix m = osg::Matrix::identity();
			osg::Matrix m=osg::Matrix::scale(2.0/xrange, 2.0 / xrange, 2.0 / xrange)*osg::Matrix::translate(-boundingBox.center().x(), -boundingBox.center().y(),-boundingBox.center().z());
			roottrans->setMatrix(m);
			roottrans->setName("scenescale");
			roottrans->addChild(root);
			if(!isdone)
				autorgbshoot();//自动拍rgb照
			qDebug() << "write";
			osgDB::Registry::instance()->writeNode(*(root->asNode()), modelpath, osgDB::Registry::instance()->getOptions());//save obj
			on_pushButton_2_clicked();
		}
		//生成点云
	}
}
/*void osgqt::on_pushButton_clicked()//scale
{
	if (selectedItemList.empty())
	{
		QMessageBox msgBox;
		msgBox.setText("Please click the model in tree widget to delete a model!");
		msgBox.exec();
		return;
	}
	double xcoord = ui->xcoord->text().toDouble();
	double ycoord = ui->ycoord->text().toDouble();
	double zcoord = ui->zcoord->text().toDouble();
	xcoord = osg::DegreesToRadians(xcoord);
	ycoord = osg::DegreesToRadians(ycoord);
	zcoord = osg::DegreesToRadians(zcoord);
	for (int i = 0; i < selectedItemList.size(); ++i)
	{
		osg::ref_ptr<osg::Node> getnode = item_node.find(selectedItemList.at(i)).value();
		osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform;
		trans = dynamic_cast<osg::MatrixTransform*>(getnode->getParent(0));
		if (trans != NULL)
		{
			osg::Matrix m = osg::Matrix::scale(xcoord, ycoord, zcoord);
			trans->setMatrix(m);
			ui->xcoord->clear();
			ui->ycoord->clear();
			ui->zcoord->clear();
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText(QString("%1 is a part.skip").arg(QString::fromStdString(getnode->getName())));
			msgBox.exec();
		}
	}
}
*/
void osgqt::on_pushButton_2_clicked()//删除所有节点，使整个程序reset
{
	for (int i = 0; i < root->getNumChildren(); i++)
	{
		root->removeChild(i);
		i--;
	}
	//treewidget->clear();
	viewmotion();
	motion.clear();
	/*is_model_componets.clear();
	model_structure_tree.clear();
	all_node.clear();*/
	delete[] modelinfo;
	modelinfo = NULL;
}
void osgqt::update_treewidget(){
    // 注释：用于更新树形结构图
    is_model_componets.clear();
    all_node.clear();
    //map_node_to_int.clear();
    //all_node_vector.clear();
	
    for(int i = 0;i<model_structure_tree.size();i++)
        model_structure_tree[i].clear();
    //treewidget->clear();

    //osg::ref_ptr<osg::Node> model =root->getChild(1)->asGroup()->getChild(0);
    InfoVisitor infoVisitor;
    infoVisitor.setNodeMaskOverride(1);
    model->accept(infoVisitor);//获取节点到all_node,获取层次信息到model_structure_tree,获取叶节点到is_model_componets用于右键点选部件
    node_item.clear();
    root_tree = new QTreeWidgetItem(treewidget);
	QString cfn = QString::fromStdString(current_file_name);
    root_tree->setText(0,cfn);    //树形控件显示的文本信息
	item_node.insert(root_tree,model);
	root_tree->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);   //设置树形控件子项的属性
	root_tree->setCheckState(0, Qt::Checked); //初始状态被选中
	QIcon dir = QIcon(":/ico/ooopic_1539861093.ico");
	QIcon dofp = QIcon(":/ico/3D_Coordinates.ico");
	root_tree->setIcon(0, dir);
    ///*
    /*QTreeWidgetItem* db = new QTreeWidgetItem(root_tree);
    db->setText(0,QString::fromStdString("db"));
    itemMap_.insert(QString::fromStdString("db"),db);
    //std::string box_name;
    //int2str(0, box_name);
    //db->setText(1,QString::fromStdString(box_name));    //树形控件显示的文本信息
    db->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);   //设置树形控件子项的属性
    db->setCheckState(0,Qt::Checked); //初始状态被选中
    db->setIcon(0,QIcon(":/ico/ooopic_1539861093.ico")); //注释： 文件夹图标
    //*/
    ///*
    for(int i=0;i<all_node.size();i++){
        osg::ref_ptr<osg::Node> parent=all_node.at(i);
        for(int j=0;j<model_structure_tree[i].size();j++){
			osg::ref_ptr<osg::Node> getnode = (model_structure_tree[i][j]);
			QString child_name = QString::fromStdString(getnode->getName());
            QTreeWidgetItem *temp_item;
            QMap<osg::ref_ptr<osg::Node>,QTreeWidgetItem*>::iterator iter_item = node_item.find(parent);
			unsigned int aaaa = getnode->getNodeMask();
            if (iter_item == node_item.end()){
                temp_item = new QTreeWidgetItem(root_tree);
                temp_item->setText(0,child_name);
                temp_item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);   //设置树形控件子项的属性
                if(aaaa>0)
                    temp_item->setCheckState(0,Qt::Checked); //初始状态被选中
                else
                    temp_item->setCheckState(0,Qt::Unchecked);
            }else{
                temp_item = new QTreeWidgetItem(iter_item.value());
                temp_item->setText(0,child_name);
                temp_item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);   //设置树形控件子项的属性
                if(aaaa>0)
                    temp_item->setCheckState(0,Qt::Checked); //初始状态被选中
                else
                    temp_item->setCheckState(0,Qt::Unchecked);
            }
            bool is_group = is_model_componets[getnode];
            if(!is_group){
                bool is_dof = isdof(*getnode);
                if(is_dof){
                    temp_item->setIcon(0,dofp); //注释： dof图标
                }
                else{
                    temp_item->setIcon(0,dir); //注释： 文件夹图标
                }
            }

            if(!child_name.isEmpty()){
                node_item.insert(getnode,temp_item);
				item_node.insert(temp_item, getnode);
            }
        }

    }

    map<osg::ref_ptr<osg::Node>,bool>::iterator itr;
    itr = is_model_componets.begin();
    while(itr != is_model_componets.end())
    {
        if(itr->second){
            on_treeWidget_itemChanged_parent(node_item[itr->first],0);
        }
        itr++;
    }
	selectedItemList.clear();
    treewidget->is_model_componets.clear();
    treewidget->is_model_componets = is_model_componets;
    treewidget->model_structure_tree.clear();
    treewidget->model_structure_tree = model_structure_tree;
   // treewidget->all_node_name.clear();
    //treewidget->all_node_name = all_node_name;
	treewidget->item_node.clear();
	treewidget->item_node = item_node;
    //treewidget->expandAll();
}
void osgqt::update_treewidget2() {
	// 注释：用于自动搭建场景里给所有dof添加运动信息到motion结构体，简单版，没有加载treewidgetitem
	is_model_componets.clear();
	all_node.clear();

	for (int i = 0; i < model_structure_tree.size(); i++)
		model_structure_tree[i].clear();
	findDofNamedNode * visitor = new findDofNamedNode();//找到所有dof节点并添加到motion结构体
	visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
	model->accept(*visitor);
}
/*void osgqt::update_treewidget2() {
	// 注释：用于自动搭建场景里更新树形结构图，并且给所有dof添加运动信息到motion结构体，添加上addmotion的判断就是给第一个dof添加运动信息的update_treewidget1函数
	is_model_componets.clear();
	all_node_name.clear();
	//map_node_to_int.clear();
	//all_node_vector.clear();
	for (int i = 0; i < model_structure_tree.size(); i++)
		model_structure_tree[i].clear();
	//treewidget->clear();

	//osg::ref_ptr<osg::Node> model =root->getChild(1)->asGroup()->getChild(0);
	InfoVisitor infoVisitor;
	infoVisitor.setNodeMaskOverride(1);
	model->accept(infoVisitor);//获取名字到all_node_name和获取层次信息到model_structure_tree
	itemMap_.clear();
	root_tree = new QTreeWidgetItem(treewidget);
	QString cfn = QString::fromStdString(current_file_name);
	root_tree->setText(0, cfn);    //树形控件显示的文本信息
	item_node.insert(root_tree, model);
	bool addmotion = false;
	for (int i = 0; i < all_node_name.size(); i++) {
		QString father_name = QString::fromStdString(all_node_name.at(i));
		for (int j = 0; j < model_structure_tree[i].size(); j++) {
			QString child_name = QString::fromStdString(model_structure_tree[i][j]);
			findGeoNamedNode* visitor = new findGeoNamedNode(child_name.toStdString());
			visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
			model->accept(*visitor);
			osg::ref_ptr<osg::Node> getnode = visitor->getNode();
			unsigned int aaaa = getnode->getNodeMask();
			QTreeWidgetItem* temp_item;
			QMap<QString, QTreeWidgetItem*>::iterator iter_item = itemMap_.find(father_name);
			if (iter_item == itemMap_.end()) {
				temp_item = new QTreeWidgetItem(root_tree);
				temp_item->setText(0, child_name);
				temp_item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);   //设置树形控件子项的属性
				if (aaaa > 0)
					temp_item->setCheckState(0, Qt::Checked); //初始状态被选中
				else
					temp_item->setCheckState(0, Qt::Unchecked);
			}
			else {
				temp_item = new QTreeWidgetItem(iter_item.value());
				temp_item->setText(0, child_name);
				temp_item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);   //设置树形控件子项的属性
				if (aaaa > 0)
					temp_item->setCheckState(0, Qt::Checked); //初始状态被选中
				else
					temp_item->setCheckState(0, Qt::Unchecked);
			}
			bool is_group = is_model_componets[child_name.toStdString()];
			if (!is_group)
			{
				bool is_dof = isdof(*getnode);
				if (is_dof)
				{
					temp_item->setIcon(0, QIcon(":/ico/3D_Coordinates.ico")); //注释： dof图标
					std::string childname = child_name.toStdString();
					if (childname.length() >= 2 && (childname.substr(childname.length() - 2, 2) == "_r"))//添加旋转运动信息
					{
						motionif x;
						x.motioninfo[0] = 0;
						x.motioninfo[1] = 90;
						x.motioninfo[2] = 0;
						x.motioninfo[3] = 5;
						motion.insert(getnode, x);
						addmotion = true;
					}
					else if (childname.length() >= 2 && (childname.substr(childname.length() - 2, 2) == "_t"))
					{
						motionif x;
						x.motioninfo[0] = 0.1;
						x.motioninfo[1] = 0;
						x.motioninfo[2] = 0;
						x.motioninfo[3] = 5;
						motion.insert(getnode, x);
						addmotion = true;
					}
					else if (childname.length() >= 3 && (childname.substr(getnode->getName().length() - 3, 3) == "_rt"))
					{
						motionif x;
						x.motioninfo[0] = 0.1;
						x.motioninfo[1] = 90;
						x.motioninfo[2] = 0;
						x.motioninfo[3] = 5;
						motion.insert(getnode, x);
						addmotion = true;
					}
				}
				else {
					temp_item->setIcon(0, QIcon(":/ico/ooopic_1539861093.ico")); //注释： 文件夹图标
				}
			}

			if (!child_name.isEmpty()) {
				itemMap_.insert(child_name, temp_item);
				item_node.insert(temp_item, getnode);
			}
		}
	}

	map<string, bool>::iterator itr;
	itr = is_model_componets.begin();
	while (itr != is_model_componets.end())
	{
		if (itr->second) {
			QString obj_name = QString::fromStdString(itr->first);
			if (itemMap_.empty())
			{
				itr++;
				continue;
			}
			QMap<QString, QTreeWidgetItem*>::iterator iter_item = itemMap_.find(obj_name);
			on_treeWidget_itemChanged_parent(iter_item.value(), 0);
		}
		itr++;
	}
	selectedItemList.clear();
	treewidget->is_model_componets.clear();
	treewidget->is_model_componets = is_model_componets;
	treewidget->model_structure_tree.clear();
	treewidget->model_structure_tree = model_structure_tree;
	treewidget->all_node_name.clear();
	treewidget->all_node_name = all_node_name;
	//treewidget->expandAll();
}*/
void osgqt::on_open_clicked(){
    // 注释：用于打开模型和之后的一系列初始化
    current_file_name = "";
    QString directory, file_name, file_path;
    QFileInfo fi;
    //QString current_path = QDir::currentPath();
    directory = QFileDialog::getOpenFileName(this,tr("Choose File"),"", tr("model(*.ive *.flt)"));
    fi = QFileInfo(directory);
    file_name = fi.fileName();
    current_file_name = file_name.toStdString();
    //cout<<"current_file_name: "<<current_file_name<<endl;
    file_path = fi.absolutePath();
    std::string  path = directory.toStdString();
    if(path=="")
        return;
	filepath = directory;
    flag_open = false;
    current_tree_item_name = "";
	// 注释： 用于更换模型
	flag_open = false;
	mouse_is_picked_empty = false;
	mouse_is_picked = false;
	tree_widget_is_picked = false;
	tree_widget_is_picked_empyt = false;
	box_is_picked = false;
	osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
	model = new osg::Node();
	model = osgDB::readNodeFile(path, a);
	model->setName(current_file_name);
	//auto aaaa = model->asGroup();

	model_state_set = model->getStateSet();
	addmodel();
	init_state_set = model->getStateSet();
	box_infomation = "";

	update_treewidget();
	flag_open = true;
    selectedItemList.clear();
    mouse_is_picked_empty = false;
    mouse_is_picked = false;
    tree_widget_is_picked = false;
    tree_widget_is_picked_empyt = false;
    box_is_picked = false;
    flag_open = true;
}
void osgqt::on_remove_clicked()//删除所选模型的树状图、所属模型的motion信息、lastmotion信息、root下的所属模型
{
	if (selectedItemList.empty())
	{
		QMessageBox msgBox;
		msgBox.setText("Please click the model in tree widget to delete a model!");
		msgBox.exec();
		return;
	}
	for (int i = 0; i < selectedItemList.size(); ++i)
	{
		osg::ref_ptr<osg::Node> getnode = item_node.find(selectedItemList.at(i)).value();
			if (getnode->getParent(0)->getName() == "trans")
			{
				QString ques = QString("Are you sure to delete %1?").arg(QString::fromStdString(getnode->getName()));
				QMessageBox msgBox(QMessageBox::NoIcon,
					"critical", ques,
					QMessageBox::Yes | QMessageBox::No, NULL);
				int result = msgBox.exec();
				if (result == QMessageBox::No)
				{
					continue;
				}
				for (QMap<osg::ref_ptr<osg::Node>, motionif>::iterator itr = motion.begin(); itr != motion.end();)//删除空节点的motion信息
				{
					QMap<QTreeWidgetItem*, osg::ref_ptr<osg::Node>>::iterator iter = item_node.begin();
					bool flag = false;
					for (; iter != item_node.end(); iter++)
					{
						if (iter.value() == itr.key())
						{
							QTreeWidgetItem* par=findpar(iter.key());
							if (par == selectedItemList.at(i))
							{
							itr = motion.erase(itr);
							flag = true;
							break;
							}		
						}
					}
					if(!flag)
						itr++;
				}
			delete selectedItemList.at(i);
			root->removeChild(getnode->getParent(0));
			i--;
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText(QString("%1 is a part.skip").arg(QString::fromStdString(getnode->getName())));
			msgBox.exec();
		}
	}
	viewmotion();
	treewidget->clearSelection();
}
/*void osgqt::removeitem(QTreeWidgetItem* fitem)
{
	if (fitem == NULL)return;
	//delete treewidget->takeTopLevelItem(treewidget->indexOfTopLevelItem(fitem));
	for (int i = 0; i < fitem->childCount(); i++)
	{
		removeitem(fitem->child(i));	
	}
	delete fitem;
}*/
QTreeWidgetItem* osgqt::findpar(QTreeWidgetItem* fitem)
{
	QTreeWidgetItem* tem = fitem;
	while (tem->parent())
	{
		tem = tem->parent();
	}
	return tem;
}

osg::ref_ptr<osg::Vec3Array> osgqt::getarray(osg::ref_ptr<osg::Node> node,osg::Matrix m){
    osg::ref_ptr<osg::Vec3Array> pointarray = new osg::Vec3Array;
    osg::ref_ptr<osg::Geode> p=node->asGroup()->getChild(0)->asGeode();
    unsigned int count = p->getNumDrawables();
    for(unsigned int geomIdx = 0;geomIdx<count;geomIdx++){
        osg::Geometry *geometry = p->getDrawable(geomIdx)->asGeometry();
        osg::Vec3Array* vertices=dynamic_cast<osg::Vec3Array*>(geometry->getVertexArray());
        pointarray->push_back(vertices->at(3)*m);
        pointarray->push_back(vertices->at(2)*m);
        pointarray->push_back(vertices->at(1)*m);
        pointarray->push_back(vertices->at(0)*m);
        pointarray->push_back(vertices->at(8)*m);
        pointarray->push_back(vertices->at(9)*m);
        pointarray->push_back(vertices->at(10)*m);
        pointarray->push_back(vertices->at(11)*m);
    }
    return pointarray;
}

void osgqt::on_tabWidget_currentChanged(int index)
{
    tab_idx = index;
}

void osgqt::on_treeWidget_itemSelectionChanged()//点击了树形图的文字
{
    //注释： 用于更新map_to_choose
    if(!box_is_picked)
	{
		QString motion_name = "";
		selectedItemList = treewidget->selectedItems();//可以选中多个item
        for ( int i=0; i!=selectedItemList.size(); ++i )
		{
            //if(QString::compare(selectedItemList.at(i)->text(0),"3D Objects")==0)
               // continue;
            //map_to_choose[selectedItemList.at(i)->text(0).toStdString()]=true;
			QString qnode_name = selectedItemList.at(i)->text(0);
			std::string node_name = qnode_name.toStdString();
			if (node_name.length() >= 2 && (node_name.substr(node_name.length() - 2, 2) == "_r" || (node_name.substr(node_name.length() - 2, 2) == "_t")))
			{
				motion_name += qnode_name + ";";
				ui->input_motion_fre->setEnabled(true);
				if (node_name.substr(node_name.length() - 2, 2) == "_r")
				{
					ui->motion_amp->setEnabled(true);
				}
				else if (node_name.substr(node_name.length() - 2, 2) == "_t")
				{
					ui->motion_amp_2->setEnabled(true);
				}
					
			}
			else if (node_name.length() >= 3 && (node_name.substr(node_name.length() - 3, 3) == "_rt"))
			{
				ui->motion_amp->setEnabled(true);
				ui->motion_amp_2->setEnabled(true);
			}
        }
		if (motion_name == "")
		{
			ui->motion_amp->clear();
			ui->motion_amp_2->clear();
			ui->input_motion_name->clear();
			ui->input_motion_fre->clear();
			ui->input_motion_fre->setEnabled(false);
			ui->motion_amp_2->setEnabled(false);
			ui->motion_amp->setEnabled(false);
		}
		ui->input_motion_name->setText(motion_name);
        tree_widget_is_picked_empyt = true;
    }
}


void osgqt::on_Reset_clicked()
{
    // 注释：重置物体位置
    ViewerWindow->setCameraManipulator(manipulator);
}