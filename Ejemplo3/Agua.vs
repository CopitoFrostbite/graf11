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
out vec3 posicion;
out vec3 luzDireccion;
out vec3 cameraPositionRelative;
out vec4 calculoAmbiente;
out vec4 calculoDifusa;
out vec4 calculoEspecular;
///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec4 difusaDia;
uniform vec4 difusaAtardecer;
uniform vec4 difusaNoche;
uniform vec4 luzAmbiental;
uniform vec3 posicionLuz;

uniform vec4 frDebil;
uniform vec4 frFuerte;
uniform vec4 luzEspecular;
uniform float atenuarLuz;
uniform float distortionFactor;
uniform float transicion;
uniform float desplaza;

uniform sampler2D shaderAgua;
uniform sampler2D shaderAgua2;
uniform sampler2D shaderAguaDistortion;
uniform sampler2D shaderAguaAzul;

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
    // Posicion vertices
    vec4 alturaDesplazada = vec4(inputPosition, 1.0);
    vec4 difusaResultante;
    vec4 factorResultante;

    // Calculate diffuse color
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
	float factorEspecular = factorResultante.z;
    // Calculate the displacement based on the normal map
    vec3 normalMap = texture(shaderAgua2, inputTexCoord).rgb * 2.0 - 1.0;
    float displacementFactor = texture(shaderAguaDistortion, inputTexCoord).r;

    // Apply the displacement in the Y axis with smoothing
    float smoothedDisplacement = smoothstep(0.1, 0.7, displacementFactor) * desplaza;
    vec3 displacement = vec3(0.0, smoothedDisplacement, 0.0) * normalMap;

    // Apply the displacement in all three axes
    alturaDesplazada.xyz += displacement;

    gl_Position = projectionMatrix * viewMatrix * worldMatrix * alturaDesplazada;
    posicion = gl_Position.xyz;

    // Store the texture coordinates for the fragment shader.
    texCoord = inputTexCoord;

    // Calculate the normal vector against the world matrix only.
    normal = normalize(vec3(worldMatrix * vec4(inputNormal, 0.0)));
    tang = normalize(vec3(worldMatrix * vec4(inputTangent, 0.0)));
    binorm = normalize(vec3(worldMatrix * vec4(inputBinormal, 0.0)));
    calculoAmbiente = (luzAmbiental*factorAmbiental);
	calculoDifusa = (difusaResultante*factorDifusa);
	calculoEspecular = (luzEspecular*factorEspecular);
    luzDireccion = normalize(-posicionLuz - posicion);

    // Pass the camera position relative to the water to the fragment shader
    vec3 cameraPosition = vec3(inverse(viewMatrix)[3].xyz);
    vec3 cameraPositionRelative = cameraPosition - posicion;
    cameraPositionRelative = cameraPositionRelative;
}





