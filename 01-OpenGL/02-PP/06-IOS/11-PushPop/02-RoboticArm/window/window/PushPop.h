//
//  PushPop.h
//  window
//
//  Created by Admin on 03/03/23.
//
#include "vmath.h"
using namespace vmath;
#pragma once

#define PUSH_POP_MAX 10

//Push Pop Implementation

mat4 stack[PUSH_POP_MAX];
int frame_counter = -1;
void pushMatrix(mat4 frame)
{
    if(frame_counter != PUSH_POP_MAX)
    {

    stack[++frame_counter]= frame;
    //printf("counter at push : %d\n",frame_counter);

    }
    else
    {
        //fprintf(gpFile,"stack overflowed\n");
    }

}

mat4 popMatrix()
{
    if (frame_counter != -1)
    {
        
        //printf( "counter at pop : %d\n", frame_counter);
        return(stack[frame_counter--]);

    }
    else
    {
        //fprintf(gpFile, "stack empty\n");
        return(mat4::identity());
    }
    
}
