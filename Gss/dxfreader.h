#ifndef DXFREADER_H
#define DXFREADER_H

#include "dx_iface.h"
#include "dx_data.h"

#include <string>
#include <iostream>
#include <algorithm>

#include <datastruct.h>
#include<QtDebug>

using namespace std;

class DxfReader
{
public:
    Line3dVec lineVec;
    vector<unsigned int> indices;
    vector<Point3d> pointvec;
    DxfReader(const string &path);
    bool load(const string &path);
    //const Line3dVec& getLineVec() const;
    //const CircleArc3dVec& getCircleArcVec() const;
    //double getTextureR() const;

private:
    bool readCadFile(const string &path);
    void findMaxMinVertex();
    void calcuCircleVec();


    Point3d maxPos,minPos;
    CircleArc3dVec circleArcVec;
    unsigned int index = 0;

    //double textureR;
};

#endif // DXFREADER_H
