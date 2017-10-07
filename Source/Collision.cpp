#include "Collision.h"

#include "Vec2D.h"
#include "GameObject.h"

double GetIntersectionDist(AXIS axis, int Norm, double planePos, GameObject& circle_obj, double circle_radius)
{
	const double& position = (axis == XAXIS) ? circle_obj.GetPos().x : circle_obj.GetPos().y;

	return Norm*(position - planePos) - circle_radius;
}

bool CheckForCircleAxisIntersection(AXIS axis, int Norm, double planePos, GameObject& circle_obj, double circle_radius)
{
	const double& velocity = (axis == XAXIS) ? circle_obj.GetVel().x : circle_obj.GetVel().y;

	double dist = GetIntersectionDist(axis, Norm, planePos, circle_obj, circle_radius);

	return (dist < 0.0 && Norm*velocity < 0.0);
}

bool CheckForCircleAxisCollision(AXIS axis, int Norm, double planePos, GameObject& circle_obj, double circle_radius)
{
	if (CheckForCircleAxisIntersection(axis, Norm, planePos, circle_obj, circle_radius))
	{
		double& position = (axis == XAXIS) ? circle_obj.GetPos().x : circle_obj.GetPos().y;
		double& velocity = (axis == XAXIS) ? circle_obj.GetVel().x : circle_obj.GetVel().y;

		double dist = GetIntersectionDist(axis, Norm, planePos, circle_obj, circle_radius);

		velocity = -velocity;
		position = position - 2 * Norm * dist;

		return true;
	}
	return false;
}

double CheckCornerCircleCollision(Vec2D& corner_pos, Vec2D& circle_pos, Vec2D& relVelocity, double circle_radius)
{
	Vec2D relPos = circle_pos - corner_pos;
	Vec2D relVel = -relVelocity;

	double A = relVel.dot(relVel);
	double B = 2 * relPos.dot(relVel);
	double C = relPos.dot(relPos) - circle_radius*circle_radius;

	double discrim = B*B - 4 * A*C;

	// If ball hits corner
	if (discrim >= 0)
		// Take largest root ie biggest time since collision equals earliest collision
		// and return that time since collision
		return (-B + sqrt(discrim)) / (2 * A);
	return -1;
}