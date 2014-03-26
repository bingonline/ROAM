#include "gfx/camera/lookat.hpp"

#include <GL/gl.h>
#include <assert.h>

#include <iostream>

LookAt::LookAt(const Vec3f &position, const Vec3f &lookat, const Vec3f &up)
	: m_position(position)
	, m_lookat(lookat)
	, m_up(up)
{
	updateModelViewMatrix();
}

void LookAt::onCameraMotion(const Vec2f &motion)
{
	Vec3f pos = m_position - m_lookat;
	Vec3f cross(pos.cross(m_up));
	cross.normalize();
	const float length = pos.length();

	// horizontal movement.
	pos = rotatePointAroundAxis(pos, cross, motion.y);
	m_up = rotatePointAroundAxis(m_up, cross, motion.y);
	m_up.normalize();

	// vertical movement.
	pos = rotatePointAroundAxis(pos, m_up, motion.x);

	m_position = pos + m_lookat;

	onCameraZoom(length / pos.length());

	// make sure that dot product is very close to zero.
	assert(fabs(pos.dot(m_up)) < 0.01f);

	updateModelViewMatrix();
}

void LookAt::onCameraZoom(float factor)
{
	Vec3f look_to_eye = -m_lookat + m_position;
	look_to_eye *= factor;
	m_position = m_lookat + look_to_eye;

	updateModelViewMatrix();
}

void LookAt::onCameraMovement(const Vec2f &motion)
{
	m_position += Vec3f(motion.x, motion.y, 0);
	m_lookat   += Vec3f(motion.x, motion.y, 0);

	updateModelViewMatrix();
}

void LookAt::setPosition(const Vec2f &position)
{
	m_position = Vec3f(position.x, position.y, m_position.z);
	m_lookat   = Vec3f(position.x, position.y, m_lookat.z);

	updateModelViewMatrix();
}

Vec3f LookAt::getPosition() const
{
	return m_position;
}

void LookAt::forceMatrix(const Mat4x4f &modelViewMatrix)
{
	m_modelViewMatrix = modelViewMatrix;
}

void LookAt::updateModelViewMatrix()
{
	Vec3f view_dir = m_lookat - m_position;
	Vec3f view_side;
	Vec3f view_up;

	view_dir.normalize();
	view_up = m_up - m_up.dot(view_dir)*view_dir;
	view_side = view_dir.cross(view_up);

	m_modelViewMatrix.set(
		 view_side.x,  view_side.y,  view_side.z, 0,
		 view_up.x  ,  view_up.y  ,  view_up.z  , 0,
		-view_dir.x , -view_dir.y , -view_dir.z , 0,
		 0          ,  0          ,  0          , 1);

	Vec4f translation(m_position.x, m_position.y, m_position.z, 1);
	Vec4f res = m_modelViewMatrix * translation;

	m_modelViewMatrix.m[12] = -res.x;
	m_modelViewMatrix.m[13] = -res.y;
	m_modelViewMatrix.m[14] = -res.z;

	m_modelViewMatrixInverse = m_modelViewMatrix.getInverse();
}

void LookAt::setupScene() const
{
	//glMultMatrixf(m_modelViewMatrix.m);
}

const Mat4x4f &LookAt::getModelViewMatrix() const
{
	return m_modelViewMatrix;
}

const Mat4x4f &LookAt::getModelViewMatrixInverse() const
{
	return m_modelViewMatrixInverse;
}
