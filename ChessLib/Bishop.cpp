#include "Bishop.h"

Bishop::Bishop(EColor color)
	: Piece(color, EType::Bishop)
{
}

PositionList Bishop::GetPattern(Position currentPos, std::function<PiecePtr(Position)> GetPieceFromBoard) const
{
	PositionList finalPositions;

	// upper left
	AddFinalPositionsByDirections(currentPos, Position(-1, -1), GetPieceFromBoard, finalPositions);
	// upper right
	AddFinalPositionsByDirections(currentPos, Position(-1,  1), GetPieceFromBoard, finalPositions);
	// lower left
	AddFinalPositionsByDirections(currentPos, Position( 1, -1), GetPieceFromBoard, finalPositions);
	// lower right
	AddFinalPositionsByDirections(currentPos, Position( 1,  1), GetPieceFromBoard, finalPositions);

	return finalPositions;
}
