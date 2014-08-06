#ifndef __CAMERA_H__
#define __CAMERA_H__

//#include "MyDefines.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

class CCamera
{
public:
	Vector3 m_v3Position;
	Vector3 m_v3Target;
	Vector3 m_v3Up;

	Matrix m_maView;
	Matrix m_maModelCamera;
	Matrix m_maProjection;

	GLfloat m_fNear;
	GLfloat m_fFar;
	GLfloat m_fFOV;
	GLfloat m_fSpeed;

	CCamera(void);
	~CCamera(void);

	void ProcessKey(unsigned char, bool);
	void Update(float);

private:
	Matrix maRotation;
	Matrix maTranslation;
	Matrix maScale;

	GLfloat m_fDeltaTime;

	Vector3 m_v3DeltaMove;

	Vector3 xaxis;
	Vector3 yaxis;
	Vector3 zaxis;
};

#endif