#ifndef PD_SPATIALCOMPONENT_HPP
#define PD_SPATIALCOMPONENT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class SpatialComponent final {
	private:
	glm::quat m_orientation;
	glm::vec4 m_position;

	public:
	static const glm::vec4 canonicalForward;
	static const glm::vec4 canonicalUp;
	static const glm::vec4 canonicalLeft;

	SpatialComponent();

	void translate(float longitude, float latitude, float altitude);
	void rotate(float roll, float pitch, float yaw);

	void setOrientation(float deg, float x, float y, float z);
	void setPosition(float x, float y, float z);

	const glm::vec4& position() const;
	const glm::quat& orientation() const;
	const glm::mat4  matrix() const;
};

#endif
