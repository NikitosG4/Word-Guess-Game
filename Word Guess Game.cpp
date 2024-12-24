
/*
By: Mykyta Garkusha

Description: "wordle" game, the solution word will be read from the file

*************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream> //standard library for i/o
#include <fstream> //standard library for file i/o
#include <string> // always include this when you use the string class
#include <cstring> //c-string functions
#include <cctype> //touppr function

using namespace std;


/*********************************************************
**********************************************************/
class wordGuess
{
public:
	string solution = "xxxxx"; //word from the file
	string currentGuess = "xxxxx"; //current word guess (must be 5 letters)
	int maxGuesses = 6; //max of 6 guesses
	int guessNumber = 0; //to let the user know which guess they are on
	string row = "*****"; //feedback about the current guess
	string alphabet = "abcdefghijklmnopqrstuvwxyz"; //to rest the lettersLeft for the next round
	string lettersLeft = "abcdefghijklmnopqrstuvwxyz";
	bool wonOrLost = false;
};



//pre and post conditions on the function prototypes
//descriptions on the function definitions
//comments throughout the code making it easier to read

void SetUpRound(wordGuess& currentGame, ifstream& in);
//Precondition: guessNumber, row, lettersLeft, wonOrLost and solution are passed to the function
//Postcondition: variables get their original values and solution gets new word from file, to prepare for next round.
// 
//Description:  get the next word from the file and set up the round

void PlayOneRound(wordGuess& currentGame);
//Precondition: currentGame variables are passed
//Postcondition: sets wonOrLost to true if guess = solution, otherwise guessNumber + 1
// 
//Description:  Play one round of the word guess game

void GetNextGuess(wordGuess& currentGame);
//Precondition: guessNumber and currentGuess are passed and used
//Postcondition: guessNumber is printed, currentGuess read from user input
// 
//Description:  get the next guess word the user

bool ProcessGuess(wordGuess& currentGame);
//Precondition: currentGame variables are passed
//Postcondition: if guess = solution, returns true. otherwise returns false. updates alphabet
// 
//Description:  returns true if the guess matches the solution
//returns false if they do not match
//updates the current row

bool CheckForAnyMatches(wordGuess& currentGame);
//Precondition: solution and guess word are passed to the function
//Postcondition: all variables stay the same.
//
//Description: returns true if any of the letters in the guess word match the solution word

void Display(wordGuess& currentGame);
//Precondition: lettersLeft and row are passed.
//Postcondition: lettersLeft and row are same.
//
////Displays the row and the alphabet letters that are left


void UpdateAlphabet(wordGuess& currentGame);
//Precondition: passed lettersLeft
//Postcondition: replaces used letters with space character
//
//Description: change any letters that have been guessed to space

void UpdateRow(wordGuess& currentGame);
//Precondition: passed row
//Postcondition: row is reset, then its printed with correct letters and those in right place are capitalized.
//Description: Changes letters in the "starword" row
//Correct letters in the right place are presented as capital letters
//Correct letters in a different location are presented as lowercase letters
//


//THE main function

int main()
{
	
	ifstream in;

	//connecting to the input file:
	in.open("words.txt");

	if (in.fail())//did not find the file
	{
		cout << "Input file did not open correctly" << endl;
		exit(1);
	}

	//instance of wordGame declared
	wordGuess wordGame;
    char playAgain;
	//ask the user if they want to play?//initialization
	cout << "Do you want to play the letter guessing game (we will only be guessing lowercase letters)? " << endl;
	cin >> playAgain;
	//loop TEST
	while (playAgain == 'y' || playAgain == 'Y')
	{
	    cout << "\nIt is assumed you will enter a word in all lowercase letters and FIVE differents letters.\n";
	    cout << "The program will not continue unless you enter a 5 letter word!\n";
	    cout << "\nCorrect letters in the right place are presented as capital letters.\n";
	    cout << "Correct letters in a different location are presented as lowercase letters.\n";
		cout << "\n**********NEW ROUND**********\n";
		//set up and play a round
		SetUpRound(wordGame, in); // processes set up
        PlayOneRound(wordGame); // processes one round of the game

		//again? loop update
		cout << "Do you want to play the another round? (y or n): " << endl;
	    cin >> playAgain;

	}
	return  0;
}


void UpdateRow(wordGuess& currentGame)
//Description: Changes letters in the "starword" row
//Correct letters in the right place are presented as capital letters
//Correct letters in a different location are presented as lowercase letters
{
    currentGame.row = "*****"; // reset row
    for (int i = 0; i < currentGame.solution.length(); i++) {
        if (currentGame.currentGuess[i] == currentGame.solution[i]) {
            currentGame.row[i] = toupper(currentGame.currentGuess[i]); // show letter is correct, in right spot
        } else if (currentGame.solution.find(currentGame.currentGuess[i]) != string::npos) {
            currentGame.row[i] = tolower(currentGame.currentGuess[i]); // show if letter is correct, in wrong spot
        }
    }
    
}


bool CheckForAnyMatches(wordGuess& currentGame)
//Description: returns true if any of the letters in the guess word match the solution word
{
	//use nested loops
	//outer loop from 0 to currentGame.solution.length()/*5*/
	for (int i = 0; i < currentGame.solution.length(); i++)
	{
		//inner loop from 0 to currentGame.currentGuess.length()/*5*/
		for (int j = 0; j < currentGame.currentGuess.length(); j++)
		{
			if (currentGame.solution[i] == currentGame.currentGuess[j]) // there is a match
			{
				return true; //there is at least one letter that matches
			}
		}
	}
	return false; //None of the letters match
}


void UpdateAlphabet(wordGuess& currentGame)
//Description: change any letters that have been guessed to space
{
	//use nested loops
	//outer loop from 0 to currentGame.lettersLeft.length()/*26*/
	for (int i = 0; i<currentGame.lettersLeft.length(); i++) // this and next lopp go through alphabet and guess 
	{
		//inner loop from 0 to currentGame.currentGuess.length()/*5*/
		for (int j = 0; j < currentGame.currentGuess.length(); j++)
		{
			if (currentGame.lettersLeft[i] == currentGame.currentGuess[j]) 
			{
                //remove the letter from the array, change it to a space
                currentGame.lettersLeft[i] = ' '; // replaces letter in i with space character
			}
        
		}
            
	}
	cout << currentGame.lettersLeft << endl;
}


void Display(wordGuess& currentGame)
//Displays the row and the alphabet letters that are left
{
	cout << endl << currentGame.row; // prints row
	cout << "\nLetters:  " << currentGame.lettersLeft << endl; // prints the letters that havent been used
}



void SetUpRound(wordGuess& currentGame, ifstream& in)
//Description:  get the next letter from the file
{
	cout << "\nSetting up a new round of the word guess game!\n";

	//reset all the member variable values
    currentGame.guessNumber = 0;
    currentGame.row = "*****";
    currentGame.lettersLeft = "abcdefghijklmnopqrstuvwxyz";
    currentGame.wonOrLost = false;
	//get the next word from the file
	in >> currentGame.solution;

}


void PlayOneRound(wordGuess& currentGame)
//Description:  Play one round of the number guess game
{

	//for the number of maxGuesses
	for (int i = 0; i < currentGame.maxGuesses; i++)
	{

		//get the next guess
		GetNextGuess(currentGame);
		if (ProcessGuess(currentGame)) //they guessed correctly
		{
			cout << "\nYou won that round!\n";
			currentGame.wonOrLost = true;
			i = currentGame.maxGuesses; //end the loop without break
		}
        
	}
	if (!currentGame.wonOrLost) {//lost the round
		cout << "\nYou did not win that round!\n";
	}
	cout << "\nTHE SOLUTION WAS '" << currentGame.solution << "'" << endl;

}



void GetNextGuess(wordGuess& currentGame)
//Description:  get the next guess from the user
{
	Display(currentGame); //display the updated row
	// Make sure the word has 5 letters
	do {
		cout << "\nEnter a word (" << currentGame.guessNumber + 1 << "): ";

		cin >> currentGame.currentGuess; // get a guess from user
		if (currentGame.currentGuess.length() != 5) { // if user does not enter word of exectly 5 letters
            cout << "Enter exactly 5 letters for each guess.\n";
        }

	} while (currentGame.currentGuess.length() != 5); //length of five letters 
	currentGame.guessNumber ++; // increase guess number by 1
}



bool ProcessGuess(wordGuess& currentGame)
//Description:  returns true if the guess matches the solution
//returns false if they do not match
{
	//update the alphabet
    UpdateAlphabet(currentGame); 
	if (currentGame.currentGuess == currentGame.solution)//they guessed the word, return true
	{
		cout << "\nYou guessed the word!\n";
		return true;
	}
	else if (CheckForAnyMatches(currentGame))//check for any match matches
	{
		cout << "\nThere are letters that match!\n";

		//update the row
		UpdateRow(currentGame); 
		//add one to the guessNumber
		
		return false; //did not get a match
	}
	else // no letters matched
	{
		cout << "\nThere are NO letters that match!\n";
		return false;
	}
}


