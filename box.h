#ifndef BOX_H
#define BOX_H
#include "visualobject.h"
class box : public VisualObject
{
public:
    box(float r, float g, float b, float u, float v);
    ~box(){};
};

#endif // BOX_H
