#include "textureprofile.h"

TextureProfile::TextureProfile(const string filename)
{
    readTextureFile(filename);

}

void TextureProfile::readTextureFile(const string filename)
{
    string fname = filename.substr(filename.find_last_of('/') + 1);
    //qDebug() << "读取贴图点位文件:" << fname.c_str();

    ifstream textureFile(filename);

    string line;
    while (getline(textureFile, line)) {
        Point3d vertex;
        auto index = line.find(' ');
        //qDebug() << index << sizeof (line);
        if(index < sizeof (line)){
            vertex.x = stod(line.substr(0, index));
            line = line.substr(index + 1);
            index = line.find(' ');
            vertex.y = stod(line.substr(0, index));
            vertex.z = stod(line.substr(index + 1));
        }
        else {
            auto index = line.find(',');
            vertex.x = stod(line.substr(0, index));
            line = line.substr(index + 1);
            index = line.find(',');
            vertex.y = stod(line.substr(0, index));
            vertex.z = stod(line.substr(index + 1));
        }
        //qDebug("%lf %lf %lf",vertex.x, vertex.y, vertex.z);
        texturePos.push_back(vertex);
    }
    //qDebug() << texturePos.size();
}

void TextureProfile::calcuTextureNormal(vector<Vertex> &vertice, vector<Point3d> &texturePos, vector<Point3d> &textureNormal)
{
    Point3d normal;

    for(vector<Point3d>::iterator it = texturePos.begin(); it != texturePos.end(); ++it){
        double dis = DBL_MAX;
        for(vector<Vertex>::iterator itt = vertice.begin(); itt != vertice.end(); ++itt){
            double temp = pow(it->x - itt->Position.x, 2.0) + pow(it->y - itt->Position.y, 2.0) + pow(it->z - itt->Position.z, 2.0);
            if(temp < dis){
                dis = temp;
                normal = itt->Normal;
            }
        }
        textureNormal.push_back(normal);
    }

//    for(vector<Point3d>::iterator it = textureNormal.begin(); it != textureNormal.end(); ++it){
//        qDebug() << it->x << it->y << it->z;
//    }
//    qDebug() << texturePos.size() << textureNormal.size();
}
