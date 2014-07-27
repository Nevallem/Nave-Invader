#pragma once
#include <allegro5/allegro_image.h>
#include <tuple>
#include <vector>

// Levels management
class LevelsMap {
private:
	// Levels definition
	// int → Number of enemies
	// int → Number of shots of a enemey
	// int → Chance of a enemy appear
	// int → Chance of a enemy shoot
	// int → Speed of enemies
	// int → Enemy life
	// int → Points gains to enemy destroyed
	// int → Points to next level
	// int → Drop HP after enemy colision
	// int → Drop HP after enemy shot
	// float → Time to complete
	static std::vector< std::tuple< int, int, int, int, int, int, int, int, int, int, float > > levelsDefinitions;
public:
	// Values
	int chancesEnemies, chancesEnemyShot, dropHPColision, dropHPShot,
		enemyLife, gainPointsToEnemyDestroyed, numEnemies, 
		numEnemyShots, pointsToNextLevel, speedEnemies;
	float timeRemaing;
	ALLEGRO_BITMAP *background;

	/// Destroys the level
	//
	// @param {std::vector< LevelsMap > &} Level
	static void destroy( LevelsMap & );

	// Start the level definitions
	//
	// @param {std::vector< LevelsMap > &} Level
	static void load( LevelsMap & );
};