#pragma once

#include "IPiece.h"

#include <string>

/**
 * @brief Interface for receiving chess game-related events and updates.
 *
 * This interface defines methods that listeners can implement to receive notifications about
 * various events and updates during a chess game.
 */
class IChessGameListener
{
public:
    /**
     * @brief Virtual destructor for the interface.
     */
    virtual ~IChessGameListener() = default;

    /**
     * @brief Called when a move is made on the chessboard.
     *
     * @param init The initial position of the piece that is moved.
     * @param fin The final position of the piece after the move.
     */
    virtual void OnMoveMade(Position init, Position fin) = 0;

    /**
     * @brief Called when the chess game is over.
     *
     * @param result The result of the game (e.g., win, draw, or loss).
     */
    virtual void OnGameOver(EGameResult result) = 0;

    /**
     * @brief Called when a pawn is eligible for upgrade.
     *
     * @param pos The position of the pawn that can be upgraded.
     */
    virtual void OnPawnUpgrade(Position pos) = 0;

    /**
     * @brief Called when a player's king is in check.
     */
    virtual void OnCheck() = 0;

    /**
     * @brief Called when the chess game is restarted.
     */
    virtual void OnGameRestarted() = 0;

    /**
     * @brief Called when the move history is updated.
     *
     * @param move The updated move that is added to the history.
     */
    virtual void OnHistoryUpdate(std::string move) = 0;

    /**
     * @brief Called when the game clock's time is updated.
     */
    virtual void OnClockUpdate() = 0;

    /**
     * @brief Called when a player's time has run out.
     */
    virtual void OnTimesUp() = 0;
};


using IChessGameListenerWeakPtr = std::weak_ptr<IChessGameListener>;
using IChessGameListenerPtr = std::shared_ptr<IChessGameListener>;