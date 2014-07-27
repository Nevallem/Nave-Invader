#include <algorithm>
#include <fstream>
#include <time.h>
#include <string>
#include <vector>
#include "game.hpp"
#include "scores.hpp"

using std::vector;
using std::string;

// Sets initial value
int Scores::lastScore = -1;

vector< vector< string > > Scores::get() {
	string line;
	std::ifstream file( "scores.txt" );
	vector< vector< string > > data;

	// Gets the data of the file
	for ( ; std::getline( file, line ); data.push_back( split( line, ',' ) ) );

	// Closes the file
	file.close();

	return data;
};

void Scores::indexer() {
	size_t i, j;
	vector< string > aux;
	vector< vector< string > > data = Scores::get();

	// Indentify the last score
	data[ MAX_HIGHSCORES ].resize( 4 );

	// Bubble Sort in data
	for ( i = 0; i < data.size(); i++ ) {
		for ( j = 0; j < data.size() - 1; j++ ) {
			if ( atoi( data[ j + 1 ][ 1 ].c_str() ) > atoi( data[ j ][ 1 ].c_str() ) ) {
				aux = data[ j ];
				data[ j ] = data[ j + 1 ];
				data[ j + 1 ] = aux;
			}
		}
	}

	// Removes the score excess
	for ( i = data.size(); i > MAX_HIGHSCORES; data.resize( --i ) );

	// Saves
	save( data );
};

void Scores::save( string name, int points ) {
	std::ofstream file( "scores.txt", std::ofstream::app );
	time_t t = time( NULL );
	struct tm *date = localtime( &t );

	// Save info
	file << name << "," << points << ","
		<< ( date->tm_mday < 10 ? "0" : "" ) << date->tm_mday << "/" // Day
		<< ( date->tm_mon + 1 < 10 ? "0" : "" ) << date->tm_mon + 1 << "/" // Month
		<< date->tm_year + 1900 // Year
		<< std::endl;

	// Close file
	file.close();

	// Index the high score
	indexer();
};

void Scores::save( vector< vector< string > > data ) {
	size_t i;
	std::ofstream file( "scores.txt" );

	// Save the scores
	for ( i = 0; i < data.size(); i++ ) {
		if ( data[ i ].size() == 4 )
			lastScore = i;

		file << data[ i ][ 0 ] << "," << data[ i ][ 1 ] << "," << data[ i ][ 2 ] << std::endl;
	}

	// Close file
	file.close();
};

void Scores::show( ALLEGRO_FONT *font, std::function< void() > callback ) {
	size_t i,
		pos_x = 15,
		pos_y = 180;
	string content;
	vector< vector< string > > data = Scores::get();

	// Drwas the High Scores
	for ( i = 0; i < data.size(); i++ ) {
		content = std::to_string( i + 1 ) + ". " + data[ i ][ 0 ];
		pos_y += 20;
		al_draw_text( font, ( i == lastScore ? ORANGE : WHITE ), pos_x + ( i == 0 ? 6 : 0 ) - ( i == 9 ? 7 : 0 ), pos_y, NULL, content.c_str() );
		al_draw_text( font, ( i == lastScore ? ORANGE : WHITE ), pos_x + 320, pos_y, ALLEGRO_ALIGN_RIGHT, data[ i ][ 1 ].c_str() );
		al_draw_text( font, ( i == lastScore ? ORANGE : WHITE ), pos_x + 450, pos_y, ALLEGRO_ALIGN_RIGHT, data[ i ][ 2 ].c_str() );
	}

	// Wait for ESC ou ENTER
	Game::getMasterEvents( callback );
};