#ifndef PD_RENDERER_HPP
#define PD_RENDERER_HPP

#define GLFW_INCLUDE_NONE

#include "Geometry.hpp"
#include "RenderComponent.hpp"
#include "ResourceCache.hpp"
#include "ShaderProgram.hpp"
#include <glm/glm.hpp>
#include <globjects/globjects.h>
#include <memory>
#include <queue>

// Forward declarations ------------------------------------------------------
class Window;
struct Light;
// ---------------------------------------------------------------------------

// RenderTask collects all the information required to render a game entity.
struct RenderTask {
	RenderComponent    keys;
	int                id;
	glm::mat4          model;
	glm::mat4          view;
	glm::mat4          projection;
	glm::vec4          eye;
	float              ambience;
	std::vector<Light> lights;
};

class Renderer {
	private:
	std::unique_ptr<globjects::Framebuffer>  m_frameBuffer;
	std::unique_ptr<globjects::Renderbuffer> m_colorAttachment;
	std::unique_ptr<globjects::Renderbuffer> m_selectionAttachment;
	std::unique_ptr<globjects::Renderbuffer> m_depthStencilAttachment;

	ResourceCache<globjects::Texture> m_textureCache;
	ResourceCache<Geometry>           m_geometryCache;
	ResourceCache<ShaderProgram>      m_shaderCache;

	std::queue<RenderTask> m_queue;

	int m_height;
	int m_width;

	void init();
	void resize(int width, int height);
	void clear();

	std::unique_ptr<globjects::Texture> loadTexture(const std::string& name);

	public:
	Renderer();

	void queue(RenderTask task);
	void draw();
};

#endif
