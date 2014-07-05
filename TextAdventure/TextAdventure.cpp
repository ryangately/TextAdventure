/*
Created by Ryan Gately on 7/2/14
Text Adventure game!
*/

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

// prototypes
string readScript(string);
string newRoom();
string parser();
void lowerCommand();

// global variables
auto done = false; // turn on when the player wins or quits the game
auto travel = false; // turn on when the player changes rooms, then back off
auto room = 0;
string command = "";
int go[4] = {}; // shows the legal movements for a room in order of north, south, east, west
string look[9] = {}; // the legal targets for the look command for the current room
string take[3] = {};
string inventory[9] = {"compass"};
ifstream script;
const string scriptPath = "script.dat"; // location of the local script file

// MAIN function
// Loops through the user's commands and script responses
// until the done flag is turned on.
int main()
{
	while (!done)
	{
		cout << newRoom() << endl;
		while (!travel)
		{
			cout << '>';
			getline(cin, command);
			lowerCommand();
			cout << parser() << endl;
		}
		travel = false;
	}
	return 0;
}

// Reads the script starting at the input token and returns
// a string for printing
string readScript(string key)
{
	string dialog = "";
	string marker = "";

	script.open(scriptPath);
	while (marker != key)
	{
		getline(script, marker);
	}

	getline(script, marker);
	while (marker != "##")
	{
		if (dialog == "")
		{
			dialog += marker;
		}
		else
		{
			dialog += "\n" + marker;
		}
		getline(script, marker);
	}

	script.close();
	return dialog;
}

// Prints the room description
string newRoom()
{
	string roomKey = "$$" + to_string(room);
	string roomVar = "%%" + to_string(room);

	//
	// Load the GO, LOOK, and TAKE arrays from the script
	//

	// Load the GO array from the script
	string marker = "";

	script.open(scriptPath);
	while (marker != roomVar)
	{
		getline(script, marker);
	}

	script >> marker;
	for (int i = 0; i < 4; i++)
	{
		go[i] = stoi(marker);
		script >> marker;
	}
	// End GO loading.

	// Load the LOOK array from the script
	auto count = 0;
	while (marker != "null")
	{
		look[count++] = marker;
		script >> marker;
	}
	// End LOOK loading.

	// Load the TAKE array from the script
	count = 0;
	script >> marker;
	while (marker != "null")
	{
		take[count++] = marker;
		script >> marker;
	}
	// End TAKE loading.

	script.close();

	return readScript(roomKey);
}

// Parses the player's command and prints a response from the script
// or an error if the command is invalid
// or quits the game if the player types QUIT
string parser()
{
	if (command == "quit")
	{
		travel = true;
		done = true;
		return "Goodbye.";
	}
	else if (command == "help")
	{
		return readScript("$$HELP");
	}

	// split the command up into its two component parts
	string com = "";
	string tar = "";
	auto split = false;
	for (unsigned int i = 0; i < command.length(); i++)
	{
		if (command.at(i) != ' ' && !split)
		{
			com += command.at(i);
		}
		else if (command.at(i) != ' ' && split)
		{
			tar += command.at(i);
		}
		else
		{
			split = true;
		}
	}

	// Process the GO command.
	if (com == "go")
	{
		if (tar == "north" && go[0] == 1)
		{
			room += 3;
			travel = true;
			return "You go north.";
		}
		else if (tar == "south" && go[1] == 1)
		{
			room -= 3;
			travel = true;
			return "You go south.";
		}
		else if (tar == "east" && go[2] == 1)
		{
			room += 1;
			travel = true;
			return "You go east.";
		}
		else if (tar == "west" && go[3] == 1)
		{
			room -= 1;
			travel = true;
			return "You go west.";
		}
		else
		{
			return "You can't go that way.";
		}
	}

	// Process the LOOK command
	// Check all of the possible look options in the array
	// If there is a match, pull the description from the script.
	else if (com == "look")
	{
		for (int i = 0; i < 9; i++)
		{
			if (tar == look[i])
			{
				return readScript("@@" + to_string(room) + look[i]);
			}
		}
		return "You can't do that.";
	}

	// Process the TAKE command
	// Check all of the available items by looking through the array.
	// If there is a match, iterate through the user's
	// inventory and add it to the first empty spot.
	else if (com == "take")
	{
		for (int i = 0; i < 3; i++)
		{
			if (tar == take[i])
			{
				for (int n = 0; n < 9; n++)
				{
					if (inventory[n] == "")
					{
						inventory[n] = tar; // add the item to the player inventory
						take[i] = ""; // remove the item from the room's take array
						break;
					}
				}
				return ("You take the " + tar + '.');
			}
		}
		return "You can't do that.";
	}

	// Shows the list of items in the player's inventory.
	else if (com == "inventory")
	{
		string invList = "Items in your inventory:";
		for (int i = 0; inventory[i] != ""; i++)
		{
			invList += ('\n' + inventory[i]);
		}
		return invList;
	}

	// DEBUG command
	// Comment this section for release builds!
	else if (com == "debug")
	{
		// DEBUG -- Print the target arrays
		cout << go[0] << go[1] << go[2] << go[3] << '\n';
		cout << look[0] << look[1] << look[2] << '\n';
		cout << take[0] << '\n';

		//cout << com << ' ' << tar << endl; // DEBUG
	}
	
	return "DEBUG";
}

// Converts the player's command string to lowercase
void lowerCommand()
{
	int length = command.length();

	for (int i = 0; i < length; i++)
	{
		command.at(i) = tolower(command.at(i));
	}
}