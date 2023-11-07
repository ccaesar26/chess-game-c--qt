#pragma once

#include "Enums.h"

/**
 * @brief Interface for retrieving various status and information about a chess game.
 *
 * This interface provides methods to access different aspects of the current state of a chess game,
 * including piece positions, possible moves, captured pieces, current player, board configuration,
 * game over conditions, and more.
 */
class IChessGameStatus
{
public:
    /**
     * @brief Virtual destructor for the interface.
     */
    virtual ~IChessGameStatus() = default;

    /**
     * @brief Retrieves a smart pointer to the chess piece at the specified position.
     *
     * @param pos The position for which to retrieve the piece pointer.
     * @return A smart pointer to the chess piece at the given position.
     */
    virtual IPiecePtr GetIPiecePtr(Position pos) const = 0;

    /**
     * @brief Retrieves a list of possible valid moves for the piece at the given position.
     *
     * @param currentPos The position of the piece for which to retrieve possible moves.
     * @return A list of positions representing possible moves for the piece.
     */
    virtual PositionList GetPossibleMoves(Position currentPos) const = 0;

    /**
     * @brief Retrieves a list of pieces captured by the specified player's color.
     *
     * @param color The color of the player whose captured pieces are to be retrieved.
     * @return A list of captured pieces belonging to the specified player.
     */
    virtual IPieceList GetCapturedPieces(EColor color) const = 0;

    /**
     * @brief Retrieves the color of the player whose turn it is.
     *
     * @return The color of the current player.
     */
    virtual EColor GetCurrentPlayer() const = 0;

    /**
     * @brief Retrieves the current configuration of the chessboard at a specific index.
     *
     * @param index The index of the configuration to retrieve.
     * @return The chessboard configuration at the specified index.
     */
    virtual CharBoard GetBoardAtIndex(int index) const = 0;

    /**
     * @brief Retrieves the number of moves made in the game so far.
     *
     * @return The total number of moves made in the game.
     */
    virtual int GetNumberOfMoves() const = 0;

    /**
     * @brief Checks if the game is over.
     *
     * @return `true` if the game is over, otherwise `false`.
     */
    virtual bool IsGameOver() const = 0;

    /**
     * @brief Checks if the game is in a draw state.
     *
     * @return `true` if the game is in a draw state, otherwise `false`.
     */
    virtual bool IsDraw() const = 0;

    /**
     * @brief Checks if the game is waiting for a draw response.
     *
     * @return `true` if the game is waiting for a draw response, otherwise `false`.
     */
    virtual bool IsWaitingForDrawResponse() const = 0;

    /**
     * @brief Checks if a specific player has won the game.
     *
     * @param player The color of the player to check.
     * @return `true` if the specified player has won, otherwise `false`.
     */
    virtual bool IsWon(EColor player) const = 0;

    /**
     * @brief Checks if the game is waiting for a pawn upgrade.
     *
     * @return `true` if the game is waiting for a pawn upgrade, otherwise `false`.
     */
    virtual bool IsWaitingForUpgrade() const = 0;

    /**
     * @brief Checks if the game is in a check state (a player's king is under threat).
     *
     * @return `true` if the game is in a check state, otherwise `false`.
     */
    virtual bool IsCheckState() const = 0;

    /**
     * @brief Checks if castling is available for the specified player and side.
     *
     * @param color The color of the player.
     * @param side The side for which to check castling availability.
     * @return `true` if castling is available for the specified player and side, otherwise `false`.
     */
    virtual bool IsCastlingAvailable(EColor color, ESide side) const = 0;
};
