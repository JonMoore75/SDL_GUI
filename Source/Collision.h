#ifndef Collision_h__
#define Collision_h__
#pragma once

enum AXIS { XAXIS, YAXIS };
class GameObject;
class Vec2D;

extern bool CheckForCircleAxisCollision(AXIS axis, int Norm, double planePos, GameObject& circle_obj, double circle_radius);
bool CheckForCircleAxisIntersection(AXIS axis, int Norm, double planePos, GameObject& circle_obj, double circle_radius);

double GetIntersectionDist(AXIS axis, int Norm, double planePos, GameObject& circle_obj, double circle_radius);

double CheckCornerCircleCollision(Vec2D& corner_pos, Vec2D& circle_pos, Vec2D& relVelocity, double circle_radius);

#endif Collision_h__
