#ifndef OBJECTS_H
#define OBJECTS_H

struct vector3
{
	float x;
	float y;
	float z;
};

struct vector4
{
	float x;
	float y;
	float z;
	float w;
};

struct matrix4
{
	float m[4][4];
};

struct vertex
{
	vector3 position;
	vector3 color;
};

#endif //OBJECTS_H
