#include"Photographer_sx.h"
#include<string>
#include<iostream>
using namespace std;

//2020.4.3  ���߽ӿں��� ����iveģ������·�� ����·��root
//                           dof����+ÿ���˶�����+���� ��ֻ��һ��dof�汾��
//                           ������ڸ߶�+�˶��뾶+���մ�����ģ�����ĵ�Ĭ����ԭ�� �汾��
//                           
//   ���
int main() {
	string ive_path = "E:\\F\\pick_flt&txt\\flt\\refrigerator\\0004\\0004.ive";
	string save_path = "E:\\F\\pick_flt&txt\\scene\\Tool_test\\";
	string dof_name = "dof_rootd_Aa001_r";
	double ani_angle = 20, ani_count = 4, cam_H = 3, track_R = 3, photo_count = 4;
	Photographer pg;
	pg.Tool_test_1(ive_path,save_path,dof_name,ani_angle,ani_count,cam_H,track_R,photo_count);
	return 0;
}