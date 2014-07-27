#include "game.hpp"
#include <chrono>
#include <random>
#include <sstream>

// Sets initial values
int Game::HP = PLAYER_HP;
int Game::points = 0;
int Game::currentLevel = 0;
bool Game::soundActive = true;
bool Game::stopQueue = false;

// Sets the initial keys state
bool Game::keys[ 8 ] = { false, false, false, false, false, false, false, false };

// Sets nullptr to bitmaps and audio
ALLEGRO_BITMAP *Game::mainBackground = nullptr;
ALLEGRO_BITMAP *Game::spaceshipPlayer = nullptr;
ALLEGRO_BITMAP *Game::playerReached = nullptr;
ALLEGRO_BITMAP *Game::spaceshipEnemy = nullptr;
ALLEGRO_BITMAP *Game::spaceshipEnemyReached = nullptr;
ALLEGRO_SAMPLE_INSTANCE *Game::inst_laser = nullptr;
ALLEGRO_SAMPLE_INSTANCE *Game::inst_explosion = nullptr;
ALLEGRO_SAMPLE_INSTANCE *Game::inst_enemyReached = nullptr;
ALLEGRO_AUDIO_STREAM *Game::soundTrack = nullptr;

void Game::setAudioStreamState( ALLEGRO_AUDIO_STREAM *as, bool state, int time ) {
	al_set_audio_stream_playing( as, state );

	// Set the time
	if ( time > -1 )
		al_seek_audio_stream_secs( as, time );
};

void Game::playSoundInstance( ALLEGRO_SAMPLE_INSTANCE *asi ) {
	// The sound needs be active
	if ( soundActive ) {
		al_stop_sample_instance( asi );
		al_play_sample_instance( asi );
	}
};

void Game::loadSounds() {
	// Main background music
	mainBgMusic = al_load_audio_stream( "audio/mainBg.ogg", 4, 1024 );
	al_attach_audio_stream_to_mixer( mainBgMusic, al_get_default_mixer() );

	// Sound track
	soundTrack = al_load_audio_stream( "audio/soundTrack.ogg", 4, 1024 );

	// Game over music
	gameOverMusic = al_load_audio_stream( "audio/gameOver.ogg", 4, 1024 );
	al_attach_audio_stream_to_mixer( gameOverMusic, al_get_default_mixer() );
	setAudioStreamState( gameOverMusic, false );

	// Explosion
	explosion = al_load_sample( "audio/explosion.wav" );
	Game::inst_explosion = al_create_sample_instance( explosion );
	al_attach_sample_instance_to_mixer( Game::inst_explosion, al_get_default_mixer() );

	// Explosion
	enemyReached = al_load_sample( "audio/enemyReached.wav" );
	Game::inst_enemyReached = al_create_sample_instance( enemyReached );
	al_attach_sample_instance_to_mixer( Game::inst_enemyReached, al_get_default_mixer() );

	// Laser
	laser = al_load_sample( "audio/laser.wav" );
	Game::inst_laser = al_create_sample_instance( laser );
	al_attach_sample_instance_to_mixer( Game::inst_laser, al_get_default_mixer() );

	// Applauses
	applauses = al_load_sample( "audio/applauses.wav" );
	inst_applauses = al_create_sample_instance( applauses );
	al_attach_sample_instance_to_mixer( inst_applauses, al_get_default_mixer() );
};

void Game::inputText( std::string &playerName, int x, int y ) {
	// Draws
	al_draw_text( font15_IG, ORANGE, x, y, NULL, ( ( playerName.size() < 1 ? "_" : "" ) + playerName ).c_str() );

	// If a char key was pressed
	if ( keys[ ANY_CHAR ] ) {
		if ( ( keybdUnichar > 32 && keybdUnichar <= 122 || keybdUnichar == 32 && playerName.size() > 0 ) && playerName.size() <= MAX_PLAYER_NAME_SIZE )
			playerName += std::string( 1, keybdUnichar );
		else if ( keybdUnichar == 8 && playerName.size() > 0 )
			playerName.resize( playerName.size() - 1 );

		keys[ ANY_CHAR ] = false;
	}
};

void Game::getMasterEvents( std::function< void() > callback ) {
	// Controls
	static int control = 0;

	// Back buffer
	al_flip_display();

	// If was the first execution of the function, returns,
	// because always in first execution, some of the keys will setted as true,
	// something that would take unwanted results
	if ( control++ == 0 ) {
		keys[ ENTER ] = false;
		keys[ ESCAPE ] = false;

		// Stop the sound track
		setAudioStreamState( soundTrack, false, -1 );

		return;
	}

	// Get user event
	if ( keys[ ESCAPE ] ) {
		stopQueue = true;
	} else if ( keys[ ENTER ] ) {
		control = 0;
		callback();

		// Restart the sound track
		setAudioStreamState( soundTrack, soundActive, -1 );
	}
};

void Game::startEventQueue( void ( Game::*callback )() ) {
	// Installs the keyboard and mouse
	al_install_keyboard();
	al_install_mouse();

	// Starts the queue events and the timer
	queue = al_create_event_queue();
	timer = al_create_timer( 1.0 / FPS );

	// Register events
	al_register_event_source( queue, al_get_keyboard_event_source() );
	al_register_event_source( queue, al_get_display_event_source( display ) );
	al_register_event_source( queue, al_get_timer_event_source( timer ) );
	al_register_event_source( queue, al_get_mouse_event_source() );

	// Stars timer (FPS synchronizer)
	al_start_timer( timer );

	// Main loop
	while ( !stopQueue ) {
		// Waits for a event
		al_wait_for_event( queue, &event );

		// Callback function
		( this->*callback )();
	}
};

void Game::startMenu() {
	// Draws background
	al_draw_bitmap( Game::mainBackground, 0, 0, NULL );
	al_draw_bitmap( logoImage, WIDTH / 8, HEIGHT / 9, NULL );
	
	// Draws message
	al_draw_text( font25_IG, WHITE, POS_X_CENTER, MSG_POS_Y_2, ALLEGRO_ALIGN_CENTER, messages[ "gameStart" ] );

	// Waits for ESC or ENTER
	getMasterEvents( [ this ] () {
		// Load images
		Game::spaceshipPlayer = al_load_bitmap( "images/spaceships/player.png" );
		Game::playerReached = al_load_bitmap( "images/spaceships/player_reached.png" );
		Game::spaceshipEnemy = al_load_bitmap( "images/spaceships/enemy.png" );
		Game::spaceshipEnemyReached = al_load_bitmap( "images/spaceships/enemy_reached.png" );
		
		// Inits all
		Background::init();
		LevelsMap::load( level );
		Explosion::loadImages();
		player.init();
		playerShots.init();
		enemies.init( level.numEnemies, level.speedEnemies );
		enemyShots.init();

		level = level;

		// Sound track
		al_attach_audio_stream_to_mixer( soundTrack, al_get_default_mixer() );
		al_set_audio_stream_gain( soundTrack, SOUND_TRACK_VOLUME );
		al_set_audio_stream_playmode( soundTrack, ALLEGRO_PLAYMODE_LOOP );

		// Stop the main music
		setAudioStreamState( mainBgMusic, false );

		gameStarted = true;
		gameRunning = true;
	} );
};

void Game::HUD() {
	// Draws
	
	// HP [text]
	al_draw_text( font15_IG, WHITE, POS_X_CENTER - 190, 7, NULL, messages[ "HUD-HP" ] );

	// HP [red bar]
	al_draw_filled_rectangle( POS_X_CENTER - 150, 7, ( POS_X_CENTER - 150 ) + HP_BAR_SIZE, 25, RED );

	// HP [green bar]
	al_draw_filled_rectangle( POS_X_CENTER - 150, 7, ( POS_X_CENTER - 150 ) + ( HP_BAR_SIZE - ( HP_BAR_SIZE - ( HP * ( ( float ) HP_BAR_SIZE / PLAYER_HP ) ) ) ), 25, GREEN );

	// HP [bar text]
	al_draw_textf( font15_IG, BLACK, POS_X_CENTER - 60, 8, ALLEGRO_ALIGN_CENTER, "%d/%d", HP, PLAYER_HP );

	// Points [value]
	al_draw_textf( font15_IG, WHITE, POS_X_CENTER + 40, 7, NULL, messages[ "HUD-points" ], points, level.pointsToNextLevel > -1 ? level.pointsToNextLevel : 0 );

	// Timer
	al_draw_textf( font15_IG, WHITE, POS_X_CENTER - 50, 30, NULL, messages[ "HUD-timer" ], level.timeRemaing > -1 ? level.timeRemaing : 0 );
};

void Game::nextLevel() {
	// Text position
	static int text_x = WIDTH + 500;

	// Updates the background and HUD
	Background::update( level.background );
	HUD();

	// Logo image
	al_draw_bitmap( logoImage, WIDTH / 8, HEIGHT / 9, NULL );

	// Message
	al_draw_textf( font17_IG, GREEN, text_x, MSG_POS_Y_1, ALLEGRO_ALIGN_CENTER, messages[ "LevelPassed-title" ], currentLevel + 1 );

	// Moves the text to left and play the applauses
	if ( text_x > POS_X_CENTER ) {
		// Play the applause
		playSoundInstance( inst_applauses );
		text_x -= 12;
	} else {
		// Message
		al_draw_textf( font17_IG, WHITE, POS_X_CENTER, MSG_POS_Y_1 + 25, ALLEGRO_ALIGN_CENTER, messages[ "LevelPassed-content" ] );
	}

	// Waits for ESC or ENTER
	getMasterEvents( [ this ] () {
		// Stop the applauses
		al_stop_sample_instance( inst_applauses );

		// Destroy the current level and load the next
		currentLevel++;
		LevelsMap::destroy( level );
		LevelsMap::load( level );
			
		// Resets
		text_x = WIDTH + 500;

		gameRunning = true;
		increasingHP = true;
		
		player.init();
		enemies.init( level.numEnemies, level.speedEnemies );
		playerShots.init();
		enemyShots.init();
	} );
};

void Game::pause() {
	// Draws background
	al_draw_bitmap( Game::mainBackground, 0, 0, NULL );
	al_draw_bitmap( logoImage, WIDTH / 8, HEIGHT / 9, NULL );

	// Draws message
	al_draw_text( font25_IG, GREEN, POS_X_CENTER, MSG_POS_Y_1, ALLEGRO_ALIGN_CENTER, messages[ "gamePaused-title" ] );
	al_draw_text( font17_IG, WHITE, POS_X_CENTER, MSG_POS_Y_2, ALLEGRO_ALIGN_CENTER, messages[ "gamePaused-content" ] );

	// Play sound track
	setAudioStreamState( mainBgMusic, soundActive, -1 );

	// Waits for ESC or ENTER
	getMasterEvents( [ this ] () {
		gameRunning = true;
		stopGame = false;
		keys[ ENTER ] = false;

		// Stop the main music
		setAudioStreamState( mainBgMusic, false );
	} );
};

void Game::gameOver() {
	// Only save the score if bigger which the last of the list
	static bool saveScore;
	saveScore = points > atoi( Scores::get()[ MAX_HIGHSCORES - 1 ][ 1 ].c_str() );

	// Text position
	static int text_x = WIDTH + 500;

	// Player name
	static std::string playerName;

	// Updates the background and HUD
	Background::update( level.background );
	HUD();

	// Logo image
	al_draw_bitmap( logoImage, WIDTH / 8, HEIGHT / 9, NULL );

	// Draws the message
	al_draw_text( font25_IG, RED, text_x, MSG_POS_Y_1, ALLEGRO_ALIGN_CENTER, messages[ "gameOver-title" ] );

	// Moves the text to left
	if ( text_x > POS_X_CENTER ) {
		text_x -= 12;
	}  else {
		al_draw_text( font17_IG, WHITE, POS_X_CENTER, MSG_POS_Y_2, ALLEGRO_ALIGN_CENTER, messages[ "gameOver-content" ] );

		// Play the music
		setAudioStreamState( gameOverMusic, soundActive, -1 );

		if ( saveScore ) {
			al_draw_text( font15_IG, WHITE, WIDTH / 2 - 150, HEIGHT / 2 + 50, NULL, messages[ "gameOver-typeYourName" ] );
			inputText( playerName, WIDTH / 2 + 40, HEIGHT / 2 + 50 );
		}
	}

	// Waits for ESC or ENTER
	getMasterEvents( [ this ] () {
		if ( saveScore && playerName.size() < 2 ) {
			return;
		} else if ( saveScore ) {
			Scores::save( playerName, points );
			showHighScores = true;
		}

		showHighScores = true;

		// Restart definitions
		points = 0;
		HP = PLAYER_HP;
		currentLevel = 0;
		text_x = WIDTH + 500;
		playerName = "";
	} );
};

void Game::highScores() {
	// Draws
	al_draw_bitmap( mainBackground, 0, 0, NULL );
	al_draw_bitmap( logoImage, WIDTH / 8, 10, NULL );

	// Play the music
	al_set_audio_stream_playing( gameOverMusic, soundActive );

	// Draws messages
	al_draw_text( font17_IG, GREEN, 590, 235, NULL, messages[ "gameOver-playAgain-1" ] );
	al_draw_text( font17_IG, WHITE, 480, 255, NULL, messages[ "gameOver-playAgain-2" ] );
	al_draw_text( font17_IG, RED, 605, 300, NULL, messages[ "gameOver-playAgain-3" ] );
	al_draw_text( font17_IG, WHITE, 565, 320, NULL, messages[ "gameOver-playAgain-4" ] );

	// Shows scores
	Scores::show( font15_NB, [ this ] () {
		// Destroy the current level and load the first level
		LevelsMap::destroy( level );
		LevelsMap::load( level );

		// Restart
		player.init();
		enemies.init( level.numEnemies, level.speedEnemies );
		playerShots.init();
		enemyShots.init();

		gameRunning = true;
		showHighScores = false;

		Scores::lastScore = -1;

		setAudioStreamState( gameOverMusic, false );
	} );
};

void Game::run() {
	static bool increaseHPAfterFinish = false;

	// If the close button was pressed, quit the game 
	if ( event.type == ALLEGRO_EVENT_DISPLAY_CLOSE ) {
		stopQueue = true;

		return;
	}

	if ( HP < 0 )
		HP = 0;

	if ( ( HP == 0 || level.timeRemaing == 0 ) || ( level.pointsToNextLevel != -1 && points >= level.pointsToNextLevel ) )
		gameRunning = false;

	// Get user event
	if ( event.type == ALLEGRO_EVENT_KEY_DOWN ) { 
		switch ( event.keyboard.keycode ) {
			case ALLEGRO_KEY_UP:
				keys[ UP ] = true;
			break;

			case ALLEGRO_KEY_DOWN:
				keys[ DOWN ] = true;
			break;

			case ALLEGRO_KEY_LEFT:
				keys[ LEFT ] = true;
			break;

			case ALLEGRO_KEY_RIGHT:
				keys[ RIGHT ] = true;
			break;

			case ALLEGRO_KEY_SPACE:
				if ( gameRunning )
					player.shoot();

				keys[ SPACE ] = true;
			break;

			case ALLEGRO_KEY_ENTER:
				keys[ ENTER ] = true;

				if ( gameRunning ) {
					stopGame = true;
					gameRunning = false;
				}
			break;

			case ALLEGRO_KEY_ESCAPE:
				keys[ ESCAPE ] = true;

				if ( gameRunning ) {
					stopGame = true;
					gameRunning = false;
				}
			break;

			// CTRL key
			case 217:
				soundActive = !soundActive;
			
				if ( !gameRunning )
					// Stop the main music
					setAudioStreamState( mainBgMusic, false );
				else
					setAudioStreamState( soundTrack, soundActive );
			break;
		}
	}

	if ( event.type == ALLEGRO_EVENT_KEY_UP ) {
		switch ( event.keyboard.keycode ) {
			case ALLEGRO_KEY_UP:
				keys[ UP ] = false;
			break;

			case ALLEGRO_KEY_DOWN:
				keys[ DOWN ] = false;
			break;

			case ALLEGRO_KEY_LEFT:
				keys[ LEFT ] = false;
			break;

			case ALLEGRO_KEY_RIGHT:
				keys[ RIGHT ] = false;
			break;

			case ALLEGRO_KEY_ENTER:
				keys[ ENTER ] = false;
			break;

			case ALLEGRO_KEY_ESCAPE:
				keys[ ESCAPE ] = false;
			break;
		}
	}
	
	if ( event.type == ALLEGRO_EVENT_KEY_CHAR ) {
		keybdUnichar = event.keyboard.unichar;
		keys[ ANY_CHAR ] = true;
	}

	// Sync FPS
	if ( event.type == ALLEGRO_EVENT_TIMER ) {
		// Show High scores
		if ( showHighScores ) {
			return highScores();
		}

		// If the game not start yet
		if ( !gameStarted )
			return startMenu();

		// Pause the game
		if ( stopGame )
			return pause();

		// Game over
		if ( HP == 0 || level.timeRemaing == 0 )
			return gameOver();

		// Next level
		if ( points >= level.pointsToNextLevel ) {
			if ( ( currentLevel + 1 ) != NUM_LEVELS ) {
				return nextLevel();
			} else {
				level.timeRemaing = -1;
				level.pointsToNextLevel = -1;
				gameRunning = true;

				if ( !increaseHPAfterFinish ) {
					increasingHP = true;
					increaseHPAfterFinish = true;
				}
			}
		}

		// Increases the HP bar
		if ( increasingHP && HP < PLAYER_HP ) {
			HP += 2;

			if ( HP >= PLAYER_HP ) {
				HP = PLAYER_HP;
				increasingHP = false;
			}
		}

		// Countdown
		if ( level.timeRemaing != -1 && countdown + 1 < getCurrentTime() ) {
			// Updates
			level.timeRemaing -= 0.02;

			if ( level.timeRemaing < 0 )
				level.timeRemaing = 0;

			countdown = getCurrentTime();
		}

		// Verify the keys state
		if ( keys[ UP ] )
			player.move( UP );
		
		if ( keys[ DOWN ] )
			player.move( DOWN );
		
		if ( keys[ LEFT ] )
			player.move( LEFT );
		
		if ( keys[ RIGHT ] )
			player.move( RIGHT );

		// Release the enemies
		enemies.release( level.numEnemies, level.chancesEnemies, level.enemyLife );

		draws = true;
	}
	
	// If draws = true and the queue event is empty
	if ( draws && al_is_event_queue_empty( queue ) ) {
		draws = false;

		// Updates
		Background::update( level.background );
		player.update();
		playerShots.update( level.numEnemies, level.gainPointsToEnemyDestroyed );
		enemies.update( player, level.numEnemies, level.dropHPColision );
		enemyShots.update( player, level.chancesEnemyShot, level.dropHPShot );
		Explosion::show( level.numEnemies );
		HUD();

		// Back buffer
		al_flip_display();
	}
};

void Game::init() {
	// Starts the primitives addons
	al_init_primitives_addon(); 
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_init_acodec_addon();
	al_install_audio();
	al_reserve_samples( SOUND_CHANNELS );

	// Loading the fonts
	font15_NB = al_load_ttf_font( "fonts/nulshock bd.ttf", 15, NULL );
	font15_IG = al_load_ttf_font( "fonts/Infinium Guardian.ttf", 15, NULL );
	font17_IG = al_load_ttf_font( "fonts/Infinium Guardian.ttf", 17, NULL );
	font25_IG = al_load_ttf_font( "fonts/Infinium Guardian.ttf", 25, NULL );

	// Loading the images
	logoImage = al_load_bitmap( "images/general/logo.png" );
	mainBackground = al_load_bitmap( "images/general/main_background.png" );

	// Loading the sounds
	loadSounds();

	// Hide the mouse cursor
	al_hide_mouse_cursor( display );

	// Starts the events queue
	startEventQueue( &Game::run );
};

Game::Game() {
	// Sets the messages //

	// Errors
	messages[ "error-boxTitle" ] = "Aviso";
	messages[ "error-boxSubTitle" ] = "Erro";
	messages[ "error-message-allegroInitFails" ] = "Falha ao iniciar o Allegro";
	messages[ "error-message-displayInitFails" ] = "Falha ao abrir o display do jogo";

	// Game over
	messages[ "gameOver-title" ] = "GAME OVER";
	messages[ "gameOver-content" ] = "Aperte ESC para sair ou ENTER para continuar";
	messages[ "gameOver-typeYourName" ] = "Digite seu nome: ";
	messages[ "gameOver-playAgain-1" ] = "ENTER";
	messages[ "gameOver-playAgain-2" ] = "para jogar novamente";
	messages[ "gameOver-playAgain-3" ] = "ESC";
	messages[ "gameOver-playAgain-4" ] = "para sair";

	// HUD
	messages[ "HUD-HP" ] = "HP: ";
	messages[ "HUD-points" ] = "Pontos: %d/%d";
	messages[ "HUD-timer" ] = "Tempo: %0.2f";

	// Level passed
	messages[ "LevelPassed-title" ] = "Nivel %d passado!";
	messages[ "LevelPassed-content" ] = "Aperte ENTER para ir a proxima fase";
	messages[ "LevelPassed-quit" ] = "Deseja realmente sair?";

	// General
	messages[ "gameStart" ] = "Aperte ENTER para iniciar o jogo";
	messages[ "gamePaused-title" ] = "JOGO PAUSADO";
	messages[ "gamePaused-content" ] = "Aperte ESC para sair ou ENTER para voltar";
};

Game::~Game() {
	// Destroys everything after execution of the game //

	// General
	al_destroy_display( display );
	al_destroy_timer( timer );
	al_destroy_event_queue( queue );

	// Fonts
	al_destroy_font( font15_NB );
	al_destroy_font( font15_IG );
	al_destroy_font( font17_IG );
	al_destroy_font( font25_IG );

	// Images
	al_destroy_bitmap( logoImage );
	al_destroy_bitmap( mainBackground );
	al_destroy_bitmap( spaceshipPlayer );
	al_destroy_bitmap( playerReached );
	al_destroy_bitmap( spaceshipEnemy );
	al_destroy_bitmap( spaceshipEnemyReached );

	// Audio
	al_destroy_audio_stream( mainBgMusic );
	al_destroy_audio_stream( soundTrack );
	al_destroy_audio_stream( gameOverMusic );
	al_destroy_sample( laser );
	al_destroy_sample( explosion );
	al_destroy_sample( applauses );
	al_destroy_sample( enemyReached );
	al_destroy_sample_instance( inst_laser );
	al_destroy_sample_instance( inst_explosion );
	al_destroy_sample_instance( inst_applauses );
	al_destroy_sample_instance( inst_enemyReached );

	// Explosion images
	if ( gameStarted )
		Explosion::destroyImages();
};

long randomNumber( int x, int y ) {
	 std::random_device seed;
	 std::uniform_real_distribution< double > random( x, y );

	 return random( seed );
};

__int64 getCurrentTime() {
	return std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
	).count();
 }

vector< std::string > split( const std::string str, char delim ) {
	vector< std::string > elems;
	std::stringstream stream( str );
	std::string item;

	for ( ; std::getline( stream, item, delim ); elems.push_back( item ) );

	return elems;
}