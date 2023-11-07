#pragma once
#include "Piece.h"

class Bishop : public Piece
{

public:

    Bishop(EColor color);

    PositionList GetPattern(Position currentPos, std::function<PiecePtr(Position)> GetPieceFromBoard) const override;

};

