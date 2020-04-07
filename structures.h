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
    std::vector<vector<int> > PartGroups;		//每个向量中为一个GT包含的group索引
    std::vector<int> PartLabel;				//每个向量中为PartGroups中对应的label
    std::vector<std::vector<double> > AABBboxes;	//每个向量中为一个长度为10的AABB包围盒矩阵
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
	int Midx;		//如果用材质库，为材质索引；否则为0
};
struct Group{
	string Gname;
	int BeginIndex;
	int EndIndex;
};
struct Vector2f{		//屏幕坐标格式
	float x;
	float y;
};
struct Material{
	string Mname;	//材质名称
	POINT3 Ka;		//GL_AMBIENT
	POINT3 Kd;		//GL_DIFFUSE
	POINT3 Ks;		//GL_SPECULAR
	float Ns;		//GL_SHINESS
	float d;		//材质透明度
};
class PIC{
public:    
    vector<POINT3> V;//V：代表顶点。格式为V X Y Z，V后面的X Y Z表示三个顶点坐标。浮点型
    vector<Texture>  VT;//表示纹理坐标。格式为VT TU TV。浮点型
    vector<Normal> VN;//VN：法向量。每个三角形的三个顶点都要指定一个法向量。格式为VN NX NY NZ。浮点型
    vector<Face> F;//F：面。面后面跟着的整型值分别是属于这个面的顶点、纹理坐标、法向量的索引。
                   //面的格式为：f Vertex1/Texture1/Normal1 Vertex2/Texture2/Normal2 Vertex3/Texture3/Normal3	或者f Vertex1 Vertex2 Vertex3
	vector<Group> G;
};
