#include "Horse.h"

Horse::Horse(EColor color)
	: Piece(color, EType::Horse)
{

}

PositionList Horse::GetPattern(Position currentPos, std::function<PiecePtr(Position)> GetPieceFromBoard) const
{
	std::array<Position, 8> possibleMovements = {
		Position(currentPos.row - 2, currentPos.col - 1),
		Position(currentPos.row - 2, currentPos.col + 1),
		Position(currentPos.row - 1, currentPos.col - 2),
		Position(currentPos.row - 1, currentPos.col + 2),
		Position(currentPos.row + 1, currentPos.col - 2),
		Position(currentPos.row + 1, currentPos.col + 2),
		Position(currentPos.row + 2, currentPos.col - 1),
		Position(currentPos.row + 2, currentPos.col + 1)	
	};

	return GetFinalPositionsFromMovementArray(possibleMovements, GetPieceFromBoard);
}