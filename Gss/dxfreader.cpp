#include "dxfreader.h"

DxfReader::DxfReader(const string &path)
{
    load(path);
}

bool DxfReader::load(const string &path)
{
    string suffixStr = path.substr(path.find_last_of('.') + 1);
    //cout << "suffixStr: " << suffixStr << endl;
    transform(suffixStr.begin(), suffixStr.end(), suffixStr.begin(), ::tolower);
    string fname = path.substr(path.find_last_of('/') + 1);
//    if (suffixStr == "dxf")
//        qDebug() << "读取贴图文件:" << fname.c_str();
    bool fileState = readCadFile(path);
    return fileState;
}

bool DxfReader::readCadFile(const string &path)
{
    bool badState = false;
    dx_data fData;
    //First read a dwg or dxf file
    dx_iface *input = new dx_iface();

    badState = input->fileImport(path, &fData);
    if (!badState) {
        cout << "Error reading file " << path << endl;
        return false;
    }
    else {
        //int i=0;
        CircleArc3d ar;
        ar.xVec = Vector3d(1.0, 0.0, 0.0);
        ar.yVec = Vector3d(0.0, 1.0, 0.0);
        Line3d ln;
        for (std::list<DRW_Entity*>::const_iterator it_ent = fData.mBlock->ent.begin(); it_ent != fData.mBlock->ent.end(); ++it_ent)
        {
            //emit notifyPrograssRate(i++ * 100/static_cast<int>(fData.mBlock->ent.size()));
            switch ((*it_ent)->eType){
            case DRW::CIRCLE:{
                    DRW_Circle* circle = static_cast<DRW_Circle*>(*it_ent);
                    ar.cet = Point3d(circle->basePoint.x, circle->basePoint.y, circle->basePoint.z);
                    ar.r = circle->radious;
                    ar.startAngle = 0.0;
                    ar.endAngle = 2 * PI;
                    circleArcVec.push_back(ar);
                    break;
            }

            case DRW::ARC:{
                    DRW_Arc* arc = static_cast<DRW_Arc*>(*it_ent);
                    ar.cet = Point3d(arc->basePoint.x, arc->basePoint.y, arc->basePoint.z);
                    ar.r = arc->radious;
                    ar.startAngle = arc->startAngle();
                    ar.endAngle = arc->endAngle();
                    circleArcVec.push_back(ar);
                    break;
            }

            case DRW::LINE:{
                DRW_Line* line = static_cast<DRW_Line*>(*it_ent);
                ln.b = Point3d(line->basePoint.x, line->basePoint.y, line->basePoint.z);
                ln.e = Point3d(line->secPoint.x, line->secPoint.y, line->secPoint.z);
                lineVec.push_back(ln);
                break;
            }

            case DRW::POLYLINE:{
                DRW_Polyline* polyline = static_cast<DRW_Polyline*>(*it_ent);
                std::vector<std::shared_ptr<DRW_Vertex>> vetlist = polyline->vertlist;
                for(unsigned int i = 0; i < vetlist.size()-1; i++){
                    ln.b = Point3d(vetlist[i]->basePoint.x, vetlist[i]->basePoint.y, vetlist[i]->basePoint.z);
                    ln.e = Point3d(vetlist[i+1]->basePoint.x, vetlist[i+1]->basePoint.y, vetlist[i+1]->basePoint.z);
                    lineVec.push_back(ln);
                }
                break;
            }

            default:
                break;
            }
        }
    }
    if(lineVec.size() == 0 && circleArcVec.size() == 0)
        return false;

    findMaxMinVertex();
    calcuCircleVec();
    //qDebug() << pointvec.size() << indices.size();
//    for (vector<Point3d>::iterator it = pointvec.begin(); it != pointvec.end(); ++it) {
//        qDebug() << it->x << it->y << it->z;
//    }
//    for (vector<unsigned int>::iterator it = indices.begin(); it != indices.end(); ++it) {
//        qDebug() << *it;
//    }
    return true;
}

void DxfReader::findMaxMinVertex()
{
    bool flag = true;

    for(Line3dVec::iterator it = lineVec.begin(); it != lineVec.end(); ++it){
        if(flag){
            minPos = it->b;
            maxPos = it->b;
            flag = false;
        }
        if(minPos.x > it->b.x) minPos.x = it->b.x;
        if(minPos.y > it->b.y) minPos.y = it->b.y;
        if(minPos.z > it->b.z) minPos.z = it->b.z;
        if(maxPos.x < it->b.x) maxPos.x = it->b.x;
        if(maxPos.y < it->b.y) maxPos.y = it->b.y;
        if(maxPos.z < it->b.z) maxPos.z = it->b.z;

        if(minPos.x > it->e.x) minPos.x = it->e.x;
        if(minPos.y > it->e.y) minPos.y = it->e.y;
        if(minPos.z > it->e.z) minPos.z = it->e.z;
        if(maxPos.x < it->e.x) maxPos.x = it->e.x;
        if(maxPos.y < it->e.y) maxPos.y = it->e.y;
        if(maxPos.z < it->e.z) maxPos.z = it->e.z;

        pointvec.push_back(it->b);
        pointvec.push_back(it->e);
        indices.push_back(index);
        index++;
        indices.push_back(index);
        index++;
    }

    //qDebug() << minPos.x << minPos.y << minPos.z << maxPos.x << maxPos.y << maxPos.z << circleArcVec.size();
}

void DxfReader::calcuCircleVec()
{
    if(circleArcVec.size()!= 0){
        for(CircleArc3dVec::iterator it = circleArcVec.begin(); it != circleArcVec.end(); ++it){
            double angle = fabs(it->startAngle - it->endAngle);
            unsigned int temp_index = index;
            bool flag = true;
            //圆
            if(fabs(angle - 2 * PI) < 0.00001){
                for(double k = 0; k < 2 * PI; k += 0.06){
                    Point3d point = it->calculatePoint3dByAngle(k);
                    pointvec.push_back(point);
                    if(flag){
                        indices.push_back(index);
                        index++;
                        flag = false;
                    }
                    else {
                        indices.push_back(index);
                        indices.push_back(index);
                        index++;
                    }
                }
                indices.push_back(temp_index);
            }
            //圆弧
            else {
                if(it->startAngle > it->endAngle){
                    it->endAngle += 2 * PI;
                }
                for(double k = it->startAngle; k < it->endAngle; k += 0.06){
                    Point3d point = it->calculatePoint3dByAngle(k);
                    pointvec.push_back(point);
                    if(flag){
                        indices.push_back(index);
                        index++;
                        flag = false;
                    }
                    else {
                        indices.push_back(index);
                        indices.push_back(index);
                        index++;
                    }
                }
                pointvec.push_back(it->calculatePoint3dByAngle(it->endAngle));
                indices.push_back(index);
                index++;
            }
        }
    }
}
