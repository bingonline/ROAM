#ifndef FIRST_PERSON_HPP
#define FIRST_PERSON_HPP

#include "gfx/camera/camera.hpp"
#include "math/mat4x4.hpp"
#include "math/vec2.hpp"
#include "math/vec3.hpp"

class FirstPerson : public Camera
{
private:

	// Viewing directions.
	Vec3f m_viewDir;
	Vec3f m_viewUp;
	Vec3f m_viewSide;

	// position of the camera.
	Vec3f m_position;

	Mat4x4f m_modelViewMatrix;
	Mat4x4f m_modelViewMatrixInverse; // cached.

public:
	FirstPerson(const Vec3f &position, const Vec3f &lookat);

	void onCameraMotion(const Vec2f &motion);
	void onCameraZoom(float factor);
	void onCameraMovement(const Vec2f &motion);
	void updateModelViewMatrix();

	void setPosition(const Vec2f &position);
	Vec3f getPosition() const;

	void forceMatrix(const Mat4x4f &modelViewMatrix);

	void setupScene() const;

	const Mat4x4f &getModelViewMatrix() const;
	const Mat4x4f &getModelViewMatrixInverse() const;

};

#endif // FIRST_PERSON_HPP
