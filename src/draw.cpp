#include <string>
#include <math.h>
#include "definitions.hpp"
#include "draw.hpp"
#include "game.hpp"

// ____________PLAYER____________ //

void Spaceship::update() {
	// Draws
	if ( reached ) {
		al_draw_bitmap( Game::playerReached, x, y, NULL );

		if ( damageDelay + PLAYER_DAMAGE_DELAY < getCurrentTime() )
			reached = false;
	} else {
		damageDelay = getCurrentTime();
		al_draw_bitmap( Game::spaceshipPlayer, x, y, NULL );
	}
};

void Spaceship::move( int direction ) {
	// Move the spaceship to the direction
	switch ( direction ) {
		case UP:
			y -= speed;
			y = ( y < PLAYER_TOP_BORDER ) ? PLAYER_TOP_BORDER : y; // Top border
		break;

		case DOWN:
			y += speed;
			y = ( ( y + 50 ) > HEIGHT ) ? PLAYER_BOT_BORDER : y; // Bot border
		break;

		case LEFT:
			x -= speed;
			x = ( x < PLAYER_LEFT_BORDER ) ? PLAYER_LEFT_BORDER : x; // Left border
		break;

		case RIGHT:
			x += speed;
			x = ( x > PLAYER_RIGHT_BORDER ) ? PLAYER_RIGHT_BORDER : x; // Right border
		break;
	}
};

void Spaceship::shoot() {
	int i = -1;

	// Runs the shots of the player
	while ( ++i < NUM_SHOTS_PLAYER ) {
		if ( playerShots[ i ].active )
			continue;

		// Sets the coordinates, active and break
		playerShots[ i ].x = x + 17;
		playerShots[ i ].y = y + 29;
		playerShots[ i ].active = true;

		// Play the laser sound
		Game::playSoundInstance( Game::inst_laser );

		break;
	}
};

void Spaceship::init() {
	// Sets the init values
	x = 20;
	y = POS_Y_CENTER;
	border_x = 30;
	border_y = 0;
	speed = SPEED_NAVE;
	reached = false;
};

void PlayerAmmo::update( int numEnemies, int gainPointsToEnemyDestroyed ) {
	int i, j;

	// Run the player shots
	for ( i = 0; i < NUM_SHOTS_PLAYER; i++ ) {
		if ( !playerShots[ i ].active )
			continue;

		// Updates the shot position
		if ( Game::keys[ SPACE ] ) {
			playerShots[ i ].x += SPEED_SHOOT_PLAYER;

			// If cross the window, active = false
			if ( playerShots[ i ].x > WIDTH ) {
				playerShots[ i ].active = false;

				continue;
			}
		}

		// Draws
		al_draw_filled_circle( playerShots[ i ].x, playerShots[ i ].y - 3, 2, WHITE );

		// Check for colisions
		for ( j = 0; j < numEnemies; j++ ) {
			if ( !enemies[ j ].active )
				continue;

			// Collision test
			if ( playerShots[ i ].x > ( enemies[ j ].x - enemies[ j ].border_x )
				&& playerShots[ i ].x < ( enemies[ j ].x + enemies[ j ].border_x )
				&& playerShots[ i ].y > ( enemies[ j ].y - enemies[ j ].border_y )
				&& playerShots[ i ].y < ( enemies[ j ].y + 50 )
			) {
				playerShots[ i ].active = false;
				
				if ( --enemies[ j ].life <= 0 ) {
					enemies[ j ].active = false;
					enemies[ j ].exploded = true;
					Game::points += gainPointsToEnemyDestroyed;
					Game::playSoundInstance( Game::inst_explosion );
				} else {
					enemies[ j ].reached = true;
					Game::playSoundInstance( Game::inst_enemyReached );
				}
			}
		}
	}
};

void PlayerAmmo::init() {
	int i;

	// Runs the shots of player and set the init values
	for ( i = 0; i < NUM_SHOTS_PLAYER; i++ ) {
		playerShots[ i ].active = false;
		playerShots[ i ].speed = SPEED_SHOOT_PLAYER;
	}
};

// ____________ENEMIES____________ //

void Enemy::move( Enemy &enemy, int numEnemies, int currentEnemy ) {
	int i;

	if ( enemy.dir == 0 ) {
		enemy.y += SPEED_ENEMIES_Y;
		
		// Bot border
		if ( enemy.y + 50 > HEIGHT )
			enemy.dir = 1;
	} else {
		enemy.y -= SPEED_ENEMIES_Y;

		// Top border
		if ( enemy.y < 45 )
			enemy.dir = 0;
	}

	// Random chance direction
	if ( randomNumber( 0, CHANCES_ENEMY_CHANGE_DIRECTION + 1 ) == CHANCES_ENEMY_CHANGE_DIRECTION )
		enemy.dir = !enemy.dir;

	// Check for colisions with another enemy
	for ( i = 0; i < numEnemies; i++ ) {
		if ( !enemies[ i ].active || i == currentEnemy )
			continue;

		// If collides, changes the direction
		if ( ( enemy.x - enemy.border_x ) < ( enemies[ i ].x + enemies[ i ].border_x )
			&& ( enemy.y - enemy.border_y ) < ( enemies[ i ].y + 25 )
			&& ( enemy.x + enemy.border_x ) > ( enemies[ i ].x - enemies[ i ].border_x )
			&& ( enemy.y + enemy.border_y ) > ( enemies[ i ].y - enemies[ i ].border_y ) 
		) {
			enemy.dir = !enemy.dir;
		}
	}
};

void Enemy::release( int numEnemies, int chancesEnemies, int enemyLife ) {
	int i = -1,
		j = -1;

	// Runs the enemies
	while ( ++i < numEnemies ) {
		// if !active, tries generate a new enemy
		if ( ( !enemies[ i ].active && !enemies[ i ].exploded ) 
			&& ( randomNumber( 0, chancesEnemies + 1 ) == chancesEnemies || getEnemiesActives( numEnemies ) < ( numEnemies / 2 ) ) 
		) {
			// Sets the enemy coordenates, active and break
			enemies[ i ].x = WIDTH;
			enemies[ i ].y = randomNumber( 60, HEIGHT - 55 );
			enemies[ i ].dir = randomNumber( 0, 2 );
			enemies[ i ].active = true;
			enemies[ i ].exploded = false;
			enemies[ i ].life = enemyLife;

			// Check for colisions with another enemy
			while ( ++j < numEnemies ) {
				if ( j == i )
					continue;

				// Colision test
				if ( ( enemies[ i ].x - enemies[ i ].border_x ) < ( enemies[ j ].x + enemies[ j ].border_x )
					&& ( enemies[ i ].y - enemies[ i ].border_y ) < ( enemies[ j ].y + 25 )
					&& ( enemies[ i ].x + enemies[ i ].border_x ) > ( enemies[ j ].x - enemies[ j ].border_x )
					&& ( enemies[ i ].y + enemies[ i ].border_y ) > ( enemies[ j ].y - enemies[ j ].border_y )
				) {
					enemies[ i ].active = false;

					break;
				}
			}
			
			break;
		}
	}
};

void Enemy::update( Spaceship &player, int numEnemies, int dropHP ) {
	int i;

	// Runs the enemies
	for ( i = 0; i < numEnemies; i++ ) {
		if ( !enemies[ i ].active )
			continue;

		enemies[ i ].x -= enemies[ i ].speed;

		Enemy::move( enemies[ i ], numEnemies, i );

		// If cross the window
		if ( enemies[ i ].x < 0 ) {
			enemies[ i ].active = false;
			Game::points -= DROP_HP_ENEMY_COLISION;

			continue;
		}

		if ( enemies[ i ].reached ) {
			al_draw_bitmap( Game::spaceshipEnemyReached, enemies[ i ].x, enemies[ i ].y, NULL );

			if ( enemies[ i ].damageDelay + ENEMY_DAMAGE_DELAY < getCurrentTime() )
				enemies[ i ].reached = false;
		} else {
			enemies[ i ].damageDelay = getCurrentTime();
			al_draw_bitmap( Game::spaceshipEnemy, enemies[ i ].x, enemies[ i ].y, NULL );
		}

		// Collision test
		if ( ( enemies[ i ].x - enemies[ i ].border_x ) < ( player.x + player.border_x )
			&& ( enemies[ i ].x + enemies[ i ].border_x ) > ( player.x - player.border_x )
			&& ( enemies[ i ].y - enemies[ i ].border_y ) < ( player.y + 25 )
			&& ( enemies[ i ].y + enemies[ i ].border_y ) > ( player.y - player.border_y )
		) {
			enemies[ i ].active = false;
			enemies[ i ].exploded = true;
			player.reached = true;
			Game::HP -= ( dropHP > Game::HP ) ? Game::HP : dropHP;
			Game::points -= DROP_HP_ENEMY_COLISION;

			// Play the explosion sound
			Game::playSoundInstance( Game::inst_explosion );
		}
	}
};

void Enemy::init( int numEnemies, int speedEnemies ) {
	int i;
	
	enemies.resize( numEnemies );

	// Runs the enemies and sets the init values
	for ( i = 0; i < numEnemies; i++ ) {
		enemies[ i ].border_x = 30;
		enemies[ i ].border_y = 30;
		enemies[ i ].speed = speedEnemies;
		enemies[ i ].active = false;
		enemies[ i ].exploded = false;
	}
};

int Enemy::getEnemiesActives( int numEnemies ) {
	int i, counter = 0;

	for ( i = 0; i < numEnemies; i++ )
		counter += enemies[ i ].active ? 1 : 0;

	return counter;
};

void EnemyAmmo::update( Spaceship &player, int chancesEnemyShot, int dropHPShot ) {
	int i;

	// Runs the shots of player
	for ( i = 0; i < NUM_ENEMIES * NUM_SHOTS_ENEMY; i++ ) {
		if ( !enemies[ i / NUM_SHOTS_ENEMY ].active )
			continue;

		// If !active
		if ( !enemyShots[ i ].active ) {
			// Tries generate a new shot
			if ( randomNumber( 0, chancesEnemyShot + 1 ) == chancesEnemyShot ) {
				enemyShots[ i ].x = enemies[ i / NUM_SHOTS_ENEMY ].x - 10;
				enemyShots[ i ].y = enemies[ i / NUM_SHOTS_ENEMY ].y + 25;
				enemyShots[ i ].active = true;
			}

			continue;
		}

		enemyShots[ i ].x -= SPEED_SHOOT_ENEMY;

		// If cross the window, active = false and continue
		if ( enemyShots[ i ].x < 0 ) {
			enemyShots[ i ].active = false;

			continue;
		}

		// Draws
		al_draw_filled_circle( enemyShots[ i ].x, enemyShots[ i ].y - 3, 2, RED );

		// Checks for colision
		if ( enemyShots[ i ].x < ( player.x + player.border_x )
			&& enemyShots[ i ].x > ( player.x - player.border_x )
			&& enemyShots[ i ].y < ( player.y + 50 )
			&& enemyShots[ i ].y > ( player.y - player.border_y )
		) {
			enemyShots[ i ].active = false;
			player.reached = true;

			Game::playSoundInstance( Game::inst_explosion );
			Game::HP -= ( dropHPShot > Game::HP ) ? Game::HP : dropHPShot;
		}
	}
};

void EnemyAmmo::init() {
	int i;

	// Runs the shots of player and set the init values
	for ( i = 0; i < NUM_ENEMIES * NUM_SHOTS_ENEMY; i++ ) {
		enemyShots[ i ].active = false;
		enemyShots[ i ].speed = SPEED_SHOOT_ENEMY;
	}
};

// ____________EXPLOSION____________ //

void Explosion::destroyImages() {
	int i;

	for ( i = 0; i < 15; i++ )
		al_destroy_bitmap( explosionImgs[ i ] );
};

void Explosion::loadImages() {
	std::string path;
	int i;

	for ( i = 0; i < 16; i++ ) {
		path = "images/explosion/explosion-" + std::to_string( i ) + ".png";
		explosionImgs.push_back( al_load_bitmap( path.c_str() ) );
	}
};

void Explosion::show( int numEnemies ) {
	int i;

	for ( i = 0; i < numEnemies; i++ ) {
		if ( enemies[ i ].exploded ) {
			al_draw_bitmap( explosionImgs[ enemies[ i ].currentImgExplosion++ ], enemies[ i ].x, enemies[ i ].y, NULL );

			if ( enemies[ i ].currentImgExplosion == 16 ) {
				enemies[ i ].exploded = false;
				enemies[ i ].currentImgExplosion = 0;
			}
		}
	}
};

// ____________BACKGROUND____________ //

void Background::update( ALLEGRO_BITMAP *img ) {
	int i;

	al_draw_bitmap( img, 0, 0, NULL );

	// Runs the backgrounds
	for ( i = 0; i < NUM_BACKGROUNDS * NUM_STARS; i++ ) {
		backgrounds[ i ].x -= backgrounds[ i ].speed;

		// If the star cross the window, back to init
		if ( backgrounds[ i ].x < 0 )
			backgrounds[ i ].x = WIDTH + 10;

		// Draws
		al_draw_pixel( backgrounds[ i ].x, backgrounds[ i ].y, WHITE );
	}
};

void Background::init() {
	int i,
		speedStars[] = { SPEED_STARS_NEAR, SPEED_STARS_MID, SPEED_STARS_FAR };

	// Runs the backgrounds
	for ( i = 0; i < NUM_BACKGROUNDS * NUM_STARS; i++ ) {
		backgrounds[ i ].speed = speedStars[ i / NUM_STARS ];
		backgrounds[ i ].x = randomNumber( 5, WIDTH - 10 );
		backgrounds[ i ].y = randomNumber( 5, HEIGHT - 10 );
	}
};