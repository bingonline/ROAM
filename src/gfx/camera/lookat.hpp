#ifndef LOOKAT_HPP
#define LOOKAT_HPP

#include "gfx/camera/camera.hpp"
#include "math/mat4x4.hpp"
#include "math/vec2.hpp"
#include "math/vec3.hpp"

/**
 * Camera class implementation, in which camera looks at some point in space.
 */
class LookAt : public Camera
{
private:
	// lookat point
	Vec3f m_lookat;

	// up vector
	Vec3f m_up;

	// position of the camera
	Vec3f m_position;

	Mat4x4f m_modelViewMatrix;
	Mat4x4f m_modelViewMatrixInverse; // cached.

public:
	LookAt(const Vec3f &position, const Vec3f &lookat, const Vec3f &up);

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

#endif // LOOKAT_HPP
