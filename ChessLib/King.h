#pragma once
#include "Piece.h"

class King : public Piece
{

public:
	
	King(EColor color);

	PositionList GetPattern(Position currentPos, std::function<PiecePtr(Position)> GetPieceFromBoard) const override;

};

