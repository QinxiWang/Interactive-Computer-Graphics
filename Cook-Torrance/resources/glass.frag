#version 330

// Fragment shader


// These get passed in from the vertex shader and are interpolated (varying) properties,
// so they change for each pixel across the triangle:
in vec4 interpSurfPosition;
in vec3 interpSurfNormal;

// Light Position in world coordinates
uniform vec4 lightPosition;

//Textures
uniform samplerCube environmentMap;

// Material Properties
uniform float eta; // The eta value to use initially. This reflects all light wavelengths in the same direction
uniform vec3 glassEta; // Contains one eta for each color channel (use eta.r, eta.g, eta.b in your code)
uniform float glassR0; // The Fresnel reflectivity when the incident angle is 0

uniform vec3 eye_world;

// This is an out variable for the final color we want to render this fragment.
out vec4 fragColor;

void main() {

	// dir vectors
    vec3 normal = normalize(interpSurfNormal);
    vec3 lightDir = normalize(lightPosition.xyz - interpSurfPosition.xyz);
    vec3 viewDir = normalize(eye_world - interpSurfPosition.xyz);
    
    //clampping
    float dotLnor = max(dot(normal, lightDir), 0.0);
    float dotNview = max(dot(normal, viewDir), 0.0);
    
    //tex mapping
//    vec3 refractTexCoord = normalize(refract(-viewDir, normal, eta));
//    vec4 refractedColor = texture(environmentMap, refractTexCoord);
    
    //refraction vector by rgb
    vec3 refracted_r = normalize(refract(-viewDir, normal, glassEta.r));
    vec3 refracted_g = normalize(refract(-viewDir, normal, glassEta.g));
    vec3 refracted_b = normalize(refract(-viewDir, normal, glassEta.b));
    
    float SchlickApprox = glassR0 + (1-glassR0) * pow(1 - dotLnor, 5);
    
    vec4 reflectedColor = texture(environmentMap, reflect(-viewDir, normal).rgb);
    vec3 refractedColor_rgb = vec3(texture(environmentMap, refracted_r).r,
                                   texture(environmentMap, refracted_g).g,
                                   texture(environmentMap, refracted_b).b);
    
    //blend reflected and refracted color
    fragColor.rgb = mix(reflectedColor.rgb, refractedColor_rgb, SchlickApprox);
    
	// Tell OpenGL to use the mix of the refracted and reflected color based on the fresnel term.
    
	// And, set the alpha component to 1.0 (completely opaque, no transparency).
	fragColor.a = 1.0;
}
