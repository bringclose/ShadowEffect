// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include <conio.h>

#include "Globals.h"
#include "Object3D.h"
#include "Vertex.h"
#include "CModel.h"
#include "Shaders.h"
#include "Camera.h"
#include "Texture.h"

CCamera mainCamera;
CCamera lighViewCamera;

Matrix maWVP;
Matrix maLightViewWVP;
Matrix maTexSpace;	//texture transform matrix

Object3D objectList[NUMBER_OBJECT];
CModel modelList[NUMBER_OBJECT];
Shaders shaderList[NUMBER_OBJECT];
CTexture textureList[NUMBER_TEXTURE2D];

bool bIsLightSource = false;

GLuint fboId;
GLuint colorTexId;
GLuint depthTexId;

void InitResource(void);
void InitObjects(void);
void DrawObject(Object3D, CCamera);
void InitFBO(void);

int Init ( ESContext *esContext )
{
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

	//Init matrix tranform coordinate from clip space to texture space (texture transform matrix)
	//...

	InitResource();
	InitObjects();
	InitFBO();
	
	return 0;
}

void Draw ( ESContext *esContext )
{
	glClearColor ( 0.0f, 0.0f, 1.0f, 0.0f );
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//draw terrain and sphere to FBO to make shadow map
	//...

	//draw scene's objects
	DrawObject(objectList[0], mainCamera);	//ligh buld
	DrawObject(objectList[1], mainCamera);	//terrain
	DrawObject(objectList[2], mainCamera);	//sphere

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	mainCamera.Update(deltaTime);

	lighViewCamera.Update(deltaTime);
	objectList[0].pos = lighViewCamera.m_v3Position;	//use bulb to similate light source

	for(int i=0; i<NUMBER_OBJECT; i++)
	{
		objectList[i].Update(deltaTime);
	}	
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	if (bIsPressed)
	{
		switch(key)
		{
		case 'O':
			esLogMessage("\n pressed O - switched to moving light source\n");
			bIsLightSource = true;
			break;
		case 'P':
			esLogMessage("\n pressed P - switched to moving main camera\n");
			bIsLightSource = false;
			break;
		}
	}
	
	if(bIsLightSource)
		lighViewCamera.ProcessKey(key, bIsPressed);	//moving light source to test shadow
	else
		mainCamera.ProcessKey(key, bIsPressed);
}

void CleanUp()
{
}

int _tmain(int argc, _TCHAR* argv[])
{
	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Shadow Effect", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	printf("Press any key...\n");
	_getch();

	return 0;
}

void InitResource()
{
	modelList[0].LoadToBuffer("../Resources/Models/Square.nfg");	//for ligh bulb
	modelList[1].LoadToBuffer("../Resources/Models/Terrain.nfg");
	modelList[2].LoadToBuffer("../Resources/Models/Bila.nfg");

	shaderList[0].Init("../Resources/Shaders/LightBulb.vs", "../Resources/Shaders/LightBulb.fs");
	shaderList[1].Init("../Resources/Shaders/Terrain.vs", "../Resources/Shaders/Terrain.fs");
	shaderList[2].Init("../Resources/Shaders/Sphere.vs", "../Resources/Shaders/Sphere.fs");

	textureList[0].CreateTexture("../Resources/Textures/heightmap.tga");
	textureList[1].CreateTexture("../Resources/Textures/blendMap.tga");
	textureList[2].CreateTexture("../Resources/Textures/Grass.tga");
	textureList[3].CreateTexture("../Resources/Textures/Rock.tga");
	textureList[4].CreateTexture("../Resources/Textures/Dirt.tga");
	textureList[5].CreateTexture("../Resources/Textures/lightbulb.tga");
}

void InitObjects()
{
	//Init beginning stage of camera
	mainCamera.m_v3Position = Vector3(0.0f, 30.0f, 80.0f);
	mainCamera.m_v3Target = Vector3(0.0f, 20.0f, 0.0f);

	mainCamera.m_fSpeed = CAM_SPEED;
	mainCamera.m_fNear = NEAR_PLANE;
	mainCamera.m_fFar = FAR_PLANE;
	mainCamera.m_fFOV = FOV_DEGREE * RADIAN;

	//Init beginning stage of camera 2th which is used as ligh ViewProjection for bulding shadow map
	lighViewCamera.m_v3Position = Vector3(0.0f, 50.0f, -10.0f);
	lighViewCamera.m_v3Target = Vector3(0.0f, 0.0f, 0.0f);

	//init projection info for lightView
	//...

	//Init beginning stage of objects
	//light bulb
	objectList[0].m_Model = &modelList[0];
	objectList[0].m_Shaders = &shaderList[0];

	objectList[0].m_NoTexture2D = 1;
	objectList[0].m_TextureList = new CTexture();
	objectList[0].m_TextureList = &textureList[5];

	objectList[0].pos = lighViewCamera.m_v3Position;
	objectList[0].scale = Vector3(10.0f, 10.0f, 10.0f);

	//terrain
	objectList[1].m_Model = &modelList[1];
	objectList[1].m_Shaders = &shaderList[1];

	objectList[1].m_NoTexture2D = 5;
	objectList[1].m_TextureList = new CTexture[5];

	objectList[1].m_TextureList[0] = textureList[0];
	objectList[1].m_TextureList[1] = textureList[1];
	objectList[1].m_TextureList[2] = textureList[2];
	objectList[1].m_TextureList[3] = textureList[3];
	objectList[1].m_TextureList[4] = textureList[4];

	objectList[1].pos = Vector3(0.0f, 0.0f, 0.0f);

	//sphere
	objectList[2].m_Model = &modelList[2];
	objectList[2].m_Shaders = &shaderList[2];

	objectList[2].m_NoTexture2D = 1;
	objectList[2].m_TextureList = new CTexture();
	objectList[2].m_TextureList = &textureList[3];

	objectList[2].pos = Vector3(0.0f, 30.0f, 0.0f);
	objectList[2].scale = Vector3(0.1f, 0.1f, 0.1f);
}

void DrawObject(Object3D obj, CCamera camera)
{
	glUseProgram(obj.m_Shaders->program);
	

	glBindBuffer(GL_ARRAY_BUFFER, obj.m_Model->m_vertexBuffer);
	
	if(obj.m_Shaders->positionAttribute != -1)
	{
		glEnableVertexAttribArray(obj.m_Shaders->positionAttribute);
		glVertexAttribPointer(obj.m_Shaders->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), POSITION_OFFSET);
	}

	if(obj.m_Shaders->uvAttribute != -1)
	{
		glEnableVertexAttribArray(obj.m_Shaders->uvAttribute);
		glVertexAttribPointer(obj.m_Shaders->uvAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), TEXCOORD_OFFSET);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//transfer matrix for drawing model to Shader
	if(obj.m_Shaders->maWorldUniform != -1)
	{
		glUniformMatrix4fv(obj.m_Shaders->maWorldUniform, 1, GL_FALSE, (GLfloat*)obj.maWorld.m);
	}

	maWVP = obj.maWorld * camera.m_maView * camera.m_maProjection;	//update WVP
	if(obj.m_Shaders->wvpUniform != -1)
	{
		glUniformMatrix4fv(obj.m_Shaders->wvpUniform, 1, GL_FALSE, (GLfloat*)maWVP.m);
	}

	//transfer texture to Shader
	int textureUnit = 0;

	for(int j=0; j<obj.m_NoTexture2D; j++)
	{
		if(obj.m_Shaders->textureUniform[j] != -1)
		{
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, obj.m_TextureList[j].m_TextureID);
			glUniform1i(obj.m_Shaders->textureUniform[j], textureUnit);

			textureUnit++;
		}
	}	

	//transfer shadow map to shader
	//...

	//transfer WVP of lightView to shader for calculate terrain's position in light space
	//...

	//transfer Texture transform matrix to shader for calculate terrain's postion in texture
	//...
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );

	//using IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.m_Model->m_indexBuffer);

	glDrawElements(GL_TRIANGLES, obj.m_Model->m_iNumIndices, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
}

void InitFBO(void)
{
	//generate FBO
	//...

	//bind FBO to attach color and depth texture
	//...

	//generate color texture 
	//...
	//initialize an empty texture with same width & height with screen
	//...
	//attach texture to GL_COLOR_ATTACHMENT0 
	//...
	//bind system default texture 
	//...

	//generate depth texture
	//...
	//initialize an empty texture with same width & height with screen
	//...
	//attach texture to GL_DEPTH_ATTACHMENT 
	//...
	//bind system default texture 
	//...
	
	//unbind FBO
	//...
}