////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

bool spawn = true, pass = false, ableToMove = true;
int setwater;
float timerhora = 600, transicion = 0;
int setdia;
float onda = 0.0f;
float awa = 0.0f;

float ondaAgua() {
	return (sin(onda += 0.001) + 1) / 2.0f;
}

float interpolacionLineal(float& interpolador, float paso) {
	interpolador += paso;
	if (interpolador > 1.0f)
		interpolador -= 1.0f;
	return interpolador;
}

GraphicsClass::GraphicsClass()
{
	m_OpenGL = 0;
	m_Camera = 0;
	m_LightShader = 0;
	m_LightShaderSky = 0;
	m_AwaShader = 0;
	m_BillShader = 0;
	m_fallShader = 0;
	m_difusaAtardecer = 0;
	angx = angy = angz = 0;
	movx = 0;
	movz = 0;
	x = 800.0f;
	y = 0.0f;
	z = -72.0f;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(OpenGLClass* OpenGL, HWND hwnd)
{
	bool result;


	// Store a pointer to the OpenGL class object.
	m_OpenGL = OpenGL;

	// Create the camera object.
	m_Camera = CameraClass::getCamera();
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(x, y, z);

	terreno = new Terreno(hwnd, m_OpenGL, 
		L"Lakes5.png", 
		L"forest.jpg", 0,
		L"forest_norm.jpg", 1,
		L"dirt.jpg", 5,
		L"dirt_norm.jpg", 7,
		L"cliff.jpg", 10,
		L"cliff_norm.jpg", 11,
		L"Piedras.jpg", 8,
		L"Piedras_normal.jpg", 9,
		L"blend3.jpg", 6,
		
		(float)3000, (float)3000);


	Agua = new Water(hwnd, m_OpenGL, L"AguaAltura.png", (float)1700, (float)1700,
		L"wat.png", L"awa2_normal.png", L"awa2.png", L"awa2spec.png",
		12, 13,3,4);

	Agua2 = new Water(hwnd, m_OpenGL, L"AguaAltura2.png", (float)120, (float)105,
		L"wat.png", L"awa2_normal.png", L"awa2.png", L"awa2spec.png",
		12, 13, 3, 4);
	
	m_AwaShader = new LightShaderClass((char*)"Agua.vs", (char*)"Agua.ps");

	if (!m_AwaShader)
	{
		return false;
	}

	result = m_AwaShader->Initialize(m_OpenGL, hwnd);
	if (!result)
	{
		MessageBox(hwnd, (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass((char*)"light.vs", (char*)"light.ps");
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_OpenGL, hwnd);
	if(!result)
	{
		MessageBox(hwnd, (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return false;
	}
	//TARDE
	// Create the light object.
	m_difusaAtardecer = new LightClass;
	if (!m_difusaAtardecer)
	{
		return false;
	}

	// Initialize the light object.
	m_difusaAtardecer->SetDiffuseColor(1.0f, 0.8f, 0.6f, 1.0f);
	/*m_Light->SetDirection(-5.0f, 0.5f, -1.0f);*/

	m_difusaDia = new LightClass;
	if (!m_difusaDia)
	{
		return false;
	}

	// Initialize the light object.
	m_difusaDia->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);

	//NOCHE
	m_difusaNoche = new LightClass;
	if (!m_difusaNoche)
	{
		return false;
	}

	// Initialize the light object.
	m_difusaNoche->SetDiffuseColor(0.6f, 0.8f, 1.0f, 1.0f);

	sky = new SkyDome(hwnd, m_OpenGL, 32, 32, 1950, L"a2.jpg", L"at.jpg", L"an.jpg", L"am.jpg", 2, 14, 15, 16);
	// Create the light shader object.
	m_LightShaderSky = new LightShaderClass((char*)"Sky.vs", (char*)"Sky.ps");
	if (!m_LightShaderSky)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShaderSky->Initialize(m_OpenGL, hwnd);
	if (!result)
	{
		MessageBox(hwnd, (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return false;
	}
	
	fall = new Billboard(hwnd, m_OpenGL, (WCHAR*)L"Modelos//fall//waterfa.png", (WCHAR*)L"Modelos//fall//waterfa_norm.png", (WCHAR*)L"Modelos//fall//waterfa_spec.png", (WCHAR*)L"Modelos//fall//waterfa21.png", (WCHAR*)L"Modelos//fall//waterfa2_norm1.png", (WCHAR*)L"Modelos//fall//waterfa2_spec1.png", 60.0f, 215, -543, 81, 82, 83,87, 88, 89);
	billarb = new Billboard(hwnd, m_OpenGL, (WCHAR*)L"Modelos//billarb//arbol.png", (WCHAR*)L"Modelos//billarb//arbol1.png", (WCHAR*)L"Modelos//billarb//arbol2.png", 60.0f, 562, -103, 105, 106, 107);
	billarb2 = new Billboard(hwnd, m_OpenGL, (WCHAR*)L"Modelos//billarb//arbolb.png", (WCHAR*)L"Modelos//billarb//arbolb1.png", (WCHAR*)L"Modelos//billarb//arbolb2.png", 60.0f, 361, -128, 117, 118, 119);
	billarb3 = new Billboard(hwnd, m_OpenGL, (WCHAR*)L"Modelos//billarb//arbolc.png", (WCHAR*)L"Modelos//billarb//arbolc1.png", (WCHAR*)L"Modelos//billarb//arbolc2.png", 60.0f, 280, -289, 120, 121, 122);
	billflor1 = new Billboard(hwnd, m_OpenGL, (WCHAR*)L"Modelos//flor//florbi.png", (WCHAR*)L"Modelos//flor//florbi1.png", (WCHAR*)L"Modelos//flor//florbi2.png", 5.0f, 102, -475, 108, 109, 110);
	billflor2 = new Billboard(hwnd, m_OpenGL, (WCHAR*)L"Modelos//flor//florbib.png", (WCHAR*)L"Modelos//flor//florbib1.png", (WCHAR*)L"Modelos//flor//florbib2.png", 5.0f, 845, -370, 111, 112, 113);
	billflor3 = new Billboard(hwnd, m_OpenGL, (WCHAR*)L"Modelos//flor//florbic.png", (WCHAR*)L"Modelos//flor//florbic1.png", (WCHAR*)L"Modelos//flor//florbic2.png", 5.0f, 188, -585, 114, 115, 116);
	// Create the light shader object.
	m_BillShader = new LightShaderClass((char*)"Bill.vs", (char*)"Bill.ps");
	if (!m_BillShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_BillShader->Initialize(m_OpenGL, hwnd);
	if (!result)
	{
		MessageBox(hwnd, (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return false;
	}

	m_fallShader = new LightShaderClass((char*)"fall.vs", (char*)"fall.ps");
	if (!m_fallShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_fallShader->Initialize(m_OpenGL, hwnd);
	if (!result)
	{
		MessageBox(hwnd, (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return false;
	}

	//modelos
	Dock = new Modelos(hwnd, m_OpenGL, "Modelos//Dock//Dock.obj", L"Modelos//Dock//Dock.png", L"Modelos//Dock//Dock2.png", L"Modelos//Dock//Dock3.png", 1, 755.0460453, 18.6666670, -520, 16);
	Cab = new Modelos(hwnd, m_OpenGL, "Modelos//cab//cab2.obj", L"Modelos//cab//wood1.png", L"Modelos//cab//wood1.1.png", L"Modelos//cab//wood1.2.png", 1, 790.0460453, 18.6666670, 70, 17);
	Cab->SetCons();
	Door = new Modelos(hwnd, m_OpenGL, "Modelos//door2//door2.obj", L"Modelos//door2//door2.png", L"Modelos//door2//door2.1.png", L"Modelos//door2//door2.2.png", 1, 790.0460453, 38.6666670, 70, 18);
	flor = new Modelos(hwnd, m_OpenGL, "Modelos//flor//flor.obj", L"Modelos//flor//flor.jpg", L"Modelos//flor//flor1.jpg", L"Modelos//flor//flor2.jpg", 1, 745.0460453, 26.6666670, -54, 19);
	flor2 = new Modelos(hwnd, m_OpenGL, "Modelos//flor2//flor2.obj", L"Modelos//flor2//flor2.jpg", L"Modelos//flor2//flor21.jpg", L"Modelos//flor2//flor22.jpg", 1, 664.0460453, 16.6666670, -43, 20);
	flor3 = new Modelos(hwnd, m_OpenGL, "Modelos//flor3//flor3.obj", L"Modelos//flor3//flor3.jpg", L"Modelos//flor3//flor31.jpg", L"Modelos//flor3//flor32.jpg", 1, 925.0460453, 26.6666670, -17, 21);
	key = new Modelos(hwnd, m_OpenGL, "Modelos//key//Key.obj", L"Modelos//key//Key.png", L"Modelos//key//key.1.png", L"Modelos//key//key.1.png", 1, 680, 16.6666670, 3, 22);
	canoa = new Modelos(hwnd, m_OpenGL, "Modelos//canoa//Canoa.obj", L"Modelos//canoa//Canoa.bmp", L"Modelos//canoa//CanoaNormal.png", L"Modelos//canoa//CanoaSpecular.png", 1, 731, 16.6666670, -610, 23);
	roca = new Modelos(hwnd, m_OpenGL, "Modelos//roca//Piedra.obj", L"Modelos//roca//PiedraColor.jpg", L"Modelos//roca//PiedraNormal.png", L"Modelos//roca//PiedraSpecular.png", 1, 738, 16.6666670, -323, 24);
	rama = new Modelos(hwnd, m_OpenGL, "Modelos//rama//rama.obj", L"Modelos//rama//rama.jpg", L"Modelos//rama//rama.1.png", L"Modelos//rama//rama.png", 1, 738, 16.6666670, 130, 25);
	seed = new Modelos(hwnd, m_OpenGL, "Modelos//seed-bag//seed.obj", L"Modelos//seed-bag//seed.jpg", L"Modelos//seed-bag//seed.1.png", L"Modelos//seed-bag//seed.2.png", 1, 738, 26.6666670, 125, 26);	
	tree1 = new Modelos(hwnd, m_OpenGL, "Modelos//tree1//Tree1.obj", L"Modelos//tree1//tree1.png", L"Modelos//tree1//tree1.1.png", L"Modelos//tree1//tree1.2.png", 1, 716, 16.6666670, -142, 28);	
	tree1b = new Modelos(hwnd, m_OpenGL, "Modelos//tree1//Tree1b.obj", L"Modelos//tree1//tree1b.png", L"Modelos//tree1//tree1b.1.png", L"Modelos//tree1//tree1b.2.png", 1, 716, 16.6666670, -142, 30);
	tree2 = new Modelos(hwnd, m_OpenGL, "Modelos//tree1//Tree1.obj", L"Modelos//tree1//tree1.png", L"Modelos//tree1//tree1.1.png", L"Modelos//tree1//tree1.2.png", 1, 937, 16.6666670, -226, 28);
	tree2b = new Modelos(hwnd, m_OpenGL, "Modelos//tree1//Tree1b.obj", L"Modelos//tree1//tree1b.png", L"Modelos//tree1//tree1b.1.png", L"Modelos//tree1//tree1b.2.png", 1, 937, 16.6666670, -226, 30);
	tree3 = new Modelos(hwnd, m_OpenGL, "Modelos//tree1//Tree1.obj", L"Modelos//tree1//tree1.png", L"Modelos//tree1//tree1.1.png", L"Modelos//tree1//tree1.2.png", 1, 605, 16.6666670, -287, 28);
	tree3b = new Modelos(hwnd, m_OpenGL, "Modelos//tree1//Tree1b.obj", L"Modelos//tree1//tree1b.png", L"Modelos//tree1//tree1b.1.png", L"Modelos//tree1//tree1b.2.png", 1, 605, 16.6666670, -287, 30);
	tree4 = new Modelos(hwnd, m_OpenGL, "Modelos//tree1//Tree1.obj", L"Modelos//tree1//tree1.png", L"Modelos//tree1//tree1.1.png", L"Modelos//tree1//tree1.2.png", 1, 391, 16.6666670, -444, 28);
	tree4b = new Modelos(hwnd, m_OpenGL, "Modelos//tree1//Tree1b.obj", L"Modelos//tree1//tree1b.png", L"Modelos//tree1//tree1b.1.png", L"Modelos//tree1//tree1b.2.png", 1, 391, 16.6666670, -444, 30);
	ben = new Modelos(hwnd, m_OpenGL, "Modelos//bench//ben.obj", L"Modelos//bench//ben.jpg", L"Modelos//bench//ben1.jpg", L"Modelos//bench//ben2.jpg", 1, 935.0460453, 30.6666670, -46, 31);
	post = new Modelos(hwnd, m_OpenGL, "Modelos//post//post.obj", L"Modelos//post//post.jpg", L"Modelos//post//post1.jpg", L"Modelos//post//post2.jpg", 1, 940.0460453, 20.6666670, -56, 32);
	lamp = new Modelos(hwnd, m_OpenGL, "Modelos//lamp//lamp.obj", L"Modelos//lamp//lamp.jpg", L"Modelos//lamp//lamp1.jpg", L"Modelos//lamp//lamp2.jpg", 1, 940.0460453, 10.6666670, -56, 33);
	tow = new Modelos(hwnd, m_OpenGL, "Modelos//tow//tow.obj", L"Modelos//tow//tow.png", L"Modelos//tow//tow1.png", L"Modelos//tow//tow2.png", 1, 164, 100.6666670, -477, 34);
	m_ModeloShader = new LightShaderClass((char*)"Modelo.vs", (char*)"Modelo.ps");
	if (!m_ModeloShader)
	{
		return false;
	}
	 
	// Initialize the light shader object.
	result = m_ModeloShader->Initialize(m_OpenGL, hwnd);
	if (!result)
	{
		MessageBox(hwnd, (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the light object.
	if (m_difusaAtardecer)
	{
		delete m_difusaAtardecer;
		m_difusaAtardecer = 0;
	}

	// Release the light shader object.
	
	
	if(m_LightShader)
	{
		m_LightShader->Shutdown(m_OpenGL);
		delete m_LightShader;
		m_LightShader = 0;
	}

	
	if (m_LightShaderSky)
	{
		m_LightShaderSky->Shutdown(m_OpenGL);
		delete m_LightShaderSky;
		m_LightShaderSky = 0;
	}

	if (m_AwaShader)
	{
		m_AwaShader->Shutdown(m_OpenGL);
		delete m_AwaShader;
		m_AwaShader = 0;
	}

	if (m_BillShader)
	{
		m_BillShader->Shutdown(m_OpenGL);
		delete m_BillShader;
		m_BillShader = 0;
	}

	if (m_fallShader)
	{
		m_fallShader->Shutdown(m_OpenGL);
		delete m_fallShader;
		m_fallShader = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the terreno object.
	if (terreno)
	{
		delete terreno;
		m_Camera = 0;
	}

	if (Agua)
	{
		delete Agua;
		m_Camera = 0;
	
	}

	if (Agua2)
	{
		delete Agua2;
		m_Camera = 0;

	}

	// Release the pointer to the OpenGL class object.
	m_OpenGL = 0;

	return;
}


bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f;
	static int frameCount = 0;
	const int MAX_FRAME_COUNT = 100;  // El valor máximo para el frameCount
	int animationSpeed = 1;  // Velocidad de animación en frames por segundo
	static float onda2 = 0.0f;
	// Update the rotation variable each frame.
	rotation += 0.0174532925f * 2.0f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	onda2 = (sin(onda2 += 0.01) + 1) / 2.0f;
	// Update the frame count based on the animation speed.
	frameCount = (frameCount + 1) % (MAX_FRAME_COUNT / animationSpeed);

	// Render the graphics scene.
	result = Render(rotation,onda2);
	if (!result)
	{
		return false;
	}

	return true;
}

void GraphicsClass::Chkcoli(Modelos* xx)
{
	if (!xx->getcons()) {
		xx->coliply(m_Camera->GetBBox()[1], m_Camera->GetBBox()[0], m_Camera->GetBBox()[3], m_Camera->GetBBox()[2], m_Camera->GetBBox()[5], m_Camera->GetBBox()[4]);
		if (xx->getcoliply() == 1)
		{

			ableToMove = false;
			m_Camera->SetPosition(px, terreno->Superficie(px, pz) + 6.5, pz);


			xx->setcoliply(0);
			ableToMove = true;
		}
	}
	else
	{
		xx->coliplyc(m_Camera->GetBBox()[1], m_Camera->GetBBox()[0], m_Camera->GetBBox()[3], m_Camera->GetBBox()[2], m_Camera->GetBBox()[5], m_Camera->GetBBox()[4]);
		if (xx->getcoliply() == 1)
		{

			ableToMove = false;
			m_Camera->SetPosition(px, terreno->Superficie(px, pz) + 6.5, pz);


			xx->setcoliply(0);
			ableToMove = true;
		}
	}
}

bool GraphicsClass::Render(float rotation, float framecount)
{
	float modelMatrix[16];
	float viewMatrix[16];
	float projectionMatrix[16];
	float lightDirection[3];
	float diffuseLightColor[4];
	float worldMatrixBill[16];
	float worldMatrix[16];
	float trasla[16], rota[16], escala[16];
	float frameCount = framecount;
	float sunPosition[3];       // Posición del sol
	float sunDirection[3];      // Dirección del sol
	float sunDiffuseColor[4];   // Color difuso del sol
	float moonPosition[3];      // Posición de la luna
	float moonDirection[3];     // Dirección de la luna
	float moonDiffuseColor[4];  // Color difuso de la luna
	int spot = 1;
	static int transicion = 0;
	/*static float pi = 3.1416;*/
	static float orbit = 0.0f;

	// Clear the buffers to begin the scene.
	m_OpenGL->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->SetRotation(angx, angy, angz);
	m_Camera->SetYPos(terreno->Superficie(m_Camera->GetXPos(), m_Camera->GetZPos()) + 6.5);
	m_Camera->SetBBox(m_Camera->GetXPos(), m_Camera->GetYPos(), m_Camera->GetZPos());
	Chkcoli(roca);
	Chkcoli(Cab);
	Chkcoli(Dock);
	Chkcoli(Door);
	Chkcoli(flor);
	Chkcoli(flor2);
	Chkcoli(flor3);
	Chkcoli(canoa);
	Chkcoli(tree1);
	Chkcoli(tree2);
	Chkcoli(tree3);
	Chkcoli(tree4);
	Chkcoli(ben);
	Chkcoli(lamp);
	Chkcoli(post);
	Chkcoli(tow);

	float cameraPosX = m_Camera->GetXPos();
	float cameraPosZ = m_Camera->GetZPos();
	float movementX = movx * cos(angy * PI / 180) + movz * sin(angy * PI / 180);
	float movementZ = -movx * sin(angy * PI / 180) + movz * cos(angy * PI / 180);
	if (ableToMove) {
		px = cameraPosX;
		pz = cameraPosZ;
		py = m_Camera->GetYPos();
		m_Camera->SetPosition(cameraPosX + movementX, terreno->Superficie(cameraPosX, cameraPosZ) + 6.5, cameraPosZ + movementZ);
		m_Camera->Render(vel);
	}
	
	float CamaraPos[3] = { m_Camera->GetXPos(), m_Camera->GetYPos(), m_Camera->GetZPos() };

	float atenuacionTransicion = 0.0f;
	
	
	if (orbit >= 0.0f && orbit <= 40.0f) {
		atenuacionTransicion = orbit / 40.0f;
		atenuacionTransicion = pow(atenuacionTransicion, 2);
		factdebil[0] = 0.5f;
		factdebil[1] = 0.6f;
		factdebil[2] = 0.6f;
		factdebil[3] = 1.0f;
		orbit += 0.1f;
		transicion = 0;
	}
	else if (orbit >= 40.0f && orbit <= 140.0f) {
		atenuacionTransicion = (orbit - 40.0f) / 100.0f;
		atenuacionTransicion = pow(atenuacionTransicion, 2);
		orbit += 0.1f;
		transicion = 1;
	}
	else if (orbit >= 140.0f && orbit <= 180.0f) {
		atenuacionTransicion = (orbit - 140.0f) / 40.0f;
		atenuacionTransicion = pow(atenuacionTransicion, 2);
		factdebil[0] = 0.5f;
		factdebil[1] = 0.6f;
		factdebil[2] = 0.6f;
		factdebil[3] = 1.0f;
		for (int i = 0; i < 4; i++) {
			factdebil[i] -= (atenuacionTransicion/10);
		}
		orbit += 0.1f;
		transicion = 2;
	}
	else if (orbit >= 180.0f) {
		atenuacionTransicion = (orbit - 180.0f) / 180.0f;
		atenuacionTransicion = pow(atenuacionTransicion, 2);
		factdebil[0] = 0.4f;
		factdebil[1] = 0.4f;
		factdebil[2] = 0.5f;
		factdebil[3] = 0.9f;
		for (int i = 0; i < 4; i++) {
			factdebil[i] += (atenuacionTransicion / 10);
		}
		orbit += 0.2f;
		transicion = 3;
	}
	if (orbit >= 360.00f) {
		orbit = 0.0f;
	}
	

	float anguloX = cos(orbit * PI / 180) * 1000.0f;
	float anguloY = sin(orbit * PI / 180) * 1000.0f;

	float sunDistance = 1950.0f;  
	sunPosition[0] = cos(orbit * PI / 180) * sunDistance;
	sunPosition[1] = sin(orbit * PI / 180) * sunDistance;
	sunPosition[2] = 100.0f;     
	sunDirection[0] = -sunPosition[0];
	sunDirection[1] = -sunPosition[1];
	sunDirection[2] = -sunPosition[2];

	// Configuración de la luz del sol
	sunDiffuseColor[0] = 1.0f;
	sunDiffuseColor[1] = 1.0f;
	sunDiffuseColor[2] = 1.0f;
	sunDiffuseColor[3] = 1.0f;

	// Configuración de la luz de la luna
	moonPosition[0] = -sunPosition[0];    
	moonPosition[1] = -sunPosition[1];
	moonPosition[2] = sunPosition[2];
	moonDirection[0] = -moonPosition[0];  
	moonDirection[1] = -moonPosition[1];
	moonDirection[2] = -moonPosition[2];

	// Color difuso de la luna (más tenue y blanca azulada)
	moonDiffuseColor[0] = 0.7f;
	moonDiffuseColor[1] = 0.7f;
	moonDiffuseColor[2] = 0.9f;
	moonDiffuseColor[3] = 1.0f;

	m_difusaAtardecer->SetDirection(sunDirection[0], sunDirection[1], sunDirection[2]);
	m_difusaDia->SetDirection(sunDirection[0], sunDirection[1], sunDirection[2]);
	m_difusaDia->SetDiffuseColor(sunDiffuseColor[0], sunDiffuseColor[1], sunDiffuseColor[2], sunDiffuseColor[3]);
	m_difusaNoche->SetDirection(moonDirection[0], moonDirection[1], moonDirection[2]);
	m_difusaNoche->SetDiffuseColor(moonDiffuseColor[0], moonDiffuseColor[1], moonDiffuseColor[2], moonDiffuseColor[3]);



	// Get the world, view, and projection matrices from the opengl and camera objects.
	m_OpenGL->GetWorldMatrix(modelMatrix);
	m_OpenGL->GetWorldMatrix(worldMatrixBill);
	m_OpenGL->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_OpenGL->GetProjectionMatrix(projectionMatrix);

	//m_difusaAtardecer->GetDirection(lightDirection);
	m_difusaAtardecer->GetDiffuseColor(this->luzDifusaAtardecer);
	m_difusaDia->GetDiffuseColor(sunDiffuseColor);
	m_difusaDia->GetDirection(sunDirection);
	
	m_difusaNoche->GetDirection(moonDirection);
	m_difusaNoche->GetDiffuseColor(moonDiffuseColor);


	//////////////////////////////////////////////////////////
	//actualizar aqui el vector de luz y el color de la misma
	//////////////////////////////////////////////////////////
#pragma region Cielo
	glDisable(GL_DEPTH_TEST);	
	m_LightShaderSky->SetShader(m_OpenGL);
	m_OpenGL->GetWorldMatrix(modelMatrix);
	m_OpenGL->MatrixRotationY(modelMatrix, radianes(-30.0f));
	

	m_LightShaderSky->PonMatriz4x4(m_OpenGL, (char*)"worldMatrix", modelMatrix);	
	m_LightShaderSky->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	m_LightShaderSky->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);
	m_LightShaderSky->Pon1Entero(m_OpenGL, (char*)"cieloM", 2);
	m_LightShaderSky->Pon1Entero(m_OpenGL, (char*)"cieloT", 14);
	m_LightShaderSky->Pon1Entero(m_OpenGL, (char*)"cieloN", 15);
	m_LightShaderSky->Pon1Entero(m_OpenGL, (char*)"cieloA", 16);
	m_LightShaderSky->Pon1Entero(m_OpenGL, (char*)"timerCielo", setdia);
	m_LightShaderSky->Pon1Entero(m_OpenGL, (char*)"trans", transicion);
	m_LightShaderSky->Pon1Flotante(m_OpenGL, (char*)"orbit", atenuacionTransicion);

	m_LightShaderSky->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
	m_LightShaderSky->PonVec4(m_OpenGL, (char*)"difusaDia", sunDiffuseColor);
	m_LightShaderSky->PonVec4(m_OpenGL, (char*)"difusaNoche", moonDiffuseColor);
	m_LightShaderSky->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);

	m_LightShaderSky->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
	m_LightShaderSky->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
	m_LightShaderSky->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
	m_LightShaderSky->PonVec3(m_OpenGL, (char*)"luzDireccion", sunDirection);
	
	m_LightShaderSky->PonVec3(m_OpenGL, (char*)"moonDirection", moonDirection);
	
	sky->Render(m_OpenGL);
	glEnable(GL_DEPTH_TEST);
	// Rotate the world matrix by the rotation value so that the triangle will spin.
	//m_OpenGL->MatrixRotationY(modelMatrix, rotation);
#pragma endregion Cielo



#pragma region Terreno
	//// Set the light shader as the current shader program and set the matrices that it will use for rendering.
	m_LightShader->SetShader(m_OpenGL);	
	m_OpenGL->GetWorldMatrix(modelMatrix);

	
	m_LightShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
	m_LightShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	m_LightShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

	m_LightShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);

	m_LightShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
	m_LightShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
	m_LightShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
	m_LightShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);

	m_LightShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
	m_LightShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);

	m_LightShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
	m_LightShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
	m_LightShader->Pon1Entero(m_OpenGL, (char*)"distortionFactor", distortionFactor);

	m_LightShader->Pon1Entero(m_OpenGL, (char*)"shaderForest", 0);
	m_LightShader->Pon1Entero(m_OpenGL, (char*)"shaderForestNorm", 1);
	m_LightShader->Pon1Entero(m_OpenGL, (char*)"shaderDirt", 5);
	m_LightShader->Pon1Entero(m_OpenGL, (char*)"shaderDirtNorm", 7);
	m_LightShader->Pon1Entero(m_OpenGL, (char*)"shaderCliff", 8);
	m_LightShader->Pon1Entero(m_OpenGL, (char*)"shaderCliffNorm", 9);
	m_LightShader->Pon1Entero(m_OpenGL, (char*)"shaderPiedra", 10);
	m_LightShader->Pon1Entero(m_OpenGL, (char*)"shaderPiedraNorm", 11);
	m_LightShader->Pon1Entero(m_OpenGL, (char*)"shaderBlend", 6);
	

	
	
	// Render the model using the light shader.
	terreno->Render(m_OpenGL);

#pragma endregion Terreno


	

#pragma region Billboards
	float lin = interpolacionLineal(awa,0.001);
	float sin = ondaAgua();
	m_OpenGL->GetWorldMatrix(worldMatrixBill);
	m_OpenGL->MatrixTranslation(worldMatrixBill, fall->x, terreno->Superficie(fall->x, fall->z) - 1, fall->z);
	m_OpenGL->MatrixTranslation(trasla, fall->x, terreno->Superficie(fall->x, fall->z) + 4, fall->z);
	m_OpenGL->MatrixRotationY(rota, -(fall->anguloBill(m_Camera->GetXPos(), m_Camera->GetZPos())));

	m_OpenGL->MatrixMultiply(worldMatrixBill, rota, trasla);

	m_fallShader->SetShader(m_OpenGL);
	m_fallShader->PonMatriz4x4(m_OpenGL, (char*)"worldMatrix", worldMatrixBill);
	m_fallShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	m_fallShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

	m_fallShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
	m_fallShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);

	m_fallShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
	m_fallShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
	m_fallShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
	m_fallShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
	m_fallShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);

	m_fallShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
	m_fallShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);

	m_fallShader->Pon1Flotante(m_OpenGL, (char*)"framecount", sin);
	m_fallShader->Pon1Flotante(m_OpenGL, (char*)"framecount2", lin);
	m_fallShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
	m_fallShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
	m_fallShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

	m_fallShader->Pon1Entero(m_OpenGL, (char*)"billtext", 81);
	m_fallShader->Pon1Entero(m_OpenGL, (char*)"billnormText", 82);
	m_fallShader->Pon1Entero(m_OpenGL, (char*)"billspecText", 83);
	m_fallShader->Pon1Entero(m_OpenGL, (char*)"billtext2", 87);
	m_fallShader->Pon1Entero(m_OpenGL, (char*)"billnormText2", 88);
	m_fallShader->Pon1Entero(m_OpenGL, (char*)"billspecText2", 89);

	fall->Render(m_OpenGL);
	//billarb
	
	m_OpenGL->GetWorldMatrix(worldMatrixBill);
	m_OpenGL->MatrixTranslation(worldMatrixBill, billarb->x, terreno->Superficie(billarb->x, billarb->z) - 1, billarb->z);
	m_OpenGL->MatrixTranslation(trasla, billarb->x, terreno->Superficie(billarb->x, billarb->z)-6 , billarb->z);
	m_OpenGL->MatrixRotationY(rota, -(billarb->anguloBill(m_Camera->GetXPos(), m_Camera->GetZPos())));

	m_OpenGL->MatrixMultiply(worldMatrixBill, rota, trasla);

	m_BillShader->SetShader(m_OpenGL);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"worldMatrix", worldMatrixBill);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

	m_BillShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
	m_BillShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);

	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
	m_BillShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
	m_BillShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);

	m_BillShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
	m_BillShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);

	
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
	m_BillShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billtext", 105);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billnormText", 106);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billspecText", 107);
	

	billarb->Render2(m_OpenGL);

	//billflor1

	m_OpenGL->GetWorldMatrix(worldMatrixBill);
	m_OpenGL->MatrixTranslation(worldMatrixBill, billflor1->x, terreno->Superficie(billflor1->x, billflor1->z) - 1, billflor1->z);
	m_OpenGL->MatrixTranslation(trasla, billflor1->x, terreno->Superficie(billflor1->x, billflor1->z) , billflor1->z);
	m_OpenGL->MatrixRotationY(rota, -(billflor1->anguloBill(m_Camera->GetXPos(), m_Camera->GetZPos())));

	m_OpenGL->MatrixMultiply(worldMatrixBill, rota, trasla);

	m_BillShader->SetShader(m_OpenGL);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"worldMatrix", worldMatrixBill);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

	m_BillShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
	m_BillShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);

	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
	m_BillShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
	m_BillShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);

	m_BillShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
	m_BillShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);

	m_BillShader->Pon1Flotante(m_OpenGL, (char*)"framecount", sin);
	m_BillShader->Pon1Flotante(m_OpenGL, (char*)"framecount2", lin);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
	m_BillShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billtext", 108);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billnormText", 109);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billspecText", 110);


	billflor1->Render2(m_OpenGL);

	//billflor2

	m_OpenGL->GetWorldMatrix(worldMatrixBill);
	m_OpenGL->MatrixTranslation(worldMatrixBill, billflor2->x, terreno->Superficie(billflor2->x, billflor2->z) - 1, billflor2->z);
	m_OpenGL->MatrixTranslation(trasla, billflor2->x, terreno->Superficie(billflor2->x, billflor2->z) , billflor2->z);
	m_OpenGL->MatrixRotationY(rota, -(billflor2->anguloBill(m_Camera->GetXPos(), m_Camera->GetZPos())));

	m_OpenGL->MatrixMultiply(worldMatrixBill, rota, trasla);

	m_BillShader->SetShader(m_OpenGL);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"worldMatrix", worldMatrixBill);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

	m_BillShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
	m_BillShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);

	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
	m_BillShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
	m_BillShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);

	m_BillShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
	m_BillShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);

	m_BillShader->Pon1Flotante(m_OpenGL, (char*)"framecount", sin);
	m_BillShader->Pon1Flotante(m_OpenGL, (char*)"framecount2", lin);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
	m_BillShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billtext", 111);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billnormText", 112);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billspecText", 113);


	billflor2->Render2(m_OpenGL);

	//billflor3

	m_OpenGL->GetWorldMatrix(worldMatrixBill);
	m_OpenGL->MatrixTranslation(worldMatrixBill, billflor3->x, terreno->Superficie(billflor3->x, billflor3->z) - 1, billflor3->z);
	m_OpenGL->MatrixTranslation(trasla, billflor3->x, terreno->Superficie(billflor3->x, billflor3->z) , billflor3->z);
	m_OpenGL->MatrixRotationY(rota, -(billflor3->anguloBill(m_Camera->GetXPos(), m_Camera->GetZPos())));

	m_OpenGL->MatrixMultiply(worldMatrixBill, rota, trasla);

	m_BillShader->SetShader(m_OpenGL);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"worldMatrix", worldMatrixBill);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

	m_BillShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
	m_BillShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);

	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
	m_BillShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
	m_BillShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);

	m_BillShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
	m_BillShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);

	m_BillShader->Pon1Flotante(m_OpenGL, (char*)"framecount", sin);
	m_BillShader->Pon1Flotante(m_OpenGL, (char*)"framecount2", lin);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
	m_BillShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billtext", 114);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billnormText", 115);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billspecText", 116);


	billflor3->Render2(m_OpenGL);
	
	//billarb2

	m_OpenGL->GetWorldMatrix(worldMatrixBill);
	m_OpenGL->MatrixTranslation(worldMatrixBill, billarb2->x, terreno->Superficie(billarb2->x, billarb2->z) - 1, billarb2->z);
	m_OpenGL->MatrixTranslation(trasla, billarb2->x, terreno->Superficie(billarb2->x, billarb2->z) - 4, billarb2->z);
	m_OpenGL->MatrixRotationY(rota, -(billarb2->anguloBill(m_Camera->GetXPos(), m_Camera->GetZPos())));

	m_OpenGL->MatrixMultiply(worldMatrixBill, rota, trasla);

	m_BillShader->SetShader(m_OpenGL);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"worldMatrix", worldMatrixBill);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

	m_BillShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
	m_BillShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);

	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
	m_BillShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
	m_BillShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);

	m_BillShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
	m_BillShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);


	m_BillShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
	m_BillShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billtext", 117);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billnormText", 118);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billspecText", 119);


	billarb2->Render2(m_OpenGL);

	//billarb3

	m_OpenGL->GetWorldMatrix(worldMatrixBill);
	m_OpenGL->MatrixTranslation(worldMatrixBill, billarb3->x, terreno->Superficie(billarb3->x, billarb3->z) - 1, billarb3->z);
	m_OpenGL->MatrixTranslation(trasla, billarb3->x, terreno->Superficie(billarb3->x, billarb3->z) - 4, billarb3->z);
	m_OpenGL->MatrixRotationY(rota, -(billarb3->anguloBill(m_Camera->GetXPos(), m_Camera->GetZPos())));

	m_OpenGL->MatrixMultiply(worldMatrixBill, rota, trasla);

	m_BillShader->SetShader(m_OpenGL);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"worldMatrix", worldMatrixBill);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

	m_BillShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
	m_BillShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);

	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
	m_BillShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
	m_BillShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
	m_BillShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);

	m_BillShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
	m_BillShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);


	m_BillShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
	m_BillShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billtext", 120);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billnormText", 121);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billspecText", 122);


	billarb3->Render2(m_OpenGL);

#pragma endregion Billboards

#pragma region Modelo
	if (spawn == true) {
		// Set the light shader as the current shader program and set the matrices that it will use for rendering.
		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, Dock->x, terreno->Superficie(Dock->x, Dock->z) + 11, Dock->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 48);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 49);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 50);
		Dock->Render(m_OpenGL);

		//caba
		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, Cab->x, terreno->Superficie(Cab->x, Cab->z) + 21, Cab->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 51);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 52);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 53);
		Cab->Render(m_OpenGL);

		//door
		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, Door->x, terreno->Superficie(Door->x, Door->z) + 21, Door->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 54);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 55);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 56);
		Door->Render(m_OpenGL);
		

		//key

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, key->x, terreno->Superficie(key->x, key->z) + 5.3, key->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 66);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 67);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 68);
		key->Render(m_OpenGL);

		//canoa
		float flot = ondaAgua();
		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, canoa->x, terreno->Superficie(canoa->x, canoa->z ) + 5.3 + (flot ), canoa->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 69);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 70);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 71);
		canoa->Render(m_OpenGL);

		//roca

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, roca->x, terreno->Superficie(roca->x, roca->z) + 5.3, roca->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 72);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 73);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 74);
		roca->Render(m_OpenGL);

		//rama

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, rama->x, terreno->Superficie(rama->x, rama->z) + 15.3, rama->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 75);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 76);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 77);
		rama->Render(m_OpenGL);
		//seed

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, seed->x, terreno->Superficie(seed->x, seed->z) + 1.3, seed->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 78);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 79);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 80);
		seed->Render(m_OpenGL);
		//flor

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, flor->x, terreno->Superficie(flor->x, flor->z) + 0, flor->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 57);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 58);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 59);
		flor->Render(m_OpenGL);

		//flor2

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, flor2->x, terreno->Superficie(flor2->x, flor2->z) + 0, flor2->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 60);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 61);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 62);
		flor2->Render(m_OpenGL);

		//flor3

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, flor3->x, terreno->Superficie(flor3->x, flor3->z) + 0, flor3->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 63);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 64);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 65);
		flor3->Render(m_OpenGL);

		//tree

		/*m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, tree->x, terreno->Superficie(tree->x, tree->z) + 5.3, tree->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", lightDirection);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 81);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 82);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 83);
		tree->Render(m_OpenGL);*/

		//tree1

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, tree1->x, terreno->Superficie(tree1->x, tree1->z) + 0, tree1->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 84);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 85);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 86);
		tree1->Render(m_OpenGL);

		//tree1b

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, tree1b->x, terreno->Superficie(tree1b->x, tree1b->z) + 0, tree1b->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 90);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 91);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 92);
		tree1b->Render(m_OpenGL);

		//tree1

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, tree2->x, terreno->Superficie(tree2->x, tree2->z) + 0, tree2->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 84);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 85);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 86);
		tree2->Render(m_OpenGL);

		//tree1b

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, tree2b->x, terreno->Superficie(tree2b->x, tree2b->z) + 0, tree2b->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 90);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 91);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 92);
		tree2b->Render(m_OpenGL);

		//tree1

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, tree3->x, terreno->Superficie(tree3->x, tree3->z) + 0, tree3->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 84);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 85);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 86);
		tree3->Render(m_OpenGL);

		//tree1b

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, tree3b->x, terreno->Superficie(tree3b->x, tree3b->z) + 0, tree3b->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 90);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 91);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 92);
		tree3b->Render(m_OpenGL);
		
		//tree1

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, tree4->x, terreno->Superficie(tree4->x, tree4->z) + 0, tree4->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 84);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 85);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 86);
		tree4->Render(m_OpenGL);

		//tree1b

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, tree4b->x, terreno->Superficie(tree4b->x, tree4b->z) + 0, tree4b->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 90);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 91);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 92);
		tree4b->Render(m_OpenGL);
		

		//tree5

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, tree1b->x, terreno->Superficie(tree1b->x, tree1b->z) + 0, tree1b->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 90);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 91);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 92);
		tree1b->Render(m_OpenGL);

		//bench

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, ben->x, terreno->Superficie(ben->x, ben->z) + 0, ben->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 93);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 94);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 95);
		ben->Render(m_OpenGL);

		//post

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, post->x, terreno->Superficie(post->x, post->z) + 0, post->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 96);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 97);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 98);
		post->Render(m_OpenGL);

		//lamp

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, lamp->x, terreno->Superficie(lamp->x, lamp->z) + 0, lamp->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 1);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 99);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 100);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 101);
		lamp->Render(m_OpenGL);

		//tow

		m_ModeloShader->SetShader(m_OpenGL);

		m_OpenGL->GetWorldMatrix(modelMatrix);
		m_OpenGL->MatrixRotationY(rota, radianes(0.0f));
		m_OpenGL->MatrixTranslation(trasla, tow->x, terreno->Superficie(tow->x, tow->z) + 0, tow->z);
		m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);

		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"modelMatrix", modelMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
		m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);

		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);
		m_ModeloShader->PonVec3(m_OpenGL, (char*)"posicionCamara", CamaraPos);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
		m_ModeloShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"spot", 0);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
		m_ModeloShader->Pon1Flotante(m_OpenGL, (char*)"shineness", shineness);

		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"colorText", 102);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"normText", 103);
		m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"specText", 104);
		tow->Render(m_OpenGL);
	}
	
	



#pragma endregion Modelo

#pragma region Agua
	//Agua
	float regresaAagua = ondaAgua();
	distortionFactor = 10;
	m_OpenGL->GetWorldMatrix(modelMatrix);
	m_OpenGL->MatrixTranslation(modelMatrix, 1000, terreno->Superficie(1000, -1000) + 6, -1200);
	m_AwaShader->SetShader(m_OpenGL);
	m_AwaShader->PonMatriz4x4(m_OpenGL, (char*)"worldMatrix", modelMatrix);
	m_AwaShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	m_AwaShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"shaderAgua", 12);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"shaderAgua2", 13);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"shaderAguaAzul", 3);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"shaderAguaDistortion", 4);

	m_AwaShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);

	m_AwaShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
	m_AwaShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
	m_AwaShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
	m_AwaShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
	m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
	m_AwaShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
	m_AwaShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);

	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"atenuar", atenuacionTransicion);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
	m_AwaShader->Pon1Flotante(m_OpenGL, (char*)"distortionFactor", distortionFactor);
	m_AwaShader->Pon1Flotante(m_OpenGL, (char*)"distortion", distortionFactor);
	m_AwaShader->Pon1Flotante(m_OpenGL, (char*)"desplaza", regresaAagua);
	m_AwaShader->PonVec3(m_OpenGL, (char*)"lightColor", lightColor);
	m_AwaShader->PonVec3(m_OpenGL, (char*)"waterColor", waterColor);

	Agua->Render(m_OpenGL);

	//Agua2
	//distortionFactor = 0.9;

	m_OpenGL->GetWorldMatrix(modelMatrix);
	m_OpenGL->MatrixTranslation(modelMatrix, 125, terreno->Superficie(125, -541) + 3, -541);
	m_AwaShader->SetShader(m_OpenGL);
	m_AwaShader->PonMatriz4x4(m_OpenGL, (char*)"worldMatrix", modelMatrix);
	m_AwaShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	m_AwaShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"shaderAgua", 12);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"shaderAgua2", 13);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"shaderAguaAzul", 3);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"shaderAguaDistortion", 4);

	m_AwaShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);

	m_AwaShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
	m_AwaShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
	m_AwaShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
	m_AwaShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);
	m_ModeloShader->PonVec4(m_OpenGL, (char*)"luzEspecular", specularLight);
	m_AwaShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
	m_AwaShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);

	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"atenuar", atenuacionTransicion);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);
	m_AwaShader->Pon1Flotante(m_OpenGL, (char*)"distortionFactor", distortionFactor);
	m_AwaShader->Pon1Flotante(m_OpenGL, (char*)"distortion", distortionFactor);
	m_AwaShader->Pon1Flotante(m_OpenGL, (char*)"desplaza", regresaAagua);
	m_AwaShader->PonVec3(m_OpenGL, (char*)"lightColor", lightColor);
	m_AwaShader->PonVec3(m_OpenGL, (char*)"waterColor", waterColor);

	Agua2->Render(m_OpenGL);

	//Agua3

	//distortionFactor = 0.51;
	m_OpenGL->GetWorldMatrix(modelMatrix);
	m_OpenGL->MatrixRotationZ(rota, radianes(-12));

	m_OpenGL->MatrixScale(escala, 1.9, 1.0, 0.6);
	m_OpenGL->MatrixTranslation(trasla, 297, terreno->Superficie(360, -578) + 18, -550);
	m_OpenGL->MatrixMultiply(modelMatrix, rota, trasla);
	m_OpenGL->MatrixRotationY(rota, radianes(15));
	m_OpenGL->MatrixMultiply(modelMatrix, rota, modelMatrix);
	m_OpenGL->MatrixMultiply(modelMatrix, escala, modelMatrix);
	m_AwaShader->SetShader(m_OpenGL);
	m_AwaShader->PonMatriz4x4(m_OpenGL, (char*)"worldMatrix", modelMatrix);
	m_AwaShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	m_AwaShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"shaderAgua", 12);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"shaderAgua2", 13);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"shaderAguaAzul", 3);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"shaderAguaDistortion", 4);

	m_AwaShader->PonVec3(m_OpenGL, (char*)"posicionLuz", sunPosition);

	m_AwaShader->PonVec4(m_OpenGL, (char*)"difusaAtardecer", luzDifusaAtardecer);
	m_AwaShader->PonVec4(m_OpenGL, (char*)"difusaDia", luzDifusaDia);
	m_AwaShader->PonVec4(m_OpenGL, (char*)"difusaNoche", luzDifusaNoche);
	m_AwaShader->PonVec4(m_OpenGL, (char*)"luzAmbiental", ambientLight);

	m_AwaShader->PonVec4(m_OpenGL, (char*)"frDebil", factdebil);
	m_AwaShader->PonVec4(m_OpenGL, (char*)"frFuerte", factfuerte);
	m_AwaShader->Pon1Flotante(m_OpenGL, (char*)"distortionFactor", distortionFactor);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"atenuarLuz", atenuacionTransicion);
	m_AwaShader->Pon1Entero(m_OpenGL, (char*)"transicion", transicion);

	m_AwaShader->Pon1Flotante(m_OpenGL, (char*)"desplaza", regresaAagua);
	m_AwaShader->PonVec3(m_OpenGL, (char*)"lightColor", lightColor);
	m_AwaShader->PonVec3(m_OpenGL, (char*)"waterColor", waterColor);

	Agua2->Render(m_OpenGL);

#pragma endregion Agua

	// Present the rendered scene to the screen.
	m_OpenGL->EndScene();
	timerhora--;
	return true;
}