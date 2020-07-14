#ifndef TEXTUREPROFILE_H
#define TEXTUREPROFILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <QDebug>

#include <datastruct.h>

using namespace std;

class TextureProfile
{
public:
    vector<Point3d> texturePos;
    TextureProfile(const string filename);
    void readTextureFile(const string filename);
    void calcuTextureNormal(vector<Vertex> &vertice, vector<Point3d> &texturePos, vector<Point3d> &textureNormal);

};

#endif // TEXTUREPROFILE_H
