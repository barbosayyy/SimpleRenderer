#include "Object.h"

ObjectSystem::ObjectSystem(){
    
}

void ObjectSystem::Init(){
    System::Init();
}

void ObjectSystem::Shutdown(){
    System::Shutdown();
}

void ObjectSystem::AddTriangle(Triangle drawable)
{
    _objects.push_back(drawable);
}