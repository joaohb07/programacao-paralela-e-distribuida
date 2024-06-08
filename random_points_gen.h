#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <bits/stdc++.h>
#include <deque>

using namespace std;

int randomInt(int a, int b)
{
    if (a > b)
        return randomInt(b, a);
    if (a == b)
        return a;
    return a + (rand() % (b - a));
}

float randomFloat()
{
    return (float)(rand()) / (float)(RAND_MAX);
}

float randomFloat(int a, int b)
{
    if (a > b)
        return randomFloat(b, a);
    if (a == b)
        return a;
    return (float)randomInt(a, b) + randomFloat();
}

deque<float> randompointgen(float boxsize, float gridsize){
    float num_by_r_c =  boxsize/gridsize;
    float step = gridsize/2;
    deque<float> points;

    for(int i = 0; i < num_by_r_c; i++){
        for(int j = 0; j < num_by_r_c; j++){
            //float rnd1 = jitter*randomFloat(step-10,step+10);
            //float rnd2 = jitter*randomFloat(step-10,step+10);
            //printf("Jitters: %f .. %f\n\n",rnd1,rnd2);
            points.push_back(i*gridsize+step);
            points.push_back(j*gridsize+step);
        }
    }
    /*int j = 0;
    for(int i = 0;i<points.size();i=i+2)
    {
        printf("Ponto %d: %f .. %f\n\n",j,points[i],points[i+1]);
        j++;
    }*/
    return points;
}