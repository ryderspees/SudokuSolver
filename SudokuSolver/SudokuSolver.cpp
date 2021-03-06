#include <iostream>
#include <vector>

using namespace std;

struct zeroIndex { int row,col; };

void printGrid(int grid[9][9]);
void printGrid(zeroIndex selectedIndex, int backtrackCount, int currentIndexInHolder, int grid[9][9]);
zeroIndex findNextZero(int grid[9][9]);
int findValidNum(zeroIndex selectedIndex, int selectedNum, int grid[9][9]);

int stepCounter = 0;

void readInput(int (&grid)[9][9]) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            cin >> grid[i][j];
        }
    }
}

int main()
{

    int selectedNum;
    int currentIndexInHolder = -1;
    int backtrackCount = 0;
    bool foundZero = false;
    bool validPlacement = true;
    zeroIndex selectedIndex;
    vector< pair<int, int> > indexHolder;

    int grid[9][9];

    readInput(grid);

    // print the original array
    printGrid(grid);


    // ALGORITHM START
    //
    // keep repeating the algorithm until there are no more zeroes in the grid
    while (findNextZero(grid).row != -1) {

        // find the next zero index and "select" it
        selectedIndex = findNextZero(grid);
        // store the value at the selected index
        selectedNum = grid[selectedIndex.row][selectedIndex.col];

        // store the index only if we are not catching up from backtracking
        if (backtrackCount == 0) {
            indexHolder.push_back(make_pair(selectedIndex.row, selectedIndex.col));
        }
        else {
            // decrement the backtrack counter if we are catching up
            backtrackCount--;
        }
        
        // increment the current index in the index holder
        currentIndexInHolder++;
        
        // find the next valid number 1-9 for the selected index
        selectedNum = findValidNum(selectedIndex, selectedNum, grid);

        // modify and print grid if there is an update to be made
        if (selectedNum != grid[selectedIndex.row][selectedIndex.col]) {
            grid[selectedIndex.row][selectedIndex.col] = selectedNum;
            printGrid(selectedIndex, backtrackCount, currentIndexInHolder, grid);
        }
        else {
            cout << "NO VALID NUMBER AT: [" << selectedIndex.row << ", " << selectedIndex.col << "]" << endl;
        }

        

        // if no numbers 1-9 are valid, go back to the previous index and find a new valid number
        while (selectedNum == 0) {
            backtrackCount++;
            currentIndexInHolder--;
            selectedIndex.row = indexHolder[currentIndexInHolder].first;
            selectedIndex.col = indexHolder[currentIndexInHolder].second;
            selectedNum = grid[selectedIndex.row][selectedIndex.col];
            selectedNum = findValidNum(selectedIndex, selectedNum, grid);

            // place the new valid number into the grid, or zero if 1-9 is not valid for this index either
            grid[selectedIndex.row][selectedIndex.col] = selectedNum;

            cout << "BACKTRACKING... [" << selectedIndex.row << ", " << selectedIndex.col << "]" << endl;
            if (selectedNum != 0) {
                printGrid(selectedIndex, backtrackCount, currentIndexInHolder, grid);
            }

        }


    }
    return 0;
}

zeroIndex findNextZero(int grid[9][9]) {
    // find the next zero index
    // selectedIndex is set to { -1,-1 } because that is returned if no zeroes are left
    zeroIndex selectedIndex = { -1,-1 };

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (grid[i][j] == 0) {
                selectedIndex.row = i;
                selectedIndex.col = j;
                return selectedIndex;
            }
        }
    }

    return selectedIndex;
}

int findValidNum(zeroIndex selectedIndex, int selectedNum, int grid[9][9]) 
{

    bool validPlacement = true;
    // find the starting index to loop through row and column
    // 0,1,2 turns into 0,3,6
    int boxRowStart = (selectedIndex.row / 3)*3;
    int boxColStart = (selectedIndex.col / 3)*3;

    // increment selected number and loop it until it reaches 9 
    // and check if the placement is valid each time
    while (selectedNum < 9) {
        selectedNum++;
        validPlacement = true;

        // check if number placement is valid in row
        for (int i = 0; i < 9; i++) {
            // skip over the row of the selected number
            if (i == selectedIndex.col) {
                continue;
            }

            // if current number equals selected number, set valid flag to false
            if (grid[selectedIndex.row][i] == selectedNum) {
                validPlacement = false;
                break;
            }

        }

        // if the placement is marked as invalid for the row, don't bother checking
        // the columns, just move on to the next number 1-9
        if (validPlacement == false) {
            continue;
        }

        // check if number placement is valid in column
        for (int j = 0; j < 9; j++) {
            // skip over the column of the selected number
            if (j == selectedIndex.row) {
                continue;
            }

            // if current number equals selected number, set valid flag to false
            if (grid[j][selectedIndex.col] == selectedNum) {
                validPlacement = false;
                break;
            }

        }

        // if the placement is marked as invalid for the column, don't bother checking
        // the box, just move on to the next number 1-9
        if (validPlacement == false) {
            continue;
        }


        // check if number placement is valid in box by looping through 
        // (boxRow,boxCol) to (boxRow+2,boxCol+2)
        
        for (int i = boxRowStart; i < (boxRowStart + 3); i++) {
            for (int j = boxColStart; j < (boxColStart + 3); j++) {
                // skip over selectedIndex
                if ((i == selectedIndex.row) && (j == selectedIndex.col)) {
                    continue;
                }

                // if current number equals selected number, set valid flag to false
                if (grid[i][j] == selectedNum) {
                    validPlacement = false;
                }
            }
        }



        // if the entire row, column, and box have been checked and the number is still valid, 
        // you've found a valid number
        if (validPlacement) {
            return selectedNum;
        }

    }
    
    // this part is only reached if 9 is not valid, return 0 to show backtracking is necessary
    return 0;
}

void printGrid(int grid[9][9]) {
    cout << "INITIAL GRID" << endl;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << grid[i][j];
            if (j == 2 || j == 5) {
                cout << " | ";
            }
            else {
                cout << " ";
            }
        }
        if (i == 2 || i == 5) {
            cout << "\n----------------------" << endl;;
        }
        else {
            cout << endl;
        }
    }
    cout << endl;
}

void printGrid(zeroIndex selectedIndex, int backtrackCount, int currentIndexInHolder, int grid[9][9]) {
    stepCounter++;
    cout << "\nSTEP " << stepCounter << endl;
    cout << "Index Modified: [" << selectedIndex.row << ", " << selectedIndex.col << "]\n";
    cout << "Backtrack Count : " << backtrackCount << " \nCurrent Index in Holder : " << currentIndexInHolder << endl;


    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << grid[i][j];
            if (j == 2 || j == 5) {
                cout << " | ";
            }
            else {
                cout << " ";
            }
        }
        if (i == 2 || i == 5) {
            cout << "\n----------------------" << endl;;
        }
        else {
            cout << endl;
        }
    }
    cout << endl;
}

