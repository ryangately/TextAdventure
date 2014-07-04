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
string lowerCommand(string);

// global variables
auto done = false; // turn on when the player wins or quits the game
auto travel = false; // turn on when the player changes rooms, then back off
auto room = 0;
string command = "";
int go[4] = {}; // shows the legal movements for a room in order of north, south, east, west
ifstream script;
const string scriptPath = "script.dat"; // location of the local script file

int main()
{
	while (!done)
	{
		cout << newRoom() << endl;
		while (!travel)
		{
			cout << ">";
			cin >> command;
			command = lowerCommand(command);
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

	getline(script, dialog);
	script.close();
	return dialog;
}

// Prints the room description
string newRoom()
{
	string roomKey = "$$" + to_string(room);
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
		string key = "$$HELP";
		return readScript(key);
	}

	// split the command up into its two component parts

	return "DEBUG";
}

// Converts the player's command string to lowercase for processing
string lowerCommand(string com)
{
	int length = com.length();

	for (int i = 0; i < length; i++)
	{
		com.at(i) = tolower(com.at(i));
	}

	return com;
}