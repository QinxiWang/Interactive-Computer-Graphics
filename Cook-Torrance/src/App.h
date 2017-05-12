#ifndef App_h
#define App_h

#include "BaseApp.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Skybox.h"
#include "TurntableManipulator.h"

namespace basicgraphics {
class App : public BaseApp {
public:
  
    App(int argc, char** argv, std::string windowName, int windowWidth, int windowHeight);
    ~App(){};
  
    void onRenderGraphics() override;
    void onEvent(std::shared_ptr<Event> event) override;

  
protected:
    
    virtual void reloadShaders();
    
    std::shared_ptr<Texture> environmentMap;
    std::shared_ptr<Skybox> skyBox;
    
    GLSLProgram shader;
    
    std::unique_ptr<Model> modelMesh;
    std::shared_ptr<TurntableManipulator> turntable;
    
    glm::vec4 lightPosition;
    bool drawLightVector;
  
};
}
#endif
