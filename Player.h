#pragma once
#include "Projectile.h"
#include "vector"
struct Player
{
	float health;
	float shotCooldown;
	float moveSpeed;
	float verticalVelocity;
	int kills;
	int shots;
	Camera3D camera;
	std::vector<Projectile> projectiles;

	Player(float hp, float sc, float ms, float vv, float k, float s, Camera3D c);
};

