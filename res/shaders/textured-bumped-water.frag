#define LIGHTING
#define BUMPED

#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform sampler2D u_diffuseTexture;        		// Diffuse map texture
uniform sampler2D u_normalmapTexture;       	// Normalmap texture
uniform vec3 u_ambientColor;                    // Ambient color
uniform vec3 u_lightColor;                      // Light color
uniform vec3 u_lightDirection;					// Light direction
#if defined(SPECULAR)
uniform float u_specularExponent;				// Specular exponent.
#endif
#if defined (SPOT_LIGHT)
uniform float u_spotLightInnerAngleCos;			// The bright spot [0.0 - 1.0]
uniform float u_spotLightOuterAngleCos;			// The soft outer part [0.0 - 1.0]
#endif
#if defined(MODULATE_COLOR)
uniform vec4 u_modulateColor;					// Modulation color
#endif
#if defined(MODULATE_ALPHA)
uniform float u_modulateAlpha;					// Modulation alpha
#endif

// Varyings
varying vec3 v_normalVector;					// Normal vector in view space
varying vec2 v_texCoord;						// Texture Coordinate
#if defined(POINT_LIGHT)
varying vec3 v_vertexToPointLightDirection;		// Light direction w.r.t current vertex in tangent space.
varying float v_pointLightAttenuation;			// Attenuation of point light.
#elif defined(SPOT_LIGHT)
varying vec3 v_spotLightDirection;				// Direction of spot light in tangent space.
varying vec3 v_vertexToSpotLightDirection;		// Direction of the spot light w.r.t current vertex in tangent space.
varying float v_spotLightAttenuation;			// Attenuation of spot light.
#else
varying vec3 v_lightDirection;					// Direction of light in tangent space.
#endif
#if defined(SPECULAR)
varying vec3 v_cameraDirection;                 // Camera direction
#endif

// Lighting
#include "lighting.frag"
#if defined(POINT_LIGHT)
#include "lighting-point.frag"
#elif defined(SPOT_LIGHT)
#include "lighting-spot.frag"
#else
#include "lighting-directional.frag"
#endif

//cesar varyings
//cesar: corner of the active tile in the animated map
varying vec2 v_normalmapOffset;
//cesar: map down the uvcoord from diffuse to normal map tiles
varying vec2 v_normalmapFactor;


void main()
{
    // Fetch diffuse color from texture.
    _baseColor = texture2D(u_diffuseTexture, v_texCoord);

    // Light the pixel
    gl_FragColor.a = _baseColor.a;
    #if defined(TEXTURE_DISCARD_ALPHA)
    if (gl_FragColor.a < 0.5)
        discard;
    #endif
    
    
    //cesar: original: gl_FragColor.rgb = getLitPixel();
    //cesar new: este es el codigo que estaba dentro de getLitPixel..
        // Fetch normals from the normal map
    vec3 normalVector = normalize(texture2D(u_normalmapTexture, v_normalmapOffset + (v_texCoord*v_normalmapFactor)).rgb * 2.0 - 1.0);
    vec3 lightDirection = normalize(v_lightDirection);
    vec3 cameraDirection = normalize(v_cameraDirection);
    gl_FragColor.rgb = computeLighting(normalVector, -lightDirection, 1.0, cameraDirection);

    

    // Global color modulation
    #if defined(MODULATE_COLOR)
    gl_FragColor *= u_modulateColor;
    #endif
    #if defined(MODULATE_ALPHA)
    gl_FragColor.a *= u_modulateAlpha;
    #endif
}
