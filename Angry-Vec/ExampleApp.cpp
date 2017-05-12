#include "ExampleApp.h"

using namespace glm;
using namespace std;

namespace basicgraphics {
    const double ExampleApp::GRAVITY = 9.8;
    
	ExampleApp::ExampleApp(int argc, char** argv, std::string windowName, int windowWidth, int windowHeight) : BaseApp(argc, argv, windowName, windowWidth, windowHeight)
	{
        time = 0;
        
        position = vec3(0,0,0);
        previousPosition = vec3(0,0,0);
        velocity = vec3(0,0,0);
        
        
        initSpeed = 22;
        initAngle = radians(45.0);
        initialVel = vec2(initSpeed*cos(initAngle), initSpeed*sin(initAngle));
        totalTime = 2.0 * initialVel.y/GRAVITY + 2;
        
        // Change the background color
        glClearColor(0.2f, 0.6f, 1.0f, 1.0f);
        
        arrowShaft.reset(new Cylinder(vec3(0, 0, 0), vec3(1, 0, 0), 0.125, vec4(1.0, 0.647, 0, 1.0)));
        
        arrowPoint.reset(new Cone(vec3(1, 0, 0), vec3(1.5, 0, 0), 0.5, vec4(1.0, 0.647, 0, 1.0)));
        
        ground.reset(new Box(vec3(-40, -2, 3), vec3(40, -4, -100), vec4(0.3, 0.9, 0.4, 1.0)));
        
        target1.reset(new Box(vec3(20, -2, -30), vec3(23, 10, -40), vec4(0.6, 0.4, 0.2, 1.0)));
        
        target2.reset(new Box(vec3(25, -2, -30), vec3(28, 10, -40), vec4(0.6, 0.4, 0.2,1.0)));
        
        target3.reset(new Box(vec3(22, 10, -30), vec3(26, 20, -40), vec4(0.6, 0.4, 0.2, 1.0)));

        
        launcher.reset(new Line(vec3(-30, -2, -35), vec3(-30, 5, -35), vec3(0,0,1), 0.5, vec4(0,0,0,1)));

        bird.reset(new Sphere(position, 1.0, vec4(1,0,0,1)));
        
	}

	ExampleApp::~ExampleApp() {}
    
    void ExampleApp::onSimulation(double rdt){
        time += rdt*0.25;
        if (time > totalTime){
            time = 0;
        }
        
        previousPosition = position;
        position = birdPos(time);
        velocity = (position - previousPosition) / (float)rdt;
    }
    
    vec3 ExampleApp::birdPos(double time) {
        double e = time;// / 4.0;
        
        return vec3(e*initialVel.x - 30, initialVel.y*e - GRAVITY*e*e*0.5 + 5, -35);
        //return vec3(e*52-30,30*(-e*e+1.2*e)+5,-35);
    }

	void ExampleApp::onRenderGraphics() {
        
        double curTime = glfwGetTime();
        onSimulation(curTime - lastTime);
        lastTime = curTime;
        
        
        
		// Setup the view matrix to set where the camera is located
		glm::vec3 eye_world = glm::vec3(0,10,50);
		glm::mat4 view = glm::lookAt(eye_world, glm::vec3(0,0,0), glm::vec3(0,1,0));
		
		// Setup the projection matrix so that things are rendered in perspective
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)_windowWidth / (GLfloat)_windowHeight, 0.01f, 100.0f);
		// Setup the model matrix
		glm::mat4 model = glm::mat4(1.0);

		// Update shader variables
		_shader.setUniform("view_mat", view);
		_shader.setUniform("projection_mat", projection);
		_shader.setUniform("model_mat", model);
		_shader.setUniform("eye_world", eye_world);

        ground->draw(_shader, model);
        target1->draw(_shader, model);
        target2->draw(_shader, model);
        target3->draw(_shader, model);
        launcher->draw(_shader, model);
        
        // TODO: make sure bird velocity makes sense. (hint, hint)
        Line velocityLine(position, position + velocity, vec3(0,0,1), 0.25, vec4(1.0, 0.65, 0, 1.0));
        velocityLine.draw(_shader, model);
        
        //drawArrow(position, velocity);
        
        mat4 translation = translate(mat4(1.0), position);
        bird->draw(_shader, translation * model);
        
        
        
	}
    
    void ExampleApp::drawArrow(vec3 position, vec3 direction){
        float dotVal = dot(normalize(direction), vec3(1,0,0));
        float angle = glm::acos(dotVal);
        if (direction.y < 0){
            angle = -angle;
        }
        
        mat4 scale = glm::scale(mat4(1.0), vec3(length(direction), 1, 1));
        mat4 rotation = toMat4(angleAxis(angle, vec3(0,0,1)));
        mat4 translation = translate(mat4(1.0), position);
        mat4 transform = translation * rotation * scale;
        arrowShaft->draw(_shader, transform);
        arrowPoint->draw(_shader, transform);
    }

	void ExampleApp::onEvent(shared_ptr<Event> event) {
		string name = event->getName();
		if (name == "kbd_ESC_down") {
			glfwSetWindowShouldClose(_window, 1);
		}
		else {
			cout << name << endl;
		}
	}
}