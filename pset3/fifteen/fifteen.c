/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
int inittiles = (d*d)-1, row, col;      //declaring variables
    
    for (row = 0; row < d; row++)       //iterates over each element, eventually forming a square grid due to using two [row][col]
    {
        for (col = 0; col < d; col++)
        {
            board[row][col] = inittiles;    //inittiles will represent the numbers that will populate the grid
            inittiles--;                    //minus 1 every time we reach a new element for decending order
        }
    }
    
    board[d-1][d-1] = 0;                //bottom right corner tile is represented by a 0 as required by check50
    
    if (d%2 == 0)                       //checks if d is a even number... if so, swap position of 1 and 2
    {
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
int row, col;

    for (row = 0; row < d; row++)
    {
        for (col = 0; col < d; col++)
        {
            if (board[row][col] == 0)
            {
                printf ("%4c ", 95);            //prints a underscore if tile is 0 (blank space)
            }
            else
            {
                printf ("%4d ", board[row][col]);
            }
        }
        printf ("\n\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
int row, col;

    for (row = 0; row < d; row++)
    {
        for (col = 0; col < d; col++)
        {
            if (board[row][col] == tile && board[row+1][col] == 0 && row+1 < d)     //bottom tile check & ensures not checking outside of array
            {
                board[row][col] = 0;
                board[row+1][col] = tile;
                return true;
            }
            else if (board[row][col] == tile && board[row-1][col] == 0 && row-1 > -1)   //top tile check
            {
                board[row][col] = 0;
                board[row-1][col] = tile;
                return true;
            }
            else if (board[row][col] == tile && board[row][col+1] == 0 && col+1 < d)    //right tile check
            {   
                board[row][col] = 0;
                board[row][col+1] = tile;
                return true;
            }
            else if (board[row][col] == tile && board[row][col-1] == 0 && col-1 > -1)   //left tile check
            {
                board[row][col] = 0;
                board[row][col-1] = tile;
                return true;
            }
        }
    }
    return false;
}


/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
int row, col, ding = 1;

    for (row = 0; row < d; row++)
    {
        for (col = 0; col < d; col++)
        {
            if (board[row][col] == ding && board[d-1][d-1] == 0)        //if each element is equal to value of ding and bottom right corner tile is equal to 0, board is in winning order
            {
                ding++;
            }
        }
    }
    
    if (ding == d*d)
    {
        return true;
    }
    return false;
}