// tictactoe.cpp : A simple m, n, k-game played in console.
//
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;
// Used for checking whether a player has won or not after a placement
// Use this array to iterate through the four directions.
int direction[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
/**
 *  Defines the cell state of the cell.
 */
enum class CellState {
    ILLEGAL = -2, //Cell index out of range
    EMPTY = -1, //Cell is not occupied by any of the player
    PLAYER0 = 0, //Cell occupied by player 0
    PLAYER1 = 1 // Cell occupied by player 1
};

/**
 * A m * n board contained an array of its cell states.
 */
class Board
{
private:
    int row;
    int col;
    int k;
    CellState* cells;
public:
    /**
     * Initializes a new Board instance for a m, n, k-game.
     *
     * @param m      The row number of the board.
     * @param n      The column number of the board.
     * @param k      Player wins if get k stones of its own in a row.
     */
    Board(int m, int n, int k)
    {
        this->row = m;
        this->col = n;
        this->k = k;
        this->cells = new CellState[m * n];
        memset(this->cells, (int)CellState::EMPTY, sizeof(CellState) * m * n);
    };

    /**
     * Initializes a new Board instance for a m, n, k-game with its cell status
     *
     * @param m      The row number of the board.
     * @param n      The column number of the board.
     * @param k      Player wins if get k stones of its own in a row.
     * @param cells  An array of its cell states.
     */
    Board(int m, int n, int k, CellState* cells)
    {
        this->row = m;
        this->col = n;
        this->k = k;
        this->cells = cells;
    };

    ~Board()
    {
        delete(this->cells);
    }
    /**
    * Retrives the number of rows of the board.
    */
    int GetRow()
    {
        return this->row;
    }

    /**
    * Retrives the number of columns of the board.
    */
    int GetCol()
    {
        return this->col;
    }

    /**
    * Retrives the number of stones needed in a row to win.
    */
    int GetK()
    {
        return this->k;
    }

    /**
    * Retrives the array of cell states from the board.
    */
    CellState* GetCells()
    {
        return this->cells;
    }

    /**
    * Retrives a specific cell state given its index.
    *
    * @param x The row index of the cell
    * @param y The column index of the cell
    *
    * @return
    * CellState::ILLEGAL if the given index is out of range
    * CellState::EMPTY if the given cell is not occupied.
    * CellState::PLAYER0 if the given cell is occupied by player 0.
    * CellState::PLAYER1 if the given cell is occupied by player 1.
    */
    CellState GetCell(int x, int y)
    {
        if (x < 0 || x >= this->row) return CellState::ILLEGAL;
        if (y < 0 || y >= this->col) return CellState::ILLEGAL;
        return this->cells[x * this->col + y];
    }

    /**
    * Get the output of a specific cell state given its index.
    *
    * @param x The row index of the cell
    * @param y The column index of the cell
    *
    * @return
    * "X" if the given cell is occupied by player 0.
    * "O" if the given cell is occupied by player 1.
    * " " otherwise.
    */
    string GetCellContent(int x, int y)
    {
        CellState player = GetCell(x, y);
        if (player == CellState::PLAYER0)
        {
            return "X";
        }
        else if (player == CellState::PLAYER1)
        {
            return "O";
        }
        else
        {
            return " ";
        }
    }

    /**
      * Print out the current board status in console.
     */
    void PrintBoard()
    {
        for (int i = 0; i < this->col; i++)
        {
            if(i < 10)
            {
                cout << "  " << i << " ";
            }
            else
            {
                cout << " " << i << " ";
            }
        }
        cout << "\n";
        for (int i = 0; i < this->col; i++)
        {
            cout << " ---";
        }
        cout << "\n";
        for (int i = 0; i < this->row; i++)
        {
            for (int j = 0; j < this->col; j++)
            {
                cout << " | " << GetCellContent(i, j);
            }
            cout << " | " << i << "\n";

            for (int j = 0; j < this->col; j++)
            {
                cout << " ---";
            }
            cout << "\n";
        }
    }

    /**
      * Return a new board with a given cell occupied by a player.
      *
      * @param x, y     The row and col index of the given cell.
      * @param player   The player occupying the cell.
     */
    Board* updateCell(int x, int y, CellState player) {
        CellState* newCells = new CellState[this->row * this->col];
        memcpy(newCells, this->cells, sizeof(CellState) * this->GetCol() * this->GetRow());
        newCells[x * this->col + y] = player;
        return new Board(this->row, this->col, this->k, newCells);
    }
};

/**
 * A game state contains the current game board, a history of player movements, and a current player.
 */
class Game {
private:
    Board* board;
    CellState player;
    vector<Board*> history;
    int moveCnt;

public:
    /**
     * Initializes a new m, n, k-game.
     *
     * @param m      The row number of the board.
     * @param n      The column number of the board.
     * @param k      Player wins if get k stones of its own in a row.
     */
    Game(int m, int n, int k) {
        this->board = new Board(m, n, k);
        this->player = CellState::PLAYER0;
        this->moveCnt = 0;
    }

    ~Game()
    {
        for (Board* b : history)
        {
            delete(b);
        }
        history.clear();
        history.shrink_to_fit();
        delete(this->board);
    }
    /**
     * Retrieves the current board of the game.
     */
    Board* getBoard()
    {
        return this->board;
    }

    /**
     * Retrieves the current player of the game.
     */
    CellState getCellState()
    {
        return this->player;
    }

    /**
     * Current player tries to occupy the cell at (x, y)
     *
     * @param x, y   The index of the cell the player wants to occupy.
     * @return       true if the player successfully makes the move. false otherwise.
     */
    bool play(int x, int y)
    {
        if (this->board->GetCell(x, y) != CellState::EMPTY)
        {
            if (this->board->GetCell(x, y) == CellState::ILLEGAL)
            {
                cout << "Illegal Input. Please try again." << endl;
            }
            else
            {
                cout << "Cell is not empty. Please try again." << endl;
            }
            return false;
        }
        this->history.push_back(this->board);
        this->board = this->board->updateCell(x, y, this->player);
        this->moveCnt++;
        this->player = this->player == CellState::PLAYER0 ? CellState::PLAYER1 : CellState::PLAYER0;
        return true;
    }

    /**
     * Retrive whether the board is full or not.
     */
    bool hasFull()
    {
        return this->moveCnt == this->getBoard()->GetCol() * this->getBoard()->GetRow();
    }

    /**
     * Check if the player has won after it makes a movement.
     *
     * @param x, y   The index of the cell the player just occupied.
     * @return       true if current player had k-stones in a row. false otherwise.
     */
    bool hasWin(int x, int y)
    {
        int k = this->board->GetK();
        for (int a = 0; a < sizeof(direction) / (sizeof(int) * 2); a++)
        {
            int i = -k + 1;
            int cnt = 0;
            while (i < k - 1)
            {
                CellState curOccupiedCellState = this->board->GetCell(x + (direction[a][0] * i), y + direction[a][1] * i);
                CellState nextOccupiedCellState = this->board->GetCell(x + (direction[a][0] * (i + 1)), y + direction[a][1] * (i + 1));
                if (curOccupiedCellState != CellState::EMPTY && curOccupiedCellState != CellState::ILLEGAL && curOccupiedCellState == nextOccupiedCellState)
                {
                    cnt++;
                }
                else
                {
                    cnt = 0;
                    if (i > 0)
                    {
                        break;
                    }
                }
                i++;
                if (cnt == k - 1)
                {
                    return true;
                }
            }
        }
        return false;
    }

    /**
      * Undo the last movement made by the player.
      *
      * @return  true if sucesfully undoed. false if there's no history to be undoed.
     */
    bool undo()
    {
        int size = this->history.size();
        if (size == 0)
        {
            return false;
        }
        this->board = this->history[size - 1];
        this->history.erase(this->history.begin() + size - 1);
        this->moveCnt--;
        this->player = this->player == CellState::PLAYER0 ? CellState::PLAYER1 : CellState::PLAYER0;
        return true;
    }

};



vector<int> ParseInput(string s)
{
    vector<int> result;
    stringstream ss(s);

    while (ss.good()) {
        string substr;
        getline(ss, substr, ',');
        try {
            result.push_back(stoi(substr));
        }
        catch (exception& err) {
            cout << "Cannot recogize input. ";
            break;
        }
    }
    return result;
}

int main()
{
    // It would make more sense if the program can parse the parameters from command lines
    // instead of this. But since no other libraries are allowed, I'm keeping this way
    // for now.
    Game* game = new Game(3, 3, 3);
    game->getBoard()->PrintBoard();
    int x, y;
    string line;
    bool hasSuccessed = false;
    bool hasEnd = false;
    while (!hasEnd)
    {
        hasSuccessed = false;
        while (!hasSuccessed)
        {
            cout << "It's player " << (int)(game->getCellState()) << "'s turn to play." << endl;
            cout << "Please enter your move:" << endl;
            getline(cin, line);
            if (line.compare("undo") == 0)
            {
                hasSuccessed = game->undo();
            }
            else
            {
                vector<int> input = ParseInput(line);
                if (input.size() == 2)
                {
                    hasSuccessed = game->play(input[0], input[1]);
                    if (game->hasWin(input[0], input[1]))
                    {
                        hasEnd = true;
                        break;
                    }
                    if (game->hasFull())
                    {
                        hasEnd = true;
                        break;
                    }
                }
                else
                {
                    cout << "Please input in format of : 'x, y', or input 'undo' to undo previous steps." << endl;
                }
            }
        }
        game->getBoard()->PrintBoard();
    }
    if (game->hasFull())
    {
        cout << "Draw." << endl;
    }
    else
    {
        cout << "Congrats! Player " << 1 - (int)(game->getCellState()) << " has won!" << endl;
    }

    delete(game);
}

