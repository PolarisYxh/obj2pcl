﻿#include "Image2PCL.h"
using namespace std;

struct dofpart
{
	Eigen::Vector4d motioninfo;
	Eigen::Vector4d color;
	Eigen::Matrix4d PutMatrix;
	Eigen::Matrix4d ModelMatrix;
};
struct campara
{
	Eigen::Matrix4d V, P, W;
};
void createSinglePCL(QString rootpath1, int ShootTimes)
{
	vector<cv::Mat> colorImgs, depthImgs;	//彩色图和深度图
	vector<Eigen::Isometry3d> poses;	//相机位姿
	QString rootpath = "D:/my/sunxun/test3";
	QString depthpath = rootpath + "/depth";
	QString labelpath = rootpath + "/label";
	QString posepath = rootpath + "/pose";
	QString pclpath = rootpath + "/pointcloud";
	QDir rootfiles(rootpath);
	QDir posefiles(posepath);
	posefiles.setFilter(QDir::Files);
	QDir labelfiles(labelpath);
	labelfiles.setFilter(QDir::Files);
	QDir depthfiles(posepath);
	depthfiles.setFilter(QDir::Files);
	QDir pclfiles(pclpath);
	if (!pclfiles.exists()) {
		bool ismkdir = rootfiles.mkdir(pclpath);
		if (!ismkdir)
			qDebug() << "Create pcl path fail" << endl;
		else
			qDebug() << "Create pcl path success" << endl;
	}
	QFileInfoList list = posefiles.entryInfoList();
	QFileInfoList depth = depthfiles.entryInfoList();
	QFileInfoList label = labelfiles.entryInfoList();

	QString listname = list.at(0).absolutePath();
	ifstream motioninfo(listname.toStdString());
	std::vector<dofpart> dofinfo;
	dofpart dof;
	while (motioninfo >> dof.motioninfo[0] >> dof.motioninfo[1] >> dof.motioninfo[2] >> dof.motioninfo[3])
	{
		motioninfo >> dof.color[0] >> dof.color[1] >> dof.color[2] >> dof.color[3];
		for (int m = 0; m < 4; m++) {
			for (int n = 0; n < 4; n++) {
				motioninfo >> dof.PutMatrix(m, n);
			}
		}
		dofinfo.push_back(dof);
	}
	campara cam;
	campara* caminfo = new campara[ShootTimes];
	for (int i = 0; i < ShootTimes; i++)
	{
		QString posename = list.at(i + 1).absoluteFilePath();
		ifstream poseinfo(posename.toStdString());
		for (int m = 0; m < 4; m++) {
			for (int n = 0; n < 4; n++) {
				poseinfo >> cam.V(m, n);
			}
		}
		for (int m = 0; m < 4; m++) {
			for (int n = 0; n < 4; n++) {
				poseinfo >> cam.P(m, n);
			}
		}
		for (int m = 0; m < 4; m++) {
			for (int n = 0; n < 4; n++) {
				poseinfo >> cam.W(m, n);
			}
		}
		caminfo[i] = cam;
	}
	qDebug() << "converting to PCL" << endl;
	// 定义点云使用的格式：这里用的是 XYZRGB
	typedef pcl::PointXYZRGB PointT;
	typedef pcl::PointCloud<PointT> PointCloud;

	// 新建一个点云
	PointCloud::Ptr pointCloud(new PointCloud);
	int j = 0; // 记录循环次数
	bool flag = true; // 记录是否还有部件运动，如果有，flag为true，先设为true，拍个静态
	while (flag)
	{
		for (int i = 0; i < ShootTimes; i++)
		{
			qDebug() << "converting ... " << i + 1 << endl;
			if (j * ShootTimes + i > depth.size() || j * ShootTimes + i > label.size())
			{
				flag = false;
				break;
			}
			QString depthname = depth.at(j * ShootTimes + i).absoluteFilePath();
			QString labelname = label.at(j * ShootTimes + i).absoluteFilePath();

			cv::Mat color = cv::imread(depthname.toStdString());
			cv::Mat depth = cv::imread(labelname.toStdString());
			//Eigen::Isometry3d T = poses[i];
			for (int u = 0; u < color.cols; u++)
				for (int v = 0; v < color.rows; v++)
				{
					unsigned int d = depth.ptr<unsigned short>(v)[u]; // 深度值
					if (d == 255) continue; // 为 255 表示没有测量到
					Eigen::Vector4d point;
					point[2] = double(d);
					point[0] = u;
					point[1] = color.rows - v;
					point[4] = 1;
					Eigen::Vector4d pointWorld = (point.transpose() * caminfo[i].W.transpose() * caminfo[i].P.transpose() * caminfo[i].V.transpose()).transpose();//外参矩阵求解

					PointT p;
					p.x = pointWorld[0] / pointWorld[3];
					p.y = pointWorld[1] / pointWorld[3];
					p.z = pointWorld[2] / pointWorld[3];
					p.b = color.data[v * color.step + u * color.channels()];
					p.g = color.data[v * color.step + u * color.channels() + 1];
					p.r = color.data[v * color.step + u * color.channels() + 2];
					pointCloud->points.push_back(p);
				}
		}
		pointCloud->is_dense = false;
		QString pclfilename = QString(pclpath + "/%1.pcd").arg(j);
		
		pcl::io::savePCDFileBinary(pclfilename.toStdString(), (PointCloud)*pointCloud);
		flag = false;
		for (int m = 0; m < dofinfo.size(); m++)
		{
			if (dofinfo[m].motioninfo[0] == 3 && dofinfo[m].motioninfo[3] > j)
			{
				flag = true;
				break;
			}
			else if (dofinfo[m].motioninfo[2] > j)
			{
				flag = true;
				break;
			}
		}
		j++;
	}
}