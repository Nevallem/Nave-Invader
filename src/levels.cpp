#include <string>
#include "game.hpp"
#include "levels.hpp"

using std::make_tuple;

// All levels definitions
std::vector< std::tuple< int, int, int, int, int, int, int, int, int, int, float > > LevelsMap::levelsDefinitions = {
	// Level 1
	make_tuple(
		NUM_ENEMIES,
		NUM_SHOTS_ENEMY,
		CHANCES_ENEMIES,
		CHANCES_ENEMY_SHOT,
		SPEED_ENEMIES_X,
		ENEMY_LIFE,
		GAIN_POINTS_ENEMY_DESTROY,
		100,
		DROP_HP_ENEMY_COLISION,
		DROP_HP_ENEMY_SHOT,
		15
	), 
	
	// Level 2
	make_tuple(
		NUM_ENEMIES,
		NUM_SHOTS_ENEMY,
		CHANCES_ENEMIES - 10,
		CHANCES_ENEMY_SHOT,
		SPEED_ENEMIES_X + 1,
		ENEMY_LIFE,
		GAIN_POINTS_ENEMY_DESTROY,
		300,
		DROP_HP_ENEMY_COLISION,
		DROP_HP_ENEMY_SHOT,
		30
	), 
	
	// Level 3
	make_tuple(
		NUM_ENEMIES + 1,
		NUM_SHOTS_ENEMY,
		CHANCES_ENEMIES - 20,
		CHANCES_ENEMY_SHOT,
		SPEED_ENEMIES_X + 1,
		ENEMY_LIFE,
		GAIN_POINTS_ENEMY_DESTROY,
		700,
		DROP_HP_ENEMY_COLISION,
		DROP_HP_ENEMY_SHOT,
		50
	), 
	
	// Level 4
	make_tuple(
		NUM_ENEMIES + 1,
		NUM_SHOTS_ENEMY,
		CHANCES_ENEMIES - 30,
		CHANCES_ENEMY_SHOT,
		SPEED_ENEMIES_X + 1,
		ENEMY_LIFE,
		GAIN_POINTS_ENEMY_DESTROY,
		1000,
		DROP_HP_ENEMY_COLISION,
		DROP_HP_ENEMY_SHOT,
		40
	), 	

	// Level 5
	make_tuple(
		NUM_ENEMIES + 1,
		NUM_SHOTS_ENEMY,
		CHANCES_ENEMIES - 35,
		CHANCES_ENEMY_SHOT,
		SPEED_ENEMIES_X + 1,
		ENEMY_LIFE,
		GAIN_POINTS_ENEMY_DESTROY + 5,
		1750,
		DROP_HP_ENEMY_COLISION,
		DROP_HP_ENEMY_SHOT,
		50
	),

	// Level 6
	make_tuple(
		NUM_ENEMIES + 2,
		NUM_SHOTS_ENEMY,
		CHANCES_ENEMIES - 40,
		CHANCES_ENEMY_SHOT + 50,
		SPEED_ENEMIES_X + 1,
		ENEMY_LIFE,
		GAIN_POINTS_ENEMY_DESTROY + 5,
		3000,
		DROP_HP_ENEMY_COLISION - 10,
		DROP_HP_ENEMY_SHOT,
		70
	), 

	// Level 7
	make_tuple(
		NUM_ENEMIES + 2,
		NUM_SHOTS_ENEMY + 1,
		CHANCES_ENEMIES - 50,
		CHANCES_ENEMY_SHOT + 10,
		SPEED_ENEMIES_X + 1,
		ENEMY_LIFE,
		GAIN_POINTS_ENEMY_DESTROY + 10,
		5715,
		DROP_HP_ENEMY_COLISION - 10,
		DROP_HP_ENEMY_SHOT - 3,
		90
	), 

	// Level 8
	make_tuple(
		NUM_ENEMIES + 2,
		NUM_SHOTS_ENEMY + 1,
		CHANCES_ENEMIES - 60,
		CHANCES_ENEMY_SHOT - 100,
		SPEED_ENEMIES_X + 2,
		ENEMY_LIFE,
		GAIN_POINTS_ENEMY_DESTROY + 10,
		7775,
		DROP_HP_ENEMY_COLISION - 10,
		DROP_HP_ENEMY_SHOT - 3,
		70
	), 

	// Level 9
	make_tuple(
		NUM_ENEMIES + 2,
		NUM_SHOTS_ENEMY + 1,
		CHANCES_ENEMIES - 80,
		CHANCES_ENEMY_SHOT - 100,
		SPEED_ENEMIES_X + 2,
		ENEMY_LIFE,
		GAIN_POINTS_ENEMY_DESTROY + 15,
		10000,
		DROP_HP_ENEMY_COLISION - 15,
		DROP_HP_ENEMY_SHOT - 3,
		70
	), 

	// Level 10
	make_tuple(
		NUM_ENEMIES + 2,
		NUM_SHOTS_ENEMY + 1,
		CHANCES_ENEMIES - 100,
		CHANCES_ENEMY_SHOT - 100,
		SPEED_ENEMIES_X + 2,
		ENEMY_LIFE,
		GAIN_POINTS_ENEMY_DESTROY + 45,
		15000,
		DROP_HP_ENEMY_COLISION - 15,
		DROP_HP_ENEMY_SHOT - 3,
		50
	)
};

void LevelsMap::destroy( LevelsMap &level ) {
	// Sets all as NULL
	level.numEnemies = NULL;
	level.numEnemyShots = NULL;
	level.chancesEnemies = NULL;
	level.chancesEnemyShot = NULL;
	level.speedEnemies = NULL;
	level.enemyLife = NULL;
	level.gainPointsToEnemyDestroyed = NULL;
	level.pointsToNextLevel = NULL;
	level.dropHPColision = NULL;
	level.dropHPShot = NULL;
	level.timeRemaing = NULL;

	// Destroy the image
	al_destroy_bitmap( level.background );

	// Set all keys as false
	for ( auto &key : Game::keys )
		key = false;
};

void LevelsMap::load( LevelsMap &level ) {
	using std::get;

	std::string path = "images/levels/" + std::to_string( Game::currentLevel + 1 ) + ".png";

	// Sets the definitions
	level.numEnemies = get< 0 >( levelsDefinitions[ Game::currentLevel ] );
	level.numEnemyShots = get< 1 >( levelsDefinitions[ Game::currentLevel ] );
	level.chancesEnemies = get< 2 >( levelsDefinitions[ Game::currentLevel ] );
	level.chancesEnemyShot = get< 3 >( levelsDefinitions[ Game::currentLevel ] );
	level.speedEnemies = get< 4 >( levelsDefinitions[ Game::currentLevel ] );
	level.enemyLife = get< 5 >( levelsDefinitions[ Game::currentLevel ] );
	level.gainPointsToEnemyDestroyed = get< 6 >( levelsDefinitions[ Game::currentLevel ] );
	level.pointsToNextLevel = get< 7 >( levelsDefinitions[ Game::currentLevel ] );
	level.dropHPColision = get< 8 >( levelsDefinitions[ Game::currentLevel ] );
	level.dropHPShot = get< 9 >( levelsDefinitions[ Game::currentLevel ] );
	level.timeRemaing = get< 10 >( levelsDefinitions[ Game::currentLevel ] );
	level.background = al_load_bitmap( path.c_str() );
};