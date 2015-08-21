#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#define GLEW_STATIC

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <stdexcept>
#include "Window.h"

#include <iostream>
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>

class RenderSystem {
	private:
		SDL_GLContext m_context;
		Window* m_window;
		
		glm::mat4 m_projectionMatrix;
	
	public:
		RenderSystem();
		RenderSystem(const RenderSystem& original);
		~RenderSystem();

		void draw(Scene* scene);
		void resizeWindow(unsigned int width, unsigned int height); //TODO: Window resizing
};

#endif

