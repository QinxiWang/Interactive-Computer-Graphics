
#include "BaseApp.h"

namespace basicgraphics {

	class ExampleApp : public BaseApp {
		public:
			ExampleApp(int argc, char** argv, std::string windowName, int windowWidth, int windowHeight);
			~ExampleApp();

		private:
            // Returns the bird location at a specific time
            virtual glm::vec3 birdPos(double time);
        
            std::unique_ptr<Box> ground;
            std::unique_ptr<Box> target1;
            std::unique_ptr<Box> target2;
            std::unique_ptr<Box> target3;
            std::unique_ptr<Line> launcher;
            std::unique_ptr<Sphere> bird;
        
        
            static const double GRAVITY;
        
            double lastTime; // Time value for the last time onSimulation was called.
        
            double time;
            
            glm::vec3 position;
            glm::vec3 previousPosition;
            glm::vec3 velocity;
            
            double initSpeed;
            double initAngle;
            glm::vec2 initialVel;
            double totalTime;

            void onSimulation(double rdt);
			void onRenderGraphics() override;
			void onEvent(std::shared_ptr<Event> event) override;
        
            std::unique_ptr<Cylinder> arrowShaft;
            std::unique_ptr<Cone> arrowPoint;
            void drawArrow(glm::vec3 position, glm::vec3 direction);
	};
}