#pragma once

#include "Enums.h"
#include "Position.h"
#include "IPiece.h"

#include <array>

using CastleValues = std::array<std::array<bool, 2>, 2>;
using CharBoard = std::array<std::array<char, 8>, 8>;

/**
 * @brief Interface for controlling chess game operations.
 *
 * This interface defines methods to manage a chess game, including resetting the game,
 * restoring game state, making moves, upgrading pawns, and performing various drawing operations.
 */
class IChessGameControl
{
public:
    /**
     * @brief Virtual destructor for the interface.
     */
    virtual ~IChessGameControl() = default;

    /**
     * @brief Resets the chess game to its initial state.
     */
    virtual void ResetGame() = 0;

    /**
     * @brief Restores the game state based on the provided configuration.
     *
     * @param inputConfig The configuration of the chessboard.
     * @param turn The color of the player whose turn it is (default is White).
     * @param castle Castle values indicating whether castling is allowed for each player (default is {true, true, true, true}).
     */
    virtual void RestoreGame(const CharBoard& inputConfig, EColor turn = EColor::White, CastleValues castle = { true, true, true, true }) = 0;

    /**
     * @brief Makes a chess move from the initial position to the final position.
     *
     * @param initialPos The starting position of the piece to move.
     * @param finalPos The destination position for the piece.
     * @param EnableNotification Flag to enable or disable move notifications (default is true).
     * @param upgradeType The type to which a pawn is upgraded upon reaching the opposite end (default is Pawn).
     * @throws OutOfBoundsException If the initial or final position is not a valid position on the chessboard.
     * @throws OccupiedByOwnPieceException If the final square is occupied by the player's own piece.
     * @throws NotInPossibleMovesException If the specified move is not among the possible valid moves for the piece.
     */
    virtual void MakeMove(Position initialPos, Position finalPos, bool EnableNotification = true, EType upgradeType = EType::Pawn) = 0;

    /**
     * @brief Upgrades a pawn to a specified piece type.
     *
     * @param upgradeType The type to which the pawn will be upgraded.
     */
    virtual void UpgradePawn(EType upgradeType) = 0;

    /**
     * @brief Performs a drawing operation on the chessboard.
     *
     * @param op The drawing operation to perform.
     */
    virtual void DrawOperation(EDrawOperation op) = 0;
};
