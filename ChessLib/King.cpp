#include "King.h"

King::King(EColor color)
	: Piece(color, EType::King)
{
}

PositionList King::GetPattern(Position currentPos, std::function<PiecePtr(Position)> GetPieceFromBoard) const
{
	std::array<Position, 8> possibleMovements = {
		Position(currentPos.row - 1, currentPos.col - 1),
		Position(currentPos.row - 1, currentPos.col),
		Position(currentPos.row - 1, currentPos.col + 1),
		Position(currentPos.row    , currentPos.col + 1),
		Position(currentPos.row + 1, currentPos.col + 1),
		Position(currentPos.row + 1, currentPos.col),
		Position(currentPos.row + 1, currentPos.col - 1),
		Position(currentPos.row    , currentPos.col - 1)	
	};

	PositionList pattern = GetFinalPositionsFromMovementArray(possibleMovements, GetPieceFromBoard);

	return pattern;
}