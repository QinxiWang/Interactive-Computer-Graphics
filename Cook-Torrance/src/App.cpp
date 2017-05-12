#include "App.h"
#include <iostream>

namespace basicgraphics{

using namespace std;
using namespace glm;

App::App(int argc, char** argv, std::string windowName, int windowWidth, int windowHeight) : BaseApp(argc, argv, windowName, windowWidth, windowHeight) {

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    


    // This load shaders from disk, we do it once when the program starts
    // up, but since you don't need to recompile to reload shaders, you can
    // even do this inteactively as you debug your shaders!  Press the R
    // key to reload them while your program is running!
    reloadShaders();
    
    // This loads the dragon model from a file and initializes an instance of the model class to store it
    modelMesh.reset(new Model("dragon.obj", 1.0, vec4(1.0)));
    
    // Initialize the texture environment map
    // Order should be:
    // +X (right)
    // -X (left)
    // +Y (top)
    // -Y (bottom)
    // +Z (front)
    // -Z (back)
    string textureFiles[] = {"desert_evening_east.jpg", "desert_evening_west.jpg", "desert_evening_up.jpg", "desert_evening_down.jpg", "desert_evening_north.jpg", "desert_evening_south.jpg"};
    environmentMap = Texture::createCubeMapFromFiles(textureFiles, true, 4);
    environmentMap->setTexParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    environmentMap->setTexParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    environmentMap->setTexParameteri(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    environmentMap->setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    environmentMap->setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    skyBox.reset(new Skybox(environmentMap));
    
    turntable.reset(new TurntableManipulator());
    turntable->setCenterPosition(vec3(0,0,0));
    
    // set the light position to roughly match up with the sun position in the skybox texture
    float radius = 10.0;
    lightPosition = vec4(-1.7*radius, 0.3*radius, -1.0*radius, 1.0);
    drawLightVector = false;
    
}
    
void App::reloadShaders()
{
    shader.compileShader("vertex.vert", GLSLShader::VERTEX);
    shader.compileShader("glass.frag", GLSLShader::FRAGMENT);
    shader.link();
    shader.use();
}
    

void App::onEvent(shared_ptr<Event> event)
{
    string name = event->getName();
    
    // Dolly the camera closer or farther away from the earth
    if (name == "kbd_R_down") {
        reloadShaders();
    }
    
    turntable->onEvent(event);
}

void App::onRenderGraphics() {
    // Setup the camera with a good initial position and view direction to see the table
    glm::mat4 view = turntable->frame();
    glm:mat4 model(1.0);
    
    // Setup the projection matrix so that things are rendered in perspective
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)_windowWidth / (GLfloat)_windowHeight, 0.1f, 100.0f);
    
    
    // Tell opengl that we want to use the shader
    shader.use();
    shader.setUniform("view_mat", view);
    shader.setUniform("projection_mat", projection);
    shader.setUniform("model_mat", model);
    
    // Properties of the material the model is made out of (the "K" terms in the equations discussed in class)
    // These values should make the model look like it is made out of a metal, like brass
    vec3 ambientReflectionCoeff(0.7, 0.7, 0.7);
    vec3 diffuseReflectionCoeff(0.7, 0.7, 0.7);
    vec3 specularReflectionCoeff(0.9, 0.9, 0.9);
    float m = 0.55;
    float r0 = 0.7;
    
    // Properties of the light source (the "I" terms in the equations discussed in class)
    // These values are for a white light so the r,g,b intensities are all the same
    vec3 ambientLightIntensity(0.3, 0.3, 0.3);
    vec3 diffuseLightIntensity(0.6, 0.6, 0.6);
    vec3 specularLightIntensity(1.0, 1.0, 1.0);
    
    
    float glassR0 = 0.4;
    float eta = 0.67;
    vec3 glassEta(0.65, 0.67, 0.68);
    
    
    // Pass these parameters into your shader programs... in shader programs these are called "uniform variables"
    
    // TODO: Set shader material properties (k coeffs)
    
    
    
    
    
    
    
    
    // Cook-Torrance specific properties
    shader.setUniform("r0", r0);
    shader.setUniform("m", m);
    
    shader.setUniform("glassR0", glassR0);
    shader.setUniform("eta", eta);
    shader.setUniform("glassEta", glassEta);
    
    // TODO: Set shader light properties (intensity and position)
    shader.setUniform("lightPosition", lightPosition);

    shader.setUniform("ambientReflectionCoeff", ambientReflectionCoeff);
    shader.setUniform("diffuseReflectionCoeff", diffuseReflectionCoeff);
    shader.setUniform("specularReflectionCoeff",specularReflectionCoeff);
    
    shader.setUniform("ambientLightIntensity", ambientLightIntensity);
    shader.setUniform("diffuseLightIntensity", diffuseLightIntensity);
    shader.setUniform("specularLightIntensity",specularLightIntensity);
    
    
    
    
    
    
    
    //This sets the texture to associate with the cubeMap sampler in the shader which is bound to texture unit 1;
    environmentMap->bind(1);
    shader.setUniform("environmentMap", 1);
    
    vec3 eyePosition = turntable->getPos();
    shader.setUniform("eye_world", eyePosition);

    // Draw the model
    modelMesh->draw(shader);
    
	// Draw the skybox. Should be the last thing to draw
    skyBox->draw(view, projection);

}
}//namespace




