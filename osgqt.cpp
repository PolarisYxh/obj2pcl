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
std::vector<string> all_node_name;  //注释：父节点信息
std::map<string,int> node_to_int; //注释：用于确认父节点是第几号结点
std::vector<std::vector<string>> model_structure_tree(30000); //注释：树形图
QTreeWidgetItem* root_tree; //注释：树形图widget
QMap<QString, QTreeWidgetItem*> itemMap_ ; //注释： 树形图每个栏目对应id
bool flag_open = false; //注释： 打开文件初始化是否完成
std::vector<string> model_components; //注释：是否是可绘制Geode

std::map<string,bool> is_model_componets; //注释：是否是可绘制Geode

const float scale_all = 4; //注释： 由于显示问题，所有模型加载时统一放大四倍，结构为root->trans(放大4倍）->model
std::string current_tree_item_name =""; //注释： 用于在树形图中高亮当前点选的部件
std::string current_file_name = ""; //注释： 用于在信息框显示当前模型名
osg::ref_ptr<osg::StateSet> init_state_set; //注释： 初始的渲染stateset
osg::ref_ptr<osg::StateSet> model_state_set; //注释： model原本的stateset
std::map<string,bool> map_to_choose; //注释： 用于标记每个部件是否被点选
bool mouse_is_picked = false;//注释： 用于判断鼠标是否在被点击
bool mouse_is_picked_empty = false; //注释： 用于判断鼠标是否点击空白出
bool tree_widget_is_picked = false; //注释： 用于判断是否点击了tree_widget
bool tree_widget_is_picked_empyt = false;//注释： 用于判断是否点击了tree_widget 空白
std::string box_infomation = ""; //注释： 用于显示文件信息
int tab_idx = 0; //注释： 确定该tab的值
std::map<string,bool> child_map; //注释： 用于在attach时防止加入到子节点上
std::vector<string> final_attach_node_name;
std::map<string,bool> is_dof_node1;



//注释： 新加功能所需全局变量
mytreewidget *treewidget; //注释： 树形图类
osg::ref_ptr<osgGA::TrackballManipulator> manipulator; //注释：控制器
bool box_is_picked = false; //注释：判断是否box_pick
bool ctrl_is_picked2 = false;//注释： 用于控制不按下ctrl时box——pick不运行
//std::map<string,int> map_node_to_int; //用于查找node
//std::vector<osg::ref_ptr<osg::Node>> all_node_vector; //所有node的集合



//dof add 所需全局变量
mytreewidget2 *treewidget2;
std::vector<string> all_node_name_2;
std::vector<std::vector<string>> model_structure_tree_2(30000);
std::map<string,bool> is_model_componets_2;
std::map<string,bool> map_to_choose_2;
osg::ref_ptr<osg::StateSet> init_state_set_2; //注释： 初始的渲染stateset
osg::ref_ptr<osg::StateSet> model_state_set_2; //注释： model原本的stateset
QTreeWidgetItem* root_tree_2; //注释：树形图widget
bool tree_widget_is_picked_empyt_2 = false;//注释： 用于判断是否点击了tree_widget 空白
std::string current_file_name2 = "";
int dof_type = 0;
int choose_dof_mode = 0;
int dof_pick2_mode = 0;
osg::Vec3d Two_point_coor_1(0,0,0);
osg::Vec3d Two_point_coor_2(0,0,0);
osg::Vec3d one_point_coor(0,0,0);
bool Two_point_is_picked_1 = false;
bool Two_point_is_picked_2 = false;
bool one_point_is_picked_1 = false;
bool one_point_is_picked_2 = false;
osg::ref_ptr<osg::Vec3Array> facepointarray;
std::vector<string> dof_done_name;
bool ok_is_clicked = false;
std::map<string,osg::Vec3> dof_HPR;
std::map<string,osg::Vec3> dof_trans;
std::map<string,bool> is_dof_node2;




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




class BoxPicker : public osgGA::GUIEventHandler
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

        if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE)
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
                            if(!flag[temp_name]&&is_model_componets[temp_name]){
                                flag[temp_name] = true;
                                osg::Vec3 bbb = (*itr)->getBound().center()* osg::computeLocalToWorld((*itr)->getParentalNodePaths()[0]) * VPM;
                                //osg::Vec3d ccc = WorldToScreen(viewer,(*itr)->getBound().center() * osg::computeLocalToWorld((*itr)->getParentalNodePaths()[0]));
                                //if(ccc.x()>xMin&&ccc.x()<xMax&&ccc.y()>yMin&&ccc.y()<yMax)
                                if(bbb.x()>xMin&&bbb.x()<xMax&&bbb.y()>yMin&&bbb.y()<yMax)
                                    map_to_choose[temp_name] = true;
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




class Open_all_node_mask1 : public osg::NodeVisitor{
    /* 注释：class InfoVisitor 主要用于更新树形结构图以及获取可绘制的Geode
   infovisitor -> accept(model) model为加载的模型
   更新全局变量all_node_name，model_structure_tree，model_components
*/
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
};






class Open_all_node_mask : public osg::NodeVisitor{
    /* 注释：class InfoVisitor 主要用于更新树形结构图以及获取可绘制的Geode
   infovisitor -> accept(model) model为加载的模型
   更新全局变量all_node_name，model_structure_tree，model_components
*/
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
};

class InfoVisitor : public osg::NodeVisitor{
    /* 注释：class InfoVisitor 主要用于更新树形结构图以及获取可绘制的Geode
   infovisitor -> accept(model) model为加载的模型
   更新全局变量all_node_name，model_structure_tree，model_components
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
        if(node.getName()!=""){
            all_node_name.push_back(node.getName());
            osg::ref_ptr<osgSim::DOFTransform> dof = dynamic_cast<osgSim::DOFTransform*>(&node);
            if(dof!=NULL){
                is_dof_node1[node.getName()] = true;
            }
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

                vector<string>::iterator ite1 = find(all_node_name.begin(), all_node_name.end(), father_name);
                int index1 = std::distance(std::begin(all_node_name), ite1);
                model_structure_tree[index1].push_back(node.getName());
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
    virtual void apply( osg::Geode& node){
        //        for ( int i = 0; i < _indent; ++i)
        //        {
        //            std::cout<< " ";
        //        }
        //        std::cout<< "["<< _indent + 1<<"]"<< node.libraryName()<< "::"<< node.className()<< "::"<<node.getName()<< std::endl;
        vector<string>::iterator iter;

        string geode_name =node.getParent(0)->getName();
        is_model_componets[geode_name] = true;


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
};


class InfoVisitor2 : public osg::NodeVisitor{
    /* 注释：class InfoVisitor 主要用于dof adder更新树形结构图以及获取可绘制的Geode
   infovisitor -> accept(model) model为加载的模型
   更新全局变量all_node_name，model_structure_tree，model_components
*/
public:
    InfoVisitor2(): osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), _indent(0){}	//向子节点遍历

    virtual void apply( osg::Node &node){									//获取访问节点对象，并执行自定义节点操作
        //当访问到默认的Node节点时，使用该函数打印节点的名称
        //并根据节点层次适当使用_indent缩进
        //        for ( int i = 0; i < _indent; ++i)
        //        {
        //            std::cout<< " ";
        //        }
        //        std::cout<< "["<< _indent + 1<< "]"<< node.libraryName()<< "::"<< node.className()<< std::endl;
        if(node.getName()!=""){
            all_node_name_2.push_back(node.getName());
            osg::ref_ptr<osgSim::DOFTransform> dof = dynamic_cast<osgSim::DOFTransform*>(&node);
            if(dof!=NULL){
                osg::Vec3 hpr = dof->getCurrentHPR();
                osg::Vec3 trans = dof->getCurrentTranslate();
                dof_HPR[node.getName()] = hpr;
                dof_trans[node.getName()] = trans;
                is_dof_node2[node.getName()] = true;
            }
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
                vector<string>::iterator ite1 = find(all_node_name_2.begin(), all_node_name_2.end(), father_name);
                int index1 = std::distance(std::begin(all_node_name_2), ite1);
                model_structure_tree_2[index1].push_back(node.getName());
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
    virtual void apply( osg::Geode& node){
        //        for ( int i = 0; i < _indent; ++i)
        //        {
        //            std::cout<< " ";
        //        }
        //        std::cout<< "["<< _indent + 1<<"]"<< node.libraryName()<< "::"<< node.className()<< "::"<<node.getName()<< std::endl;
        vector<string>::iterator iter;

        string geode_name =node.getParent(0)->getName();
        is_model_componets_2[geode_name] = true;


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
};





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

class findDofNamedNode :public osg::NodeVisitor{
    /* 注释：class findGeoNamedNode 主要用于查找指定名字的结点
 * 返回具有指定名字的结点
 */
public:
    findDofNamedNode(const std::string name) : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN){
        resultNode = NULL;
        this->name = name;
    }

    virtual void apply(osg::Node &searchNode){
        if (searchNode.getName() == name){
            osgSim::DOFTransform* dynamicTry = dynamic_cast<osgSim::DOFTransform*>(&searchNode);
            if (dynamicTry){
                resultNode = dynamicTry;
            }
        }
        traverse(searchNode);
    }

    osgSim::DOFTransform* getNode(){
        return resultNode;
    }

private:
    osgSim::DOFTransform* resultNode;
    std::string name;
};

class KeyboardHandler :public osgGA::GUIEventHandler{
    /* 注释：class keyboardHandler 用于处理键盘人机交互事件处理器
 * 按D键打印all_node_name,model_structure_tree,model_components 用于调试
 */
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

};

class Pick :public osgGA::GUIEventHandler {
    /* 注释: class Pick 主要用于鼠标人机交互时间
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
*/
    /*
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
    void pick(float x, float y,osgViewer::View *viewer) {
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
        }else{
            mouse_is_picked_empty = true;
            current_tree_item_name = "";
        }
    }
};


class Pick2 :public osgGA::GUIEventHandler {
    /* 注释: class Pick 主要用于鼠标人机交互时间
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
*/
    /*
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
    void pick(float x, float y,osgViewer::View *viewer) {
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
        }
    }
};













osgqt::osgqt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::osgqt)
{
    // 注释：构造函数，用于一些初始化



    ui->setupUi(this);
    QMainWindow* mw = new QMainWindow(this);
    mw->setWindowFlags(Qt::Widget);
    ViewerWindow = new QtOsgView(mw);
    manipulator = new osgGA::TrackballManipulator;
    ViewerWindow->setCameraManipulator(manipulator);
    ViewerWindow->addEventHandler(new osgViewer::WindowSizeHandler);
    ViewerWindow->addEventHandler(new Pick());
    ViewerWindow->addEventHandler(new KeyboardHandler());
    ViewerWindow->getCamera()->setClearColor(osg::Vec4f(255.0f, 255.0f, 255.0f, 1.0f));

    // 环境光
    ViewerWindow->getLight()->setAmbient(osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
    // 漫反射光
    ViewerWindow->getLight()->setDiffuse(osg::Vec4(0.1f, 0.1f, 0.1f, 1.0f));

    osg::ref_ptr<BoxPicker> picker = new BoxPicker;
    ViewerWindow->addEventHandler(picker.get());
    //ViewerWindow->grabKeyboard();
    QFont font ( "Microsoft YaHei", 10, 75);
    ui->copyright1->setFont(font);
    ui->copyright2->setFont(font);
    ui->done_name_info->setReadOnly(true);
    ui->dof_add_info->setReadOnly(true);

    mw->setCentralWidget(ViewerWindow);
    ui->gridLayout->addWidget(mw);
    treewidget = new mytreewidget(ui->tabWidget->widget(0));
    treewidget->resize(211,831);

    treewidget2 = new mytreewidget2(ui->tabWidget->widget(1));
    treewidget2->resize(211,631);



    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    connect(treewidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));
    connect(treewidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(on_treeWidget_itemChanged_child(QTreeWidgetItem*,int)));
    connect(treewidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(on_treeWidget_itemChanged_parent(QTreeWidgetItem*,int)));
    connect(treewidget,SIGNAL(itemSelectionChanged()),this,SLOT(on_treeWidget_itemSelectionChanged()));


    connect(ui->input_group_name,SIGNAL(returnPressed()),this,SLOT(on_group_button_clicked()));
    connect(ui->input_attach_name,SIGNAL(returnPressed()),this,SLOT(on_attach_clicked()));
    connect(ui->input_Rename,SIGNAL(returnPressed()),this,SLOT(on_rename_clicked()));


    //dof adder 信号
    connect(treewidget2,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(on_treeWidget_itemChanged_child(QTreeWidgetItem*,int)));
    connect(treewidget2,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(on_treeWidget_itemChanged_parent(QTreeWidgetItem*,int)));
    connect(treewidget2,SIGNAL(itemSelectionChanged()),this,SLOT(on_treeWidget_itemSelectionChanged2()));
    connect(treewidget2,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged2(QTreeWidgetItem*,int)));

    timer->start(1);

    QMainWindow* mw2 = new QMainWindow(this);
    mw2->setWindowFlags(Qt::Widget);
    ViewerWindow2 = new QtOsgView(mw2);
    ViewerWindow2->setCameraManipulator(new osgGA::TrackballManipulator);
    ViewerWindow2->addEventHandler(new osgViewer::WindowSizeHandler);
    ViewerWindow2->addEventHandler(new Pick2());
    ViewerWindow2->addEventHandler(new KeyboardHandler());
    ViewerWindow2->getCamera()->setClearColor(osg::Vec4f(255.0f, 255.0f, 255.0f, 1.0f));
    // 环境光
    ViewerWindow2->getLight()->setAmbient(osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
    // 漫反射光
    ViewerWindow2->getLight()->setDiffuse(osg::Vec4(0.1f, 0.1f, 0.1f, 1.0f));

    mw2->setCentralWidget(ViewerWindow2);
    ui->gridLayout_2->addWidget(mw2);
    ui->opof_1->setCheckable(false);
    ui->opof_2->setCheckable(false);
    ui->tp_1->setCheckable(false);
    ui->tp_2->setCheckable(false);
    ui->translation->setCheckable(false);
    ui->rotation->setCheckable(false);
    ui->spiral->setCheckable(false);
    ui->tp->setDisabled(true);
    ui->opof->setDisabled(true);
    ui->ok->setDisabled(true);
    ui->cancel->setDisabled(true);






}

osgqt::~osgqt(){
    // 注释：析构函数
    delete ui;
}

void osgqt::on_rename_clicked(){
    // 注释：用于改变结点名字
    QString text = ui->input_Rename->text();
    ui->input_Rename->setText("");
    ui->group_error_message->setText("");
    std::string Re_name = text.toStdString();

    QList<QTreeWidgetItem*> selectedItemList = treewidget->selectedItems();
    if(selectedItemList.size()==0){
        ui->group_error_message->setText("Please select object that you want to rename!");
        return;
    }
    if(selectedItemList.size()>1){
        ui->group_error_message->setText("Please select only one object that you want to rename!");
        return;
    }

    if(Re_name=="3D Object"||Re_name=="root"||Re_name=="trans"||Re_name=="db"){
        ui->group_error_message->setText("Sorry,this name is invalid!");
        return;
    }

    bool is_existed = false;
    for(int i = 0;i<all_node_name.size();i++){
        if(all_node_name[i]==Re_name)
            is_existed = true;
    }
    if(is_existed){
        ui->group_error_message->setText("\""+text+"\""+" already exists,please choose another name!");
        return;
    }

    string current_name = selectedItemList.at(0)->text(0).toStdString();
    if(current_name=="db"||current_name=="3D Object"){
        ui->group_error_message->setText("This item can't change its name!");
        return;
    }

    findGeoNamedNode* visitor = new findGeoNamedNode(current_name);
    visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
    ViewerWindow->getSceneData()->asGroup()->accept(*visitor);

    osg::ref_ptr<osg::Node> getnode = visitor->getNode();
    getnode->setName(Re_name);

    bool temp = map_to_choose[current_name];

    map_to_choose[Re_name] = temp;
    map_to_choose.erase(current_name);
    update_treewidget();
    tree_widget_is_picked = true;
}

void osgqt::dfs(string father_name){
    //注释： 用于判断是否向子节点attach
    int father_idx = node_to_int[father_name];
    child_map[father_name] = true;
    for(int i = 0;i<model_structure_tree[father_idx].size();i++){
        dfs(model_structure_tree[father_idx].at(i));
    }
}

void osgqt::dfs2(string root_name){
    //注释： 用于添加attach的结点
    if(child_map[root_name]){
        final_attach_node_name.push_back(root_name);
        return;
    }
    int father_idx = node_to_int[root_name];
    for(int i = 0;i<model_structure_tree[father_idx].size();i++){
        dfs2(model_structure_tree[father_idx].at(i));
    }
}

void osgqt::on_attach_clicked(){
    //注释： 用于讲一个节点附加到其他节点上

    QString text = ui->input_attach_name->text();
    ui->input_attach_name->setText("");
    ui->group_error_message->setText("");
    std::string attach_name = text.toStdString();

    QList<QTreeWidgetItem*> selectedItemList = treewidget->selectedItems();
    if(selectedItemList.size()==0){
        ui->group_error_message->setText("Please select object that you want to attach to another group!");
        return;
    }
    if(attach_name=="3D Object"||attach_name=="root"||attach_name=="trans"){
        ui->group_error_message->setText("Sorry,this name is invalid!");
        return;
    }

    bool is_existed = false;
    for(int i = 0;i<all_node_name.size();i++){
        if(all_node_name[i]==attach_name)
            is_existed = true;
    }
    if(!is_existed){
        ui->group_error_message->setText("\""+text+"\""+" doesn't exist,please choose another name!");
        return;
    }




    bool is_componet = is_model_componets[attach_name];

    if(is_componet){
        ui->group_error_message->setText("This name is not belong to a group,it's a componet name!");
        return;
    }

    std::vector<string> attach_node_name;
    attach_node_name.clear();

    child_map.clear();
    node_to_int.clear();
    for(int i = 0;i<all_node_name.size();i++){
        node_to_int[all_node_name.at(i)] = i;
    }


    bool is_3dobject = false;
    for ( int i=0; i!=selectedItemList.size(); ++i ){
        attach_node_name.push_back(selectedItemList.at(i)->text(0).toStdString());
        if(QString::compare(selectedItemList.at(i)->text(0),"3D Object")==0||QString::compare(selectedItemList.at(i)->text(0),"db")==0){
            is_3dobject = true;
            break;
        }
    }
    if(is_3dobject){
        ui->group_error_message->setText("You can not move \"3D Object\" or \"db\"!");
        return;
    }


    for(int i = 0;i<attach_node_name.size();i++){
        dfs(attach_node_name.at(i));
    }

    bool is_choose = false;

    map<string,bool>::iterator it;
    it = child_map.begin();
    while(it != child_map.end())
    {
        qDebug()<<QString::fromStdString(it->first)<<" "<<it->second;
        if((it->first==attach_name)&&it->second){
            is_choose = true;
            break;
        }
        it++;
    }
    if(is_choose){
        ui->group_error_message->setText("You can not move to the group you has been chosen or its child");
        return;
    }

    final_attach_node_name.clear();
    dfs2("db");



    std::vector<osg::ref_ptr<osg::Node>> attach_node;
    attach_node.clear();

    for(int i = 0;i<final_attach_node_name.size();i++){
        findGeoNamedNode* visitor = new findGeoNamedNode(final_attach_node_name[i]);
        visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
        ViewerWindow->getSceneData()->asGroup()->accept(*visitor);

        osg::ref_ptr<osg::Node> getnode = visitor->getNode();
        attach_node.push_back(getnode);
        qDebug()<<QString::fromStdString(getnode->getName());
    }

    findGeoNamedNode* visitor = new findGeoNamedNode(attach_name);
    visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
    ViewerWindow->getSceneData()->asGroup()->accept(*visitor);

    osg::ref_ptr<osg::Node> attach_node_father = visitor->getNode();

    for(int i = 0;i<attach_node.size();i++){
        osg::ref_ptr<osg::Node> child = attach_node[i];
        attach_node_father->asGroup()->addChild(child);
        attach_node[i]->getParent(0)->asGroup()->removeChild(attach_node[i]);
    }

    update_treewidget();
}

void osgqt::on_Ungroup_clicked(){
    ui->group_error_message->setText("");
    QList<QTreeWidgetItem*> selectedItemList = treewidget->selectedItems();
    if(selectedItemList.size()==0){
        ui->group_error_message->setText("Please select one group to ungroup!");
        return;
    }
    if(selectedItemList.size()>1){
        ui->group_error_message->setText("Please select only one group to ungroup!");
        return;
    }

    string ungroup_name = selectedItemList.at(0)->text(0).toStdString();
    if(ungroup_name=="db"||ungroup_name=="3D Object"){
        ui->group_error_message->setText("This item is invaild!");
        return;
    }

    bool is_componet = is_model_componets[ungroup_name];
    if(is_componet){
        ui->group_error_message->setText("This item is a componet,please choose a group!");
        return;
    }

    findGeoNamedNode* visitor = new findGeoNamedNode(ungroup_name);
    visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
    ViewerWindow->getSceneData()->asGroup()->accept(*visitor);

    osg::ref_ptr<osg::Node> getnode = visitor->getNode();

    unsigned int num= getnode->asGroup()->getNumChildren();
    osg::ref_ptr<osg::Node> father_node = getnode->getParent(0);
    for(unsigned int i = 0;i<num;i++){
        osg::ref_ptr<osg::Node> child_node = getnode->asGroup()->getChild(0);
        getnode->asGroup()->removeChild(child_node);
        father_node->asGroup()->addChild(child_node);

    }
    father_node->asGroup()->removeChild(getnode);
    map_to_choose.erase(ungroup_name);
    update_treewidget();

}

void osgqt::on_group_button_clicked(){
    // 注释： 用于合并结点生成group

    QString text = ui->input_group_name->text();
    ui->input_group_name->setText("");
    ui->group_error_message->setText("");
    std::string group_name = text.toStdString();
    if(group_name==""){
        ui->group_error_message->setText("Please input group name!");
        return;}
    bool is_existed = false;
    for(int i = 0;i<all_node_name.size();i++){
        if(all_node_name[i]==group_name)
            is_existed = true;
    }
    if(is_existed){
        ui->group_error_message->setText("\""+text+"\""+" already exists!");
        return;
    }
    std::vector<string> group_node_name;
    std::set<string> group_node_father_name;
    group_node_name.clear();
    group_node_father_name.clear();

    QList<QTreeWidgetItem*> selectedItemList = treewidget->selectedItems();
    for ( int i=0; i!=selectedItemList.size(); ++i ){
        group_node_name.push_back(selectedItemList.at(i)->text(0).toStdString());
    }

    //cout<<"group_node_name"<<endl;

    std::vector<osg::ref_ptr<osg::Node>> group_node;
    group_node.clear();

    for(int i = 0;i<group_node_name.size();i++){
        if(group_node_name[i]=="db"||group_node_name[i]=="3D Object")
            continue;
        findGeoNamedNode* visitor = new findGeoNamedNode(group_node_name[i]);
        visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
        ViewerWindow->getSceneData()->asGroup()->accept(*visitor);

        osg::ref_ptr<osg::Node> getnode = visitor->getNode();
        group_node.push_back(getnode);
        string node_father_name = getnode->getParent(0)->getName();
        group_node_father_name.insert(node_father_name);
        //cout<<group_node_name[i]<<" "<<endl;
    }
    //cout<<""<<endl;
    //cout<<"group_node_father_name.size: "<<group_node_father_name.size()<<endl;

    if(group_node_father_name.size()==0){
        ui->group_error_message->setText("Please select object that you want to group!");
        return;
    }
    if(group_node_father_name.size()>1){
        ui->group_error_message->setText("Object must be at one level");
        return;
    }


    osg::ref_ptr<osg::Node> father_node = group_node.at(0)->getParent(0);

    osg::ref_ptr<osg::Group> grouped_node = new osg::Group;
    grouped_node->setName(group_name);
    for(int i = 0;i<group_node.size();i++){
        osg::ref_ptr<osg::Node> child = group_node[i];
        grouped_node->addChild(child);
        father_node->asGroup()->removeChild(group_node[i]);
    }
    father_node->asGroup()->addChild(grouped_node);

    map_to_choose[group_name] = false;
    update_treewidget();

}

void osgqt::on_select_clicked(){
    //注释：批量选择
    QList<QTreeWidgetItem*> selectedItemList = treewidget->selectedItems();
    for ( int i=0; i!=selectedItemList.size(); ++i ){
        if(QString::compare(selectedItemList.at(i)->text(0),"3D Object"))
            selectedItemList.at(i)->setCheckState(0,Qt::Checked);
    }
    for ( int i=0; i!=selectedItemList.size(); ++i ){
        on_treeWidget_itemChanged_child(selectedItemList.at(i),0);
        on_treeWidget_itemChanged_parent(selectedItemList.at(i),0);
    }
}

void osgqt::on_unselect_clicked(){
    //注释：批量取消
    QList<QTreeWidgetItem*> selectedItemList = treewidget->selectedItems();
    for ( int i=0; i!=selectedItemList.size(); ++i ){
        if(QString::compare(selectedItemList.at(i)->text(0),"3D Object"))
            selectedItemList.at(i)->setCheckState(0,Qt::Unchecked);
    }
    for ( int i=0; i!=selectedItemList.size(); ++i ){
        on_treeWidget_itemChanged_child(selectedItemList.at(i),0);
        on_treeWidget_itemChanged_parent(selectedItemList.at(i),0);
    }
}

void osgqt::on_Delete_node_clicked(){
    //注释： 删除节点
    std::vector<string> node_name;
    node_name.clear();

    QList<QTreeWidgetItem*> selectedItemList = treewidget->selectedItems();
    for ( int i=0; i!=selectedItemList.size(); ++i ){
        node_name.push_back(selectedItemList.at(i)->text(0).toStdString());
    }


    std::vector<osg::ref_ptr<osg::Node>> node;
    node.clear();

    for(int i = 0;i<node_name.size();i++){
        if(node_name[i]=="db"||node_name[i]=="3D Object")
            continue;
        findGeoNamedNode* visitor = new findGeoNamedNode(node_name[i]);
        visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
        ViewerWindow->getSceneData()->asGroup()->accept(*visitor);

        osg::ref_ptr<osg::Node> getnode = visitor->getNode();
        node.push_back(getnode);
    }

    for(int i = 0;i<node.size();i++){
        node[i]->getParent(0)->asGroup()->removeChild(node[i]);
    }
    map_to_choose.clear();
    update_treewidget();
}

void osgqt::on_treeWidget_itemChanged_child(QTreeWidgetItem *item, int column)
{
    // 注释：用于树形图修改子节点勾选状态
    QString itemText = item->text(column);
    current_tree_item_name = itemText.toStdString();

    if(QString::compare(itemText,"3D Object")==0)
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
    QTreeWidgetItem *parent = itm->parent();


    if(parent)
    {
        QString itemText_parent = parent->text(column);
        if(QString::compare(itemText_parent,"3D Object")==0)
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
    QString itemText_parent = parent->text(column);
    if(parent)
    {
        if(QString::compare(itemText_parent,"3D Object")==0)
            return;
        parent->setCheckState(0,Qt::PartiallyChecked);
        SetParentPartiallyChecked(parent,column);
    }
}

void osgqt::treeItemChanged(QTreeWidgetItem *item, int column){
    //auto aaa = ui->treeWidget;
    // 注释：用于勾选时隐藏或显示结点



    QString node_name = item->text(0);
    if(QString::compare(node_name,"3D Object")!=0){
        findGeoNamedNode* visitor = new findGeoNamedNode(node_name.toStdString());
        visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
        ViewerWindow->getSceneData()->asGroup()->accept(*visitor);
        osg::ref_ptr<osg::Node> getnode = visitor->getNode();
        //osg::ref_ptr<osg::Node> getnode = all_node_vector[map_node_to_int[node_name.toStdString()]];
        if(getnode!=NULL){
            if(item->checkState(0)== Qt::Checked||item->checkState(0) == Qt::PartiallyChecked)
                getnode->setNodeMask(1);
            else
                getnode->setNodeMask(0);
        }
    }

    /*
    QTreeWidgetItem *tree = ui->treeWidget->itemAt(0,0);
    if(model_components.size()>0){

        QString aaaa = item->text(1);
        int com_idx = aaaa.toInt();
        //        std::cout<<"sel_tree_c2"<<aaaa.toStdString()<<endl;
        if(com_idx>=0&&com_idx<model_components.size()&&com_idx<tree->childCount())
            if(tree->child(com_idx)->checkState(0)== Qt::Checked)
                model_components.at(com_idx)->setNodeMask(1);
            else
                model_components.at(com_idx)->setNodeMask(0);


        if(Boxes_components.size()>0){
            QTreeWidgetItemIterator it(ui->treeWidget);
            for(int i=0; i<model_components.size()+1; i++)
                it++;
            int box_num = (*it)->childCount();
            if(Boxes_components.size()>0){
                int nums = box_num>Boxes_components.size()?Boxes_components.size():box_num;
                for(int i=0; i<nums; i++){
                    if((*it)->child(i)->checkState(0)== Qt::Checked)
                        Boxes_components.at(i)->setNodeMask(1);
                    else
                        Boxes_components.at(i)->setNodeMask(0);
                }
            }
        }
    }
    */


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

    if(tab_idx==1&&ok_is_clicked){
        string temp_done_name_info = "";
        for(int i = 0;i<dof_done_name.size();i++){
            temp_done_name_info = temp_done_name_info + dof_done_name.at(i)+": done!\n";
        }
        ui->done_name_info->setText(QString::fromStdString(temp_done_name_info));
        ok_is_clicked = false;
    }
    if(tab_idx==0&&treewidget->error_flag){
        ui->group_error_message->setText(QString::fromStdString(treewidget->error_message));
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
            ViewerWindow->getSceneData()->asGroup()->accept(*visitor);

            osg::ref_ptr<osg::Node> getnode = visitor->getNode();
            attach_node.push_back(getnode);
            qDebug()<<QString::fromStdString(getnode->getName());
        }

        findGeoNamedNode* visitor = new findGeoNamedNode(drop_name);
        visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
        ViewerWindow->getSceneData()->asGroup()->accept(*visitor);

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

    if(tab_idx==0&&box_is_picked){
        map<string,bool>::iterator it;
        it = map_to_choose.begin();
        while(it != map_to_choose.end())
        {
            QMap<QString,QTreeWidgetItem*>::iterator iter_item = itemMap_.find(QString::fromStdString(it->first));
            if (iter_item != itemMap_.end()){
                iter_item.value()->setSelected(it->second);
            }


            findGeoNamedNode* visitor = new findGeoNamedNode(it->first);
            visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
            ViewerWindow->getSceneData()->asGroup()->accept(*visitor);

            osg::ref_ptr<osg::Node> getnode = visitor->getNode();
            osg::ref_ptr<osg::StateSet> now_state = new osg::StateSet;
            if(it->second){
                osg::ref_ptr<osg::Material> material = new osg::Material;
                osg::Vec4 redColor(0,191,255, 1.0f);
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


                now_state->setAttributeAndModes(material.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
                getnode->setStateSet(now_state);
            }else{
                now_state = init_state_set.get();
                getnode->setStateSet(now_state);
            }
            it++;
        }
        box_is_picked = false;
    }

    if(tab_idx==1&&tree_widget_is_picked_empyt_2){
        /*
        QTreeWidgetItemIterator item(ui->treeWidget);
        while (*item) {
                (*item)->setSelected(map_to_choose[(*item)->text(0).toStdString()]);
            ++item;
        }
        */
        //auto aaaa = tree_widget_is_picked;
        //auto bbbb = tree_widget_is_picked_empyt;
        //tree_widget_is_picked = false;
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
    }

    if(tab_idx==0&&(mouse_is_picked||tree_widget_is_picked||tree_widget_is_picked_empyt)){
        /*
        QTreeWidgetItemIterator item(ui->treeWidget);
        while (*item) {
                (*item)->setSelected(map_to_choose[(*item)->text(0).toStdString()]);
            ++item;
        }
        */
        //auto aaaa = tree_widget_is_picked;
        //auto bbbb = tree_widget_is_picked_empyt;
        tree_widget_is_picked = false;
        tree_widget_is_picked_empyt = false;
        map<string,bool>::iterator it;
        it = map_to_choose.begin();
        while(it != map_to_choose.end())
        {

            QMap<QString,QTreeWidgetItem*>::iterator iter_item = itemMap_.find(QString::fromStdString(it->first));
            if (iter_item != itemMap_.end()){
                iter_item.value()->setSelected(it->second);
            }


            findGeoNamedNode* visitor = new findGeoNamedNode(it->first);
            visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
            ViewerWindow->getSceneData()->asGroup()->accept(*visitor);

            osg::ref_ptr<osg::Node> getnode = visitor->getNode();
            osg::ref_ptr<osg::StateSet> now_state = new osg::StateSet;
            if(it->second){
                osg::ref_ptr<osg::Material> material = new osg::Material;
                osg::Vec4 redColor(0,191,255, 0.5f);
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


                now_state->setAttributeAndModes(material.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
                getnode->setStateSet(now_state);
            }else{
                now_state = init_state_set.get();
                getnode->setStateSet(now_state);
            }
            it ++;
        }
    }
    if(tab_idx==0&&mouse_is_picked_empty){
        osg::ref_ptr<osg::StateSet> now_state = new osg::StateSet;
        now_state = init_state_set.get();


        map<string,bool>::iterator it;
        it = map_to_choose.begin();
        while(it != map_to_choose.end())
        {
            if(it->second){
                findGeoNamedNode* visitor = new findGeoNamedNode(it->first);
                visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
                ViewerWindow->getSceneData()->asGroup()->accept(*visitor);

                osg::ref_ptr<osg::Node> getnode = visitor->getNode();
                getnode->setStateSet(now_state);
                it->second = false;
            }
            it ++;
        }
        mouse_is_picked_empty = false;
        QTreeWidgetItemIterator item(treewidget);
        while (*item) {
            (*item)->setSelected(false);
            ++item;
        }
    }

    /*
    QTreeWidgetItemIterator it(ui->treeWidget);
    while (*it) {
        (*it)->setSelected(false);
        if (QString::compare((*it)->text(0),QString::fromStdString(current_tree_item_name))==0){
            (*it)->setSelected(true);
        }
        ++it;
    }
    */
}

void osgqt::addmodel(osg::ref_ptr<osg::Node> model){
    //注释：用于加载模型
    loadedModel = model;
    osg::ref_ptr<osg::Group> root = new osg::Group;
    osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform;
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

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(geo.get());

    camera->addChild(geode.get());
    root->addChild(camera);



    ViewerWindow->setSceneData(root.get());
}

void osgqt::addmodel2(osg::ref_ptr<osg::Node> model){
    //注释：用于加载模型
    loadedModel2 = model;
    osg::ref_ptr<osg::Group> root = new osg::Group;
    osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform;
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

    ViewerWindow2->setSceneData(root.get());
}

void osgqt::update_treewidget(){
    // 注释：用于更新树形结构图
    is_model_componets.clear();
    all_node_name.clear();
    is_dof_node1.clear();
    //map_node_to_int.clear();
    //all_node_vector.clear();
    for(int i = 0;i<model_structure_tree.size();i++)
        model_structure_tree[i].clear();
    treewidget->clear();

    osg::ref_ptr<osg::Node>model = ViewerWindow->getSceneData()->asGroup()->getChild(0)->asGroup()->getChild(0);
    InfoVisitor infoVisitor;
    infoVisitor.setNodeMaskOverride(1);
    model->accept(infoVisitor);
    itemMap_.clear();
    root_tree = new QTreeWidgetItem(treewidget);
    root_tree->setText(0,"3D Object");    //树形控件显示的文本信息
    ///*
    QTreeWidgetItem* db = new QTreeWidgetItem(root_tree);
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
    for(int i=0;i<all_node_name.size();i++){
        QString father_name = QString::fromStdString(all_node_name.at(i));
        for(int j=0;j<model_structure_tree[i].size();j++){
            QString child_name = QString::fromStdString(model_structure_tree[i][j]);
            findGeoNamedNode* visitor = new findGeoNamedNode(child_name.toStdString());
            visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
            ViewerWindow->getSceneData()->asGroup()->accept(*visitor);
            osg::ref_ptr<osg::Node> getnode = visitor->getNode();
            unsigned int aaaa = getnode->getNodeMask();
            QTreeWidgetItem *temp_item;
            QMap<QString,QTreeWidgetItem*>::iterator iter_item = itemMap_.find(father_name);
            if (iter_item == itemMap_.end()){
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
            bool is_group = is_model_componets[child_name.toStdString()];
            if(!is_group){
                bool is_dof = is_dof_node1[child_name.toStdString()];
                if(is_dof){
                    temp_item->setIcon(0,QIcon(":/ico/3D_Coordinates.ico")); //注释： dof图标
                }
                else{
                    temp_item->setIcon(0,QIcon(":/ico/ooopic_1539861093.ico")); //注释： 文件夹图标
                }
            }

            if(!child_name.isEmpty()){
                itemMap_.insert(child_name,temp_item);
            }
        }

    }

    map<string,bool>::iterator itr;
    itr = is_model_componets.begin();
    while(itr != is_model_componets.end())
    {
        if(itr->second){
            QString obj_name = QString::fromStdString(itr->first);
            QMap<QString,QTreeWidgetItem*>::iterator iter_item = itemMap_.find(obj_name);
            on_treeWidget_itemChanged_parent(iter_item.value(),0);
        }
        itr++;
    }
    map<string,bool>::iterator it;
    it = map_to_choose.begin();
    while(it != map_to_choose.end())
    {
        map_to_choose[it->first] = false;
        it++;
    }
    treewidget->is_model_componets.clear();
    treewidget->is_model_componets = is_model_componets;
    treewidget->model_structure_tree.clear();
    treewidget->model_structure_tree = model_structure_tree;
    treewidget->all_node_name.clear();
    treewidget->all_node_name = all_node_name;
    treewidget->expandAll();
}

/*
void osgqt::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if ((event->pos() - startPos).manhattanLength() < QApplication::startDragDistance())
        {
            QDrag *drag = new QDrag(this);
            QMimeData *data = new QMimeData;
            drag->setMimeData(data);
            drag->exec(Qt::MoveAction);
        }
    }
}
*/






void osgqt::on_open_clicked(){
    // 注释：用于打开模型和之后的一系列初始化
    current_file_name = "";
    QString directory, file_name, file_path;
    QFileInfo fi;
    //QString current_path = QDir::currentPath();
    directory = QFileDialog::getOpenFileName(this,tr("ChooseFile"),"","*.ive");
    fi = QFileInfo(directory);
    file_name = fi.fileName();
    current_file_name = file_name.toStdString();
    //cout<<"current_file_name: "<<current_file_name<<endl;
    file_path = fi.absolutePath();
    if(!directory.isEmpty()){
        if(ui->directorycomboBox->findText(directory)==-1)
            ui->directorycomboBox->addItem(directory);
        ui->directorycomboBox->setCurrentIndex((ui->directorycomboBox->findText(directory)));
    }
    std::string  path = directory.toStdString();
    if(path=="")
        return;
    flag_open = false;
    current_tree_item_name = "";

    map_to_choose.clear();
    mouse_is_picked_empty = false;
    mouse_is_picked = false;
    tree_widget_is_picked = false;
    tree_widget_is_picked_empyt = false;
    box_is_picked = false;
    //cout<<"path: "<<path<<endl;
    osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(path,a);

    //auto aaaa = model->asGroup();


    model_state_set = model->getStateSet();
    addmodel(model);
    init_state_set = model->getStateSet();
    box_infomation = "";
    //std::cout<<file_name.toStdString()<<std::endl;

    //*/

    update_treewidget(); //注释:更新tree_widget

    /*
    for(int i=0; i<model_components.size(); i++){
        QString this_name = QString::fromStdString(model_components.at(i)->getName());

        QTreeWidgetItem* group1 = new QTreeWidgetItem(root_tree);
        group1->setText(0,this_name);    //树形控件显示的文本信息
        std::string box_name;
        int2str(i, box_name);
        group1->setText(1,QString::fromStdString(box_name));    //树形控件显示的文本信息
        group1->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);   //设置树形控件子项的属性
        group1->setCheckState(0,Qt::Checked); //初始状态被选中

    }
    */

    /*
    QTreeWidgetItem* box_tree = new QTreeWidgetItem(ui->treeWidget);
    box_tree->setText(0, "Marked Boxes");
    */



    flag_open = true;
}

void osgqt::on_directorycomboBox_currentIndexChanged(int index)
{
    // 注释： 用于更换模型


    flag_open = false;
    current_tree_item_name = "";
    current_file_name = "";
    map_to_choose.clear();
    mouse_is_picked_empty = false;
    mouse_is_picked = false;
    tree_widget_is_picked = false;
    tree_widget_is_picked_empyt = false;
    box_is_picked = false;
    QString s = ui->directorycomboBox->currentText();
    QFileInfo fi = QFileInfo(s);
    current_file_name = fi.fileName().toStdString();
    std::string  path = s.toStdString();

    osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(path,a);

    //auto aaaa = model->asGroup();

    model_state_set = model->getStateSet();
    addmodel(model);
    init_state_set = model->getStateSet();
    box_infomation = "";

    update_treewidget();
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

void osgqt::on_toolButton_pressed()
{
    //注释： 用于选择存储路径


    QString directory = ui->directorycomboBox->currentText();
    QString file_name, file_path;
    QFileInfo fi;
    fi = QFileInfo(directory);
    file_name = fi.fileName();
    //cout<<"current_file_name: "<<current_file_name<<endl;
    file_path = fi.absolutePath();

    QString fileName2 = QFileDialog::getSaveFileName(this,
                                                     tr("Save file"),
                                                     file_path,
                                                     tr("ive (*.ive);;flt (*.flt)"));

    if (!fileName2.isNull())
    {
        QString save_file_name;
        QFileInfo save_fi;
        save_fi = QFileInfo(fileName2);
        save_file_name = save_fi.fileName();
        map<string,bool>::iterator it;
        it = map_to_choose.begin();
        while(it != map_to_choose.end())
        {
            it->second = false;
            it++;
        }
        tree_widget_is_picked = true;

        Open_all_node_mask1 open;
        open.setNodeMaskOverride(1);

        osg::ref_ptr<osg::Node> save_node = new osg::Node;
        if(loadedModel==NULL)
            return;
        osg::ref_ptr<osg::Node> model = ViewerWindow->getSceneData()->asGroup()->getChild(0)->asGroup()->getChild(0);
        save_node = dynamic_cast<osg::Node*>(model->clone(osg::CopyOp::DEEP_COPY_ALL));
        save_node->accept(open);


        osgDB::writeNodeFile(*save_node, fileName2.toStdString());


        DOFAdder *dofadder = new DOFAdder();

        dofadder->generate_dofnode_added_model(fileName2.toStdString(),fileName2.toStdString());




        update_treewidget();
        string info = save_file_name.toStdString()+" has been saved";
        ui->saved->setText(QString::fromStdString(info));
    }
    else
    {
        return;
    }
    if(!fileName2.isEmpty()){
        if(ui->comboBox->findText(fileName2)==-1)
            ui->comboBox->addItem(fileName2);
        ui->comboBox->setCurrentIndex((ui->comboBox->findText(fileName2)));
    }

}


void osgqt::on_tabWidget_currentChanged(int index)
{
    tab_idx = index;
}

void osgqt::on_treeWidget_itemSelectionChanged()
{
    //注释： 用于更新map_to_choose
    if(!box_is_picked){
        map<string,bool>::iterator it;
        it = map_to_choose.begin();
        while(it != map_to_choose.end())
        {
            it->second=false;
            it++;
        }
        QList<QTreeWidgetItem*> selectedItemList = treewidget->selectedItems();
        for ( int i=0; i!=selectedItemList.size(); ++i ){
            if(QString::compare(selectedItemList.at(i)->text(0),"3D Object")==0)
                continue;
            map_to_choose[selectedItemList.at(i)->text(0).toStdString()]=true;
        }
        tree_widget_is_picked_empyt = true;
    }

}


void osgqt::on_Reset_clicked()
{
    // 注释：重置物体位置
    ViewerWindow->setCameraManipulator(manipulator);
}




void osgqt::on_open2_clicked()
{
    current_file_name2 = "";
    QString directory, file_name, file_path;
    QFileInfo fi;
    //QString current_path = QDir::currentPath();
    directory = QFileDialog::getOpenFileName(this,tr("ChooseFile"),"","*.ive");
    fi = QFileInfo(directory);
    file_name = fi.fileName();
    current_file_name2 = file_name.toStdString();
    //cout<<"current_file_name: "<<current_file_name<<endl;
    file_path = fi.absolutePath();
    if(!directory.isEmpty()){
        if(ui->comboBox_2->findText(directory)==-1)
            ui->comboBox_2->addItem(directory);
        ui->comboBox_2->setCurrentIndex((ui->comboBox_2->findText(directory)));
    }
    std::string  path = directory.toStdString();
    if(path=="")
        return;
    ui->done_name_info->setText("");
    ok_is_clicked = false;
    dof_done_name.clear();
    ui->tp->setDisabled(false);
    ui->opof->setDisabled(false);
    ui->ok->setDisabled(false);
    ui->cancel->setDisabled(false);
    Two_point_coor_1.x()=0.0f;
    Two_point_coor_1.y()=0.0f;
    Two_point_coor_1.z()=0.0f;
    Two_point_coor_2.x()=0.0f;
    Two_point_coor_2.y()=0.0f;
    Two_point_coor_2.z()=0.0f;
    one_point_coor.x()=0.0f;
    one_point_coor.y()=0.0f;
    one_point_coor.z()=0.0f;
    Two_point_is_picked_1 = false;
    Two_point_is_picked_2 = false;
    one_point_is_picked_1 = false;
    one_point_is_picked_2 = false;

    ui->opof_1->setCheckable(false);
    ui->opof_2->setCheckable(false);
    ui->tp_1->setCheckable(false);
    ui->tp_2->setCheckable(false);
    ui->translation->setCheckable(false);
    ui->rotation->setCheckable(false);
    ui->spiral->setCheckable(false);
    choose_dof_mode = 0;
    dof_type = 0;
    dof_pick2_mode = 0;

    tree_widget_is_picked_empyt_2 = false;
    map_to_choose_2.clear();
    dof_HPR.clear();
    dof_trans.clear();
    //cout<<"path: "<<path<<endl;
    osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(path,a);

    //auto aaaa = model->asGroup();


    model_state_set_2 = model->getStateSet();
    addmodel2(model);
    init_state_set_2 = model->getStateSet();





    //std::cout<<file_name.toStdString()<<std::endl;

    //*/

    update_treewidget_2();
}


void osgqt::update_treewidget_2(){
    // 注释：用于更新树形结构图
    is_model_componets_2.clear();
    all_node_name_2.clear();
    is_dof_node2.clear();
    //map_node_to_int.clear();
    //all_node_vector.clear();
    for(int i = 0;i<model_structure_tree_2.size();i++)
        model_structure_tree_2[i].clear();
    treewidget2->clear();

    osg::ref_ptr<osg::Node>model = ViewerWindow2->getSceneData()->asGroup()->getChild(0)->asGroup()->getChild(0);
    InfoVisitor2 infoVisitor;
    infoVisitor.setNodeMaskOverride(1);
    model->accept(infoVisitor);
    itemMap_.clear();
    root_tree_2 = new QTreeWidgetItem(treewidget2);
    root_tree_2->setText(0,"3D Object");    //树形控件显示的文本信息
    root_tree_2->setExpanded(true);
    ///*
    QTreeWidgetItem* db = new QTreeWidgetItem(root_tree_2);
    db->setText(0,QString::fromStdString("db"));
    itemMap_.insert(QString::fromStdString("db"),db);

    //std::string box_name;
    //int2str(0, box_name);
    //db->setText(1,QString::fromStdString(box_name));    //树形控件显示的文本信息
    db->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);   //设置树形控件子项的属性
    db->setCheckState(0,Qt::Checked); //初始状态被选中
    db->setIcon(0,QIcon(":/ico/ooopic_1539861093.ico")); //注释： 文件夹图标
    db->setExpanded(true);
    //*/
    ///*
    for(int i=0;i<all_node_name_2.size();i++){
        QString father_name = QString::fromStdString(all_node_name_2.at(i));
        for(int j=0;j<model_structure_tree_2[i].size();j++){
            QString child_name = QString::fromStdString(model_structure_tree_2[i][j]);
            findGeoNamedNode* visitor = new findGeoNamedNode(child_name.toStdString());
            visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
            ViewerWindow2->getSceneData()->asGroup()->accept(*visitor);
            osg::ref_ptr<osg::Node> getnode = visitor->getNode();
            unsigned int aaaa = getnode->getNodeMask();
            QTreeWidgetItem *temp_item;
            QMap<QString,QTreeWidgetItem*>::iterator iter_item = itemMap_.find(father_name);
            if (iter_item == itemMap_.end()){
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
            bool is_group = is_model_componets_2[child_name.toStdString()];
            if(!is_group){
                bool is_dof = is_dof_node2[child_name.toStdString()];
                if(is_dof){
                    temp_item->setIcon(0,QIcon(":/ico/3D_Coordinates.ico")); //注释： dof图标
                    temp_item->setExpanded(true);
                }
                else{

                    temp_item->setIcon(0,QIcon(":/ico/ooopic_1539861093.ico")); //注释： 文件夹图标
                }
            }
            if(!child_name.isEmpty()){
                itemMap_.insert(child_name,temp_item);
            }
        }

    }

    map<string,bool>::iterator itr;
    itr = is_model_componets_2.begin();
    //auto aaa = is_model_componets_2;
    while(itr != is_model_componets_2.end())
    {
        if(itr->second){
            QString obj_name = QString::fromStdString(itr->first);
            QMap<QString,QTreeWidgetItem*>::iterator iter_item = itemMap_.find(obj_name);
            on_treeWidget_itemChanged_parent(iter_item.value(),0);
        }
        itr++;
    }
    map<string,bool>::iterator it;
    it = map_to_choose_2.begin();
    while(it != map_to_choose_2.end())
    {
        map_to_choose_2[it->first] = false;
        string aaaa = it->first;
        qDebug()<<QString::fromStdString(aaaa)<<" "<<it->second;
        it++;
    }
    //treewidget->is_model_componets.clear();
    //treewidget->is_model_componets = is_model_componets;
    //treewidget->model_structure_tree.clear();
    //treewidget->model_structure_tree = model_structure_tree;
    //treewidget->all_node_name.clear();
    //treewidget->all_node_name = all_node_name;
    //auto aaaaaaa = ViewerWindow2->getSceneData()->asGroup()->getChild(0)->asGroup();
    //treewidget2->expandAll();
}



void osgqt::on_treeWidget_itemSelectionChanged2()
{
    //注释： 用于更新map_to_choose
    map<string,bool>::iterator it;
    it = map_to_choose_2.begin();
    while(it != map_to_choose_2.end())
    {
        it->second=false;
        it++;
    }
    QList<QTreeWidgetItem*> selectedItemList = treewidget2->selectedItems();
    for ( int i=0; i!=selectedItemList.size(); ++i ){
        if(QString::compare(selectedItemList.at(i)->text(0),"3D Object")==0)
            continue;
        map_to_choose_2[selectedItemList.at(i)->text(0).toStdString()]=true;
    }
    tree_widget_is_picked_empyt_2 = true;
}

void osgqt::on_toolButton_2_clicked()
{


    QString directory = ui->comboBox_2->currentText();
    QString file_name, file_path;
    QFileInfo fi;
    fi = QFileInfo(directory);
    file_name = fi.fileName();
    //cout<<"current_file_name: "<<current_file_name<<endl;
    file_path = fi.absolutePath();

    QString fileName2 = QFileDialog::getSaveFileName(this,
                                                     tr("Save file"),
                                                     file_path,
                                                     tr("ive (*.ive);;flt (*.flt)"));

    if (!fileName2.isNull())
    {
        QString save_file_name;
        QFileInfo save_fi;
        save_fi = QFileInfo(fileName2);
        save_file_name = save_fi.fileName();

        map<string,bool>::iterator it;
        it = map_to_choose_2.begin();
        while(it != map_to_choose_2.end())
        {
            it->second = false;
            it++;
        }
        tree_widget_is_picked_empyt_2 = true;

        Open_all_node_mask open;
        open.setNodeMaskOverride(1);

        osg::ref_ptr<osg::Node> save_node = new osg::Node;
        if(loadedModel2==NULL)
            return;
        osg::ref_ptr<osg::Node> model = ViewerWindow2->getSceneData()->asGroup()->getChild(0)->asGroup()->getChild(0);

        save_node = dynamic_cast<osg::Node*>(model->clone(osg::CopyOp::DEEP_COPY_ALL));
        save_node->accept(open);


        osgDB::writeNodeFile(*save_node, fileName2.toStdString());
        update_treewidget_2();
        string info = save_file_name.toStdString()+" has been saved";
        ui->label->setText(QString::fromStdString(info));
    }
    else
    {
        return;
    }
    if(!fileName2.isEmpty()){
        if(ui->comboBox_3->findText(fileName2)==-1)
            ui->comboBox_3->addItem(fileName2);
        ui->comboBox_3->setCurrentIndex((ui->comboBox_3->findText(fileName2)));
    }

}


void osgqt::on_comboBox_2_currentIndexChanged(int index)
{
    ok_is_clicked = false;
    ui->done_name_info->setText("");
    ui->tp->setDisabled(false);
    ui->opof->setDisabled(false);
    ui->ok->setDisabled(false);
    ui->cancel->setDisabled(false);
    Two_point_coor_1.x()=0.0f;
    Two_point_coor_1.y()=0.0f;
    Two_point_coor_1.z()=0.0f;
    Two_point_coor_2.x()=0.0f;
    Two_point_coor_2.y()=0.0f;
    Two_point_coor_2.z()=0.0f;
    one_point_coor.x()=0.0f;
    one_point_coor.y()=0.0f;
    one_point_coor.z()=0.0f;
    Two_point_is_picked_1 = false;
    Two_point_is_picked_2 = false;
    one_point_is_picked_1 = false;
    one_point_is_picked_2 = false;
    dof_HPR.clear();
    dof_trans.clear();


    ui->opof_1->setCheckable(false);
    ui->opof_2->setCheckable(false);
    ui->tp_1->setCheckable(false);
    ui->tp_2->setCheckable(false);
    ui->translation->setCheckable(false);
    ui->rotation->setCheckable(false);
    ui->spiral->setCheckable(false);
    dof_pick2_mode = 0;
    choose_dof_mode = 0;
    dof_type = 0;
    current_file_name2 = "";
    tree_widget_is_picked_empyt_2 = false;
    map_to_choose_2.clear();

    QString s = ui->comboBox_2->currentText();
    QFileInfo fi = QFileInfo(s);
    current_file_name2 = fi.fileName().toStdString();
    std::string  path = s.toStdString();

    //cout<<"path: "<<path<<endl;
    osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(path,a);

    //auto aaaa = model->asGroup();


    model_state_set_2 = model->getStateSet();


    addmodel2(model);





    init_state_set_2 = model->getStateSet();





    //std::cout<<file_name.toStdString()<<std::endl;

    //*/

    update_treewidget_2();
}


void osgqt::treeItemChanged2(QTreeWidgetItem *item, int column){
    //auto aaa = ui->treeWidget;
    // 注释：用于勾选时隐藏或显示结点



    QString node_name = item->text(0);
    if(QString::compare(node_name,"3D Object")!=0){
        findGeoNamedNode* visitor = new findGeoNamedNode(node_name.toStdString());
        visitor->setNodeMaskOverride(1);  //注释： 覆盖nodemask，可查找隐藏节点
        ViewerWindow2->getSceneData()->asGroup()->accept(*visitor);
        osg::ref_ptr<osg::Node> getnode = visitor->getNode();
        //osg::ref_ptr<osg::Node> getnode = all_node_vector[map_node_to_int[node_name.toStdString()]];
        if(getnode!=NULL){
            if(item->checkState(0)== Qt::Checked||item->checkState(0) == Qt::PartiallyChecked)
                getnode->setNodeMask(1);
            else
                getnode->setNodeMask(0);
        }
    }

    /*
    QTreeWidgetItem *tree = ui->treeWidget->itemAt(0,0);
    if(model_components.size()>0){

        QString aaaa = item->text(1);
        int com_idx = aaaa.toInt();
        //        std::cout<<"sel_tree_c2"<<aaaa.toStdString()<<endl;
        if(com_idx>=0&&com_idx<model_components.size()&&com_idx<tree->childCount())
            if(tree->child(com_idx)->checkState(0)== Qt::Checked)
                model_components.at(com_idx)->setNodeMask(1);
            else
                model_components.at(com_idx)->setNodeMask(0);


        if(Boxes_components.size()>0){
            QTreeWidgetItemIterator it(ui->treeWidget);
            for(int i=0; i<model_components.size()+1; i++)
                it++;
            int box_num = (*it)->childCount();
            if(Boxes_components.size()>0){
                int nums = box_num>Boxes_components.size()?Boxes_components.size():box_num;
                for(int i=0; i<nums; i++){
                    if((*it)->child(i)->checkState(0)== Qt::Checked)
                        Boxes_components.at(i)->setNodeMask(1);
                    else
                        Boxes_components.at(i)->setNodeMask(0);
                }
            }
        }
    }
    */


}

void osgqt::on_Animation_test_clicked()
{
    QList<QTreeWidgetItem*> selectedItemList = treewidget2->selectedItems();
    std::vector<osg::ref_ptr<osgSim::DOFTransform>> dof_nodes;
    for ( int i=0; i!=selectedItemList.size(); ++i ){
        // 士林：animation test代码
        std::string node_name=selectedItemList.at(i)->text(0).toStdString();
        findDofNamedNode find_dof(node_name);
        ViewerWindow2->getSceneData()->accept(find_dof);
        osg::ref_ptr<osgSim::DOFTransform> dof_node=find_dof.getNode();
        //如果不是dof，报错
        if(dof_node==nullptr)
        {
            ui->dof_add_info->setText("the nodes you selected contain none-dof node");
            return;
        }
        else
            dof_nodes.push_back(dof_node);
    }
    for(unsigned int i=0;i<dof_nodes.size();i++)
    {   osg::ref_ptr<osgSim::DOFTransform> dofnode = dof_nodes.at(i);
        std::string node_name=dofnode->getName();
        Motion_type motion_type;
        //根据后缀名确定运动类型
        if(node_name.length()>=1&&(node_name.substr(node_name.length()-2,2)=="_r"))
            motion_type=R;
        else if(node_name.length()>=1&&(node_name.substr(node_name.length()-2,2)=="_t"))
            motion_type=T;
        else if(node_name.length()>=2&&(node_name.substr(node_name.length()-3,3)=="_rt"))
            motion_type=RT;
        else
            motion_type=NONE;
        osg::Vec3 curvec;
        switch (motion_type)
        {
        case R:{

            curvec=dofnode->getCurrentHPR();
            curvec=dofnode->getCurrentHPR();
            curvec.set(curvec.x(),curvec.y()+DEGREE_PER_STEP,curvec.z());
            dofnode->setCurrentHPR(curvec);
            break;}
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
}

void osgqt::on_translation_clicked()
{
    if(ui->translation->isChecked())
        dof_type = 1;
}

void osgqt::on_rotation_clicked()
{
    if(ui->rotation->isChecked())
        dof_type = 2;
}

void osgqt::on_spiral_clicked()
{
    if(ui->spiral->isChecked())
        dof_type = 3;
}

void osgqt::on_tp_clicked()
{
    ui->opof_1->setChecked(false);
    ui->opof_2->setChecked(false);
    ui->tp_1->setChecked(false);
    ui->tp_2->setChecked(false);
    ui->translation->setChecked(false);
    ui->rotation->setChecked(false);
    ui->spiral->setChecked(false);
    QWidget::update();
    ui->opof_1->setCheckable(false);
    ui->opof_2->setCheckable(false);
    ui->tp_1->setCheckable(true);
    ui->tp_2->setCheckable(true);
    ui->translation->setCheckable(true);
    ui->rotation->setCheckable(true);
    ui->spiral->setCheckable(true);

    ui->mode_info->setText("Two Points Mode");
    choose_dof_mode = 1;
    dof_type = 0;
    dof_pick2_mode = 0;

    findGeoNamedNode* visitor1 = new findGeoNamedNode("red_dot_1");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor1);
    osg::ref_ptr<osg::Node> getnode1 = visitor1->getNode();
    if(getnode1!=NULL){
        getnode1->getParent(0)->removeChild(getnode1);
    }
    findGeoNamedNode* visitor2 = new findGeoNamedNode("green_dot_1");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor2);
    osg::ref_ptr<osg::Node> getnode2 = visitor2->getNode();
    if(getnode2!=NULL){
        getnode2->getParent(0)->removeChild(getnode2);
    }
    findGeoNamedNode* visitor3 = new findGeoNamedNode("blue_dot_1");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor3);
    osg::ref_ptr<osg::Node> getnode3 = visitor3->getNode();
    if(getnode3!=NULL){
        getnode3->getParent(0)->removeChild(getnode3);
    }
    findGeoNamedNode* visitor4 = new findGeoNamedNode("lineeeeeeeee");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor4);
    osg::ref_ptr<osg::Node> getnode4 = visitor4->getNode();
    if(getnode4!=NULL){
        getnode4->getParent(0)->removeChild(getnode4);
    }
}

void osgqt::on_opof_clicked()
{
    ui->opof_1->setChecked(false);
    ui->opof_2->setChecked(false);
    ui->tp_1->setChecked(false);
    ui->tp_2->setChecked(false);
    ui->translation->setChecked(false);
    ui->rotation->setChecked(false);
    ui->spiral->setChecked(false);
    QWidget::update();
    ui->opof_1->setCheckable(true);
    ui->opof_2->setCheckable(true);
    ui->tp_1->setCheckable(false);
    ui->tp_2->setCheckable(false);
    ui->translation->setCheckable(true);
    ui->rotation->setCheckable(true);
    ui->spiral->setCheckable(true);
    ui->mode_info->setText("One Point and One Face Mode");
    choose_dof_mode = 2;
    dof_type = 0;
    dof_pick2_mode = 0;

    findGeoNamedNode* visitor1 = new findGeoNamedNode("red_dot_1");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor1);
    osg::ref_ptr<osg::Node> getnode1 = visitor1->getNode();
    if(getnode1!=NULL){
        getnode1->getParent(0)->removeChild(getnode1);
    }
    findGeoNamedNode* visitor2 = new findGeoNamedNode("green_dot_1");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor2);
    osg::ref_ptr<osg::Node> getnode2 = visitor2->getNode();
    if(getnode2!=NULL){
        getnode2->getParent(0)->removeChild(getnode2);
    }
    findGeoNamedNode* visitor3 = new findGeoNamedNode("blue_dot_1");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor3);
    osg::ref_ptr<osg::Node> getnode3 = visitor3->getNode();
    if(getnode3!=NULL){
        getnode3->getParent(0)->removeChild(getnode3);
    }
    findGeoNamedNode* visitor4 = new findGeoNamedNode("lineeeeeeeee");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor4);
    osg::ref_ptr<osg::Node> getnode4 = visitor4->getNode();
    if(getnode4!=NULL){
        getnode4->getParent(0)->removeChild(getnode4);
    }
}


void osgqt::on_ok_clicked()
{

    QList<QTreeWidgetItem*> selectedItemList = treewidget2->selectedItems();
    if(selectedItemList.size()!=1)
    {
        ui->dof_add_info->setText("please select only one dof node");
        return;
    }
    std::string node_name=selectedItemList.at(0)->text(0).toStdString();
    findDofNamedNode find_dof(node_name);
    ViewerWindow2->getSceneData()->accept(find_dof);
    osg::ref_ptr<osgSim::DOFTransform> dof_node=find_dof.getNode();


    //判断是不是dof节点，用命名
    if(dof_node==NULL)
    {
        ui->dof_add_info->setText("the node you selected is not a dof");
        return;
    }

    Motion_type motion_type;
    switch (dof_type) {
    case 1:
        motion_type=T;
        break;
    case 2:
        motion_type=R;
        break;
    case 3:
        motion_type=RT;
        break;
    default:
        motion_type=NONE;
        break;
    }

    // 按照不同的模式选择
    if(choose_dof_mode == 1){
        if(motion_type==NONE){
            ui->dof_add_info->setText("please choose a dof type");
            return;
        }else{
            if(Two_point_is_picked_1&&Two_point_is_picked_2){

                qDebug()<<"point1: "<<Two_point_coor_1.x()<<" "<<Two_point_coor_1.y()<<" "<<Two_point_coor_1.z();
                qDebug()<<"point2: "<<Two_point_coor_2.x()<<" "<<Two_point_coor_2.y()<<" "<<Two_point_coor_2.z();
                DOFCalculation::modify_dof_two_point(dof_node,Two_point_coor_1,Two_point_coor_2,motion_type);
                bool temp = map_to_choose_2[node_name];
                map_to_choose_2.erase(node_name);
                map_to_choose_2[dof_node->getName()] = temp;
                vector<string>::iterator it = dof_done_name.begin();
                for(;it != dof_done_name.end();)
                {
                    if(*it == node_name)
                        it = dof_done_name.erase(it);
                    else
                        ++it;
                }

                dof_done_name.push_back(dof_node->getName());

                ui->dof_add_info->setText("successfully add");
            }else{
                ui->dof_add_info->setText("please choose two point");
                return;
            }
        }
    }else if(choose_dof_mode == 2){
        if(motion_type==NONE){
            ui->dof_add_info->setText("please choose a dof type");
            return;
        }else{
            if(one_point_is_picked_1&&one_point_is_picked_2){
                Triangle f((*facepointarray)[0],(*facepointarray)[1],(*facepointarray)[2]);
                qDebug()<<"point: "<<one_point_coor.x()<<" "<<one_point_coor.y()<<" "<<one_point_coor.z();
                qDebug()<<"facepoint1: "<<f.get_a().x()<<" "<<f.get_a().y()<<" "<<f.get_a().z();
                qDebug()<<"facepoint1: "<<f.get_b().x()<<" "<<f.get_b().y()<<" "<<f.get_b().z();
                qDebug()<<"facepoint1: "<<f.get_c().x()<<" "<<f.get_c().y()<<" "<<f.get_c().z();
                DOFCalculation::modify_dof_one_point_one_face(dof_node,one_point_coor,f,motion_type);
                bool temp = map_to_choose_2[node_name];
                map_to_choose_2.erase(node_name);
                map_to_choose_2[dof_node->getName()] = temp;
                vector<string>::iterator it = dof_done_name.begin();
                for(;it != dof_done_name.end();)
                {
                    if(*it == node_name)
                        it = dof_done_name.erase(it);
                    else
                        ++it;
                }
                dof_done_name.push_back(dof_node->getName());

                ui->dof_add_info->setText("successfully add");
            }else{
                ui->dof_add_info->setText("please choose a point and a face");
                return;
            }
        }
    }else{
        ui->dof_add_info->setText("please choose a mode");
        return;
    }
    update_treewidget_2();
    ui->opof_1->setChecked(false);
    ui->opof_2->setChecked(false);
    ui->tp_1->setChecked(false);
    ui->tp_2->setChecked(false);
    ui->translation->setChecked(false);
    ui->rotation->setChecked(false);
    ui->spiral->setChecked(false);
    QWidget::update();
    ui->opof_1->setCheckable(false);
    ui->opof_2->setCheckable(false);
    ui->tp_1->setCheckable(false);
    ui->tp_2->setCheckable(false);
    ui->translation->setCheckable(false);
    ui->rotation->setCheckable(false);
    ui->spiral->setCheckable(false);

    ui->mode_info->setText("");

    findGeoNamedNode* visitor1 = new findGeoNamedNode("red_dot_1");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor1);
    osg::ref_ptr<osg::Node> getnode1 = visitor1->getNode();
    if(getnode1!=NULL){
        getnode1->getParent(0)->removeChild(getnode1);
    }
    findGeoNamedNode* visitor2 = new findGeoNamedNode("green_dot_1");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor2);
    osg::ref_ptr<osg::Node> getnode2 = visitor2->getNode();
    if(getnode2!=NULL){
        getnode2->getParent(0)->removeChild(getnode2);
    }
    findGeoNamedNode* visitor3 = new findGeoNamedNode("blue_dot_1");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor3);
    osg::ref_ptr<osg::Node> getnode3 = visitor3->getNode();
    if(getnode3!=NULL){
        getnode3->getParent(0)->removeChild(getnode3);
    }
    findGeoNamedNode* visitor = new findGeoNamedNode("faceeeeeeee");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor);
    osg::ref_ptr<osg::Node> getnode = visitor->getNode();
    if(getnode!=NULL){
        getnode->getParent(0)->removeChild(getnode);
    }
    findGeoNamedNode* visitor4 = new findGeoNamedNode("lineeeeeeeee");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor4);
    osg::ref_ptr<osg::Node> getnode4 = visitor4->getNode();
    if(getnode4!=NULL){
        getnode4->getParent(0)->removeChild(getnode4);
    }

    Two_point_coor_1.x()=0.0f;
    Two_point_coor_1.y()=0.0f;
    Two_point_coor_1.z()=0.0f;
    Two_point_coor_2.x()=0.0f;
    Two_point_coor_2.y()=0.0f;
    Two_point_coor_2.z()=0.0f;
    one_point_coor.x()=0.0f;
    one_point_coor.y()=0.0f;
    one_point_coor.z()=0.0f;
    Two_point_is_picked_1 = false;
    Two_point_is_picked_2 = false;
    one_point_is_picked_1 = false;
    one_point_is_picked_2 = false;
    ok_is_clicked = true;
    dof_type = 0;
    choose_dof_mode = 0;
    dof_pick2_mode = 0;
}

void osgqt::on_cancel_clicked()
{
    ui->opof_1->setChecked(false);
    ui->opof_2->setChecked(false);
    ui->tp_1->setChecked(false);
    ui->tp_2->setChecked(false);
    ui->translation->setChecked(false);
    ui->rotation->setChecked(false);
    ui->spiral->setChecked(false);
    QWidget::update();
    ui->opof_1->setCheckable(false);
    ui->opof_2->setCheckable(false);
    ui->tp_1->setCheckable(false);
    ui->tp_2->setCheckable(false);
    ui->translation->setCheckable(false);
    ui->rotation->setCheckable(false);
    ui->spiral->setCheckable(false);
    ui->mode_info->setText("");
    
    findGeoNamedNode* visitor1 = new findGeoNamedNode("red_dot_1");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor1);
    osg::ref_ptr<osg::Node> getnode1 = visitor1->getNode();
    if(getnode1!=NULL){
        getnode1->getParent(0)->removeChild(getnode1);
    }
    findGeoNamedNode* visitor2 = new findGeoNamedNode("green_dot_1");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor2);
    osg::ref_ptr<osg::Node> getnode2 = visitor2->getNode();
    if(getnode2!=NULL){
        getnode2->getParent(0)->removeChild(getnode2);
    }
    findGeoNamedNode* visitor3 = new findGeoNamedNode("blue_dot_1");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor3);
    osg::ref_ptr<osg::Node> getnode3 = visitor3->getNode();
    if(getnode3!=NULL){
        getnode3->getParent(0)->removeChild(getnode3);
    }
    findGeoNamedNode* visitor = new findGeoNamedNode("faceeeeeeee");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor);
    osg::ref_ptr<osg::Node> getnode = visitor->getNode();
    if(getnode!=NULL){
        getnode->getParent(0)->removeChild(getnode);
    }
    findGeoNamedNode* visitor4 = new findGeoNamedNode("lineeeeeeeee");
    ViewerWindow2->getSceneData()->asGroup()->accept(*visitor4);
    osg::ref_ptr<osg::Node> getnode4 = visitor4->getNode();
    if(getnode4!=NULL){
        getnode4->getParent(0)->removeChild(getnode4);
    }
    Two_point_coor_1.x()=0.0f;
    Two_point_coor_1.y()=0.0f;
    Two_point_coor_1.z()=0.0f;
    Two_point_coor_2.x()=0.0f;
    Two_point_coor_2.y()=0.0f;
    Two_point_coor_2.z()=0.0f;
    one_point_coor.x()=0.0f;
    one_point_coor.y()=0.0f;
    one_point_coor.z()=0.0f;
    Two_point_is_picked_1 = false;
    Two_point_is_picked_2 = false;
    one_point_is_picked_1 = false;
    one_point_is_picked_2 = false;

    ui->mode_info->setText("");
    ui->dof_add_info->setText("");
    dof_type = 0;
    choose_dof_mode = 0;
    dof_pick2_mode = 0;
}

void osgqt::on_tp_1_clicked()
{
    if(!ui->tp_1->isCheckable())
        return;
    dof_pick2_mode = 1;
    QList<QTreeWidgetItem*> selectedItemList = treewidget2->selectedItems();
    if(selectedItemList.size()==1)
    {
        findGeoNamedNode* visitor4 = new findGeoNamedNode("lineeeeeeeee");
        ViewerWindow2->getSceneData()->asGroup()->accept(*visitor4);
        osg::ref_ptr<osg::Node> getnode4 = visitor4->getNode();
        if(getnode4!=NULL){
            getnode4->getParent(0)->removeChild(getnode4);
        }

        std::string node_name=selectedItemList.at(0)->text(0).toStdString();
        findGeoNamedNode* visitor1 = new findGeoNamedNode(node_name);
        ViewerWindow2->getSceneData()->asGroup()->accept(*visitor1);
        osg::ref_ptr<osg::Node> getnode1 = visitor1->getNode();
        osg::Vec3 center = getnode1->getBound().center();
        //qDebug()<<center.x()<<" "<<center.y()<<" "<<center.z()<<" ";

        findGeoNamedNode* visitor = new findGeoNamedNode("red_dot_1");
        ViewerWindow2->getSceneData()->asGroup()->accept(*visitor);
        osg::ref_ptr<osg::Node> getnode = visitor->getNode();
        if(getnode!=NULL){
            getnode->getParent(0)->removeChild(getnode);
        }
        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
        geode->setName("red_dot_1");
        osg::TessellationHints* hints = new osg::TessellationHints;
        hints->setDetailRatio(0.5f);
        osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(center,0.003);
        osg::ref_ptr<osg::ShapeDrawable> shapedrawable = new osg::ShapeDrawable(sphere,hints);
        osg::Vec4 color(0.0f,0.0f,1.0f,1.0f);
        shapedrawable->setColor(color);
        geode->addDrawable(shapedrawable);
        ViewerWindow2->getSceneData()->asGroup()->addChild(geode);
        Two_point_is_picked_1 = true;
        Two_point_coor_1.x() = center.x();
        Two_point_coor_1.y() = center.y();
        Two_point_coor_1.z() = center.z();

        findGeoNamedNode* visitor2 = new findGeoNamedNode("green_dot_1");
        ViewerWindow2->getSceneData()->asGroup()->accept(*visitor2);
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
            ViewerWindow2->getSceneData()->asGroup()->addChild(geodeline);
        }

    }
}

void osgqt::on_tp_2_clicked()
{    if(!ui->tp_2->isCheckable())
        return;
    dof_pick2_mode = 2;
    QList<QTreeWidgetItem*> selectedItemList = treewidget2->selectedItems();
    if(selectedItemList.size()==1)
    {        findGeoNamedNode* visitor4 = new findGeoNamedNode("lineeeeeeeee");
        ViewerWindow2->getSceneData()->asGroup()->accept(*visitor4);
        osg::ref_ptr<osg::Node> getnode4 = visitor4->getNode();
        if(getnode4!=NULL){
            getnode4->getParent(0)->removeChild(getnode4);
        }
        std::string node_name=selectedItemList.at(0)->text(0).toStdString();
        findGeoNamedNode* visitor1 = new findGeoNamedNode(node_name);
        ViewerWindow2->getSceneData()->asGroup()->accept(*visitor1);
        osg::ref_ptr<osg::Node> getnode1 = visitor1->getNode();
        osg::Vec3 center = getnode1->getBound().center();
        //qDebug()<<center.x()<<" "<<center.y()<<" "<<center.z()<<" ";

        findGeoNamedNode* visitor = new findGeoNamedNode("green_dot_1");
        ViewerWindow2->getSceneData()->asGroup()->accept(*visitor);
        osg::ref_ptr<osg::Node> getnode = visitor->getNode();
        if(getnode!=NULL){
            getnode->getParent(0)->removeChild(getnode);
        }
        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
        geode->setName("green_dot_1");
        osg::TessellationHints* hints = new osg::TessellationHints;
        hints->setDetailRatio(0.5f);
        osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(center,0.003);
        osg::ref_ptr<osg::ShapeDrawable> shapedrawable = new osg::ShapeDrawable(sphere,hints);
        osg::Vec4 color(1.0f,1.0f,0.0f,1.0f);
        shapedrawable->setColor(color);
        geode->addDrawable(shapedrawable);
        ViewerWindow2->getSceneData()->asGroup()->addChild(geode);
        Two_point_is_picked_2 = true;
        Two_point_coor_2.x() = center.x();
        Two_point_coor_2.y() = center.y();
        Two_point_coor_2.z() = center.z();



        findGeoNamedNode* visitor2 = new findGeoNamedNode("red_dot_1");
        ViewerWindow2->getSceneData()->asGroup()->accept(*visitor2);
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
            ViewerWindow2->getSceneData()->asGroup()->addChild(geodeline);
        }

    }
}

void osgqt::on_opof_1_clicked()
{    if(!ui->opof_1->isCheckable())
        return;
    dof_pick2_mode = 3;
    QList<QTreeWidgetItem*> selectedItemList = treewidget2->selectedItems();
    if(selectedItemList.size()==1)
    {
        std::string node_name=selectedItemList.at(0)->text(0).toStdString();
        findGeoNamedNode* visitor1 = new findGeoNamedNode(node_name);
        ViewerWindow2->getSceneData()->asGroup()->accept(*visitor1);
        osg::ref_ptr<osg::Node> getnode1 = visitor1->getNode();
        osg::Vec3 center = getnode1->getBound().center();
        //qDebug()<<center.x()<<" "<<center.y()<<" "<<center.z()<<" ";
        findGeoNamedNode* visitor = new findGeoNamedNode("blue_dot_1");
        ViewerWindow2->getSceneData()->asGroup()->accept(*visitor);
        osg::ref_ptr<osg::Node> getnode = visitor->getNode();
        if(getnode!=NULL){
            getnode->getParent(0)->removeChild(getnode);
        }
        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
        geode->setName("blue_dot_1");
        osg::TessellationHints* hints = new osg::TessellationHints;
        hints->setDetailRatio(0.5f);
        osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(center,0.003);
        osg::ref_ptr<osg::ShapeDrawable> shapedrawable = new osg::ShapeDrawable(sphere,hints);
        osg::Vec4 color(0.0f,0.0f,1.0f,1.0f);
        shapedrawable->setColor(color);
        geode->addDrawable(shapedrawable);
        ViewerWindow2->getSceneData()->asGroup()->addChild(geode);
        one_point_is_picked_1 = true;
        one_point_coor.x() = center.x();
        one_point_coor.y() = center.y();
        one_point_coor.z() = center.z();
    }
}

void osgqt::on_opof_2_clicked()
{if(!ui->opof_2->isCheckable())
        return;
    dof_pick2_mode = 4;
}

void osgqt::on_reset2_clicked()
{
    ViewerWindow2->setCameraManipulator(new osgGA::TrackballManipulator);
}



void osgqt::on_resetanimation_clicked()
{
    QList<QTreeWidgetItem*> selectedItemList = treewidget2->selectedItems();
    std::vector<osg::ref_ptr<osgSim::DOFTransform>> dof_nodes;
    for ( int i=0; i!=selectedItemList.size(); ++i ){
        // 士林：animation test代码
        std::string node_name=selectedItemList.at(i)->text(0).toStdString();
        findDofNamedNode find_dof(node_name);
        ViewerWindow2->getSceneData()->accept(find_dof);
        osg::ref_ptr<osgSim::DOFTransform> dof_node=find_dof.getNode();
        //如果不是dof，报错
        if(dof_node==nullptr)
        {
            ui->dof_add_info->setText("the nodes you selected contain none-dof node");
            return;
        }
        else
            dof_nodes.push_back(dof_node);
    }
    for(int i = 0;i<dof_nodes.size();i++){
        string node_name = dof_nodes.at(i)->getName();
        osg::Vec3 hpr = dof_HPR[node_name];
        osg::Vec3 trans = dof_trans[node_name];
        dof_nodes.at(i)->setCurrentHPR(hpr);
        dof_nodes.at(i)->setCurrentTranslate(trans);
    }
}

