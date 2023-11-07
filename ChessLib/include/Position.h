#pragma once

#include <utility>
#include <vector>

/**
 * @brief Represents a position on a chessboard using row and column coordinates.
 */
struct Position
{
    int row; ///< The row coordinate of the position.
    int col; ///< The column coordinate of the position.

    /**
     * @brief Constructor for initializing the position with row and column coordinates.
     *
     * @param r The row coordinate (default is 0).
     * @param c The column coordinate (default is 0).
     */
    Position(int r = 0, int c = 0);

    /**
     * @brief Constructor for initializing the position with a pair of integers.
     *
     * @param p A pair of integers representing row and column coordinates.
     */
    Position(std::pair<int, int> p);

    /**
     * @brief Converts the position to a pair of integers.
     *
     * @return A pair of integers representing row and column coordinates.
     */
    std::pair<int, int> ToPair();

    /**
     * @brief Overloaded equality operator for comparing positions.
     *
     * @param P The position to compare with.
     * @return `true` if the positions are equal, otherwise `false`.
     */
    bool operator==(const Position& P) const;

    /**
     * @brief Overloaded inequality operator for comparing positions.
     *
     * @param P The position to compare with.
     * @return `true` if the positions are not equal, otherwise `false`.
     */
    bool operator!=(const Position& P) const;
};

/**
 * @brief Inline constructor implementation for initializing position with row and column coordinates.
 */
inline Position::Position(int r, int c)
    : row(r)
    , col(c)
{
}

/**
 * @brief Inline constructor implementation for initializing position with a pair of integers.
 */
inline Position::Position(std::pair<int, int> p)
    : row(p.first)
    , col(p.second)
{
}

/**
 * @brief Inline method implementation to convert the position to a pair of integers.
 */
inline std::pair<int, int> Position::ToPair()
{
    return std::make_pair(row, col);
}

/**
 * @brief Inline method implementation of the equality operator for comparing positions.
 */
inline bool Position::operator==(const Position& P) const
{
    return (row == P.row && col == P.col);
}

/**
 * @brief Inline method implementation of the inequality operator for comparing positions.
 */
inline bool Position::operator!=(const Position& P) const
{
    return (row != P.row || col != P.col);
}


using PositionList = std::vector<Position>;