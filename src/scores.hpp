#pragma once

// Scores of the player
class Scores {
private:
	// Index the high scores
	static void indexer();
public:
	// Last score of the player
	static int lastScore;

	// Gets all scores
	static std::vector< std::vector< std::string > > get();

	// Saves a score
	//
	// @param {std::string} Name
	// @param {int} Points
	static void save( std::string, int );

	// Saves a list of scores
	//
	// @param {std::vector< std::vector< std::string > >} High scores
	static void save( std::vector< std::vector< std::string > > );

	// Shows the scores
	//
	// @param {ALLEGRO_FONT *} Font
	// @param {std::function< void() >} Callback function, after ENTER button be pressed
	static void show( ALLEGRO_FONT *, std::function< void() > );
};