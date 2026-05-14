#include "../../include/my_shell.h"

int command_exit()
{
        printf(
    "             /\n"
    "            /  \\\n"
    "           /++++\\\\\n"
    "          /++++++\\\\\n"
    "         /++++++++\\\\\n"
    "        /++++++++++\\\\\n"
    "       /____________\\\\\n"
    "          .-''''-.\n"
    "        .'  o  o  '.\n"
    "       /     ^      \\\n"
    "      |   \\_____/   |\n"
    "      |             |\n"
    "       \\  \\___/   /\n"
    "        '._____.-'\n"
    "         /|   |\\\\\n"
    "        /_|___|_\\\\\n"
    "          /   \\\n"
    "         /_____\n"
    "\n"
    "   Thanks for using Mega Shell ❄️\n"
    );
    disable_raw_mode();
    exit(EXIT_SUCCESS);
}