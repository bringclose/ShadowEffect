#pragma once

#include "Math.h"

#define NUMBER_OBJECT		3
#define NUMBER_TEXTURE2D	6

#define PI			3.14f
#define RADIAN		PI/180.0f
#define MAX_TEXTURE_UNIT	10

#define FOV_DEGREE	45.0f
#define FOV			RADIAN * FOV_DEGREE
#define ASPECT		(GLfloat)Globals::screenWidth / (GLfloat)Globals::screenHeight
#define NEAR_PLANE	0.1f
#define FAR_PLANE	1000.0f
#define CAM_SPEED	100.0f

#define POSITION_OFFSET	0
#define NORMAL_OFFSET	(char*)0 + sizeof(Vector3)*1
#define BINORMAL_OFFSET	(char*)0 + sizeof(Vector3)*2
#define TANGENT_OFFSET	(char*)0 + sizeof(Vector3)*3
#define TEXCOORD_OFFSET	(char*)0 + sizeof(Vector3)*4

class Globals
{
public:
	static const int screenWidth = 960;
	static const int screenHeight = 720;
};