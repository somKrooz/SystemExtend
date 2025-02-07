#ifndef APPLICATION_H
#define APPLICATION_H

#include <Entity.h>
#include <vector>
#include <memory> 


class Application
{
private:
    std::vector<std::unique_ptr<Entity>> comp;
public:
    void AddEntity(std::unique_ptr<Entity> ent); 
    void DrawEntity();
    void UpdateEntity(float Dt);
};


#endif