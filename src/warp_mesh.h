
#ifndef WARP_H
#define WARP_H

#include "include.h"

using namespace std;

long get_array_index(int,int,int, int);
void modify_image(unsigned char *) ;
void stretch_array_1D( unsigned char *,  int, int, int, int );
void dilate_layer(unsigned char *, int, int,  int);
void change_neighbours(unsigned char *, long, int);
vector<long> neighbouring_elements(long,int);

#endif
