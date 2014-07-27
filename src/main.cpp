/**
 * @author Roger Lima (rogerlima@outlook.com)
 * @date 27/jul/2014
 */
#include <allegro5/allegro_native_dialog.h>
#include "game.hpp"

int main() {
	// Intance of main class
	Game game;

	// If the allegro init fail
	if ( !al_init() ) {
		al_show_native_message_box( NULL, game.messages[ "error-boxTitle" ], game.messages[ "error-boxSubTitle" ], game.messages[ "error-message-allegroInitFails" ], NULL, ALLEGRO_MESSAGEBOX_ERROR );

		return EXIT_FAILURE;
	}

	// Create the display
	game.display = al_create_display( WIDTH, HEIGHT );

	// If the opening of the display fail
	if ( !game.display ) {
		al_show_native_message_box( NULL, game.messages[ "error-boxTitle" ], game.messages[ "error-boxSubTitle" ], game.messages[ "error-message-displayInitFails" ], NULL, ALLEGRO_MESSAGEBOX_ERROR );

		return EXIT_FAILURE;
	}

	// Starts the game
	game.init();

	return EXIT_SUCCESS;
}