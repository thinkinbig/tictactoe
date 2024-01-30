#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <stddef.h>
#include <array>
#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace std;

const char EMPTY = ' ';

template <size_t N>
struct Point
{
    const size_t x;
    const size_t y;

    Point(const size_t &x, const size_t &y)
    {
        if (x < 0 || x >= N || y < 0 || y >= N)
        {
            throw std::out_of_range("Chess board out of range");
        }
        this->x = x;
        this->y = y;
    }

    Point(size_t &&x, size_t &&y) : x(std::move(x)), y(std::move(y))
    {
        if (this->x < 0 || this->x >= N || this->y < 0 || this->y >= N)
        {
            throw std::out_of_range("Chess board out of range");
        }
    }

    ~Point() = default;

    bool operator==(const Point &point) const noexcept
    {
        return x == point.x && y == point.y;
    }

    std::ostream &operator<<(std::ostream &os) const noexcept
    {
        os << "(" << x << ", " << y << ")";
        return os;
    }

    const size_t &getX() const noexcept
    {
        return x;
    }

    const size_t &getY() const noexcept
    {
        return y;
    }
};

template <size_t N>
class ChessBoard
{
public:
    ChessBoard() { initBoard(); };
    ~ChessBoard() = default;

    string getBoardString()
    {
        string boardString = "";
        for (auto i = 0; i < N; i++)
        {
            boardString += "|";
            for (auto j = 0; j < N; j++)
            {
                boardString += string(1, board[i][j]) + "|";
            }

            boardString += "\n";
        }
        return boardString;
    }

    void setPoint(const size_t &x, const size_t &y, const char &ch)
    {
        if (checkOutOfRange(x, y))
        {
            throw std::out_of_range("Chess board out of range");
        }
        board[x][y] = ch;
    }

    const array<array<char, N>, N> &getBoard() const noexcept
    {
        return board;
    }

private:
    array<array<char, N>, N> board;

    void initBoard() noexcept
    {
        for (auto i = 0; i < N; i++)
        {
            for (auto j = 0; j < N; j++)
            {
                board[i][j] = EMPTY;
            }
        }
    }

    inline bool checkOutOfRange(const size_t &x, const size_t &y) noexcept
    {
        return x < 0 || x >= N || y < 0 || y >= N;
    }
};

class Player
{
public:
    Player(const char &ch, const int &playerNumber) : ch(ch), playerNumber(playerNumber) {}
    Player(const char &&ch, const int &&playerNumber) : ch(std::move(ch)), playerNumber(std::move(playerNumber)) {}
    ~Player() = default;

    bool operator==(const Player &player) const noexcept
    {
        return playerNumber == player.getPlayerNumber();
    }

    template <size_t N>
    void setPoint(ChessBoard<N> &board, const size_t &x, const size_t &y) const
    {
        if (board.getBoard()[x][y] != EMPTY)
        {
            throw std::invalid_argument("Chess board point is not empty");
        }
        board.setPoint(x, y, ch);
    }

    char &getChess() noexcept
    {
        return ch;
    }

    const int &getPlayerNumber() const noexcept
    {
        return playerNumber;
    }

private:
    char ch;
    int playerNumber;
};

constexpr size_t WIN_COUNT = 5;

template <size_t N>
class Command
{
public:
    virtual bool execute(ChessBoard<N> &board, Player &player) = 0;
    virtual ~Command() = default;

protected:
    static const size_t SIZE = N < WIN_COUNT ? N : WIN_COUNT;
};

template <size_t N>
class RowWinCommand : public Command<N>
{
public:
    RowWinCommand(const size_t &x, const size_t &y) : x(x), y(y) {}
    RowWinCommand(const size_t &&x, const size_t &&y) : x(std::move(x)), y(std::move(y)) {}
    ~RowWinCommand() = default;

    bool execute(ChessBoard<N> &board, Player &player) override
    {
        for (auto i = 0; i < this->SIZE; i++)
        {
            if (board.getBoard()[x][i] != player.getChess())
            {
                return false;
            }
        }
        return true;
    }

private:
    size_t x;
    size_t y;
};

template <size_t N>
class ColWinCommand : public Command<N>
{
public:
    ColWinCommand(const size_t &x, const size_t &y) : x(x), y(y) {}
    ColWinCommand(const size_t &&x, const size_t &&y) : x(std::move(x)), y(std::move(y)) {}
    ~ColWinCommand() = default;

    bool execute(ChessBoard<N> &board, Player &player) override
    {
        for (auto i = 0; i < this->SIZE; i++)
        {
            if (board.getBoard()[i][y] != player.getChess())
            {
                return false;
            }
        }
        return true;
    }

private:
    size_t x;
    size_t y;
};

template <size_t N>
class DiagonalWinCommand : public Command<N>
{
private:
    size_t x;
    size_t y;

public:
    DiagonalWinCommand(const size_t &x, const size_t &y) : x(x), y(y) {}
    DiagonalWinCommand(const size_t &&x, const size_t &&y) : x(std::move(x)), y(std::move(y)) {}
    ~DiagonalWinCommand() = default;

    bool execute(ChessBoard<N> &board, Player &player) override
    {
        if (isOnMainDiagonal(x, y))
        {
            for (auto i = 0; i < this->SIZE; i++)
            {
                if (board.getBoard()[i][i] != player.getChess())
                {
                    return false;
                }
            }
            return true;
        }
        else if (isOnAntiDiagonal(x, y))
        {
            for (auto i = 0; i < this->SIZE; i++)
            {
                if (board.getBoard()[i][N - 1 - i] != player.getChess())
                {
                    return false;
                }
            }
            return true;
        }

        return false;
    }

private:
    bool isOnMainDiagonal(size_t x, size_t y)
    {
        return x == y;
    }

    bool isOnAntiDiagonal(size_t x, size_t y)
    {
        return x + y == N - 1;
    }
};

template <size_t N>
class WinCommandInvoker
{
public:
    WinCommandInvoker() = default;
    ~WinCommandInvoker()
    {
        commands.clear();
    }

    void addCommand(shared_ptr<Command<N>> command)
    {
        commands.push_back(command);
    }

    bool execute(ChessBoard<N> &board, Player &player)
    {
        for (const auto &command : commands)
        {
            if (command->execute(board, player))
            {
                return true;
            }
        }
        return false;
    }

    static WinCommandInvoker<N> &getInstance()
    {
        static WinCommandInvoker<N> instance;
        return instance;
    }

private:
    vector<shared_ptr<Command<N>>> commands;
};

template <size_t N>
class TicTacToe
{
public:
    TicTacToe()
    {
        for (auto i = 0; i < N; i++)
        {
            WinCommandInvoker<N>::getInstance().addCommand(make_shared<RowWinCommand<N>>(i, 0));
            WinCommandInvoker<N>::getInstance().addCommand(make_shared<ColWinCommand<N>>(0, i));
        }
        WinCommandInvoker<N>::getInstance().addCommand(make_shared<DiagonalWinCommand<N>>(0, 0));
        WinCommandInvoker<N>::getInstance().addCommand(make_shared<DiagonalWinCommand<N>>(N - 1, 0));
    }
    ~TicTacToe() = default;

    bool play(ChessBoard<N> &board, Player &player, Point<N> &point)
    {
        player.setPoint(board, point.getX(), point.getY());
        return WinCommandInvoker<N>::getInstance().execute(board, player);
    }

    Player &getPlayer1() noexcept
    {
        return player1;
    }

    Player &getPlayer2() noexcept
    {
        return player2;
    }

private:
    Player player1{'X', 1};
    Player player2{'O', 2};
};

// 泛型的IO接口类
template <typename T>
class IOInterface
{
public:
    virtual T read() = 0;
    virtual void write(const T &output) = 0;
};

// 字符串IO类

class StringIO : public IOInterface<string>
{
public:
    void write(const string &output) override
    {
        std::cout << output;
    }

    string read() override
    {
        string str;
        std::cin >> str;
        return str;
    }

    static StringIO &getInstance()
    {
        static StringIO instance;
        return instance;
    }
};

class StringIOCommand
{
public:
    template <size_t N>
    std::shared_ptr<Point<N>> readPoint()
    {
        auto x = io.read();
        auto xNum = stoi(x);

        auto y = io.read();
        auto yNum = stoi(y);
        return std::make_shared<Point<N>>(xNum, yNum);
    }

    void writeLine(const string &output)
    {
        io.write(output + "\n");
    }

    static StringIOCommand &getInstance()
    {
        static StringIOCommand instance;
        return instance;
    }

private:
    StringIO io = StringIO::getInstance();
    ChessBoard<8> board;
};

template <size_t N>
class TicTacToeGame
{
private:
    ChessBoard<N> board;
    TicTacToe<N> game;
    Player &player1 = game.getPlayer1();
    Player &player2 = game.getPlayer2();
    StringIOCommand &io = StringIOCommand::getInstance();

    static TicTacToeGame &getInstance()
    {
        static TicTacToeGame instance;
        return instance;
    }

public:
    void start()
    {
        bool isWin = false;
        Player currentPlayer = player1;
        while (!isWin)
        {
            try
            {
                io.writeLine(board.getBoardString());
                io.writeLine("Player " + std::to_string(currentPlayer.getPlayerNumber()) + "'s turn (x, y): ");
                std::shared_ptr<Point<N>> point = io.readPoint<N>();
                isWin = game.play(board, currentPlayer, *point);
                currentPlayer = currentPlayer == player1 ? player2 : player1;
            }
            catch (const std::exception &e)
            {
                io.writeLine("Error: " + string(e.what()) + "\n");
            }
        }
        currentPlayer = currentPlayer == player1 ? player2 : player1;
        io.writeLine(board.getBoardString());
        io.writeLine("Player " + std::to_string(currentPlayer.getPlayerNumber()) + " win!");
    }
};

#endif // CHESSBOARD_H