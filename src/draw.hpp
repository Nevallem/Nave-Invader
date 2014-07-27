#pragma once
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <vector>

using std::vector;

// Spaceship
class Spaceship {
public:
	int x, y, speed, border_x, border_y;
	__int64 damageDelay;

	// Nave state
	bool reached;

	/**
	 * Moves the nave
	 *
	 * @param {int} Direction
	 */
	void move( int );

	/**
	 * Updates the postion of the nave
	 */
	void update();

	/**
	 * Shoot
	 */
	void shoot();

	/**
	 * Initialize  
	 */
	void init();
};

// Ammo of the Spaceship
class PlayerAmmo {
public:
	int x, y, speed;

	// Shot state
	bool active;

	/**
	 * Updade the position of the shot(s)
	 *
	 * @param {int} Number of enemies
	 * @param {int} Points gains to enemy destroyed
	 * @param {ALLEGRO_SAMPLE_INSTANCE *} Explosion sound
	 */
	static void update( int, int );

	/**
	 * Initialize  
	 */
	static void init();
};

// Enemies
class Enemy {
public:
	int x, y, speed, border_x, border_y, life, currentImgExplosion, dir;
	__int64 damageDelay;

	// Enemy state
	bool active, exploded, reached;

	/**
	 * Moves the enemy
	 *
	 * @param {Enemy &} The enemy
	 * @param {int} Number of enemies
	 * @param {int} Index of the current enemy
	 *
	 * dir 0 = down
	 * dir 1 = up
	 */
	static void move( Enemy &, int, int );

	/**
	 * Shoot
	 *
	 * @param {ALLEGRO_SAMPLE_INSTANCE *} Laser sound
	 */
	void shoot();

	/**
	 * Release the comet(s)
	 * @param {int} Number of enemies
	 * @param {int} Chance of enemies
	 * @param {int} Life of enemy
	 */
	static void release( int, int, int );

	/**
	 * Updade the position of the comet(s)
	 *
	 * @param {Spacheship &} The spaecship
	 * @param {int} Number of enemies
	 * @param {int} HP drop after colision
	 */
	static void update( Spaceship &, int, int );

	/**
	 * Initialize
	 *
	 * @param {int} Number of enemies
	 * @param {int} Speed of enemies
	 */
	static void init( int, int );

	/**
	 * Verify the ammount of enemies actives in the window
	 *
	 * @param {int} Number of enemies
	 */
	static int getEnemiesActives( int );
};

// Ammo of the enemy
class EnemyAmmo {
public:
	int x, y, speed;

	// Shot state
	bool active;

	/**
	 * Updade the position of the shot(s)
	 *
	 * @param {Spaceship &} Player
	 * @param {int} Chance enemy shoot
	 * @param {int} Drop hp
	 */
	static void update( Spaceship &, int, int );

	/**
	 * Initialize  
	 */
	static void init();
};

class Explosion {
public:
	/**
	 * Destroy explosion images
	 */
	static void destroyImages();

	/**
	 * Load explosion images
	 */
	static void loadImages();

	/**
	 * Show the explosion
	 *
	 * @param {int} Num of enemies
	 */
	static void show( int );
};

// Background
class Background {
public:
	int x, y, speed;

	// Updade the position of the stars
	static void update( ALLEGRO_BITMAP * );

	// Init the background
	static void init();
};

// Starts the arrays of the objets
static vector< PlayerAmmo > playerShots( NUM_SHOTS_PLAYER );
static vector< vector< EnemyAmmo > > enemyShots( NUM_ENEMIES );
static vector< Enemy > enemies;
static vector< Background > backgrounds( NUM_STARS );
static vector< ALLEGRO_BITMAP * > explosionImgs;