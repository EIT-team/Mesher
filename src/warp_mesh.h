
#ifndef WARP_H
#define WARP_H

#include "include.h"

using namespace std;

long get_array_index(int,int,int, int);
void modify_image(unsigned char *, int) ;
void stretch_array_1D( unsigned char *,  int, int, int, char, int );
void dilate_layer(unsigned char *, int, int,  int);
void change_neighbours(unsigned char *, long, int);
vector<long> neighbouring_elements(long,int);

void random_stretch(unsigned char*, int);
void random_dilate(unsigned char*, int);
vector<int> get_layers(unsigned char*, int);

#endif
