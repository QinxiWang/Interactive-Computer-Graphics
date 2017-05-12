#version 330

// Fragment shader


// These get passed in from the vertex shader and are interpolated (varying) properties,
// so they change for each pixel across the triangle:
in vec4 interpSurfPosition; // position on the surface in world coordinates
in vec3 interpSurfNormal; // normal in world coordinates

//TODO: add a uniform variable for the light position
uniform vec4 lightPosition; // in world coordinates

//Textures
uniform samplerCube environmentMap; // The cube map containing the environment to reflect


//Material Properties
uniform float r0; // The Fresnel reflectivity when the incident angle is 0
uniform float m; // The roughness of the material

// TODO: add other material properties variables
uniform vec3 ambientReflectionCoeff;
uniform vec3 diffuseReflectionCoeff;
uniform vec3 specularReflectionCoeff;


// TODO: Light Properties
uniform vec3 ambientLightIntensity;
uniform vec3 diffuseLightIntensity;
uniform vec3 specularLightIntensity;

// The camera location in world coordinates
uniform vec3 eye_world;

// This is an out variable for the final color we want to render this fragment.
out vec4 fragColor;

void main() {

    // Start with black and then add lighting to the final color as we calculate it
	vec3 materialColor = vec3(0.0, 0.0, 0.0);
    
    //dir vec
    vec3 lightDir = normalize(lightPosition.xyz - interpSurfPosition.xyz);
    vec3 normal = normalize(interpSurfNormal);
    vec3 viewDir = normalize(eye_world - interpSurfNormal);
    vec3 H = normalize(lightDir + viewDir);
    
    //clamp
    float dotHnor = max(dot(H,normal), 0.0);
    float dotLnor = max(dot(normal, lightDir), 0.0);
    float dotHview = max(dot(H, viewDir), 0.0);
    float dotNview = max(dot(normal, viewDir), 0.0);
    
    //torrance para
    float D = exp(-pow(tan(acos(dotHnor)),2) / pow(m,2)) / (4 * pow(m,2) * pow(dotHnor, 4));
    float G = min(1.0, min(2 * dotHnor * dotNview / dotHview, 2 * dotHnor * dotLnor / dotHview));
    float F = r0 + (1-r0) * pow(1 - dotLnor, 5);
    
    // Ambient
    vec3 ambient = ambientReflectionCoeff * ambientLightIntensity;
    
    // Diffuse
    vec3 diffuse = diffuseReflectionCoeff * diffuseLightIntensity * dotLnor;
    
    // Specular
    vec3 specular = specularReflectionCoeff * specularLightIntensity * (D*F*G)/dotNview;

	// TODO: Tell OpenGL to use the r,g,b compenents of materialColor mixed with the reflected color for the color of this fragment (pixel).
    materialColor = ambient + diffuse + specular;
    fragColor.rgb = mix(materialColor, texture(environmentMap, reflect(-viewDir, normal)).rgb, F);
    
	// And, set the alpha component to 1.0 (completely opaque, no transparency).
	fragColor.a = 1.0;
}
