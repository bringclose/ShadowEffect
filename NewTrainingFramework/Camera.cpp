#include "StdAfx.h"
#include "Camera.h"
#include "Globals.h"

#include <math.h>

CCamera::CCamera(void):m_fNear(0)
						,m_fFar(0)
						,m_fFOV(0)
						,m_fSpeed(0)
{
	m_v3Position.x = 0.0f;
	m_v3Position.y = 0.0f;
	m_v3Position.z = 10.0f;

	m_v3Target.x = 0.0f;
	m_v3Target.y = 0.0f;
	m_v3Target.z = -1.0f;

	m_v3Up.x = 0.0f;
	m_v3Up.y = 1.0f;
	m_v3Up.z = 0.0f;
}

CCamera::~CCamera(void)
{
}

void CCamera::ProcessKey(unsigned char keyCode, bool isPressed)
{
	Vector4 v4LocalTarget = Vector4(0.0f, 0.0f, -(m_v3Position - m_v3Target).Length(), 1.0f);
	Vector4 v4LocalNewTarget;
	Vector4 v4WorldNewTarget;

	if(isPressed)
	{
		switch(keyCode)
		{
			case 'W':
				esLogMessage("\n pressed W \n");
				m_v3DeltaMove = -zaxis * m_fDeltaTime * m_fSpeed;
				m_v3Position += m_v3DeltaMove;
				m_v3Target += m_v3DeltaMove;
				break;
			case 'S':
				esLogMessage("\n pressed S \n");
				m_v3DeltaMove = zaxis * m_fDeltaTime * m_fSpeed;
				m_v3Position += m_v3DeltaMove;
				m_v3Target += m_v3DeltaMove;
				break;
			case 'A':
				esLogMessage("\n pressed A \n");
				m_v3DeltaMove = -xaxis * m_fDeltaTime * m_fSpeed;
				m_v3Position += m_v3DeltaMove;
				m_v3Target += m_v3DeltaMove;
				break;
			case 'D':
				esLogMessage("\n pressed D \n");
				m_v3DeltaMove = xaxis * m_fDeltaTime * m_fSpeed;
				m_v3Position += m_v3DeltaMove;
				m_v3Target += m_v3DeltaMove;
				break;
			case '&':	//UP
				esLogMessage("\n pressed UP \n");
				v4LocalNewTarget = v4LocalTarget * maRotation.SetRotationX(m_fDeltaTime * m_fSpeed * RADIAN/10.0f);
				v4WorldNewTarget = v4LocalNewTarget * m_maModelCamera;
				m_v3Target.x = v4WorldNewTarget.x;
				m_v3Target.y = v4WorldNewTarget.y;
				m_v3Target.z = v4WorldNewTarget.z;
				break;
			case '(':	//DOWN
				esLogMessage("\n pressed DOWN \n");
				v4LocalNewTarget = v4LocalTarget * maRotation.SetRotationX(-m_fDeltaTime * m_fSpeed * RADIAN/10.0f);
				v4WorldNewTarget = v4LocalNewTarget * m_maModelCamera;
				m_v3Target.x = v4WorldNewTarget.x;
				m_v3Target.y = v4WorldNewTarget.y;
				m_v3Target.z = v4WorldNewTarget.z;
				break;
			case '%':	//LEFT
				esLogMessage("\n pressed LEFT \n");
				v4LocalNewTarget = v4LocalTarget * maRotation.SetRotationY(m_fDeltaTime * m_fSpeed * RADIAN/10.0f);
				v4WorldNewTarget = v4LocalNewTarget * m_maModelCamera;
				m_v3Target.x = v4WorldNewTarget.x;
				m_v3Target.y = v4WorldNewTarget.y;
				m_v3Target.z = v4WorldNewTarget.z;
				break;
			case '\'':	//RIGHT
				esLogMessage("\n pressed RIGHT \n");
				v4LocalNewTarget = v4LocalTarget * maRotation.SetRotationY(-m_fDeltaTime * m_fSpeed * RADIAN/10.0f);
				v4WorldNewTarget = v4LocalNewTarget * m_maModelCamera;
				m_v3Target.x = v4WorldNewTarget.x;
				m_v3Target.y = v4WorldNewTarget.y;
				m_v3Target.z = v4WorldNewTarget.z;
				break;
			case 'Q':
				esLogMessage("\n pressed Q \n");
				m_v3DeltaMove = yaxis * m_fDeltaTime * m_fSpeed;
				m_v3Position += m_v3DeltaMove;
				m_v3Target += m_v3DeltaMove;
				break;
			case 'E':
				esLogMessage("\n pressed E \n");
				m_v3DeltaMove = -yaxis * m_fDeltaTime * m_fSpeed;
				m_v3Position += m_v3DeltaMove;
				m_v3Target += m_v3DeltaMove;
				break;
			case '\ ':
				esLogMessage("\n Reset Camera \n");
				m_v3Position.x = 0.0f;
				m_v3Position.y = 10.0f;
				m_v3Position.z = 10.0f;

				m_v3Target.x = 0.0f;
				m_v3Target.y = 0.0f;
				m_v3Target.z = 0.0f;

				m_v3Up.x = 0.0f;
				m_v3Up.y = 1.0f;
				m_v3Up.z = 0.0f;
				break;
			default:
				esLogMessage("\n not camera movement key \n");
		}
	}
}

void CCamera::Update(float deltaTime)
{
	m_maView.SetIdentity();
	m_maModelCamera.SetIdentity();
	m_maProjection.SetIdentity();
	maRotation.SetIdentity();
	maTranslation.SetIdentity();

	m_maProjection.SetPerspective(m_fFOV, (float)Globals::screenWidth / (float)Globals::screenHeight, m_fNear, m_fFar);

	m_fDeltaTime = deltaTime;

	zaxis = (m_v3Position - m_v3Target).Normalize();
	xaxis = (m_v3Up.Cross(zaxis)).Normalize();
	yaxis = (zaxis.Cross(xaxis)).Normalize();

	//calculate view matrix
	m_maView.m[0][0] = xaxis.x;
	m_maView.m[0][1] = yaxis.x;
	m_maView.m[0][2] = zaxis.x;
	m_maView.m[0][3] = 0.0f;

	m_maView.m[1][0] = xaxis.y;
	m_maView.m[1][1] = yaxis.y;
	m_maView.m[1][2] = zaxis.y;
	m_maView.m[1][3] = 0.0f;

	m_maView.m[2][0] = xaxis.z;
	m_maView.m[2][1] = yaxis.z;
	m_maView.m[2][2] = zaxis.z;
	m_maView.m[2][3] = 0.0f;

	m_maView.m[3][0] = -m_v3Position.Dot(xaxis);
	m_maView.m[3][1] = -m_v3Position.Dot(yaxis);
	m_maView.m[3][2] = -m_v3Position.Dot(zaxis);
	m_maView.m[3][3] = 1.0f;


	//calculate world matrix
	//rotation matrix
	maRotation.m[0][0] = xaxis.x;
	maRotation.m[0][1] = xaxis.y;
	maRotation.m[0][2] = xaxis.z;
	maRotation.m[0][3] = 0.0f;

	maRotation.m[1][0] = yaxis.x;
	maRotation.m[1][1] = yaxis.y;
	maRotation.m[1][2] = yaxis.z;
	maRotation.m[1][3] = 0.0f;

	maRotation.m[2][0] = zaxis.x;
	maRotation.m[2][1] = zaxis.y;
	maRotation.m[2][2] = zaxis.z;
	maRotation.m[2][3] = 0.0f;

	maRotation.m[3][0] = 0.0f;
	maRotation.m[3][1] = 0.0f;
	maRotation.m[3][2] = 0.0f;
	maRotation.m[3][3] = 1.0f;

	//translation matrix
	maTranslation.m[0][0] = 1.0f;
	maTranslation.m[0][1] = 0.0f;
	maTranslation.m[0][2] = 0.0f;
	maTranslation.m[0][3] = 0.0f;

	maTranslation.m[1][0] = 0.0f;
	maTranslation.m[1][1] = 1.0f;
	maTranslation.m[1][2] = 0.0f;
	maTranslation.m[1][3] = 0.0f;

	maTranslation.m[2][0] = 0.0f;
	maTranslation.m[2][1] = 0.0f;
	maTranslation.m[2][2] = 1.0f;
	maTranslation.m[2][3] = 0.0f;

	maTranslation.m[3][0] = m_v3Position.x;
	maTranslation.m[3][1] = m_v3Position.y;
	maTranslation.m[3][2] = m_v3Position.z;
	maTranslation.m[3][3] = 1.0f;

	m_maModelCamera = maRotation * maTranslation;
}
