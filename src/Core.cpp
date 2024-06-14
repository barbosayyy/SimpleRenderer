#include "Core.h"

int GetMatPos(int width, int x, int y){
    return x*width+y;
}

float ProjRemap(float value){
    return (1+value)/2;
}