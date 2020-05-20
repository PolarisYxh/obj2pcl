#include"Photographer_sx.h"
#include"Tool_test_2.h"
#include"Tool_test_3.h"
#include<string>
#include<iostream>
using namespace std;


//��ȡ�ļ���������mat�ļ���ȫ·��
void handleFiles(string path, vector<string>& files, vector<string>& IVE_models, vector<string>& IVE_model_names)
{
	long long  hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;//�����洢�ļ���Ϣ�Ľṹ��
	string p;
	string filename;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)  //��һ�β���
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))  //������ҵ������ļ���
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)  //�����ļ��в���
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					handleFiles(p.assign(path).append("\\").append(fileinfo.name), files, IVE_models, IVE_model_names);
				}
			}
			else //������ҵ��Ĳ����ļ���
			{
				string f_name = fileinfo.name;
				int suffix_begin = f_name.find(".");
				string suffix = f_name.substr(suffix_begin, f_name.size() - suffix_begin);
				if (suffix == ".ive")//ʶ��model.ive
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
		_findclose(hFile); //��������
	}
}

//2020.4.3  ���߽ӿں��� ����iveģ������·�� ����·��root
//                           dof����+ÿ���˶�����+���� ��ֻ��һ��dof�汾��
//                           ������ڸ߶�+�˶��뾶+���մ�����ģ�����ĵ�Ĭ����ԭ�� �汾��
//                           
//   ���
int main() {

	string ive_path = "G:/models";
	vector<string> files;
	vector<string> IVE_models;
	vector<string> IVE_model_names;
	handleFiles(ive_path, files, IVE_models, IVE_model_names);

	string save_path = "G:/modelsgai/";
	//string dof_name = "dof_011_002_t";
	string dof_name = "dof_rootd_Aa001_r";
	double ani_range = 90, ani_count = 2, cam_H = 3, track_R = 3, photo_count = 4;//���������޸�ani_angleΪani_range
	//�������ļ���
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
		pg.new_dataset(IVE_models[idx], save_path+IVE_model_names[idx]);
		/*osg::Group* root = new osg::Group; 
		osgDB::Options* a = new osgDB::Options(std::string("noTriStripPolygons"));
		osg::ref_ptr<osg::Node> scene1 = osgDB::readNodeFile(IVE_models[0], a);
		osg::ref_ptr<osg::Node> scene2 = scene1->asGroup()->getChild(0);
		osg::ref_ptr<osg::Node> model1 = scene1->asGroup()->getChild(0)->asGroup()->getChild(0);
		osg::ref_ptr<osg::Node> model2 = scene1->asGroup()->getChild(0)->asGroup()->getChild(0)->asGroup()->getChild(0); //��"root"���
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
	

	// initialize lib�������������ʼ����
	/*if (!Tool_test_3Initialize())
	{
		std::cout << "Could not initialize Tool_test_3!" << std::endl;
		return -1;
	}
	// Ϊ���������ڴ�ռ䣬���Բ����mwArray
	const char* root_str  = save_path.c_str();
	const char* dof_name_str = dof_name.c_str();
	mwArray root_path(root_str);
	mwArray dof_name_mat(dof_name_str);
	mwArray ani_count_mat(1, 1, mxDOUBLE_CLASS);// 1��1��ʾ����Ĵ�С������maltabֻ��һ�ֱ��������Ǿ���Ϊ�˺�Cpp�����ӹ죬���ó�1*1�ľ���mxDOUBLE_CLASS��ʾ�����ľ��ȣ�
	mwArray photo_count_mat(1, 1, mxDOUBLE_CLASS);
	mwArray ani_range_mat(1, 1, mxDOUBLE_CLASS);

	//�����������SetData�������ำֵ
	ani_count_mat.SetData(&ani_count, 1);
	photo_count_mat.SetData(&photo_count, 1);
	ani_range_mat.SetData(&ani_range, 1);

	//Tool_test_2( root_path, ani_count_mat,  photo_count_mat);
	Tool_test_3(root_path, ani_range_mat, ani_count_mat, photo_count_mat, dof_name_mat);
	// ������һЩ��ֹ���õĳ���
	Tool_test_3Terminate();
	// terminate MCR
	mclTerminateApplication();*/

	return 0;
}