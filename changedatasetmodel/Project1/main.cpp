#include"Photographer_sx.h"
#include"Tool_test_2.h"
#include"Tool_test_3.h"
#include<string>
#include<iostream>
using namespace std;


//获取文件夹下所有mat文件的全路径
void handleFiles(string path, vector<string>& files, vector<string>& IVE_models, vector<string>& IVE_model_names)
{
	long long  hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;//用来存储文件信息的结构体
	string p;
	string filename;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)  //第一次查找
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))  //如果查找到的是文件夹
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)  //进入文件夹查找
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					handleFiles(p.assign(path).append("\\").append(fileinfo.name), files, IVE_models, IVE_model_names);
				}
			}
			else //如果查找到的不是文件夹
			{
				string f_name = fileinfo.name;
				int suffix_begin = f_name.find(".");
				string suffix = f_name.substr(suffix_begin, f_name.size() - suffix_begin);
				if (suffix == ".ive")//识别model.ive
				{
					string this_path = path + "\\";
					filename = f_name;
					string fullpath = this_path;
					fullpath.append(f_name);
					IVE_models.push_back(fullpath);
					IVE_model_names.push_back(f_name);
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile); //结束查找
	}
}

//2020.4.3  工具接口函数 读入ive模型所在路径 保存路径root
//                           dof名称+每次运动幅度+次数 【只有一个dof版本】
//                           相机所在高度+运动半径+拍照次数【模型中心点默认是原点 版本】
//                           
//   输出
int main() {

	//string ive_path = "D:/my/sunxun/obj2pcl/changedatasetmodel/新建文件夹";
	string ive_path = "G:/waternclock";//modelnonorm1
	vector<string> files;
	vector<string> IVE_models;
	vector<string> IVE_model_names;
	handleFiles(ive_path, files, IVE_models, IVE_model_names);

	string save_path = "G:/waternclockgai/";//modelnonorm1gai
	//string dof_name = "dof_011_002_t";
	string dof_name = "dof_rootd_Aa001_r";
	double ani_range = 90, ani_count = 2, cam_H = 3, track_R = 3, photo_count = 4;//变量名字修改ani_angle为ani_range
	//创建子文件夹
	std::string sub1 = save_path + "pose";
	std::string sub2 = save_path + "depth";
	std::string sub3 = save_path + "label";
	mkdir(sub1.c_str());
	mkdir(sub2.c_str());
	mkdir(sub3.c_str());
	//mkdir(sub4.c_str());

	Photographer pg;

	int num_of_file = IVE_models.size();

	for (int idx = 0; idx < num_of_file; idx++) {

		//pg.Tool_test_3(IVE_models[idx], save_path, dof_name, ani_range, ani_count, cam_H, track_R, photo_count, IVE_model_names[idx]);
		std::cout << save_path + IVE_model_names[idx]<<endl;
		pg.new_dataset(IVE_models[idx], save_path+IVE_model_names[idx]);
		/*osg::Group* root = new osg::Group; 
		osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
		osg::ref_ptr<osg::Node> scene1 = osgDB::readNodeFile(IVE_models[0], a);
		osg::ref_ptr<osg::Node> scene2 = scene1->asGroup()->getChild(0);
		osg::ref_ptr<osg::Node> model1 = scene1->asGroup()->getChild(0)->asGroup()->getChild(0);
		osg::ref_ptr<osg::Node> model2 = scene1->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup()->getChild(0); //到"root"结点
		osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer; 
		root->addChild(scene1);
		root->addChild(scene2);
		//root->addChild(model1);
		//root->addChild(model2);
		viewer->setSceneData(root);
		viewer->run();*/

	}
	return 0;

	//pg.Tool_test_1(ive_path, save_path, dof_name, ani_range, ani_count, cam_H, track_R, photo_count);
	

	// initialize lib，这里必须做初始化！
	/*if (!Tool_test_3Initialize())
	{
		std::cout << "Could not initialize Tool_test_3!" << std::endl;
		return -1;
	}
	// 为变量分配内存空间，可以查帮助mwArray
	const char* root_str  = save_path.c_str();
	const char* dof_name_str = dof_name.c_str();
	mwArray root_path(root_str);
	mwArray dof_name_mat(dof_name_str);
	mwArray ani_count_mat(1, 1, mxDOUBLE_CLASS);// 1，1表示矩阵的大小（所有maltab只有一种变量，就是矩阵，为了和Cpp变量接轨，设置成1*1的矩阵，mxDOUBLE_CLASS表示变量的精度）
	mwArray photo_count_mat(1, 1, mxDOUBLE_CLASS);
	mwArray ani_range_mat(1, 1, mxDOUBLE_CLASS);

	//调用类里面的SetData函数给类赋值
	ani_count_mat.SetData(&ani_count, 1);
	photo_count_mat.SetData(&photo_count, 1);
	ani_range_mat.SetData(&ani_range, 1);

	//Tool_test_2( root_path, ani_count_mat,  photo_count_mat);
	Tool_test_3(root_path, ani_range_mat, ani_count_mat, photo_count_mat, dof_name_mat);
	// 后面是一些终止调用的程序
	Tool_test_3Terminate();
	// terminate MCR
	mclTerminateApplication();*/

	return 0;
}
void main1()
{
	//QString  directory = "G:/modelsgai";
	//QDir dir(directory);
	//dir.setFilter(QDir::Files);
	//QFileInfoList list = dir.entryInfoList();
	//for (int filenum = 0; filenum < list.size(); filenum++)//依次打开场景txt文件
	//{
	//	QFileInfo fileInfo = list.at(filenum);
	//	string ive_name = fileInfo.absoluteFilePath().toStdString();
	//	string path = "G:/rgbd/" + fileInfo.baseName().toStdString();
	//	if (0 != access(path.c_str(), 0))
	//	{ // if this folder not exist, create a new one.
	//		mkdir(path.c_str());   // 返回 0 表示创建成功，-1 表示失败
	//		//换成 ::_mkdir  ::_access 也行，不知道什么意思
	//	}
	//	Tool_test_2(ive_name, path, 4, fileInfo.baseName().toStdString());
	//}
	string  directory = "G:/modelsgai";
	vector<string> files;
	vector<string> IVE_models;
	vector<string> IVE_model_names;
	handleFiles(directory, files, IVE_models, IVE_model_names);
}