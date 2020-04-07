#include <vector>
#include <string>
#include <stdio.h>
using namespace std;
#define BSIZE 512
#define NSIZE 5
#define MPATH "cd C:\\Users\\USER_CC\\Documents\\MatlabProjects\\"

struct BoxRInfo{
	int rot[3];
	double volumn;
};

inline string Num2String(int inputNum){
	char a[100];
    std::sprintf(a,"%d", inputNum);
	string s = a;
	return s;

}
struct POINT3{
    double X;
    double Y;
    double Z;
};
struct selAABB{
	int model_Idx;
	int merge_Idx;
};
struct GTinfo{
    std::vector<vector<int> > PartGroups;		//ÿ��������Ϊһ��GT������group����
    std::vector<int> PartLabel;				//ÿ��������ΪPartGroups�ж�Ӧ��label
    std::vector<std::vector<double> > AABBboxes;	//ÿ��������Ϊһ������Ϊ10��AABB��Χ�о���
};
struct Texture{
    double TU;
    double TV;
};
struct Normal{
    double NX;
    double NY;
    double NZ;
};
struct Face{
    int V[3];
    int T[3];
    int N[3];  
	int Midx;		//����ò��ʿ⣬Ϊ��������������Ϊ0
};
struct Group{
	string Gname;
	int BeginIndex;
	int EndIndex;
};
struct Vector2f{		//��Ļ�����ʽ
	float x;
	float y;
};
struct Material{
	string Mname;	//��������
	POINT3 Ka;		//GL_AMBIENT
	POINT3 Kd;		//GL_DIFFUSE
	POINT3 Ks;		//GL_SPECULAR
	float Ns;		//GL_SHINESS
	float d;		//����͸����
};
class PIC{
public:    
    vector<POINT3> V;//V�������㡣��ʽΪV X Y Z��V�����X Y Z��ʾ�����������ꡣ������
    vector<Texture>  VT;//��ʾ�������ꡣ��ʽΪVT TU TV��������
    vector<Normal> VN;//VN����������ÿ�������ε��������㶼Ҫָ��һ������������ʽΪVN NX NY NZ��������
    vector<Face> F;//F���档�������ŵ�����ֵ�ֱ������������Ķ��㡢�������ꡢ��������������
                   //��ĸ�ʽΪ��f Vertex1/Texture1/Normal1 Vertex2/Texture2/Normal2 Vertex3/Texture3/Normal3	����f Vertex1 Vertex2 Vertex3
	vector<Group> G;
};
