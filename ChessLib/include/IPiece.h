#pragma once

#include "Enums.h"
#include "Position.h"

#include <memory>
#include <array>
#include <vector>
#include <functional>

/**
 * @brief Interface for representing a chess piece.
 *
 * This interface defines methods to retrieve information about a chess piece, such as its color and type.
 */
class IPiece
{
public:
    /**
     * @brief Virtual destructor for the interface.
     */
    virtual ~IPiece() = default;

    /**
     * @brief Retrieves the color of the chess piece.
     *
     * @return The color of the chess piece (White or Black).
     */
    virtual EColor GetColor() const = 0;

    /**
     * @brief Retrieves the type of the chess piece.
     *
     * @return The type of the chess piece (e.g., King, Queen, Pawn, etc.).
     */
    virtual EType GetType() const = 0;
};


using IPiecePtr = std::shared_ptr<IPiece>;
using IPieceList = std::vector<IPiecePtr>;