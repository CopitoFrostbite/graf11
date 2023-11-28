////////////////////////////////////////////////////////////////////////////////
// Filename: light.vs
////////////////////////////////////////////////////////////////////////////////
#version 400


/////////////////////
// INPUT VARIABLES //
/////////////////////
in vec3 inputPosition;
in vec3 inputNormal;
in vec2 inputTexCoord;
in vec3 inputTangent;
in vec3 inputBinormal;

//////////////////////
// OUTPUT VARIABLES //
//////////////////////
out vec2 texCoord;
out vec3 normal;
out vec3 tang;
out vec3 binorm;
out vec4 calculoAmbiente;
out vec4 calculoDifusa;
out vec3 luzDireccion;

///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec4 difusaAtardecer;
uniform vec4 difusaDia;
uniform vec4 difusaNoche;
uniform vec4 luzAmbiental;

uniform vec3 posicionLuz;

uniform vec4 frDebil;
uniform vec4 frFuerte;

uniform float atenuarLuz;
uniform float transicion;

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	// Calculate the position of the vertex against the world, view, and projection matrices.
	gl_Position = modelMatrix * vec4(inputPosition, 1.0f);
	gl_Position = viewMatrix * gl_Position;
	gl_Position = projectionMatrix * gl_Position;

	// Store the texture coordinates for the pixel shader.
	texCoord = inputTexCoord;
	
	// Calculate the normal vector against the world matrix only.
	normal = vec3(modelMatrix * vec4(inputNormal,0));
	vec3 tangente = vec3(modelMatrix * vec4(inputTangent,0));
	vec3 binormal = vec3(modelMatrix * vec4(inputBinormal,0));

	vec4 difusaResultante;
	vec4 factorResultante;

	if (transicion == 0)
	{
		difusaResultante = mix(difusaAtardecer, difusaDia, atenuarLuz);
		factorResultante = mix(frDebil, frFuerte, atenuarLuz);
	}
	else if (transicion == 1)
	{
		difusaResultante = mix (difusaDia, difusaAtardecer, atenuarLuz);
		factorResultante = frFuerte;
	}
	else if (transicion == 2)
	{
		difusaResultante = mix (difusaAtardecer, difusaNoche, atenuarLuz);
		factorResultante = mix (frFuerte, frDebil, atenuarLuz);
	}
	else if (transicion == 3)
	{
		difusaResultante = mix (difusaNoche, difusaAtardecer, atenuarLuz);
		factorResultante = frDebil;
	}

	float factorAmbiental = factorResultante.x;
	float factorDifusa = factorResultante.y;

	// Normalize the normal vector.
	
	texCoord = inputTexCoord;
	normal = normalize(normal);
	tang = normalize(tangente);
	binorm = normalize(binormal);	
	calculoAmbiente = (luzAmbiental*factorAmbiental);
	calculoDifusa = (difusaResultante*factorDifusa);
	luzDireccion = normalize(-posicionLuz);
}