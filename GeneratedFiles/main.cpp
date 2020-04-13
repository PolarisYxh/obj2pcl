#include"Photographer_sx.h"
#include<string>
#include<iostream>
using namespace std;

//2020.4.3  工具接口函数 读入ive模型所在路径 保存路径root
//                           dof名称+每次运动幅度+次数 【只有一个dof版本】
//                           相机所在高度+运动半径+拍照次数【模型中心点默认是原点 版本】
//                           
//   输出
int main() {
	string ive_path = "E:\\F\\pick_flt&txt\\flt\\refrigerator\\0004\\0004.ive";
	string save_path = "E:\\F\\pick_flt&txt\\scene\\Tool_test\\";
	string dof_name = "dof_rootd_Aa001_r";
	double ani_angle = 20, ani_count = 4, cam_H = 3, track_R = 3, photo_count = 4;
	Photographer pg;
	pg.Tool_test_1(ive_path,save_path,dof_name,ani_angle,ani_count,cam_H,track_R,photo_count);
	return 0;
}