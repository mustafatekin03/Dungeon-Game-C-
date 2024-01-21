#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using std::cout, std::endl, std::ifstream, std::string;

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char **loadLevel(const string &fileName, int &maxRow, int &maxCol, Player &player)

{
    bool door = false;
    bool escape = false;
    int playerRow;
    int playerCol;
    ifstream myfile;
    myfile.open(fileName);

    if (myfile.is_open())
    {
        while (myfile)
        {
            myfile >> maxRow;
            if (myfile.fail())
                return nullptr;
            if (maxRow <= 0)
            {
                maxRow = 0;
                return nullptr;
            }

            myfile >> maxCol;
            if (myfile.fail())
                return nullptr;
            if (maxCol <= 0)
            {
                maxCol = 0;
                return nullptr;
            }
            if (double(INT32_MAX) / maxCol < maxRow || double(INT32_MAX) / maxRow < maxCol)
            {
                return nullptr;
            }
            if (maxCol > 999999 || maxRow > 999999)
                return nullptr;

            myfile >> playerRow;
            if (myfile.fail())
                return nullptr;
            if (playerRow < 0 || playerRow >= maxRow)
                return nullptr;

            player.row = playerRow;
            myfile >> playerCol;
            if (myfile.fail())
                return nullptr;
            if (playerCol < 0 || playerCol >= maxCol)
                return nullptr;
            player.col = playerCol;
            // checks for integer overflow
            if (INT32_MAX < maxRow || INT32_MAX < maxCol || INT32_MAX < playerRow || INT32_MAX < playerCol)
                return nullptr;
            // creates dungeon level board to play on
            char **board = createMap(maxRow, maxCol);
            if (board == nullptr)
            {
                return nullptr;
            }
            // goes through all instances in the file
            for (int i = 0; i < maxRow; i++)
            {
                for (int j = 0; j < maxCol; j++)
                {

                    if (myfile.eof())
                        return nullptr;
                    myfile >> board[i][j];
                    // checks for unexpected inputs from file
                    if (!(board[i][j] == TILE_OPEN || board[i][j] == TILE_TREASURE || board[i][j] == TILE_AMULET || board[i][j] == TILE_MONSTER || board[i][j] == TILE_PILLAR || board[i][j] == TILE_DOOR || board[i][j] == TILE_EXIT))
                    {
                        return nullptr;
                    }
                    if (i == playerRow && j == playerCol)
                        board[i][j] = TILE_PLAYER;
                    // iterates for doors and exits
                    if (board[i][j] == TILE_DOOR)
                        door = true;
                    if (board[i][j] == TILE_EXIT)
                        escape = true;
                }
            }
            // checks if there are extra characters in file
            char blank;
            myfile >> blank;
            if ((myfile.good()))
            {
                return nullptr;
            }
            // checks whether or not there is a exit or a door
            if (!(door || escape))
            {
                return nullptr;
            }
            return board;
        }
    }
    else
    {
        return nullptr;
    }
    return nullptr;
}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int &nextRow, int &nextCol)
{
    if (input == MOVE_UP)
        nextRow -= 1;
    else if (input == MOVE_DOWN)
        nextRow += 1;
    else if (input == MOVE_RIGHT)
        nextCol += 1;
    else if (input == MOVE_LEFT)
        nextCol -= 1;
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char **createMap(int maxRow, int maxCol)
{
    char **map = new char *[maxRow];
    for (int i = 0; i < maxRow; i++)
    {
        map[i] = new char[maxCol];
    }
    for (int i = 0; i < maxRow; i++)
    {
        for (int j = 0; j < maxCol; j++)
        {
            map[i][j] = TILE_OPEN;
        }
    }
    return map;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char **&map, int &maxRow)
{
    if (map != nullptr)
    {
        for (int i = 0; i < maxRow; ++i)
        {
            delete[] map[i];
            // map[i] = nullptr;
        }
        delete[] map;
        map = nullptr;
        maxRow = 0;
    }
    maxRow = 0;
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char **resizeMap(char **map, int &maxRow, int &maxCol)
{
    if (map == nullptr)
    {
        return nullptr;
    }
    if (double(INT32_MAX) / (2 * maxRow) < (2 * maxCol) || double(INT32_MAX) / (2 * maxCol) < (2 * maxRow))
    {
        return nullptr;
    }
    if (maxRow <= 0 || maxCol <= 0)
    {
        return nullptr;
    }
    char **bigMap = new char *[maxRow * 2];
    for (int i = 0; i < maxRow * 2; ++i)
    {
        bigMap[i] = new char[maxCol * 2];
    }
    // loops through to double the size of the map
    for (int i = 0; i < maxRow; i++)
    {
        for (int j = 0; j < maxCol; j++)
        {
            bigMap[i][j] = map[i][j];
            bigMap[i + maxRow][j] = map[i][j];
            bigMap[i][j + maxCol] = map[i][j];
            bigMap[i + maxRow][j + maxCol] = map[i][j];
        }
    }
    // removes duplicate player from lower part of the board
    for (int i = maxRow; i < maxRow * 2; i++)
    {
        for (int j = 0; j < maxCol * 2; j++)
        {
            if (bigMap[i][j] == TILE_PLAYER)
            {
                bigMap[i][j] = TILE_OPEN;
            }
        }
    }
    // removes duplicate player from upper right side of board
    for (int i = 0; i < maxRow * 2; i++)
    {
        for (int j = maxCol; j < maxCol * 2; j++)
        {
            if (bigMap[i][j] == TILE_PLAYER)
            {
                bigMap[i][j] = TILE_OPEN;
            }
        }
    }
    for (int i = 0; i < maxRow; ++i)
    {
        delete[] map[i];
    }
    delete[] map;
    map = nullptr;

    maxRow *= 2;
    maxCol *= 2;
    return bigMap;
    //**********Possibly return nullptr**********
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure.
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char **map, int maxRow, int maxCol, Player &player, int nextRow, int nextCol)
{
    // checks for a change in players position
    if (nextRow != player.row)
    {
        // goes through if it is a valid function only
        if (nextRow >= 0 && nextRow < maxRow && (map[nextRow][player.col] != TILE_MONSTER && map[nextRow][player.col] != TILE_PILLAR))
        {
            map[player.row][player.col] = TILE_OPEN;
            if (map[nextRow][player.col] == TILE_TREASURE)
            {
                player.row = nextRow;
                map[player.row][player.col] = TILE_PLAYER;
                player.treasure++;
                return STATUS_TREASURE;
            }
            else if (map[nextRow][player.col] == TILE_AMULET)
            {
                player.row = nextRow;
                map[player.row][player.col] = TILE_PLAYER;
                return STATUS_AMULET;
            }
            else if (map[nextRow][player.col] == TILE_DOOR)
            {
                player.row = nextRow;
                map[player.row][player.col] = TILE_PLAYER;
                return STATUS_LEAVE;
            }
            else if (map[nextRow][player.col] == TILE_EXIT)
            {
                if (player.treasure > 0)
                {
                    player.row = nextRow;
                    map[player.row][player.col] = TILE_PLAYER;
                    return STATUS_ESCAPE;
                }
                else
                {
                    nextRow = player.row;
                    map[player.row][player.col] = TILE_PLAYER;
                    return STATUS_STAY;
                }
            }
            else
            {
                player.row = nextRow;
                map[player.row][player.col] = TILE_PLAYER;
                return STATUS_MOVE;
            }
        }
        // invalid moves get processed here.
        else
        {
            nextRow = player.row;
            return STATUS_STAY;
        }
    }
    // Same as above only for change of column
    else if (nextCol != player.col)
    {

        if (nextCol >= 0 && nextCol < maxCol && (map[player.row][nextCol] != TILE_MONSTER && map[player.row][nextCol] != TILE_PILLAR))
        {
            map[player.row][player.col] = TILE_OPEN;
            if (map[player.row][nextRow] == TILE_TREASURE)
            {
                player.col = nextCol;
                map[player.row][player.col] = TILE_PLAYER;
                player.treasure++;
                return STATUS_TREASURE;
            }
            else if (map[player.row][nextCol] == TILE_AMULET)
            {
                player.col = nextCol;
                map[player.row][player.col] = TILE_PLAYER;
                return STATUS_AMULET;
            }
            else if (map[player.row][nextCol] == TILE_DOOR)
            {
                // MIGHT NOT BE NECESSARY
                player.col = nextCol;
                map[player.row][player.col] = TILE_PLAYER;
                return STATUS_LEAVE;
            }
            else if (map[player.row][nextCol] == TILE_EXIT)
            {
                if (player.treasure > 0)
                {
                    player.col = nextCol;
                    map[player.row][player.col] = TILE_PLAYER;
                    return STATUS_ESCAPE;
                }
                else
                {
                    nextCol = player.col;
                    map[player.row][player.col] = TILE_PLAYER;
                    return STATUS_STAY;
                }
            }
            else
            {
                player.col = nextCol;
                map[player.row][player.col] = TILE_PLAYER;
                return STATUS_MOVE;
            }
        }
        // invalid moves get processed
        else
        {
            nextCol = player.col;
            return STATUS_STAY;
        }
    }

    return 0;
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char **map, int maxRow, int maxCol, const Player &player)
{
    // checks and moves monsters that are above the player that also have a line of sight on the player
    for (int i = player.row - 1; i >= 0; i--)
    {
        if (map[i][player.col] == TILE_PILLAR)
        {
            break;
        }
        if (map[i][player.col] == TILE_MONSTER)
        {
            map[i][player.col] = TILE_OPEN;
            map[i + 1][player.col] = TILE_MONSTER;
        }
    }
    // checks and moves monsters that are below the player that also have a line of sight on the player
    for (int i = player.row + 1; i < maxRow; i++)
    {
        if (map[i][player.col] == TILE_PILLAR)
        {
            break;
        }
        if (map[i][player.col] == TILE_MONSTER)
        {
            map[i][player.col] = TILE_OPEN;
            map[i - 1][player.col] = TILE_MONSTER;
        }
    }
    // checks and moves monsters that are left the player that also have a line of sight on the player
    for (int i = player.col - 1; i >= 0; i--)
    {
        if (map[player.row][i] == TILE_PILLAR)
        {
            break;
        }
        if (map[player.row][i] == TILE_MONSTER)
        {
            map[player.row][i] = TILE_OPEN;
            map[player.row][i + 1] = TILE_MONSTER;
        }
    }
    // checks and moves monsters that are right the player that also have a line of sight on the player
    for (int i = player.col + 1; i < maxCol; i++)
    {
        if (map[player.row][i] == TILE_PILLAR)
        {
            break;
        }
        if (map[player.row][i] == TILE_MONSTER)
        {
            map[player.row][i] = TILE_OPEN;
            map[player.row][i - 1] = TILE_MONSTER;
        }
    }
    // checks if player is dead
    if (map[player.row][player.col] == TILE_MONSTER)
    {
        return true;
    }
    // if player is not dead then it returns false
    return false;
}