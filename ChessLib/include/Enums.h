#pragma once

/**
 * @brief Enumeration for draw-related operations.
 */
enum class EDrawOperation
{
    Request, ///< A draw request.
    Accept,  ///< Acceptance of a draw request.
    Decline  ///< Decline of a draw request.
};

/**
 * @brief Enumeration for specifying the side of the chessboard.
 */
enum class ESide
{
    Queenside, ///< The queenside of the chessboard.
    Kingside   ///< The kingside of the chessboard.
};

/**
 * @brief Enumeration for specifying the color of a chess piece or player.
 */
enum class EColor
{
    White, ///< The color white.
    Black  ///< The color black.
};

/**
 * @brief Enumeration for specifying the type of a chess piece.
 */
enum class EType
{
    Rook,   ///< A rook chess piece.
    Horse,  ///< A knight (horse) chess piece.
    King,   ///< A king chess piece.
    Queen,  ///< A queen chess piece.
    Bishop, ///< A bishop chess piece.
    Pawn    ///< A pawn chess piece.
};

/**
 * @brief Enumeration for specifying the result of a chess game.
 */
enum class EGameResult
{
    WhitePlayerWon, ///< White player wins the game.
    BlackPlayerWon, ///< Black player wins the game.
    Draw            ///< The game ends in a draw.
};
