#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include"globalvariable.h"
# define PI 3.14159265358979323846/* pi */
#include<array>
#include<vector>

using std::vector;
using std::array;

typedef glm::dvec2 Vector2d;
typedef glm::dvec3 Vector3d;
typedef glm::dvec2 Point2d;
typedef glm::dvec3 Point3d;

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
};

//三角面片的顶点索引
typedef struct tri_face
{
    unsigned int vertex_index0,vertex_index1,vertex_index2;
};


//线的存储结构（于模型点数据对应，仅保存点索引值）
typedef struct Line{
    unsigned int b,e;
    unsigned int &operator[] (unsigned int i){
        switch (i) {
            case 0:return b;
            case 1:return e;
        }
    }
    bool operator==(const Line & bL) const
    {
        if((b==bL.b && e==bL.e)||(b==bL.e && e==bL.b))
            return true;
        else
            return false;
    }
} IndexLine3d;


typedef struct
{
    Point2d b;//线的起点坐标
    Point2d e;//线的终点坐标
} Line2d;


//真实线的存储结构（于Line相对应，其保存真实点数据）
typedef struct {
    Point3d b;
    Point3d e;
} Line3d;


//三角面片的存储结构（于模型点数据对应，仅保存点索引值）
typedef struct {
    unsigned int P0Index,P1Index,P2Index;
    unsigned int &operator[] (unsigned int i){
        switch (i) {
            case 0:return P0Index;
            case 1:return P1Index;
            case 2:return P2Index;
        }
    }
    const unsigned int &operator[] (unsigned int i) const{
        switch (i) {
            case 0:return P0Index;
            case 1:return P1Index;
            case 2:return P2Index;
        }
    }
} Triangle;


//三角面片的存储结构（边存，与模型边数据对应，仅保存边索引值）
typedef struct {
    unsigned int S0Index,S1Index,S2Index;
    unsigned int &operator[] (unsigned int i){
        switch (i) {
            case 0:return S0Index;
            case 1:return S1Index;
            case 2:return S2Index;
        }
    }
    const unsigned int &operator[] (unsigned int i) const{
        switch (i) {
            case 0:return S0Index;
            case 1:return S1Index;
            case 2:return S2Index;
        }
    }
} TriangleSide;



//圆(弧)的存储结构（圆是一种特殊的圆弧）
typedef struct{
    Point3d cet;//圆心坐标
    double r;//圆弧的半径
    Vector3d xVec;//圆弧对应x轴正向单位向量
    Vector3d yVec;//圆弧对应y轴正向单位向量
    double startAngle, endAngle;//圆弧的起始角和终止角(弧度制)
    Point3d calculatePoint3dByAngle(const double &angle)const//通过弧度值计算椭圆上的点
    {
        return Point3d(this->cet.x + this->r*this->xVec.x * cos(angle) + this->r*this->yVec.x * sin(angle),this->cet.y + this->r*this->xVec.y * cos(angle) + this->r*this->yVec.y * sin(angle),this->cet.z + this->r*this->xVec.z * cos(angle) + this->r*this->yVec.z * sin(angle));
    }
} CircleArc3d;


//圆(弧)投影二维后的存储结构
typedef struct{
    double r;//半径
    Point2d cet;//圆心坐标
    double startAngle, endAngle;//起始角和终止角(弧度制)
} CircleArc2d;


//椭圆(弧)的存储结构
typedef struct{
    Point3d cet;//中心坐标
    Vector3d lenV;//长轴方向
    Vector3d shortV;//短轴方向
    double lLen;//长轴长
    double sLen;//短轴长
    double startAngle, endAngle;//圆弧的起始角和终止角(弧度制)
    Point3d calculatePoint3dByAngle(const double &angle)const//通过弧度值计算椭圆上的点
    {
        Point2d p2d(this->lLen*cos(angle),this->sLen*sin(angle));
        return Point3d(this->cet.x+p2d.x*this->lenV.x+p2d.y*this->shortV.x,this->cet.y+p2d.x*this->lenV.y+p2d.y*this->shortV.y,this->cet.z+p2d.x*this->lenV.z+p2d.y*this->shortV.z);
    }
} EllipseArc3d;


//椭圆(弧)投影二维后的存储结构
typedef struct{
    Point2d cet;//中心坐标
    Point2d endPoint;//主轴端点坐标
    double ratio;//短轴与长轴比例
    double startAngle, endAngle;//结束角度(弧度制)
} EllipseArc2d;


//三维坐标系中定义的二维坐标系
typedef struct{
    Point3d cet;//坐标系原点坐标
    Vector3d xVector;//x轴正向坐标
    Vector3d yVector;//y轴正向坐标
} coordSystem2dIn3d;


//存储Line2d的vector
typedef vector<Line2d>  Line2dVec;

//存储CircleArc2d的vector
typedef vector<CircleArc2d>  CircleArc2dVec;

//存储EllipseArc2d的vector
typedef vector<EllipseArc2d>  EllipseArc2dVec;

//存储Line3d的vector
typedef vector<Line3d> Line3dVec;

//存储CircleArc3d的vector
typedef vector<CircleArc3d>  CircleArc3dVec;

//存储EllipseArc3d的vector
typedef vector<EllipseArc3d> EllipseArc3dVec;


#endif // DATASTRUCT_H
