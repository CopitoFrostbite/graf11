////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_



///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "openglclass.h"
#include "cameraclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "Terreno.h"
#include "SkyDome.h"
#include "Billboard.h"
#include "Modelos.h"
#include "Water.h"
#define PI 3.14159265f

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 3000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(OpenGLClass*, HWND);
	void Shutdown();
	bool Frame();
	void Chkcoli(Modelos* x);

	float ambientLight[4] = { 0.8f, 0.9f, 1.0f, 1.0f }; //r//g//b
	float specularLight[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float luzDifusaAtardecer[4] = { 1.0f, 0.5f, 0.5f, 1.0f };
	float luzDifusaDia[4];
	float luzDifusaNoche[4];
	
	float factfuerte[4] = { 0.7f, 0.8f, 0.8f, 1.0f }; //fa, fd, fs
	float factdebil[4] = { 0.5f, 0.6f, 0.6f, 1.0f }; //fa, fd, fs
	float lightColor[3] = { 0.0f, 0.0f, 1.0f }; 
	float waterColor[3] = { 0.0f, 0.7f, 1.0f }; 
	
	float distortionFactor = 10;
	float shineness = 10.0f;

	float radianes(float angulo) {
		
		return (angulo * (PI / 180));
	}

	float angx, angy, angz, vel,x,y,z, px, py, pz, movx,movz;

private:
	bool Render(float,float);

private:
	OpenGLClass* m_OpenGL;
	CameraClass* m_Camera;	
	Terreno* terreno;
	Water* Agua, * Agua2;
	SkyDome* sky;
	Billboard* fall, * fall2, * billarb, * billarb2, * billarb3, * billflor1, * billflor2, * billflor3;
	Modelos* Dock,*Cab, * Door,* flor, * flor2, * flor3, * ben, * key, * roca, * canoa, * rama, * seed, * tree, * tree1, * tree2, * tree3, * tree4, * tree1b,  * tree2b, * tree3b, * tree4b, * post, * lamp,* tow;
	LightShaderClass* m_LightShader;
	LightShaderClass* m_LightShaderSky;	
	LightShaderClass* m_ModeloShader;
	LightShaderClass* m_BillShader, * m_fallShader;
	LightClass* m_difusaAtardecer;
	LightShaderClass* m_AwaShader;
	LightClass* m_difusaDia;
	LightClass* m_difusaNoche;
	
};

#endif