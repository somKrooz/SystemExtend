#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>

class Entity {
public:   
    ~Entity() = default;          
    virtual void Draw() =0;       
    virtual void Update(float Dt)=0;      
};

#endif
