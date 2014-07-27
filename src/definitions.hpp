#pragma once

// General
#define FPS 60
#define NUM_LEVELS 10
#define MAX_HIGHSCORES 10
#define MAX_PLAYER_NAME_SIZE 15
#define MAX_TEXT_SCORE 31

// Audio
#define SOUND_CHANNELS 5
#define SOUND_TRACK_VOLUME 0.4

// Background
#define NUM_BACKGROUNDS 3
#define NUM_STARS 100
#define SPEED_STARS_FAR 1
#define SPEED_STARS_MID 3
#define SPEED_STARS_NEAR 6

// Colors
#define BLACK al_map_rgb( 0, 0, 0 )
#define BLUE al_map_rgb( 0, 0, 255 )
#define GREEN al_map_rgb( 0, 255, 0 )
#define ORANGE al_map_rgb( 255, 102, 0 )
#define RED al_map_rgb( 255, 0, 0 )
#define WHITE al_map_rgb( 255, 255, 255 )
#define YELLOW al_map_rgb( 255, 255, 0 )

// Enemies
#define CHANCES_ENEMY_CHANGE_DIRECTION 150
#define CHANCES_ENEMIES 100
#define CHANCES_ENEMY_SHOT 150
#define ENEMY_DAMAGE_DELAY 50
#define ENEMY_LIFE 2
#define NUM_ENEMIES 5
#define NUM_SHOTS_ENEMY 2
#define SPEED_ENEMIES_X 3
#define SPEED_ENEMIES_Y 1
#define SPEED_SHOOT_ENEMY 20

// Positions
#define HP_POS_X 55
#define MSG_POS_Y_1 ( HEIGHT - 100 )
#define MSG_POS_Y_2 ( HEIGHT - 50 )
#define POINTS_POS_X 275
#define POS_X_CENTER ( WIDTH / 2 )
#define POS_Y_CENTER ( HEIGHT / 2 )

// Player
#define DROP_HP_ENEMY_COLISION 20
#define DROP_HP_ENEMY_SHOT 5
#define DROP_POINTS_ENEMY_COLISION 10
#define DROP_POINTS_ENEMY_PASS 8
#define PLAYER_DAMAGE_DELAY 200
#define GAIN_POINTS_ENEMY_DESTROY 5
#define HP_BAR_SIZE 175
#define NUM_SHOTS_PLAYER 3
#define PLAYER_HP 100
#define SPEED_NAVE 10
#define SPEED_SHOOT_PLAYER 30
#define PLAYER_TOP_BORDER 30
#define PLAYER_BOT_BORDER ( HEIGHT - 50 )
#define PLAYER_LEFT_BORDER 0
#define PLAYER_RIGHT_BORDER ( WIDTH / 3 )

// Resolution definitions
#define WIDTH 800
#define HEIGHT 400

// Keys map
enum KeysMap { UP, DOWN, LEFT, RIGHT, SPACE, ENTER, ESCAPE, ANY_CHAR };