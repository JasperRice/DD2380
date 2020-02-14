#include "player.hpp"

#include <stdlib.h>
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    // Parse parameters
    bool init = false;
    bool verbose = false;
    bool fast = false;
    for (int i = 1; i < argc; ++i)
    {
        std::string param(argv[i]);
        if (param == "init" || param == "i")
            init = true;
        else if (param == "verbose" || param == "v")
            verbose = true;
        else if (param == "fast" || param == "f")
            fast = true;
        else
        {
            std::cerr << "Unknown parameter: '" << argv[i] << "'" << std::endl;
            return -1;
        }
    }

    // Start the game by sending the starting board without moves if the parameter "init" is given
    if (init)
    {
        std::string message = checkers::GameState().toMessage();
        std::cerr << "Sending initial board: '" << message << "'" << std::endl;
        std::cout << message << std::endl;
    }

    checkers::Player player;

    std::string input_message;
    while (std::getline(std::cin, input_message))
    {

        // Get game state from standard input
        //std::cerr << "Receiving: '" << input_message << "'" << std::endl;
        checkers::GameState input_state(input_message);

        // See if we would produce the same message
        if (input_state.toMessage() != input_message)
        {
            std::cerr << "*** ERROR! ***" << std::endl;
            std::cerr << "Interpreted: '" << input_message << "'" << std::endl;
            std::cerr << "As:          '" << input_state.toMessage() << "'" << std::endl;
            std::cerr << input_state.toString(input_state.getNextPlayer()) << std::endl;
            assert(false);
        }

        // Print the input state
        if (verbose)
        {
            std::cerr << input_state.toMessage() << std::endl;
            std::cerr << input_state.toString(input_state.getNextPlayer()) << std::endl;
        }

        // Quit if this is end of game
        if (input_state.getMove().isEOG())
            break;

        // Deadline is one second from when we receive the message
        checkers::Deadline deadline = checkers::Deadline::now() + (fast ? 0.1 : 1.0);

        // Figure out the next move
        checkers::GameState output_state = player.play(input_state, deadline);

		if (deadline < checkers::Deadline::now()) {
			exit(152);
		}

        // Print the output state
        if (verbose)
        {
            std::cerr << output_state.toMessage() << std::endl;
            std::cerr << output_state.toString(input_state.getNextPlayer())    << std::endl;
        }

        // Send the next move
        std::string output_message = output_state.toMessage();
        //std::cerr << "Sending: '" << output_message << "'"<< std::endl;
        std::cout << output_message << std::endl;

        // Quit if this is end of game
        if (output_state.getMove().isEOG())
            break;
    }
}
