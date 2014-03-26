#include "gfx/camera/camera.hpp"

Vec3f Camera::screenSpacePointTo3DRay(
const Mat4x4f &proj, const Mat4x4f &modelview, const Vec2i &resolution, const Vec2i &screen_point, float z) const
{
	float viewport[4] = {
		0, 0, resolution.x, resolution.y
	};

	// calculate inverse of projection x modelview matrices.
	Mat4x4f projXviewInv = (proj * modelview).getInverse();

	// transformation to NDC (normalized device coordinates) [-1, 1]
	Vec4f ndcNormalized(
		 (screen_point.x - viewport[0]) / viewport[2] * 2.0f - 1.0f,
		-(screen_point.y - viewport[1]) / viewport[3] * 2.0f + 1.0f,
		 2.0 * z - 1.0f,
		 1.0f);

	Vec4f res = projXviewInv * ndcNormalized;

	if (fabs(res.w) < EPSILON) {
		return Vec3f(0.0f, 0.0f, 0.0f);
	}

	// to normalize x, y and z values.
	res.w = 1.0 / res.w;

	return Vec3f(res.x * res.w, res.y * res.w, res.z * res.w);
}

Vec3d Camera::collisionPointOnPlane(
const Mat4x4f &proj, const Mat4x4f &modelview, const Vec2i &resolution,
const Vec2i &screen_point, Vec3f planeNormal) const
{
	Vec3f start = screenSpacePointTo3DRay(
		proj, modelview, resolution, screen_point, 0.0);
	Vec3f end = screenSpacePointTo3DRay(
		proj, modelview, resolution, screen_point, 1.0);

	Vec3f line = start - end;
	float len = (-start).dot(planeNormal / line.dot(planeNormal));
	Vec3f i = start + (line * len);
	return Vec3d(i.x, i.y, i.z);
}

Vec3f Camera::rotatePointAroundAxis(
const Vec3f &point, const Vec3f &axis, float theta)
{
	return point * cosf(theta)
	     + (point * (point.dot(axis))) * (1-cosf(theta))
	     + axis.cross(point) * sinf(theta);
}
