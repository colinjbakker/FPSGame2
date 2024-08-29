#include "Projectile.h"

Projectile::Projectile(Vector3 startPos, Vector3 startDirection, float dmg) : position(startPos), direction(startDirection),  damage(dmg), hitPoint({ 0.0f, 0.0f, 0.0f }) {
	collisionRay.position = position;
	collisionRay.direction = direction;
	time = 0.0f;
	hit = false;
}

void Projectile::UpdateTime(float dt) {
	time += dt;
}