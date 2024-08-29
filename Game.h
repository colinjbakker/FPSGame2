#pragma once
#include "Enemy.h"
#include "vector"

struct Game {
	int gameState;
	int level;
	int round;
	int enemyCount;
	int desiredEnemies;
	std::vector<Enemy> enemies;

	Game(int gs, int lvl, int rnd, int ec, int de);
	bool incrementRound();
	void spawnEnemies();
};

