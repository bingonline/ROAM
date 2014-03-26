#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "math/mat4x4.hpp"
#include "math/vec2.hpp"
#include "math/vec3.hpp"

class Camera
{
public:
	/**
	 * On camera motion event.
	 *
	 * @param amount of motion
	 */
	virtual void onCameraMotion(const Vec2f &motion) = 0;

	/**
	 * On camera zoom event.
	 *
	 * @param amount of zoom.
	 */
	virtual void onCameraZoom(float factor) = 0;

	/**
	 * On camera movement event.
	 *
	 * @param amount of movement.
	 */
	virtual void onCameraMovement(const Vec2f &motion) = 0;

	/**
	 * Set camera position and lookot to given x and y point.
	 *
	 * @param position desu
	 */
	virtual void setPosition(const Vec2f &position) = 0;

	virtual Vec3f getPosition() const = 0;

	/**
	 * Force movel view matrix overwrite
	 *
	 * @param matrix desu
	 */
	virtual void forceMatrix(const Mat4x4f &modelViewMatrix) = 0;

	/**
	 * Updates model view for camera.
	 *
	 * This function is called automatically by camera class.
	 */
	virtual void updateModelViewMatrix() = 0;

	/**
	 * Setup the modelview matrix for opengl.
	 */
	virtual void setupScene() const = 0;

	/**
	 * @return modelview matrix
	 */
	virtual const Mat4x4f &getModelViewMatrix() const = 0;

	/**
	 * @return modelview matrix inverse.
	 */
	virtual const Mat4x4f &getModelViewMatrixInverse() const = 0;

	/**
	 * Converts 2d screen space point to 3d ray starting from the
	 * camera position.
	 *
	 * @param  current projection matrix
	 * @param  current modelview matrix
	 * @param  resolution of the screen, e.g. 800 x 600
	 * @param  point on screen
	 * @param  z The window point for the z value.
	 *           between 0 and 1.
	 *           0 meaning the near clipping plane and 1 for the far.
	 *
	 * @return ray
	 */
	Vec3f screenSpacePointTo3DRay(
		const Mat4x4f &proj, const Mat4x4f &modelview, const Vec2i &resolution, 
		const Vec2i &screen_point, float z) const;

	/**
	 * Calculates the collision point of screen space ray on world plane.
	 *
	 * The plane is expeted to have a point on origin and is defined by
	 * its normal.
	 *
	 * @param  current projection matrix
	 * @param  current modelview matrix
	 * @param  resolution of the screen, e.g. 800 x 600
	 * @param  point on screen
	 * @param  plane normal.
	 *
	 * @return plane point.
	 */
	Vec3d collisionPointOnPlane(
		const Mat4x4f &proj, const Mat4x4f &modelview, const Vec2i &resolution, 
		const Vec2i &screen_point, Vec3f planeNormal) const;

	/**
	 * Helper function for camera calcuation.
	 *
	 * Rotates given point around given axis for theta amount (in radians). 
	 *
	 * @param point to rotate
	 * @param axis to rotate around
	 * @param amount to rotate in radians
	 *
	 * @return rotated point.
	 */
	Vec3f rotatePointAroundAxis(
		const Vec3f &point, const Vec3f &axis, float theta);
};

#endif // CAMERA_HPP
