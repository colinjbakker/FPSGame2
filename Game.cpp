#include "Game.h"

Game::Game(int gs, int lvl, int rnd, int ec, int de) : gameState(gs), level(lvl), round(rnd), enemyCount(ec), desiredEnemies(de) {}

bool Game::incrementRound() {
    //returns true if level increments
    round++;
    if (round == 4) {
        level++;
        round = 0;
        gameState = 1;
        return true;
    }
    return false;
}

void Game::spawnEnemies() {
    desiredEnemies = (5 + 5 * level) * round;
    while (enemies.size() < desiredEnemies) {
        float angle = rand() % 360 * PI / 180;
        Vector3 spawnPos = { 71.0f * cos(angle),0.0f,71.0f * sin(angle) };
        float factor = 1 + level * 0.05f + round * 0.01f;
        if (enemies.size() < desiredEnemies / 5.0f) {
            enemies.push_back(Enemy(spawnPos, 200 * factor, 2.0f, 10 * factor, 20.0f * factor));
        }
        else if (enemies.size() < (desiredEnemies / 5.0f * 2.0f)) {
            enemies.push_back(Enemy(spawnPos, 100 * factor, 1.0f, 15 * factor, 10.0f * factor));
        } 
        else {
            enemies.push_back(Enemy(spawnPos, 50 * factor, 0.5f, 20 * factor, 5.0f * factor));
        }
    }
}