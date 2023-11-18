#include <iostream>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <string>

// Arrow key constants to control Snake Direction
#define ARROW_UP 72
#define ARROW_DOWN 80
#define ARROW_LEFT 75
#define ARROW_RIGHT 77

using namespace std;

const int width = 50;                          // Width of Box
const int height = 20;                         // Height of Box


enum Direction //Use only one variable at a time
{
    STOP = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT
}; // To control Snake Direction


// Class Instructions to display the instructions of the game
class Inst
{
public:
    void Start()
    {
        cout << "\n\t\t---------------------------------------" << endl;
        cout << "\t\t\tWelcome to Snake Game" << endl;
        cout << "\t\t---------------------------------------" << endl;
    }

    void Instrctions()
    {
        cout << "\n\t\tFollow these Instruction to Play the game:- " << endl;
        cout<<endl;
        cout << "\t\tUse Arrow Key UP to move Snake Up" << endl;
        cout << "\t\tUse Arrow Key DOWN to move Snake Down" << endl;
        cout << "\t\tUse Arrow Key Right to move Snake Right" << endl;
        cout << "\t\tUse Arrow Key LEFT to move Snake Left" << endl;
        cout << "\n\t\t---------------------------------------" << endl;
        cout<<endl;
        cout << "Press any key to Continue...." << endl;
    }
};

class Snake 
{
protected:
    int x, y;                   // Coordinates of the snake's head
    Direction dir;              // Current direction of the snake
    int tailX[100], tailY[100]; // Array to store coordinates of snake's tail
    int numTail;                // Length of the snake's tail
};

class Food
{
protected:
    int foodX, foodY; // Coordinates of the food
};

//Main Class

class Game : public Snake, public Food, public Inst
{
public:
    int score;         // Current score of the player
    bool gameOver;     // Flag to determine if the game is over
    string playerName; // Name of the player

    //Defualt constructor
    Game()
    {

        gameOver = false;
        x = width / 2;
        y = height / 2;
        foodX = rand() % width;
        foodY = rand() % height;
        score = 0;
        numTail = 0;
        dir = STOP;
    }

    // Initialize the game by getting the player's name
    void initialize()
    {

        system("CLS");

        cout << "Player Name: ";
        cin >> playerName;
    }

    void drawBoard()
    {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);  //Use to retrive Handle, Handle is an idetifier to show the board
        system("CLS");

        cout << endl
             << endl;
        for (int i = 0; i < width; i++) //To show colors on the board and buffering the board
        {
            SetConsoleTextAttribute(h, 5);
            cout << char(219); //Color of the board
        }
        cout << endl;

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (j == 0 || j == width - 1)
                {
                    SetConsoleTextAttribute(h, 5);
                    cout << char(219);
                }
                // used to set color on the foreground and background h,5 means we want purple foreground and dark background
                else if (i == y && j == x)
                {
                    SetConsoleTextAttribute(h, 2);// Green and Black color
                    cout << char(254); 
                }
                //Both 254 and 219 are solid block colors to show on the board on console
                else if (i == foodY && j == foodX)
                {
                    SetConsoleTextAttribute(h, 15);//white black
                    cout << "*";
                }
                else
                {
                    bool print = false;
                    for (int k = 0; k < numTail; k++)
                    {
                        if (i == tailY[k] && j == tailX[k])
                        {
                            SetConsoleTextAttribute(h, 2);
                            cout << char(254);
                            print = true;
                        }
                    }
                    if (!print)
                    {
                        cout << " ";
                    }
                }
            }
            cout << endl;
        }

        for (int i = 0; i < width; i++)
        {
            SetConsoleTextAttribute(h, 5);
            cout << char(219);
        }
        cout << endl;
        cout << endl;

        SetConsoleTextAttribute(h, 15);
        cout << "\tPlayer Name: " << playerName << endl;
        cout << "\t Score: " << score;
    }

    // Get user input to change the snake's direction
    void getInput()
    {
        if (_kbhit())  //to check the recent keystrokes on the console
        {
            int key = _getch();// returns non-zero if the arrow key is pressed
            if (key == 0xE0 || key == 0) // 0xE0 means 224 in decimal which is  prefix for keyboard keys
            { // Check for extended key
                key = _getch();
                switch (key)
                {
                case ARROW_UP:
                    dir = UP;
                    break;
                case ARROW_LEFT:
                    dir = LEFT;
                    break;
                case ARROW_DOWN:
                    dir = DOWN;
                    break;
                case ARROW_RIGHT:
                    dir = RIGHT;
                    break;
                default:
                    break;
                }
            }
        }
    }

    // Update the game state based on the current direction and collision
    void updateGame()
    {
        int prevX = tailX[0];//It stores the previous position of tail in x-axis
        int prevY = tailY[0];//It stores the previous position of tail in y-axis
        tailX[0] = x;
        tailY[0] = y;
        int prev2X, prev2Y;// To store current position of tail in x-axis and y-axis

        // Update the positions of the tail segments
        for (int i = 1; i < numTail; i++)
        {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X; // Storing the current position to previous position updating it
            prevY = prev2Y;
        }

        // Update the head position based on the current direction
        switch (dir)
        {
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        default:
            break;
        }

        // Check for collision with the walls
        if (x < 0 || x > width || y < 0 || y > height)
        {
            gameOver = true;
        }

        // Check for collision with the tail segments
        for (int i = 0; i < numTail; i++)
        {
            if (x == tailX[i] && y == tailY[i])
            {
                gameOver = true;
            }
        }

        // Check if the snake has eaten the food
        if (x == foodX && y == foodY)
        {
            score += 5;
            foodX = rand() % width;
            foodY = rand() % height;
            numTail++;
        }
    }

    // Start the game and continue until the game is over
    void play()
    {
        initialize();

        while (!gameOver)
        {
            drawBoard();
            getInput();
            updateGame();
            Sleep(30); // food takes 30 milliseconds to reappear
        }
    }
};

int main()
{
    Game G1;
        G1.Start();
        G1.Instrctions();
        getch();
        G1.play();

        system("CLS");

        cout<<"\a\t\tGame Over"<<endl;
        cout<<"\t\tThanks for Playing"<<endl;
        cout<<"\t\tPress any key to exit"<<endl;
        getch();

    

        

    return 0;
}
