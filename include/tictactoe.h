#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <cstddef>
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
    size_t x, y;

    Point(size_t &x, size_t &y)
    {
        if (x < 0 || x >= N || y < 0 || y >= N)
        {
            throw std::out_of_range("Chess board out of range");
        }
        this->x = x;
        this->y = y;
    }

    Point(size_t &&x, size_t &&y) : x(x), y(y)
    {
        if (this->x < 0 || this->x >= N || this->y < 0 || this->y >= N)
        {
            throw std::out_of_range("Chess board out of range");
        }
    }

    Point(Point &&other) : x(other.x), y(other.y) {}

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

    bool isBoardFull()
    {
        for (auto i = 0; i < N; i++)
        {
            for (auto j = 0; j < N; j++)
            {
                if (board[i][j] == EMPTY)
                {
                    return false;
                }
            }
        }
        return true;
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

    Player(const char &&ch, const int &&playerNumber) : ch(ch), playerNumber(playerNumber) {}

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

    RowWinCommand(const size_t &&x, const size_t &&y) : x(x), y(y) {}

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
    size_t x, y;
};

template <size_t N>
class ColWinCommand : public Command<N>
{
public:
    ColWinCommand(const size_t &x, const size_t &y) : x(x), y(y) {}

    ColWinCommand(size_t &&x, size_t &&y) : x(x), y(y) {}

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
    size_t x, y;
};

template <size_t N>
class DiagonalWinCommand : public Command<N>
{
private:
    size_t x, y;

public:
    DiagonalWinCommand(const size_t &x, const size_t &y) : x(x), y(y) {}

    DiagonalWinCommand(const size_t &&x, const size_t &&y) : x(x), y(y) {}

    ~DiagonalWinCommand() = default;

    bool execute(ChessBoard<N> &board, Player &player) override
    {
        if (isOnMainDiagonal())
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
        else if (isOnAntiDiagonal())
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
    inline bool isOnMainDiagonal() const noexcept
    {
        return x == y;
    }

    inline bool isOnAntiDiagonal() const noexcept
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

    void addCommand(unique_ptr<Command<N>> command)
    {
        commands.push_back(std::move(command));
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
    vector<unique_ptr<Command<N>>> commands;
};

template <size_t N>
class TicTacToe
{
public:
    TicTacToe()
    {
        for (auto i = 0; i < N; i++)
        {
            WinCommandInvoker<N>::getInstance().addCommand(make_unique<RowWinCommand<N>>(i, 0));
            WinCommandInvoker<N>::getInstance().addCommand(make_unique<ColWinCommand<N>>(0, i));
        }
        WinCommandInvoker<N>::getInstance().addCommand(make_unique<DiagonalWinCommand<N>>(0, 0));
        WinCommandInvoker<N>::getInstance().addCommand(make_unique<DiagonalWinCommand<N>>(N - 1, 0));
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

#include <iostream>
#include <unordered_map>
#include <functional>
#include <string>

enum class IOEventType
{
    StartGame,
    EndGame,
    Move,
    Invalid
};

class ConsoleService
{
public:
    IOEventType readEvent()
    {
        string input = io.read();
        if (input == "m")
        {
            return IOEventType::Move;
        }
        else if (input == "s")
        {
            return IOEventType::StartGame;
        }
        else if (input == "e" || input == "q")
        {
            return IOEventType::EndGame;
        }
        else
        {
            return IOEventType::Invalid;
        }
    }

    void writeLine(const string &output)
    {
        io.write(output + "\n");
    }

    static ConsoleService &getInstance()
    {
        static ConsoleService instance;
        return instance;
    }

    string readLine()
    {
        return io.read();
    }

    using IOEventCallback = std::function<void()>;

private:
    StringIO io = StringIO::getInstance();
    ChessBoard<8> board;
    std::unordered_map<IOEventType, IOEventCallback> callbacks;

public:
    void registerEvent(IOEventType eventType, IOEventCallback callback)
    {
        callbacks[eventType] = callback;
    }

    void unregisterEvent(IOEventType eventType)
    {
        callbacks.erase(eventType);
    }

    void unregisterAllEvents()
    {
        callbacks.clear();
    }

    void triggerEvent(
        IOEventType eventType, std::function<void()> onError = []() {})
    {
        auto it = callbacks.find(eventType);
        if (it != callbacks.end())
        {
            try
            {
                it->second(); // 尝试执行对应的事件处理器
            }
            catch (const std::exception &e)
            {
                writeLine(e.what());
                onError();
            }
        }
        else
        {
            std::cout << "Invalid event triggered." << std::endl;
        }
    }
};

template <size_t N>
class TicTacToeGame
{
private:
    ChessBoard<N> board;
    TicTacToe<N> game;
    Player &player1 = game.getPlayer1();
    Player &player2 = game.getPlayer2();
    ConsoleService &io = ConsoleService::getInstance();
    bool exitGame{};
    bool gameStarted{};
    Player *currentPlayer = &player1;

public:
    TicTacToeGame()
    {
        io.registerEvent(IOEventType::Move, [&]()
                         { move(); });
        io.registerEvent(IOEventType::Invalid, [&]()
                         { throw std::invalid_argument("Invalid input"); });
        io.registerEvent(IOEventType::StartGame, [&]()
                         { startGame(); });
        io.registerEvent(IOEventType::EndGame, [&]()
                         { exit(); });
    }

    ~TicTacToeGame()
    {
        io.unregisterAllEvents();
    }

    void start()
    {
        io.writeLine("Welcome to TicTacToe!");
        io.writeLine("Input 'm' to move.");
        io.writeLine("Input 's' to start game.");
        io.writeLine("Input 'e' or 'q' to exit game.");

        while (!exitGame)
        {
            io.writeLine("Input command:");
            io.triggerEvent(io.readEvent());
        }
    }

private:
    void win(Player &player)
    {
        io.writeLine(board.getBoardString());
        io.writeLine("Player " + std::to_string(player.getPlayerNumber()) + " win!");
        gameStarted = false;
        io.triggerEvent(IOEventType::EndGame);
    }

    void draw()
    {
        io.writeLine(board.getBoardString());
        io.writeLine("Draw!");
        gameStarted = false;
        io.triggerEvent(IOEventType::EndGame);
    }

    void startGame()
    {
        io.writeLine("Game start!");
        gameStarted = true;
        io.triggerEvent(io.readEvent());
    }

    void exit()
    {
        io.writeLine("Game exit!");
        exitGame = true;
    }

    Point<N> parsePoint(const string &input)
    {
        if (input.length() != 3 || input[1] != ',')
        {
            io.triggerEvent(IOEventType::Invalid);
        }
        size_t x = input[0] - '0';
        size_t y = input[2] - '0';
        return Point<N>(x, y);
    }

    void tryParseAndMove(const std::string &input)
    {
        try
        {
            Point<N> point = parsePoint(input);
            makeMove(point);
        }
        catch (const std::exception &e)
        {
            io.triggerEvent(IOEventType::Invalid, [&]()
                            { io.writeLine(e.what());
                            io.writeLine("Please input again:");
                          io.triggerEvent(IOEventType::Move); });
        }
    }

    void makeMove(Point<N> &point)
    {
        if (game.play(board, *currentPlayer, point))
        {
            win(*currentPlayer);
        }
        else if (board.isBoardFull())
        {
            draw();
        }
        else
        {
            currentPlayer = currentPlayer == &player1 ? &player2 : &player1;
            io.triggerEvent(io.readEvent());
        }
    }

    void move()
    {
        if (!gameStarted)
        {
            io.triggerEvent(IOEventType::Invalid, [&]()
                            { io.writeLine("Game not started"); });
            return;
        }
        io.writeLine(board.getBoardString());
        io.writeLine("Player " + std::to_string(currentPlayer->getPlayerNumber()) + " move:");
        io.writeLine("Input point (x, y):");
        string input = io.readLine();
        tryParseAndMove(input);
    }
};

#endif // TICTACTOE_H
