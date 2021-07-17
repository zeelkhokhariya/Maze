
//-----------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------

#define MAX_DIMENSION 20

// constant definitions for the different cell states
const char WALL    = '1';
const char SPACE   = '0';
const char VISITED = '.';
const char MOUSE   = 'm';
const char EXIT    = 'e';

typedef enum BOOL { false, true } Boolean;

struct CELL
{
    int row;
    int column;
};
typedef struct CELL Cell;

typedef struct CELL_NODE CellNode;
struct CELL_NODE
{
    Cell     cell;
    CellNode *next;
};

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------

CellNode *top = NULL;

// a 2D array used to store the maze
char maze[MAX_DIMENSION][MAX_DIMENSION];
int mazeRows;
int mazeCols;

// holds the location of the mouse and escape hatch
Cell mouse;
Cell escape;

//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------

// basic cell manipulation

// returns true if the cells are at the same position in our maze
Boolean equalCells(const Cell cell1, const Cell cell2);
// returns a new cell object
Cell makeCell(const int row, const int col);
// returns true if the cell is within our maze
Boolean validCell(const Cell theCell);

// routines for managing our backtracking

// returns true if there are no more cells to try
Boolean noMoreCells();
// returns the next cell to try for a path out of the maze
Cell nextCell();
// introduces a new cell to try
void addCell(const Cell cell);

void printMaze();
void loadMaze();
// returns true if there's a solution to the maze
Boolean solveMaze();

// our invariant checker
void checkState();

//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------

int main( int argc, char *argv[] )
{
    loadMaze();

    if ( solveMaze() )
        printf( "The mouse is free!!!!\n" );
    else
        printf( "The mouse is trapped!!!!\n" );

    printf( "\nEnd of processing\n" );

    return EXIT_SUCCESS;
}


//////////////////////////////////////////////
// Cell routines
//////////////////////////////////////////////

// compare the two cell
Boolean equalCells( const Cell cell1, const Cell cell2 )
{


    if ( cell1.row  == cell2.row && cell1.column == cell2.column) {
        return true;
    }
    else {
        return false;
    }
}

// retrun Cell after making from row and column
Cell makeCell( const int row, const int col )
{
    Cell temp;
    temp.row =row;
    temp.column = col;

    return temp;
}
// return Boolean based on validity of cell in the maze
Boolean validCell( const Cell theCell )
{
    if ( theCell.row < mazeRows && theCell.column < mazeCols ) {
        return true;
    }
    else {
        return false;
    }
}
// return the boolean size of list
Boolean noMoreCells()
{
    if ( top == NULL ) {
        return true;
    }
    else {
        return false;
    }
}

// return the next cell of the List
Cell nextCell()
{
    Cell temp;
    temp = top->cell;
    top = top -> next;
    return  temp;
}

// Add the cell to the list
void addCell(const Cell cell)
{
    struct CELL_NODE *temp=NULL;
    temp =(struct CELL_NODE*) malloc(sizeof(struct CELL_NODE));


    if(temp != NULL)
    {
        if(top == NULL)
        {
            temp ->cell = cell;
            temp ->next = NULL;
            top = temp;
        }else
        {
            temp ->cell = cell;
            temp ->next = top;
            top = temp;
        }
    }else
    {
        free(temp);
        temp = NULL;
    }

    assert(top != NULL);
}
//////////////////////////////////////////////
// List routines
//////////////////////////////////////////////
// Solve the maze and return boolean based on if we find path
Boolean solveMaze()
{


    for (int i = 0; i < mazeRows; i++) {
        for (int j = 0; j < mazeCols; j++) {
            if (maze[i][j] == MOUSE) {
                mouse = makeCell(i,j);

            }

            if (maze[i][j] == EXIT) {
                escape = makeCell(i,j);
            }

        }
    }
    Cell unvisited;
    Cell currentCell = mouse;
    printMaze();
    while(!equalCells(currentCell, escape))
    {
        printf("\n");
        maze[currentCell.row][currentCell.column]= VISITED;
        printMaze();


        if(maze[currentCell.row+1][currentCell.column] == SPACE || maze[currentCell.row+1][currentCell.column] == EXIT)
        {
            unvisited = makeCell(currentCell.row+1,currentCell.column );

            checkState();
            if (validCell(unvisited)) {
                addCell(unvisited);
            }
        }

        if(maze[currentCell.row-1][currentCell.column] == SPACE || maze[currentCell.row-1][currentCell.column] == EXIT )
        {
            unvisited = makeCell(currentCell.row-1,currentCell.column );
            assert(unvisited.row < mazeRows && unvisited.column < mazeCols);
            checkState();
            if (validCell(unvisited)) {
                addCell(unvisited);
            }
        }
        if(maze[currentCell.row][currentCell.column+1] == SPACE || maze[currentCell.row][currentCell.column+1] == EXIT  )
        {
            unvisited = makeCell(currentCell.row,currentCell.column + 1 );
            assert(unvisited.row < mazeRows && unvisited.column < mazeCols);
            checkState();
            if (validCell(unvisited)) {
                addCell(unvisited);
            }
        }
        if(maze[currentCell.row][currentCell.column -1] == SPACE || maze[currentCell.row][currentCell.column -1] == EXIT )
        {
            unvisited = makeCell(currentCell.row,currentCell.column -1);
            assert(unvisited.row < mazeRows && unvisited.column < mazeCols);
            checkState();
            if (validCell(unvisited)) {
                addCell(unvisited);
            }
        }

        if(noMoreCells())
        {
            printf("the mouse is trapped: we tried all routes and failed to find an escape");
            assert(top == NULL);
            return false;
        }else {
            assert(top != NULL);
            currentCell = nextCell();
        }

    }
    return true;
}





//////////////////////////////////////////////
// Maze routines
//////////////////////////////////////////////
// prints the maze
void printMaze()
{
    checkState();
    for (int i = 0; i < mazeRows; i++)
    {
        for (int j = 0; j < mazeCols; j++)
        {
            printf("%c", maze[i][j]);
        }
        printf("\n");
    }
}
// checks the state of the maze by considering the dimension
void checkState()
{
    assert(mazeRows <= MAX_DIMENSION);
    assert(mazeCols <= MAX_DIMENSION);
    assert(maze != NULL);
}

/* Loads the maze using standard input*/
void loadMaze()
{
    scanf("%d",&mazeRows);
    scanf("%d",&mazeCols);

    for (int i = 0; i < mazeRows; i++)
    {
        for (int j = 0; j < mazeCols; j++)
        {
            scanf("%s",&maze[i][j]);
        }

        checkState();





    }
}


