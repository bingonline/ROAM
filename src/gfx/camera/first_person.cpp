#include "gfx/camera/first_person.hpp"

#include <GL/gl.h>

#include <assert.h>
#include <iostream>

FirstPerson::FirstPerson(const Vec3f &position, const Vec3f &lookat)
	: m_position(position)
	, m_viewDir(lookat - position)
{
	m_viewDir.normalize();

	m_viewUp = Vec3f(0, 1, 0) - Vec3f(0, 1, 0).dot(m_viewDir) * m_viewDir;
	m_viewUp.normalize();

	m_viewSide = m_viewDir.cross(m_viewUp);

	updateModelViewMatrix();
}

void FirstPerson::onCameraMotion(const Vec2f &motion)
{
	// horizontal movement.
	Vec3f viewDir = rotatePointAroundAxis(m_viewDir, m_viewSide, motion.y);
	Vec3f viewUp  = rotatePointAroundAxis(m_viewUp,  m_viewSide, motion.y);

	// vertical movement.
	viewDir = rotatePointAroundAxis(viewDir, viewUp, motion.x);

	m_viewDir = viewDir;
	m_viewUp  = viewUp;

	m_viewDir.normalize();
	m_viewUp.normalize();

	m_viewSide = m_viewDir.cross(m_viewUp);

	// make sure that dot product is very close to zero.
	assert(fabs(m_viewDir.dot(m_viewUp)) < 0.01f);

	updateModelViewMatrix();
}

void FirstPerson::onCameraZoom(float factor)
{
}

void FirstPerson::onCameraMovement(const Vec2f &motion)
{
	m_position += m_viewDir * motion.y;
	m_position += m_viewSide * motion.x;

	updateModelViewMatrix();
}

void FirstPerson::updateModelViewMatrix()
{
	m_modelViewMatrix.set(
		 m_viewSide.x,  m_viewSide.y,  m_viewSide.z, 0,
		 m_viewUp.x  ,  m_viewUp.y  ,  m_viewUp.z  , 0,
		-m_viewDir.x , -m_viewDir.y , -m_viewDir.z , 0,
		 0           ,  0           ,  0           , 1);

	Vec4f translation(m_position.x, m_position.y, m_position.z, 1);
	Vec4f res = m_modelViewMatrix * translation;

	m_modelViewMatrix.m[12] = -res.x;
	m_modelViewMatrix.m[13] = -res.y;
	m_modelViewMatrix.m[14] = -res.z;

	m_modelViewMatrixInverse = m_modelViewMatrix.getInverse();
}

void FirstPerson::setPosition(const Vec2f &position)
{
	updateModelViewMatrix();
}

Vec3f FirstPerson::getPosition() const
{
	return m_position;
}

void FirstPerson::forceMatrix(const Mat4x4f &modelViewMatrix)
{
	m_viewSide.x = modelViewMatrix.m[0];
	m_viewSide.y = modelViewMatrix.m[4];
	m_viewSide.z = modelViewMatrix.m[8];
	m_viewUp.x = modelViewMatrix.m[1];
	m_viewUp.y = modelViewMatrix.m[5];
	m_viewUp.z = modelViewMatrix.m[9];
	m_viewDir.x = -modelViewMatrix.m[2];
	m_viewDir.y = -modelViewMatrix.m[6];
	m_viewDir.z = -modelViewMatrix.m[10];

	Vec4f tmp(modelViewMatrix.m[12], modelViewMatrix.m[13], modelViewMatrix.m[14], 1);
	Vec4f res = tmp * modelViewMatrix;

	m_position.x = -res.x;
	m_position.y = -res.y;
	m_position.z = -res.z;

	m_modelViewMatrix = modelViewMatrix;
}

void FirstPerson::setupScene() const
{
	glMultMatrixf(m_modelViewMatrix.m);
}

const Mat4x4f &FirstPerson::getModelViewMatrix() const
{
	return m_modelViewMatrix;
}

const Mat4x4f &FirstPerson::getModelViewMatrixInverse() const
{
	return m_modelViewMatrixInverse;
}
