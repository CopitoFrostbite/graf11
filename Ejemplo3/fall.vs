////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
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
out vec2 texCoord2;
out vec3 normal;
out vec3 tang;
out vec3 binorm;
out vec3 posicion;
out vec3 luzDireccion;
out vec4 calculoEspecular;
out vec4 calculoAmbiente;
out vec4 calculoDifusa;
out vec3 calculoVista;

///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec4 difusaAtardecer;
uniform vec4 difusaDia;
uniform vec4 difusaNoche;
uniform vec4 luzAmbiental;
uniform vec4 luzEspecular;

uniform vec3 posicionLuz;
uniform vec3 posicionCamara;

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
    gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(inputPosition, 1.0f);
    posicion = gl_Position.xyz;

    normal = normalize(mat3(worldMatrix) * inputNormal);
    tang = normalize(mat3(worldMatrix) * inputTangent);
    binorm = normalize(mat3(worldMatrix) * inputBinormal);

    vec4 difusaResultante;
    vec4 factorResultante;

    if (transicion == 0)
    {
        difusaResultante = mix(difusaAtardecer, difusaDia, atenuarLuz);
        factorResultante = mix(frDebil, frFuerte, atenuarLuz);
    }
    else if (transicion == 1)
    {
        difusaResultante = mix(difusaDia, difusaAtardecer, atenuarLuz);
        factorResultante = frFuerte;
    }
    else if (transicion == 2)
    {
        difusaResultante = mix(difusaAtardecer, difusaNoche, atenuarLuz);
        factorResultante = mix(frFuerte, frDebil, atenuarLuz);
    }
    else if (transicion == 3)
    {
        difusaResultante = mix(difusaNoche, difusaAtardecer, atenuarLuz);
        factorResultante = frDebil;
    }

    float factorAmbiental = factorResultante.x;
    float factorDifusa = factorResultante.y;
    float factorEspecular = factorResultante.z;

    // Store the texture coordinates for the pixel shader.
    texCoord = inputTexCoord;
    
    // Define the offset texture coordinates for the second texture set.
    texCoord2 = vec2(inputTexCoord.x, inputTexCoord.y * 0.5 + 0.5);
    
    normal = normalize(normal);
    tang = normalize(tang);
    binorm = normalize(binorm);
    calculoAmbiente = luzAmbiental * factorAmbiental;
    calculoDifusa = difusaResultante * factorDifusa;
    calculoEspecular = luzEspecular * factorEspecular;
    luzDireccion = normalize(-posicionLuz - posicion);
    calculoVista = normalize(posicionCamara - posicion);
}
