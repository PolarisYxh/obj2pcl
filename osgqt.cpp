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
   model -> accept(infovisitor) model为加载的模型
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
	//auto widget = new QWidget;
   // QMainWindow* mw = new QMainWindow(this);
   // mw->setWindowFlags(Qt::Widget);
    ViewerWindow = new QtOsgView(this);
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
    QFont font ( "Microsoft YaHei", 9, 75);
    ui->copyright1->setFont(font);
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

	osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
	kinect = osgDB::readNodeFile("./resource/kinect.obj", a);

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
	connect(ui->showButton_1, SIGNAL(pressed()), this, SLOT(showButton_1_pressed()));

	//connect(ui->input_camera_height, SIGNAL(editingFinished()), this, SLOT(camera_height_ok()));
	//connect(ui->input_shooting_radius, SIGNAL(editingFinished()), this, SLOT(shooting_radius_ok()));
	//connect(ui->input_shooting_times, SIGNAL(editingFinished()), this, SLOT(shooting_times_ok()));

    timer->start(1);
}

osgqt::~osgqt(){
    // 注释：析构函数
    delete ui;
}

/*多运动轴的函数
void osgqt::motion_comfirm_clicked() {
	QString amp=ui->input_motion_amp->text();
	QString fre = ui->input_motion_fre->text();
	if (fre == "" || amp == "")
	{
		QMessageBox msgBox; 
		msgBox.setText("Please input motion amplitude or motion frequency!"); 
		msgBox.exec();
		//ui->setting_message->append("please input motion amplitude or motion frequency!");
		return;
	}
	map<string, bool>::iterator it;
	it = map_to_choose.begin();
	while (it != map_to_choose.end())
	{
		if(it->second == true)break;
		it++;
	}
	if (it == map_to_choose.end())
	{
		QMessageBox msgBox;
		msgBox.setText("Please add a motion part!");
		msgBox.exec();
		return;
	}
	QVector2D ampfre;
	ampfre[0] = amp.toInt();
	ampfre[1] = fre.toInt();
	motion.insert(QString::fromStdString(it->first),ampfre);
	//map_to_choose.clear();
	//ui->setting_message->append(QString("motion part:%1,amplitude:%2,frequency:%3;")
	//	.arg(QString::fromStdString(it->first)).arg(ampfre[0]).arg(ampfre[1]));
	ui->input_motion_amp->clear();
	ui->input_motion_fre->clear();
	viewmotion();
}
void osgqt::motion_delete_clicked() {
	map<string, bool>::iterator it;
	it = map_to_choose.begin();
	while (it != map_to_choose.end())
	{
		if (it->second == true)break;
		it++;
	}	
	if (it == map_to_choose.end())
	{
		QMessageBox msgBox;
		msgBox.setText("Please select a motion part to remove!");
		msgBox.exec();
		return;
	}
	motion.remove(QString::fromStdString(it->first));
	viewmotion();
}
void osgqt::viewmotion()
{
	QMap<QString, QVector2D>::iterator it;
	it = motion.begin();
	ui->setting_message->clear();
	ui->setting_message->append("motion part:amplitude,frequency;");
	while (it != motion.end())
	{
		ui->setting_message->append(QString("%1:%2,%3;")
			.arg(it.key()).arg(it.value().x()).arg(it.value().y()));
		it++;
	}
}*/
void osgqt::motion_comfirm_clicked() 
{
	QString amp = ui->input_motion_amp->text();
	QString fre = ui->input_motion_fre->text();
	if (fre == "" || amp == "")
	{
		QMessageBox msgBox;
		msgBox.setText("Please input motion range or motion frequency!");
		msgBox.exec();
		//ui->setting_message->append("please input motion amplitude or motion frequency!");
		return;
	}
	map<string, bool>::iterator it;
	it = map_to_choose.begin();
	while (it != map_to_choose.end())
	{
		if (it->second == true)break;
		it++;
	}
	if (it == map_to_choose.end())
	{
		QMessageBox msgBox;
		msgBox.setText("Please add a motion part!");
		msgBox.exec();
		return;
	}
	motionname = QString::fromStdString(it->first);
	ui->input_motion_name->setText(motionname);
	motion[0] = amp.toFloat();
	motion[1] = fre.toFloat();
	ui->input_motion_name->setDisabled(true);
	ui->input_motion_amp->setDisabled(true);
	ui->input_motion_fre->setDisabled(true);
}
void osgqt::motion_play_clicked()
{

}
void osgqt::camera_confirm_clicked() {
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
	//ui->setting_message->append(QString("camera\nheight,radius,times;"));
	//ui->setting_message->append(QString("%1,%2,%3;").arg(camera_height).arg(shooting_radius).arg(shooting_times));
	ui->input_camera_height->setDisabled(true);
	ui->input_shooting_radius->setDisabled(true);
	ui->input_shooting_times->setDisabled(true);
}
void osgqt::camera_delete_clicked()
{
	//ui->setting_message->clear();
	//ui->setting_message->append(QString("camera\nheight,radius,times;"));
	ui->input_camera_height->clear();
	ui->input_shooting_radius->clear();
	ui->input_shooting_times->clear();
	camera_height = 0;
	shooting_radius = 0;
	shooting_times = 0;
	ui->input_camera_height->setDisabled(false);
	ui->input_shooting_radius->setDisabled(false);
	ui->input_shooting_times->setDisabled(false);
}
void osgqt::removenode(std::string name)
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
	QString h = ui->input_camera_height->text();
	QString r = ui->input_shooting_radius->text();
	QString n = ui->input_shooting_times->text();

	if (h == "" || r == "" || n == "")
	{
		QMessageBox msgBox;
		msgBox.setText("Please input camera's parameter!");
		msgBox.exec();
		return;
	}
	shooting_times = n.toInt();

	float thea = 2 * osg::PI / shooting_times;
	//MT->removeChild(kinect);
	removenode("kinectMT");
	for (int i = 0; i < shooting_times; i++)
	{
		osg::Vec3d eyes = osg::Vec3d(r.toDouble() * cos(i * thea), r.toDouble() * sin(i * thea), h.toDouble());
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
		osg::Matrix m = osg::Matrix::scale(0.5, 0.5, 0.5) * Vf;
		//osg::Matrix m = osg::Matrix::rotate(osg::inDegrees(90.0f),0.0f, 1.0f, 0.0f)* osg::Matrix::rotate(osg::inDegrees(-90.0f), 1.0f, 0.0f, 0.0f)
			//* osg::Matrix::scale(1, 1, 1)
			//* osg::Matrix::translate(0.5f, 0, 0);
		osg::MatrixTransform* MT = new osg::MatrixTransform;
		MT->setName("kinectMT");
		MT->setMatrix(m);
		MT->addChild(kinect);
		root->addChild(MT);
		ViewerWindow->setSceneData(root.get());
	}
	drawCircle();
}
void osgqt::drawCircle()
{
	camera_height = ui->input_camera_height->text().toFloat();
	shooting_radius = ui->input_shooting_radius->text().toFloat();
	if (ui->input_shooting_radius->text() == "")shooting_radius = 0.5;
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
void osgqt::startButton_pressed() {
	QString directory;
	//QString current_path = QDir::currentPath();
	if(savepicpath!="")
		directory = QFileDialog::getExistingDirectory(this, tr("Choose Save Path"), savepicpath);
	else if(savepicpath != "")
		directory = QFileDialog::getExistingDirectory(this, tr("Choose Save Path"), savepclpath);
	else directory = QFileDialog::getExistingDirectory(this, tr("Choose Save Path"), "./");
	//fi = QFileInfo(directory);
	//file_name = fi.fileName();
	//file_path = fi.absolutePath();
	/*if (!directory.isEmpty()) {
		if (ui->comboBox->findText(directory) == -1)
			ui->comboBox->addItem(directory);
		ui->comboBox->setCurrentIndex((ui->comboBox->findText(directory)));
	}*/
	savepicpath = directory;
	if (savepicpath == "")
		return;
	if (shooting_radius == 0 || shooting_times == 0|| motion.empty())
	{
		QMessageBox msgBox;
		msgBox.setText("Please set the camera or animation parameter above!");
		msgBox.exec();
		return;
	}
	QMessageBox msgBox;
	/*QString ques = QString("motion part:%1,amplitude:%2,frequency:%3;\n").arg(motion.firstKey())
		.arg(motion.first().x()).arg(motion.first().y()) + QString("camera height:%1,camera radius:%2,camera times:%3;\n")
		.arg(camera_height).arg(shooting_radius).arg(shooting_times)+QString("Continue shooting?");*/
	QString ques=QString("motion part:%1,amplitude:%2,frequency:%3;\n").arg(motionname).arg(motion[0]).arg(motion[1]) 
		+ QString("camera height:%1,camera radius:%2,camera times:%3;\n\n").arg(camera_height).arg(shooting_radius).arg(shooting_times) 
		+ QString("Continue shooting?");
	QMessageBox messageBox(QMessageBox::NoIcon,
		"critical", ques,
		QMessageBox::Yes | QMessageBox::No, NULL);
	int result = messageBox.exec();
	if (result == QMessageBox::No)
	{
		motion.clear();
		camera_height = 0;
		shooting_radius = 0;
		shooting_times = 0;
		return;
	}
	savepicpath += "/";
	pg.Tool_test_1(filepath.toStdString(), savepicpath.toStdString(), motion.firstKey().toStdString(), 
		motion.first().x(), motion.first().y(), camera_height, shooting_radius, shooting_times);
}
void osgqt::generateButton_pressed() 
{
	QString directory;
	//QString current_path = QDir::currentPath();
	if (savepicpath != "")
		directory = QFileDialog::getExistingDirectory(this, tr("Choose Save Path"), savepicpath);
	else if (savepicpath != "")
		directory = QFileDialog::getExistingDirectory(this, tr("Choose Save Path"), savepclpath);
	else directory = QFileDialog::getExistingDirectory(this, tr("Choose Save Path"), "./");
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
	std::string path= savepclpath.toStdString()+"/";
	const char * str = path.c_str();
	//const char* str = "D:\\my\\sunxun\\test1";
	double anitimes = motion.begin()->y();
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
	//mclTerminateApplication();
	
}
void osgqt::showButton_pressed() {

}
void osgqt::showButton_1_pressed() {

}

/*void osgqt::camera_height_ok() //在一定高度上画圆
{
	if (ui->input_camera_height->text() == "")return;
	drawCircle();
}
void osgqt::shooting_radius_ok() 
{
	if (ui->input_shooting_radius->text()=="")return;
	drawCircle();
}

void osgqt::shooting_times_ok()
{
	//if (ui->input_shooting_times->text() == "")return;
	QString h = ui->input_camera_height->text();
	QString r = ui->input_shooting_radius->text();
	QString n = ui->input_shooting_times->text();
	if (h == "" || r == "" || n == "")
	{
		return;
	}
	shooting_times = n.toInt();

	float thea = 2 * osg::PI / shooting_times;
	//MT->removeChild(kinect);
	removenode("kinectMT");
	for (int i = 0; i < shooting_times; i++)
	{
		osg::Vec3d eyes = osg::Vec3d(r.toDouble() * cos(i * thea), r.toDouble() * sin(i * thea), h.toDouble());
		osg::Vec3d	center = osg::Vec3d(0.0, 0.0, 0.0);
		osg::Vec3d up = osg::Vec3d(0.0, 0.0, -1.0);
		ViewerWindow->getCamera()->setViewMatrixAsLookAt(eyes, center, up);
		osg::Matrix V = ViewerWindow->getCamera()->getViewMatrix(); // 视图矩阵
		osg::Matrix Vni = osg::Matrix::inverse(V);
		osg::Matrix Vtrans = osg::Matrix
		    (1,0,0,0,
			0,-1,0,0,
			0,0,1,0,
			0,0,0,1);
		osg::Matrix Vf =Vni*Vtrans;
		osg::Matrix m = osg::Matrix::scale(0.5, 0.5, 0.5)*Vf;
		//osg::Matrix m = osg::Matrix::rotate(osg::inDegrees(90.0f),0.0f, 1.0f, 0.0f)* osg::Matrix::rotate(osg::inDegrees(-90.0f), 1.0f, 0.0f, 0.0f)
			//* osg::Matrix::scale(1, 1, 1)
			//* osg::Matrix::translate(0.5f, 0, 0);
		osg::MatrixTransform* MT = new osg::MatrixTransform;
		MT->setName("kinectMT");
		MT->setMatrix(m);
		MT->addChild(kinect);
		root->addChild(MT);
		ViewerWindow->setSceneData(root.get());
	}
}*/

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
	if (itm == NULL)return;
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

    if(tab_idx==0&&(mouse_is_picked||tree_widget_is_picked||tree_widget_is_picked_empyt)){
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
}

void osgqt::addmodel(osg::ref_ptr<osg::Node> model){
    //注释：用于加载模型
    loadedModel = model;
    //root = new osg::Group;
	//removenode("trans");
	int num = root->getNumChildren();
	//int num1 = root->get();
	//for (int i = 0; i < num; i++)
	//{
	root->removeChild(0,num);//删除root下所有节点
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
	camera->setName("camera");
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(geo.get());

    camera->addChild(geode.get());
    root->addChild(camera);
    ViewerWindow->setSceneData(root.get());
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

    osg::ref_ptr<osg::Node> model = ViewerWindow->getSceneData()->asGroup()->getChild(0)->asGroup()->getChild(0);
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
	filepath = directory;
    flag_open = false;
    current_tree_item_name = "";

    map_to_choose.clear();
    mouse_is_picked_empty = false;
    mouse_is_picked = false;
    tree_widget_is_picked = false;
    tree_widget_is_picked_empyt = false;
    box_is_picked = false;
    //cout<<"path: "<<path<<endl;
    /*osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(path,a);

    //auto aaaa = model->asGroup();


    model_state_set = model->getStateSet();
    addmodel(model);
    init_state_set = model->getStateSet();
    box_infomation = "";
    //std::cout<<file_name.toStdString()<<std::endl;

    */

    //update_treewidget(); //注释:更新tree_widget
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
	flag_open = true;
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

/*void osgqt::on_Animation_test_clicked()
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
}*/