#pragma once
#include "raylib.h"
#include "raymath.h"

struct Projectile {
	Vector3 position;
	Vector3 direction;
	bool hit;
	Ray collisionRay;
	Vector3 hitPoint;
	float damage;
	float time;
	Projectile(Vector3 startPos, Vector3 startDirection, float dmg);
	void UpdateTime(float dt);
};

