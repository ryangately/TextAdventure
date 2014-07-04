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
ifstream script;
const string scriptPath = "script.dat"; // location of the local script file

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
	// Load all three target arrays from the script
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

	// DEBUG -- Print the target arrays
	//cout << go[0] << go[1] << go[2] << go[3] << '\n';
	//cout << look[0] << look[1] << look[2] << '\n';
	//cout << take[0] << '\n';

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
	for (int i = 0; i < command.length(); i++)
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

	//cout << com << ' ' << tar << endl; // DEBUG
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