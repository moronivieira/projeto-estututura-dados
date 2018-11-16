#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED

#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>

using namespace std;

enum type {BLACK, RED};

struct node {
	enum type color;
	string key;
	struct node *left, *right, *parent;
};

struct node *temp, *nill, *root;

#endif // STRUCT_H_INCLUDED
