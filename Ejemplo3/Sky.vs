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
out vec3 posicion;
out vec3 luzDireccion;
out vec3 posicionLuzSol;
out vec3 cameraPositionRelative;

///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec4 difusaDia;
uniform vec4 luzAmbiental;
uniform vec3 posicionLuz;
uniform vec4 frDebil;
uniform vec4 frFuerte;

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
    // Calculate the position of the vertex against the world, view, and projection matrices.
    gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(inputPosition, 1.0);

    // Store the texture coordinates for the pixel shader.
    texCoord = inputTexCoord;

    // Calculate the fragment normal using tangent and binormal.
    normal = normalize(mat3(worldMatrix) * inputNormal);
    tang = normalize(mat3(worldMatrix) * inputTangent);
    binorm = normalize(mat3(worldMatrix) * inputBinormal);

    // Pass additional variables for lighting calculations
    posicion = vec3(worldMatrix * vec4(inputPosition, 1.0));
    luzDireccion = normalize(posicionLuz - posicion);
    cameraPositionRelative = vec3(viewMatrix * vec4(0.0, 0.0, 0.0, 1.0)) - posicion;
}
