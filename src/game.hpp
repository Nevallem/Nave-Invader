#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <map>
#include <functional>
#include <string>
#include "definitions.hpp"
#include "draw.hpp"
#include "levels.hpp"
#include "scores.hpp"

// Generates a random number
//
// @param {int} Range int
// @param {int} Range end
long randomNumber( int, int );

// Returns the current time in milliseconds
__int64 getCurrentTime();

// Split a string
vector< std::string > split( const std::string, char );

// Main class
class Game {
private:
	// Save the key was pressed. Used to that user type your name
	int keybdUnichar;

	// Game state
	bool draws = false,
		increasingHP = false,
		gameRunning = false,
		gameStarted = false,
		showHighScores = false,
		stopGame = false;
	static bool stopQueue;

	// Countdown
	__int64 countdown = getCurrentTime();

	// Drawing
	Spaceship player;
	Enemy enemies;
	PlayerAmmo playerShots;
	EnemyAmmo enemyShots;

	// Allegro event
	ALLEGRO_EVENT event;

	// Current level
	LevelsMap level;

	// Show the highscores
	void highScores();

	// Heads-up display
	void HUD();

	// Game over
	void gameOver();

	// Requires a input text for the player
	//
	// @param {std::string &} Input
	// @param {int} Position x
	// @param {int} Position y
	void inputText( std::string &, int, int );

	// Load sounds
	void loadSounds();

	// Next level
	void nextLevel();

	// Pauses the game
	void pause();

	// Main function
	void run();

	// Starts the queue of events
	//
	// @param {void ( Game::* )()} The main function which will run the game
	void startEventQueue( void ( Game::* )() );

	// Menu initial
	void startMenu();

	// Set audio stream state
	//
	// @param {ALLEGRO_AUDIO_STREAM *} Audios stream
	// @param {boolean} State
	// @param {int=0} Time
	static void setAudioStreamState( ALLEGRO_AUDIO_STREAM *, bool, int = 0 );
public:
	// Stats of the player
	static int HP, points, currentLevel;

	// Keys state
	static bool keys[ 8 ];

	// Sound state
	static bool soundActive;

	// _________AUDIO__________ //

	// Main background music
	ALLEGRO_AUDIO_STREAM *mainBgMusic = nullptr;

	// Gamer over music
	ALLEGRO_AUDIO_STREAM *gameOverMusic = nullptr;

	// Sound track
	static ALLEGRO_AUDIO_STREAM *soundTrack;

	// Enemy Reached
	ALLEGRO_SAMPLE *enemyReached = nullptr;
	static ALLEGRO_SAMPLE_INSTANCE *inst_enemyReached;

	// Explosion
	ALLEGRO_SAMPLE *explosion = nullptr;
	static ALLEGRO_SAMPLE_INSTANCE *inst_explosion;

	// Applauses
	ALLEGRO_SAMPLE *applauses = nullptr;
	ALLEGRO_SAMPLE_INSTANCE *inst_applauses = nullptr;

	// Laser
	ALLEGRO_SAMPLE *laser = nullptr;
	static ALLEGRO_SAMPLE_INSTANCE *inst_laser;

	// _________AUDIO__________ //

	// Images
	ALLEGRO_BITMAP *logoImage = nullptr;
	static ALLEGRO_BITMAP *mainBackground;
	static ALLEGRO_BITMAP *spaceshipPlayer;
	static ALLEGRO_BITMAP *playerReached;
	static ALLEGRO_BITMAP *spaceshipEnemy;
	static ALLEGRO_BITMAP *spaceshipEnemyReached;

	// Fonts
	ALLEGRO_FONT *font15_NB = nullptr;
	ALLEGRO_FONT *font15_IG = nullptr;
	ALLEGRO_FONT *font17_IG = nullptr;
	ALLEGRO_FONT *font25_IG = nullptr;

	// Display
	ALLEGRO_DISPLAY *display = nullptr;
	
	// Event queue
	ALLEGRO_EVENT_QUEUE *queue = nullptr;

	// Timer
	ALLEGRO_TIMER *timer = nullptr;

	// Messages of the game
	std::map< const char *, const char * > messages;
	
	//Inits the game
	void init();

	// Wait for ESCAPE or ENTER keys
	//
	// @param {std::function< void() >} Callback function, after ENTER button be pressed
	static void getMasterEvents( std::function< void() > );

	// Play sounds
	//
	// @param {ALLEGRO_SAMPLE_INSTANCE *} Sound instance
	static void playSoundInstance( ALLEGRO_SAMPLE_INSTANCE * );

	/**
	 * Constructor
	 */
	Game();

	/**
	 * Desconstructor
	 */
	~Game();
};