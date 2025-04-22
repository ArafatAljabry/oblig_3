#ifndef OBJECTMESH_H
#define OBJECTMESH_H

#include "VisualObject.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <QDebug>
#include <QVector3D>
#include "Vertex.h"
class ObjectMesh : public VisualObject
{
public:
    ObjectMesh(const std::string& filename);
private:
    bool readObjFile(const std::string& filename);
};

#endif // OBJECTMESH_H
