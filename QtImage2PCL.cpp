#include "QtImage2PCL.h"
using namespace std;

int createPCL::createSinglePCL(QString rootpath)
{

	
	vector<cv::Mat> colorImgs, depthImgs; // 彩色图和深度图
	//directory = QFileDialog::getExistingDirectory(this, tr("Choose pictures save Path"), savepicpath);
	//QString rootpath = "D:/my/sunxun/test3";
	QString depthpath = rootpath + "/depth";
	QString labelpath = rootpath + "/label";
	QString posepath = rootpath + "/pose";
	QString pclpath = rootpath + "/pointcloud";
	QDir rootfiles(rootpath);

	QDir posefiles(posepath);
	QDir labelfiles(labelpath);
	QDir depthfiles(depthpath);
	posefiles.setFilter(QDir::Files);
	labelfiles.setFilter(QDir::Files);
	depthfiles.setFilter(QDir::Files);
	if (!posefiles.exists()||!labelfiles.exists()||!depthfiles.exists())
	{
		QMessageBox msgBox;
		msgBox.setText("no label,pose or depth diretory");
		msgBox.exec();
		return 0;
	}
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

	

	std::string listname = list.at(0).absoluteFilePath().toStdString();
	ifstream motioninfo(listname);
	if (!motioninfo)
	{
		QMessageBox msgBox;
		msgBox.setText("no pose files");
		msgBox.exec();
		return 0;
	}
	int ShootTimes,tem ;
	motioninfo >> ShootTimes >> tem>>tem>>tem;
	if (ShootTimes == 0)
		return 0;
	QProgressDialog process("processing image2pcl.....", "cancel", 0, depth.size());
	process.setWindowModality(Qt::WindowModal);
	process.show();
	std::vector<dofpart> dofinfo;
	dofpart dof;

	while (motioninfo >> dof.motioninfo[0] >> dof.motioninfo[1] >> dof.motioninfo[2] >> dof.motioninfo[3])
	{
		motioninfo >> dof.color[0] >> dof.color[1] >> dof.color[2] >> dof.color[3];
		for (int m = 0; m < 4; m++) {
			for (int n = 0; n < 4; n++) {
				motioninfo >> dof.PutMatrix(m,n);
			}
		}
		dofinfo.push_back(dof);
	}
	motioninfo.close();
	

	// 新建一个点云
	int j = 0; // 记录循环次数,输出的循环次数
	bool flag = true; // 记录是否还有部件运动，如果有，flag为true，先设为true，拍个静态
	while (flag)
	{
		
		QString pclfilename = QString(pclpath + "/%1.txt").arg(j);
		ofstream pclfile(pclfilename.toStdString());
		for (int i = 0; i < ShootTimes; i++)
		{
			if (j * ShootTimes + i > depth.size() || j * ShootTimes + i > label.size()|| j * ShootTimes + i + 1>list.size())
			{
				flag = false;
				break;
			}
			process.setValue(j * ShootTimes + i);
			if (process.wasCanceled())
				break;
			qApp->processEvents();
			//读取相机信息
			std::string posename = list.at(j * ShootTimes + i + 1).absoluteFilePath().toStdString();
			ifstream poseinfo(posename);
			osg::Matrix VPW;
			for (int m = 0; m < 4; m++) {
				for (int n = 0; n < 4; n++) {
					poseinfo >> VPW(m, n);
				}
			}
			poseinfo.close();
			//读取相片
			QString depthname = depth.at(j * ShootTimes + i).absoluteFilePath();
			QString labelname = label.at(j * ShootTimes + i).absoluteFilePath();
			qDebug() << "converting: " << depthname<< endl;
			cv::Mat color = cv::imread(labelname.toStdString());
			cv::Mat depth = cv::imread(depthname.toStdString(),-1);
			
			for (int u = 0; u < color.cols; u++)
				for (int v = 0; v < color.rows; v++)
				{
					unsigned char d = depth.ptr<unsigned char>(v)[u]; // 深度值
					if (d == 255) continue; // 为 255 表示没有测量到
					osg::Vec4d point((double)u,(double)(color.rows - v) ,(double)d/255.0,1.0);
					double p[6];
					osg::Vec4d pointWorld = point * osg::Matrix::inverse(VPW);//外参矩阵求解

					p[0] = pointWorld[0] / pointWorld[3];
					p[1] = pointWorld[1] / pointWorld[3];
					p[2] = pointWorld[2] / pointWorld[3];
					/*p[3] = color.data[v * color.step + u * color.channels()];
					p[4] = color.data[v * color.step + u * color.channels() + 1];
					p[5] = color.data[v * color.step + u * color.channels() + 2];*/
					p[5] = color.ptr<uchar>(v)[u * 3];
					p[4] = color.ptr<uchar>(v)[u * 3 + 1];
					p[3] = color.ptr<uchar>(v)[u*3+2];
					pclfile << p[0] << " " << p[1] << " " << p[2] << " " << p[3] << " " << p[4] << " " << p[5]<<std::endl;
				}
		}
		pclfile.close();
		qDebug() << "write finished " << pclfilename << endl;
		/*flag = false;
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
		}*/
		j++;
	}
	return j;
}
