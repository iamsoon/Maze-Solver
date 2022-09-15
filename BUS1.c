#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//By Amelia Soon (iamsoon)
//Date: March 17, 2022
//Spaghetti code that could definitely be improved on with more experience.

int mazeRow=0,mazeCol=0;
int maze[50][50];
int main() {
    int choice, mazeLoaded=0;
    puts("Welcome to the BUS Project Milestone 1 program!");
    do {
        puts("1: Generate a random maze");
        puts("2: Load a preexisting maze");
        puts("3: View a preexisting maze");
        puts("4: Solve a preexisting maze");
        puts("5: Auto-solve a preexisting maze");
        puts("Any other input: Exit the program\n");
        printf("Please make your selection: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                randomMaze();
                mazeLoaded=1;
                break;
            case 2:
                makeArray();
                mazeLoaded=1;
                break;
            case 3:
                if (mazeLoaded==1) {
                    printMaze(maze, mazeRow, mazeCol);
                }
                else {
                    printf("Please load or create a maze first.\n\n");
                    getch();
                }
                break;
            case 4:
                if (mazeLoaded==1) {
                    pathfindSetup(maze, 4);
                }
                else {
                    printf("Please load or create a maze first.\n\n");
                    getch();
                }
                break;
            case 5:
                if (mazeLoaded==1) {
                    pathfindSetup(maze, 5);
                }
                else {
                    printf("Please load or create a maze first.\n\n");
                    getch();
                }
                break;
            default:
                printf("Thank you for using our maze solver.");
                getch();
                break;
        }
    } while (choice>0&&choice<6);

    return 0;
}

void randomMaze() {
    //setup
    int num;
    FILE* in = fopen("maze.txt", "w");
    printf("Please input number of rows: ");
    scanf("%d", &mazeRow);
    printf("Please input number of columns: ");
    scanf("%d", &mazeCol);

    //procedure
    srand(time(0));
    fprintf(in, "%d %d\n", mazeRow, mazeCol);
    for (int col=0;col<mazeCol+2;col++) { //Top border of maze
        maze[0][col]=1;
    }
    for (int row=0;row<mazeRow;row++) {
        maze[row][0]=1; //Left border
        for (int col=0;col<mazeRow;col++) { //Assign each point within maze borders with 0 or 1 randomly
            num = rand()%2;
            fprintf(in, "%d ", num); //Put value in doc
            maze[row][col]=num;
        }
        maze[row][mazeCol+1]=1; //Right border
        fprintf(in, "\n");
    }
    for (int col=0;col<mazeCol+2;col++) { //Bottom border
        maze[mazeRow+1][col]=1; 
    }
    fclose(in);
    printf("Maze has been created with filename maze.txt, located in the same folder as this program.\n\n");

    return;
}

void makeArray() {
    FILE* in;
    int current;
    char name[40];
    printf("Input file name (and location if the file is not in the same folder as this program): ");
    scanf("%s", &name);
    in = fopen(name, "r");
    if (in==NULL) { //if file name not found
        printf("\nError loading file.\n\n");
        getch();
        return;
    }
    fscanf(in, "%d %d", &mazeRow, &mazeCol);

    for (int col=0;col<mazeCol+2;col++) { //top border
        maze[0][col]=1;
    }
    for (int row=1;row<mazeRow+1;row++) {
        maze[row][0]=1; //left border
        for (int col=1;col<mazeCol+1;col++) { //read value and assign to associated point in array
            fscanf(in, "%d", &current);
            maze[row][col]=current;
        }
        maze[row][mazeCol+1]=1; //right border
    }
    for (int col=0;col<mazeCol+2;col++) {
        maze[mazeRow+1][col]=1; //bottom border
    }
    fclose(in);
    printf("\nMaze loaded successfully.\n\n");
    return;
}

void printMaze(int maze[50][50], int mazeRow, int mazeCol) {
    for (int col=0;col<mazeCol+2;col++) { //number rows and columns to make location identification easier
        printf("%d", col%10); 
    }
    printf("\n");
    for (int row=1;row<mazeRow+1;row++) {
        printf("%d", row%10);
        for (int col=1;col<mazeCol+1;col++) { //print according to value at location
            if (maze[row][col]==0) {
                printf(".");
            }
            else if (maze[row][col]==1) {
                printf("|");
            }
            else if (maze[row][col]==2) {
                printf("E");
            }
            else if (maze[row][col]==3) {
                printf("C");
            }
            else {
                printf("Error: Document cannot be read as a maze.\n\n");
                return 0;
            }
        }
        printf("|\n");
    }
    printf("\n");
    return;
}

void pathfindSetup(int maze[50][50], int choice) {
    printMaze(maze, mazeRow, mazeCol);
    int startx, starty, endx, endy, top=0;
    int* topLoc = &top;
    char* directions = calloc(mazeRow*mazeCol, sizeof(char));
    do { //input start and end points
        printf("Input the starting point row (1-%d) and column (1-%d): ", mazeRow, mazeCol);
        scanf("%d %d",&startx,&starty);
        if (startx<1||starty<1||startx>mazeRow||starty>mazeCol) { //check position is valid
            printf("Chosen starting point is outside maze boundaries.");
        }
        if (maze[startx][starty]==1) {
            printf("Chosen starting point is in a wall.\n");
        }
        maze[startx][starty]=3;
    } while (startx<1||starty<1||startx>mazeRow||starty>mazeCol||maze[startx][starty]==1);
    do {
        printf("Input the end point row (1-%d) and column (1-%d): ", mazeRow,mazeCol);
        scanf("%d %d",&endx,&endy);
        if (endx<1||endy<1||endx>mazeRow||endy>mazeCol) {
            printf("Chosen end point is outside maze boundaries.");
        }
        if (maze[endx][endy]==1) {
            printf("Chosen end point is in a wall.\n");
        }
    } while(endx<1||endy<1||endx>mazeRow||endy>mazeCol||maze[endx][endy]==1);
    maze[endx][endy] = 2;

    if (choice==4) {
        solveMaze(maze, startx, starty);
    }
    else if (choice==5) {
        pathfind(maze, startx, starty, directions, topLoc);
    }
    return;
}

void solveMaze(int maze[50][50], int x, int y, int endx, int endy) {
    //up(w) = x-1, down(s) = x+1, left(a) = y-1, right(d) = y+1
    //0=path, 1=wall, 2=end, 3=car
    char direction;
    do{
        system("cls");
        printMaze(maze, mazeRow, mazeCol);
        printf("Move C using the w,a,s,d keys: ");
        direction=getch();
        printf("%c", direction);
        if (direction=='w') {
            if (maze[x-1][y]!=1) {
                if (maze[x-1][y]==2) {
                    maze[x-1][y]=3;
                    maze[x][y]=0;
                    system("cls");
                    printMaze(maze, mazeRow, mazeCol); //Clears maze then reprints with updated car information
                    printf("End point reached.\n\n");
                    return;
                }
                maze[x-1][y]=3;
                maze[x][y]=0;
                x--;
            }
        }
        if (direction=='s') {
            if (maze[x+1][y]!=1) {
                if (maze[x+1][y]==2) {
                    maze[x+1][y]=3;
                    maze[x][y]=0;
                    system("cls");
                    printMaze(maze, mazeRow, mazeCol);
                    printf("End point reached.\n\n");
                    return;
                }
                maze[x+1][y]=3;
                maze[x][y]=0;
                x++;
            }
        }
        if (direction=='a') {
            if (maze[x][y-1]!=1) {
                if (maze[x][y-1]==2) {
                    maze[x][y-1]=3;
                    maze[x][y]=0;
                    system("cls");
                    printMaze(maze, mazeRow, mazeCol);
                    printf("End point reached.\n\n");
                    return;
                }
                maze[x][y-1]=3;
                maze[x][y]=0;
                y--;
            }
        }
        if (direction=='d') {
            if (maze[x][y+1]!=1) {
                if (maze[x][y+1]==2) {
                    maze[x][y+1]=3;
                    maze[x][y]=0;
                    system("cls");
                    printMaze(maze, mazeRow, mazeCol);
                    printf("End point reached.\n\n");
                    return;
                }
                maze[x][y+1]=3;
                maze[x][y]=0;
                y++;
            }
        }
    }while (1);
}

void pathfind(int maze[50][50], int x, int y, char* directions, int* topLoc) {
    //up = x-1, down = x+1, left = y-1, right = y+1
    //0=path, 1=wall, 2=end, 3=path taken before, 4=dead end
    if (maze[x][y]==2){
        if (directions[0]==NULL) {
            printf("\nStart and end point are the same.\n\n");
        }
        else {
            printf("\nDirections (u=up, d=down, l=left, r=right): %s\n\n", directions);
        }
        return;
    }
    else{
        //going further into the maze
        if (maze[x-1][y]==0||maze[x-1][y]==2) {
            maze[x][y]=3;
            directions[*topLoc]='u'; //direction added to stack
            *topLoc++;
            pathfind(maze, x--, y, directions, topLoc);
            return;
        }
        if (maze[x+1][y]==0||maze[x+1][y]==2) {
            maze[x][y]=3;
            directions[*topLoc]='d';
            *topLoc++;
            pathfind(maze, x++, y, directions, topLoc);
            return;
        }
        if (maze[x][y-1]==0||maze[x][y-1]==2) {
            maze[x][y]=3;
            directions[*topLoc]='l';
            *topLoc++;
            pathfind(maze, x, y--, directions, topLoc);
            return;
        }
        if (maze[x][y+1]==0||maze[x][y+1]==2) {
            maze[x][y]=3;
            directions[*topLoc]='r';
            *topLoc++;
            pathfind(maze, x, y++, directions, topLoc);
            return;
        }

        //going back out the maze
        if (maze[x][y+1]==3) {
            *topLoc--; //direction removed from stack
            if (maze[x][y-1]!=3&&maze[x+1][y]!=3&&maze[x-1][y]!=3) { //Check to see if other branches have been searched
                maze[x][y]=4;
            }
            pathfind(maze, x, y++, directions, topLoc);
            return;
        }
        if (maze[x][y-1]==3) {
            *topLoc--;
            if (maze[x][y+1]!=3&&maze[x-1][y]!=3&&maze[x+1][y]!=3) {
                maze[x][y]=4;
            }
            pathfind(maze, x, y--, directions, topLoc);
            return;
        }
        if (maze[x+1][y]==3) {
            *topLoc--;
            if (maze[x-1][y]!=3&&maze[x][y+1]!=3&&maze[x][y-1]!=3) {
                maze[x][y]=4;
            }
            pathfind(maze, x++, y, directions, topLoc);
            return;
        }
        if (maze[x-1][y]==3) {
            *topLoc--;
            if (maze[x+1][y]!=3&&maze[x][y-1]!=3&&maze[x][y+1]!=3) {
                maze[x][y]=4;
            }
            pathfind(maze, x--, y, directions, topLoc);
            return;
        }
    }
    printf("Maze is impossible to solve.\n\n"); //If whole maze has been searched
    return;
}

