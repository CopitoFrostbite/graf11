////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
private:
	static CameraClass* instancia;
	struct VectorType 
	{ 
		float x, y, z;
	};
	CameraClass();
	CameraClass(const CameraClass&);
public:
	
	~CameraClass();

	void SetPosition(float, float, float);
	void SetBBox(float, float, float);
	void SetRotation(float, float, float);
	void SetYPos(float);
	float GetXPos();
	float GetYPos();
	float GetZPos();
	float* GetBBox();
	static CameraClass* getCamera();

	

	/*bool colisionaArea(float esquinaIn[3], float esquinaFin[3]);*/
	bool colisionaArea(float, float, float, float, float, float);

	void Render(float);
	void GetViewMatrix(float*);

private:
	void MatrixRotationYawPitchRoll(float*, float, float, float);
	void TransformCoord(VectorType&, float*);
	void BuildViewMatrix(VectorType, VectorType, VectorType);
	
private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_viewMatrix[16];
	float m_bbox[6];
};

#endif