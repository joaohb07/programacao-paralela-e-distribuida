#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <bits/stdc++.h>
#include <deque>

using namespace std;

deque<float> pointgen(float boxsize, float gridsize){
    float num_by_r_c =  boxsize/gridsize;
    float step = gridsize/2;
    deque<float> points;

    for(int i = 0; i < num_by_r_c; i++){
        for(int j = 0; j < num_by_r_c; j++){
            points.push_back(i*gridsize+step);//x do ponto
            points.push_back(j*gridsize+step);//y do ponto
        }
    }
    return points;
}