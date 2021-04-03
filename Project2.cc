#include <iostream>
#include <fstream>
#include <iomanip>
#include <conio.h>
#include <chrono>
#include <windows.h>
#include <cctype>
#include<bits/stdc++.h>

using namespace std;

/*
My game is an escape room. To win, you must exit with the needed Key Card item. To acquire the Key Card item, you must complete a series of games.

To move, use WASD.
W = UP
A = LEFT
S = DOWN
D = RIGHT

Any further needed instructions will be provided along the way.
The points you get is the final time it takes you to escape the room.
*/

void viewHighScores();
void mainMenu();
void printHighScores(string theArray0[], int theArray[], int numberUsed);
void addHighScore(string playerName, int playerTime);
string moveMap(char theMap[][14], int& xPos, int& yPos, char& face, int interact, string playerName, string& inventory, string& notes);
void printMap(char theMap[][14], int xStart, int xStop, int yStart, int yStop, int xPos, int yPos, char face);
bool ticTacToe();
void clickToWin();
void newLine(istream& someStream);
bool rockPaperScissors();
void mazeGame();

void enterName(string& playerName)
{
    bool isValid(true);

    do
    {
        cout << "Enter your name: ";
        cin >> playerName;
        newLine(cin);
        isValid = true;
        if (playerName.length() > 7)
        {
            isValid = false;
            cout << "The name must be less than 8 characters!\n";
        }
    } while (!isValid);
}

int inspect(int xPos, int yPos)
{
    int specialCoordinates[15][2] = {{4,1}, {2, 8}, {6, 8}, {2, 2}, {3, 2}, {5, 2}, {6, 2}, {2, 5}, {3, 5}, {5, 5}, {6, 5}, {2, 10}, {2, 11}, {6, 10}, {6, 11}};
    for (int i = 0; i < 15; i++)
    {
        if (xPos == specialCoordinates[i][0] && yPos == specialCoordinates[i][1])
        {
            cout << "Press X to interact with ";
            switch(i)
            {
                case 0: cout << "exit door.\n"; break;
                case 1: cout << "door to the 3D printer lab.\n"; break;
                case 2: cout << "door to the professor's office.\n"; break;
                case 11:
                case 13: cout << "door to exit to the computer room.\n"; break;
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10: cout << "computer.\n"; break;
                case 12: cout << "3D printer.\n"; break;
                case 14: cout << "professor's desk.\n"; break;
            }
            return i;
        }
    }
    return -1;
}

void playNewGame()
{
    char theMap[9][14] =   {    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '-', '-', '-', '-', '-'},
                                {'-', '-', '-', '-', '-', '-', '-', '-', '-', '|', ' ', ' ', ' ', '|'},
                                {'|', ' ', ' ', 'D', ' ', ' ', 'D', ' ', ' ', '#', ' ', ' ', 'E', '|'},
                                {'|', ' ', ' ', 'D', ' ', ' ', 'D', ' ', ' ', '|', ' ', ' ', ' ', '|'},
                                {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', '-', '-', '-', '|'},
                                {'|', ' ', ' ', 'D', ' ', ' ', 'D', ' ', ' ', '|', ' ', ' ', 'X', '|'},
                                {'|', ' ', ' ', 'D', ' ', ' ', 'D', ' ', ' ', '#', ' ', ' ', 'X', '|'},
                                {'-', '-', '-', '-', '-', '-', '-', '-', '-', '|', ' ', ' ', 'X', '|'},
                                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '-', '-', '-', '-', '-'}
                            };
    string playerName, inventory(""), notes("");
    int playerTime, xPos(4), yPos(1), interact(-1);
    char face('>');
    enterName(playerName);
    cout << "TIP: The faster you escape, the higher you'll end up on the leaderboard!\nLoading game...\n\n";
    Sleep(3000);
    cout << "Uh oh, you fell asleep during class and now everybody is gone. You get up to leave to find that the door is locked- you're TRAPPED! Don't  panic. Find the clues to escape.\n";
    auto beginTime = chrono::steady_clock::now();
    printMap(theMap, 1, 8, 0, 10, xPos, yPos, face);
    cout << "Use WASD to move!\n";
    while ("Game Over" != moveMap(theMap, xPos, yPos, face, inspect(xPos, yPos), playerName, inventory, notes))
    {
        printMap(theMap, 1, 8, 0, 10, xPos, yPos, face); //MainMap
    }
    auto endTime = chrono::steady_clock::now();
    playerTime = chrono::duration_cast<chrono::seconds>(endTime - beginTime).count();
    addHighScore(playerName, playerTime);
    cout << "You've escaped with a time of " << playerTime/60 << " minutes and " << playerTime%60 << " seconds!\nPress any key to view high scores!\n";
    getch();
    viewHighScores();
}

bool login(string info, string prompt, string answer, string correctMsg, string incorrectMsg)
{
    string input;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << info;
    do
    {
        cout << prompt;
        cin >> input;
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        if (input == "exit")
            return false;
        else if (input == answer)
        {
            cout << correctMsg << endl;
            Sleep(1000);
            return true;
        }
        else
        {
            cout << endl << incorrectMsg << endl;
        }
    } while (true);
}

string moveMap(char theMap[][14], int& xPos, int& yPos, char& face, int interact, string playerName, string& inventory, string& notes)
{
    char input('0');
    bool isValid(true);
    do
    {
        input = getch();
        isValid = true;
        switch(input)
        {
            case 'w':
            case 'W':
                face = '^'; xPos--; break;
            case 'a':
            case 'A':
                face = '<'; yPos--; break;
            case 's':
            case 'S':
                face = 'v'; xPos++; break;
            case 'd':
            case 'D':
                face = '>'; yPos++; break;
            case 'i':
            case 'I':
                cout << "~~~~~~~~~~~~~~~~~~~~\nINVENTORY\n" << inventory << "\nNOTES\n" << notes << "\n~~~~~~~~~~~~~~~~~~~~\n";
                Sleep(1000);
                break;
            case 'x':
            case 'X':
                switch(interact)
                {
                    case -1: cout << "There's nothing near you to interact with!\n"; break;
                    case 0:
                        if(1 + inventory.find("Card"))
                        {
                            return "Game Over";
                        }
                        else
                        {
                            cout << "The exit door requires a key card to be unlocked!\n";
                            Sleep(1000);
                        }
                        break;
                    case 1:
                        if (login("This door to the 3D Printing Lab is locked by a number pad. (Input \"exit\" to leave door)\n\n 1 | 2 | 3 \n-----------\n 4 | 5 | 6 \n-----------\n 7 | 8 | 9 \n-----------\n * | 0 | #\n\n", "Input the correct 6-digit code to unlock the door: ", "101498", "Unlocking door...", "Incorrect code!"))
                        {
                            xPos = 2;
                            yPos = 10;
                            face = '>';
                            printMap(theMap, 0, 5, 9, 14, xPos, yPos, face);
                            while ("Main Room" != moveMap(theMap, xPos, yPos, face, inspect(xPos, yPos), playerName, inventory, notes))
                            {
                                printMap(theMap, 0, 5, 9, 14, xPos, yPos, face);
                            }
                            xPos = 2;
                            yPos = 8;
                            face = '<';
                        }
                        break;
                    case 2: if (1 + inventory.find("Plastic"))
                        {
                            xPos = 6;
                            yPos = 10;
                            face = '>';
                            cout << "The key you printed fits in the professors door!\nUnlocking door...\n";
                            Sleep(1000);
                            printMap(theMap, 4, 9, 9, 14, xPos, yPos, face);
                            while ("Main Room" != moveMap(theMap, xPos, yPos, face, inspect(xPos, yPos), playerName, inventory, notes))
                            {
                                printMap(theMap, 4, 9, 9, 14, xPos, yPos, face);
                            }
                            xPos = 6;
                            yPos = 8;
                            face = '<';
                        }
                        else
                        {
                            cout << "You need a key to unlock this door!\n";
                            Sleep(1000);
                        }
                        break;
                    case 3:
                    case 5:
                    case 7:
                    case 9: cout << "This computer doesn't seem to be working... try a different one.\n"; Sleep(500); break;
                    case 4:
                        if (notes.find("02052000") == -1 && login("The screen reads...\nStation 2.2 (Input \"exit\" to leave computer)\n\nLOGIN\n\nHint: 4 Digits\n", "Password: ", "0706", "Logging in...", "Incorrect Password!"))
                        {
                            if (ticTacToe())
                            {
                                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                                Sleep(1000);
                                cout << "'02052000' has been added to your notes!\n";
                                notes += "-02052000\n";
                            }
                        }
                        else if (notes.find("02052000") != -1)
                        {
                            cout << "You've already learned the code '02052000'!\n";
                            Sleep(1000);
                        }
                        break;
                    case 6:
                        if (notes.find("0706") == -1 && login("The screen reads...\nStation 2.4 (Input \"exit\" to leave computer)\n\nLOGIN\n\nHint: \"password\"\n", "Password: ", "password", "Logging in...", "Incorrect Password!"))
                        {
                            clickToWin();
                            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                            Sleep(1000);
                            cout << "'0706' has been added to your notes!\n";
                            notes += "-0706\n";
                        }
                        else if (notes.find("0706") != -1)
                        {
                            cout << "You've already learned the code '0706'\n";
                            Sleep(1000);
                        }
                        break;
                    case 8:
                        if (inventory.find("Blueprint") == -1 && login("The screen reads...\nStation 1.2 (Input \"exit\" to leave computer)\n\nLOGIN\n\nHint: 8-digit code\n", "Password: ", "02052000", "Logging in...", "Incorrect Password!"))
                        {
                            if (rockPaperScissors())
                            {
                                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                                Sleep(1000);
                                cout << "USB with Blueprint has been added to your inventory!\n";
                                inventory += "-USB with Blueprint\n";
                            }
                        }
                        else if (inventory.find("Blueprint") != -1)
                        {
                            cout << "You've already found USB with Blueprint!\n";
                            Sleep(1000);
                        }


                        break;
                    case 10:
                        if (notes.find("101498") == -1 && login("The screen reads...\nStation 1.4 (Input \"exit\" to leave computer)\n\nLOGIN\n\nHint: Bob's father has 5 kids: Ha, He, Hi, Ho, and __.\n", "Password: ", "bob", "Logging in...", "Incorrect Password!"))
                        {
                            mazeGame();
                            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                            Sleep(1000);
                            cout << "'101498' has been added to your notes!\n";
                            notes += "-101498\n";
                        }
                        else if (notes.find("101498") != -1)
                        {
                            cout << "You've already learned the code '101498'!\n";
                            Sleep(1000);
                        }
                        break;
                    case 11:
                    case 13: return "Main Room";
                    case 12:
                        if((1 + inventory.find("Blueprint")) && (inventory.find("Plastic") == -1))
                        {
                            cout << "You insert the USB with Blueprint and it begins printing something.\n";
                            Sleep(2000);
                            cout << "You grab the print and begin peeling the excess plastic. \n";
                            Sleep(2000);
                            cout << "Oh, it's a plastic key! This might be useful for unlocking something...\n";
                            Sleep(2000);
                            cout << "Plastic Key added to your inventory!\n";
                            Sleep(2000);
                            inventory += "-Plastic Key\n";
                        }
                        else if (inventory.find("Blueprint") == -1)
                        {
                            cout << "Hmm... it looks like it needs a blueprint before it can do anything...\n";
                            Sleep(1000);
                        }
                        else if (inventory.find("Plastic") != -1)
                        {
                            cout << "You've already made a Plastic Key!\n";
                            Sleep(1000);
                        }
                        break;
                    case 14:
                        if (inventory.find("Card") == -1)
                        {
                            cout << "You scan your professor's desk and find a cookie, an old boba drink, and...\n";
                            Sleep(2000);
                            cout << "A Key Card! Nice, now you can finally get out!\n";
                            Sleep(1000);
                            cout << "Key Card has been added to your inventory!\n";
                            inventory += "-Key Card\n";
                        }
                        else if (inventory.find("Card") != -1)
                        {
                            cout << "You've already found Key Card!\n";
                            Sleep(1000);
                        }

                }
                break;
            default: isValid = false;
        }
        if (theMap[xPos][yPos] != ' ')
        {
            switch(input)
            {
                case 'w':
                case 'W':
                    xPos++; break;
                case 'a':
                case 'A':
                    yPos++; break;
                case 's':
                case 'S':
                    xPos--; break;
                case 'd':
                case 'D':
                    yPos--; break;
            }
        }
    } while (!isValid);
    return "Game Continues";
}

void printMap(char theMap[][14], int xStart, int xStop, int yStart, int yStop, int xPos, int yPos, char face)
{
    cout << "\nPress I to open inventory & view notes!\n";
    for (int i = xStart; i < xStop; i++)
    {
        for (int k = yStart; k < yStop; k++)
        {
            if (xPos == i && yPos == k)
                cout << face << " ";
            else
                cout << theMap[i][k] << " ";
        }
        cout << endl;
    }
}

void mySwap(int& n1, int& n2)
{
    int temp = n2;
    n2 = n1;
    n1 = temp;
}

void mySwap(string& n1, string& n2)
{
    string temp = n2;
    n2 = n1;
    n1 = temp;
}

int findNextSmallestIndex(int theArray[], int numberUsed, int startingIndex)
{
    int smallestIndex = startingIndex;
    for (int i = startingIndex + 1; i < numberUsed; i++)
    {
        if (theArray[smallestIndex] > theArray[i])
            smallestIndex = i;
    }
    return smallestIndex;
}

void sortHighScores(string highScoreNames[], int highScoreTimes[], int numberUsed)
{
    for (int i = 0; i < numberUsed - 1; i++)
    {
        int nextSmallestIndex = findNextSmallestIndex(highScoreTimes, numberUsed, i);
        mySwap(highScoreTimes[i], highScoreTimes[nextSmallestIndex]);
        mySwap(highScoreNames[i], highScoreNames[nextSmallestIndex]);
    }
}

int findInsertLocation(int theArray[], int numberUsed, int playerTime)
{
    for (int i = 0; i < numberUsed; i++)
    {
        if (playerTime <= theArray[i])
            return i;
    }
    return -1;
}

void addHighScore(string playerName, int playerTime)
{
    //Declare variables
    string highScoreNames[11];
    int highScoreTimes[11] = {0}, counter(0);
    ifstream inputFile;
    ofstream outputFile;

    //Open highScores.txt
    inputFile.open("highScores.txt");
    if (inputFile.fail())
    {
        cout << "ERROR: \"highScores.txt\" not found!\n";
        exit(1);
    }

    //Transfer scores from text file to arrays
    while (inputFile >> highScoreNames[counter] >> highScoreTimes[counter] && counter++ < 10);

    inputFile.close();

    sortHighScores(highScoreNames, highScoreTimes, counter);
    int index = findInsertLocation(highScoreTimes, counter, playerTime);
    if (index + 1)
    {
        for (int i = counter; i > index; i--)
        {
            highScoreNames[i] = highScoreNames[i - 1];
            highScoreTimes[i] = highScoreTimes[i - 1];
        }
        highScoreNames[index] = playerName;
        highScoreTimes[index] = playerTime;
    }
    else
    {
        highScoreNames[counter] = playerName;
        highScoreTimes[counter] = playerTime;
    }
    //Open highScores.txt
    outputFile.open("highScores.txt", ofstream::out | ofstream::trunc);

    //Transfer scores from arrays to text file
    for (int i = 0; i < counter + 1 && i < 10; i++)
    {
        outputFile << highScoreNames[i] << " " << highScoreTimes[i] << endl;
    }

    outputFile.close();
}

void viewHighScores()
{
    //Declare variables
    string highScoreNames[10];
    int highScoreTimes[10] = {0}, counter(0);
    ifstream inputFile;

    //Open highScores.txt
    inputFile.open("highScores.txt");
    if (inputFile.fail())
    {
        cout << "ERROR: \"highScores.txt\" not found!\n";
        exit(1);
    }

    //Display high scores
    cout.setf(ios::left);
    cout << "\n   HIGH SCORES ---------------\n\t   " << setw(7) << "NAMES" << " TIME\n";
    while (inputFile >> highScoreNames[counter] >> highScoreTimes[counter] && counter < 10)
    {
        cout << "\t" << counter++ + 1 << ". " << setw(7) << highScoreNames[counter] << " " << highScoreTimes[counter]/60 << "m " << highScoreTimes[counter]%60 << "s " << endl;
    }
    cout << "   ---------------------------\nPress any button to return to the main menu!\n\n";

    //Return to main menu
    inputFile.close();
    _getch();
    mainMenu();
}

void newLine(istream& someStream)
{
    char c;
    do
    {
        someStream.get(c);
    } while (c != '\n');
}

void mainMenu()
{
    char startMenuInput('0');
    bool isValid(true);
    cout << "   ~~ESCAPE THE CLASSROOM~~\n\n   MAIN MENU ---------------\n\tA. Play New Game\n\tB. View High Scores\n\tC. Exit Game\n   -------------------------\nMake a selection: ";
    do
    {
        cin >> startMenuInput;
        newLine(cin);
        isValid = true;
        switch (startMenuInput)
        {
            case 'A':
            case 'a': playNewGame(); break;
            case 'B':
            case 'b': viewHighScores(); break;
            case 'C':
            case 'c': cout << "Game exited. Please press any button to close the window!\n";
                exit(0);
            default: cout << "Please make a valid selection: ";
                isValid = false;
        }
        newLine(cin);
    } while (!isValid);
}

void clickToWin()
{
    bool isValid(true), isFixed(false);
    int n1(rand()%2+1), counter(0);
    cout << " ______________________________\n | __________________________ |\n | |                        | |\n | |                        | |\n | |                        | |\n | |                        | |\n | |                        | |\n | |                        | |\n | |________________________| |\n |____________________________|\nThis computer looks important, but the screen is all black... Press any key to try and fix it!\n";
    do
    {
        _getch();
        cout << "Hmm, let's see if that worked...\n";
        Sleep(500);
        if (counter++ == n1)
            isFixed = true;
        else
            cout << "No response...Try again!\n";
    } while (!isFixed);
    cout << "Nice job! The computer is looks like it's working now!\nBooting up...\n";
    Sleep(2000);
    cout << " ______________________________\n | __________________________ |\n | |XXXXXXX0XXXXXXXXXXXXXXX7| |\n | |XXXXXXXXXXXXXXXXXXXXXXXX| |\n | |XXXXXXXXXX0XXXXXXXXXXXXX| |\n | |XXXXXXXXXXXXXXXXXXXXXXXX| |\n | |6XXXXXXXXXXXXXXXXXXXXXXX| |\n | |XXXXXXXXXXXXXXXXXXXXXXXX| |\n | |________________________| |\n |____________________________|\nWhoa, that's a lot of junk! Maybe there's some sort of password hidden in here...\nPress any key to leave the computer.\n";
    _getch();
}

//TIC TAC TOE START HERE
void printTTT(char theArray[])
{
    cout << "\n " << theArray[0] << " | " << theArray[1] << " | " << theArray[2] << "\n---+---+---\n " << theArray[3] << " | " << theArray[4] << " | " << theArray[5] << "\n---+---+---\n " << theArray[6] << " | " << theArray[7] << " | " << theArray[8] <<  "\n\n";
}

void tTTPlayerMove(char theArray[])
{
    char c1('0');
    int n1(0);
    bool isValid(false);
    cout << "Your move.\n";
    do
    {
        isValid = false;
        c1 = getch();
        n1 = c1 - 48;
        if ((n1 > 1 || n1 < 9) && theArray[n1-1] == c1)
        {
            isValid = true;
            theArray[n1-1] = 'X';
        }
        if (!isValid)
            cout << c1 << " is not a valid selection!\n";
    } while (!isValid);
}

bool tTTGameOverLine(char tTTValues[], int n1, int n2, int n3, char& c1)
{
    c1 = tTTValues[n1];
    if (tTTValues[n2] == c1 && tTTValues[n3] == c1)
        return true;
    return false;
}

bool tTTGameOver(char tTTValues[], int winningSets[][3], char& winner)
{
    int emptySpaces(0);
    for (int i = 0; i < 8; i++)
    {
        if (tTTGameOverLine(tTTValues, winningSets[i][0], winningSets[i][1], winningSets[i][2], winner))
        {
            if (winner == 'X')
                cout << "Game Over. You Win!\n";
            else
                cout << "Game Over. You Lose!\n";
            return true;
        }
    }
    for (int i = 0; i < 9; i++)
    {
        if (isdigit(tTTValues[i]))
            emptySpaces++;
    }
    if (emptySpaces == 0)
    {
        cout << "Game Over. Tie Match!\n";
        winner = '0';
        return true;
    }
    return false;
}

int maxSeqSearch(int theArray[], int numberUsed)
{
    int maximumIndex(0);
    for (int i = 0; i < numberUsed; i++)
    {
        if (theArray[maximumIndex] <= theArray[i])
            maximumIndex = i;
    }
    return maximumIndex;
}

void playAnalysis(int spotAppeal[],int winningSets[][3], bool comPlacements[], bool playerPlacements[], int comNum, int playerNum, int gain, int i)
{
    if (comPlacements[0] + comPlacements[1] + comPlacements[2] == comNum && playerPlacements[0] + playerPlacements[1] + playerPlacements[2] == playerNum)
    {
        for (int j = 0; j < 3; j++)
        {
            if (comNum > playerNum)
            {
                if (!comPlacements[j]);
                    spotAppeal[winningSets[i][j]] += gain;
            }
            else
                if (!playerPlacements[j]);
                    spotAppeal[winningSets[i][j]] += gain;
        }
    }
}
void tTTPlayLine(char tTTValues[], int winningSets[][3], int spotAppeal[], int i)
{
    bool comPlacements[3] = {0}, playerPlacements[3] = {0};
    for (int k = 0; k < 3; k++)
    {
        if(tTTValues[winningSets[i][k]] == 'O')
            comPlacements[k] = true;
        else if(tTTValues[winningSets[i][k]] == 'X')
            playerPlacements[k] = true;
    }
    int combinationsToCareFor[5][2] = {{2, 0}, {0, 2}, {1, 0}, {0, 1}, {0, 0}}, gain[5] = {100, 20, 1, 1, 1};
    for (int d = 0; d < 5; d++)
        playAnalysis(spotAppeal, winningSets, comPlacements, playerPlacements, combinationsToCareFor[d][0], combinationsToCareFor[d][1], gain[d], i);
}

void tTTComMove(char tTTValues[], int winningSets[][3])
{
    int spotAppeal[9] = {0}, playerPlacements[9] = {0};
    for (int i = 0; i < 9; i++)
    {
        if (tTTValues[i] == 'X')
            playerPlacements[i] = 1;
    }
    if (playerPlacements[0] + playerPlacements[2] + playerPlacements[5] + playerPlacements[8] == 2 && playerPlacements[1] + playerPlacements[3] + playerPlacements[5] + playerPlacements[7] == 0)
        tTTValues[3] = 'O';
    else
    {
        for (int i = 0; i < 8; i++)
        {
            tTTPlayLine(tTTValues, winningSets, spotAppeal, i);
        }
        for (int k = 0; k < 9; k++)
        {
            if (!isdigit(tTTValues[k]))
                spotAppeal[k] = 0;
        }
        tTTValues[maxSeqSearch(spotAppeal, 9)] = 'O';
    }
}

bool ticTacToe()
{
    int winningSets[8][3] = { {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {6, 4, 2} };
    char tTTValues[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'}, winner('0');
    cout << "Hi, I'm an impossibly smart Tic Tac Toe AI!\n";
    Sleep(1000);
    cout << "Let's play Tic Tac Toe! If you can tie me or beat me, I'll give you an 8-digit code you'll need!\n";
    Sleep(1500);
    cout << "I'll be O, and you can be X. To make a selection, press numbers 1-9 to place an X at the corresponding location.\nYou start!\n";
    printTTT(tTTValues);
    do
    {
        tTTPlayerMove(tTTValues);
        printTTT(tTTValues);
        if(tTTGameOver(tTTValues, winningSets, winner))
            break;
        cout << "*thinking of response*\n";
        Sleep(1000);
        tTTComMove(tTTValues, winningSets);
        printTTT(tTTValues);
    } while (!tTTGameOver(tTTValues, winningSets, winner));
    Sleep(1000);
    if (winner == '0' || winner == 'X')
    {
        cout << "Good game! The 8-digit code you want is 02052000.\n";
        Sleep(2000);
        cout << "Press any button to leave the computer!\n";
        getch();
        return true;
    }
    else
    {
        cout << "You lost! Maybe come back later when you're ready.\n";
        Sleep(2000);
        cout << "Press any button to leave the computer!\n";
        getch();
        return false;
    }
}
//TIC TAC TOE END HERE

void printMove(int aMove, string name)
{
    switch(aMove)
    {
        case 0: cout << name << " went with ROCK!\n"; break;
        case 1: cout << name << " went with PAPER!\n"; break;
        case 2: cout << name << " went with SCISSORS!\n"; break;
    }
}

int resultRPS(int comMove, int playerMove, int allPossibilities[][2])
{
    for (int i = 0; i < 9; i++)
    {
        if (comMove == allPossibilities[i][0] && playerMove == allPossibilities[i][1])
            return i;
    }
}
void checkRPS(int comMove, int playerMove, int& wins, int& loses)
{
    int allPossibilites[9][2] = {{0, 0}, {1, 1}, {2, 2}, {0, 1}, {1, 2}, {2, 0}, {0, 2}, {1, 0}, {2, 1}};
    int resultIndex = resultRPS(comMove, playerMove, allPossibilites);
    printMove(playerMove, "You");
    printMove(comMove, "AI");
    if (resultIndex <= 2)
    {
        cout << "Nobody wins this round, TIE!\n\n";
    }
    else if (resultIndex <= 5)
    {
        cout << "You WIN this round!\n\n";
        wins++;
    }
    else
    {
        cout << "You LOSE this round!\n\n";
        loses++;
    }
}

bool rockPaperScissors()
{
    int wins(0), loses(0), comMove(0), playerMove(0);
    cout << "Hi, I'm a Rock Paper Scissors AI!\n";
    Sleep(1000);
    cout << "Lets play Rock Paper Scissors! If you win best of 3, I'll give you a USB with Blueprint you'll need!\n";
    Sleep(1500);
    cout << "Input 'r' for rock, 'p' for paper, or 's' for scissors! Let's play!\n\n";
    do
    {
        srand(time(NULL));
        comMove = rand()%3;
        cout << "Wins: " << wins << " Loses: " << loses << "\nChoose a move to make. ('r', 'p', or 's')\n";
        playerMove = getch();
        switch(playerMove)
        {
            case 'r': playerMove = 0; checkRPS(comMove, playerMove, wins, loses); break;
            case 'p': playerMove = 1; checkRPS(comMove, playerMove, wins, loses); break;
            case 's': playerMove = 2; checkRPS(comMove, playerMove, wins, loses); break;
            default: cout << "Please make a valid selection.\n\n";
        }
        Sleep(1000);
    } while (wins < 2 && loses < 2);
    if (wins == 2)
    {
        cout << "Wins: 2 Loses: " << loses << endl;
        Sleep(1000);
        cout << "Good game! Take the USB with the blueprint!\n";
        Sleep(2000);
        cout << "Press any button to leave the computer!\n";
        getch();
        return true;
    }
    else
    {
        cout << "You lost! Maybe come back later when you're ready.\n";
        Sleep(2000);
        cout << "Press any button to leave the computer!\n";
        getch();
        return false;
    }
}

void printMaze(char maze[][16], int xPos, int yPos)
{
    for (int i = 0; i < 14; i++)
    {
        for (int k = 0; k < 16; k++)
        {
            if (i == xPos && k == yPos)
                cout << "O ";
            else
                cout << maze[i][k] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void mazeGame()
{
    char maze[14][16] = {   {'|', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '|'},
                            {'|', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' '},
                            {'|', ' ', '|', '-', ' ', '|', '-', ' ', '|', '-', '|', ' ', '|', ' ', '-', '|'},
                            {'|', ' ', '|', ' ', ' ', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', '|'},
                            {'|', ' ', '|', ' ', '-', ' ', '-', '-', '-', '-', '-', '-', '|', '-', ' ', '|'},
                            {'|', ' ', '|', ' ', '|', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', ' ', '|'},
                            {'|', ' ', '|', ' ', '|', ' ', ' ', ' ', '|', ' ', '|', ' ', '|', '-', '-', '|'},
                            {' ', ' ', '|', ' ', '|', '-', '-', '-', '|', ' ', '|', ' ', '|', '-', ' ', '|'},
                            {'|', ' ', '|', ' ', '|', ' ', ' ', ' ', ' ', ' ', '|', ' ', '|', ' ', ' ', '|'},
                            {'|', ' ', '|', ' ', '|', ' ', '-', '-', '-', '-', '|', ' ', '|', ' ', '-', '|'},
                            {'|', ' ', '|', ' ', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', ' ', ' ', '|'},
                            {'|', ' ', '|', '-', '|', '-', '-', '-', '-', '-', '-', '-', '-', '-', ' ', '|'},
                            {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
                            {'|', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '|'}
                        };
    int xPos(7), yPos(0);
    char input('0');
    cout << "Escape the maze for the 6-digit code you'll need!\n";
    Sleep(2000);
    cout << "You're character is an O. Use WASD to move.\n";
    Sleep(2000);
    printMaze(maze, xPos, yPos);
    do
    {
        input = getch();
        switch(input)
        {
            case 'w':
            case 'W': xPos--; break;
            case 'a':
            case 'A': yPos--; break;
            case 's':
            case 'S': xPos++; break;
            case 'd':
            case 'D': yPos++; break;
        }
        if (maze[xPos][yPos] != ' ')
        {
            switch(input)
            {
                case 'w':
                case 'W': xPos++; break;
                case 'a':
                case 'A': yPos++; break;
                case 's':
                case 'S': xPos--; break;
                case 'd':
                case 'D': yPos--; break;
            }
        }
        printMaze(maze, xPos, yPos);
    } while (!(xPos == 1 && yPos == 15));
    cout << "Nice job! The 6-digit code is 101498.\n";
    Sleep(1000);
    cout << "Press any key to exit computer!\n";
    getch();
}

int main()
{
    //srand(time(NULL));
    //mainMenu();
    cout << "hi!";
}


