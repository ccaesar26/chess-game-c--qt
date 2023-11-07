#pragma once

#include "Piece.h"

class Horse : public Piece
{
public:
    Horse(EColor color);

	PositionList GetPattern(Position currentPos, std::function<PiecePtr(Position)> GetPieceFromBoard) const override;

};

