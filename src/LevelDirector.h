#pragma once

#include <vector>
#include <memory>
#include <random>

class Entity;

class LevelDirector
{
	public:
		LevelDirector(std::vector<std::unique_ptr<Entity>>& entityList);
		~LevelDirector() = default;

		void update(float dt);
	private:
		void spawnEnemy(float dt);
		void spawnLedge(float dt);
		void spawnWall(float dt);

		std::vector<std::unique_ptr<Entity>>& m_entities;
		std::mt19937 m_rng;

		static constexpr const float enemySpawnInterval = 2.0f;
		float m_timeUntilEnemySpawn = enemySpawnInterval;
		float m_timeUntilLedgeSpawn = 3.5f;
		float m_timeUntilWallSpawn = enemySpawnInterval + 1.25f;

};
