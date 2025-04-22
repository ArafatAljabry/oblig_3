#include "VisualObject.h"

VisualObject::VisualObject()
{
    mMatrix.setToIdentity();
}

void VisualObject::move(float x, float y, float z)
{
    mMatrix.translate(x, y, z);
}

void VisualObject::scale(float s)
{
    mMatrix.scale(s);
}

void VisualObject::rotate(float t, float x, float y, float z)
{
    mMatrix.rotate(t, x, y, z);
}

void VisualObject::setPosition(const QVector3D& pos) {
    mMatrix.setColumn(3, QVector4D(pos, 1.0f));
}

QVector3D VisualObject::getPosition()
{
    return QVector3D(mMatrix(0,3), mMatrix(1,3), mMatrix(2,3));
}


