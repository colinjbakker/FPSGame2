#include "Enemy.h"
#include "raymath.h"

Enemy::Enemy(Vector3 p, float h, float sz, float spd, float dmg) : position(p), health(h), size(sz), speed(spd), damage(dmg) {
	bound.min = Vector3Subtract(position, { size, 0, size });
	bound.max = Vector3Add(position, { size, size * 4.0f, size });
	square = (floor(position.x / 5)+10) + 20 * (floor(position.z / 5)+10);
}

void Enemy::UpdatePosition(Vector3 playerPos, float dt) {
	Vector3 direction = Vector3Scale(Vector3Normalize(Vector3Multiply(Vector3Subtract(playerPos, position), {1.0f, 0.0f, 1.0f})), speed * dt);
	position = Vector3Add(position, direction);
	bound.min = Vector3Subtract(position, { size, 0, size });
	bound.max = Vector3Add(position, { size, size * 4.0f, size });
	square = (floor(position.x / 5) + 10) + 20 * (floor(position.z / 5) + 10);
	attackTimer -= dt;
}

float Enemy::Attack(float dt) {
	if (attackTimer <= 0.0f) {
		attackTimer = 1.0f;
		return damage;
	}
	else {
		attackTimer -= dt;
		return 0.0f;
	}
}