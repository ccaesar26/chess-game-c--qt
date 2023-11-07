#include "Rook.h"

Rook::Rook(EColor color)
	: Piece(color, EType::Rook)
{}

PositionList Rook::GetPattern(Position currentPos, std::function<PiecePtr(Position)> GetPieceFromBoard) const
{
	PositionList finalPositions;

	// up
	AddFinalPositionsByDirections(currentPos, Position(-1,  0), GetPieceFromBoard, finalPositions);
	// down
	AddFinalPositionsByDirections(currentPos, Position( 1,  0), GetPieceFromBoard, finalPositions);
	// left
	AddFinalPositionsByDirections(currentPos, Position( 0, -1), GetPieceFromBoard, finalPositions);
	// right
	AddFinalPositionsByDirections(currentPos, Position( 0,  1), GetPieceFromBoard, finalPositions);

	return finalPositions;
}
