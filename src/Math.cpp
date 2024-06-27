#include "Math.h"

int GetIndexInMatrix(int width, int height, int x, int y){
    int result = (y==0) ? y*width+0 : (y-1)*width+x;
    if(result > width*height){
        return -1;
    }
    else{
        return result-1;
    }
}

float ProjRemap(float value){
    return (1+value)/2;
}