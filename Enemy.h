#pragma once
#include "raylib.h"

struct Enemy {
	Vector3 position;
	BoundingBox bound;
	int square;
	float size;
	float health;
	float speed;
	float attackTimer;
	float damage;

	Enemy(Vector3 p, float h, float sz, float spd, float dmg);
	void UpdatePosition(Vector3 playerPos, float dt);
	float Attack(float dt);
};

