#define LIGHTING
#define BUMPED

// Inputs
attribute vec4 a_position;									// Vertex Position							(x, y, z, w)
attribute vec3 a_normal;									// Vertex Normal							(x, y, z)
attribute vec2 a_texCoord;									// Vertex Texture Coordinate				(u, v)
attribute vec3 a_tangent;									// Vertex Tangent							(x, y, z)
attribute vec3 a_binormal;									// Vertex Binormal/Bitangent				(x, y, z)
#if defined(SKINNING)
attribute vec4 a_blendWeights;								// Vertex blend weight, up to 4				(0, 1, 2, 3) 
attribute vec4 a_blendIndices;								// Vertex blend index int u_matrixPalette	(0, 1, 2, 3)
#endif

// Uniforms
uniform mat4 u_worldViewProjectionMatrix;					// Matrix to transform a position to clip space
uniform mat4 u_inverseTransposeWorldViewMatrix;				// Matrix to transform a normal to view space
#if defined(SPECULAR) || defined(SPOT_LIGHT) || defined(POINT_LIGHT)
uniform mat4 u_worldViewMatrix;								// Matrix to tranform a position to view space
uniform mat4 u_worldMatrix;								    // Matrix to tranform a position to world space
#endif
#if defined(SKINNING)
uniform vec4 u_matrixPalette[SKINNING_JOINT_COUNT * 3];		// Array of 4x3 matrices
#endif
#if defined(SPECULAR)
uniform vec3 u_cameraPosition;                 				// Position of the camera in view space
#endif
#if defined(TEXTURE_REPEAT)
uniform vec2 u_textureRepeat;
#endif
#if defined(TEXTURE_OFFSET)
uniform vec2 u_textureOffset;
#endif
#if defined(POINT_LIGHT)
uniform vec3 u_pointLightPosition;							// Position of light
uniform float u_pointLightRangeInverse;						// Inverse of light range
#elif defined(SPOT_LIGHT)
uniform vec3 u_spotLightPosition;							// Position of light
uniform float u_spotLightRangeInverse;						// Inverse of light range
uniform vec3 u_spotLightDirection;							// Direction of light
#else
uniform vec3 u_lightDirection;								// Direction of light
#endif

//cesar uniforms
//cesar: corner of the active tile in the animated map
uniform vec2 u_normalmapOffset;
//cesar: map down the uvcoord from diffuse to normal map tiles
uniform vec2 u_normalmapFactor;


// Varyings
varying vec3 v_normalVector;								// Normal vector in view space
varying vec2 v_texCoord;									// Texture Coordinate
#if defined(SPECULAR)
varying vec3 v_cameraDirection;								// Direction the camera is looking at in tangent space
#endif
// Lighting
#if defined(POINT_LIGHT)
varying vec3 v_vertexToPointLightDirection;					// Direction of point light w.r.t current vertex in tangent space
varying float v_pointLightAttenuation;						// Attenuation of point light
#include "lighting-point.vert"
#elif defined(SPOT_LIGHT)
varying vec3 v_vertexToSpotLightDirection;					// Direction of the spot light w.r.t current vertex in tangent space
varying float v_spotLightAttenuation;						// Attenuation of spot light
varying vec3 v_spotLightDirection;							// Direction of spot light in tangent space
#include "lighting-spot.vert"
#else
varying vec3 v_lightDirection;								// Direction of light
#include "lighting-directional.vert"
#endif

// Skinning
#if defined(SKINNING)
#include "skinning.vert"
#else
#include "skinning-none.vert" 
#endif

//cesar varyings
//cesar: corner of the active tile in the animated map
varying vec2 v_normalmapOffset;
//cesar: map down the uvcoord from diffuse to normal map tiles
varying vec2 v_normalmapFactor;



void main()
{
    // Get the position, normal, tangents and binormals.
    vec4 position = getPosition();
    vec3 normal = getNormal();
    vec3 tangent = getTangent();
    vec3 binormal = getBinormal();
    
    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * position;

    // Transform the normal, tangent and binormals to view space.
	mat3 inverseTransposeWorldViewMatrix = mat3(u_inverseTransposeWorldViewMatrix[0].xyz, u_inverseTransposeWorldViewMatrix[1].xyz, u_inverseTransposeWorldViewMatrix[2].xyz);
    vec3 normalVector = normalize(inverseTransposeWorldViewMatrix * normal);
    
    // Create a transform to convert a vector to tangent space.
    vec3 tangentVector  = normalize(inverseTransposeWorldViewMatrix * tangent);
    vec3 binormalVector = normalize(inverseTransposeWorldViewMatrix * binormal);
    mat3 tangentSpaceTransformMatrix = mat3(tangentVector.x, binormalVector.x, normalVector.x, tangentVector.y, binormalVector.y, normalVector.y, tangentVector.z, binormalVector.z, normalVector.z);
    
    // Apply light.
    applyLight(tangentSpaceTransformMatrix);
    
    // Texture transformation.
    v_texCoord = a_texCoord;
    #if defined(TEXTURE_REPEAT)
    v_texCoord *= u_textureRepeat;
    #endif
    #if defined(TEXTURE_OFFSET)
    v_texCoord += u_textureOffset;
    #endif
    
    //cesar
    v_normalmapOffset  = u_normalmapOffset;
    v_normalmapFactor = u_normalmapFactor;

}