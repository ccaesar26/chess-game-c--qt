#include "Queen.h"

Queen::Queen(EColor color)
	: Piece(color, EType::Queen)
{

}

PositionList Queen::GetPattern(Position currentPos, std::function<PiecePtr(Position)> GetPieceFromBoard) const
{
	PositionList finalPositions;

	// upper left
	AddFinalPositionsByDirections(currentPos, Position(-1, -1), GetPieceFromBoard, finalPositions);
	// up
	AddFinalPositionsByDirections(currentPos, Position(-1, 0), GetPieceFromBoard, finalPositions);
	// upper right
	AddFinalPositionsByDirections(currentPos, Position(-1, 1), GetPieceFromBoard, finalPositions);
	// right
	AddFinalPositionsByDirections(currentPos, Position(0, 1), GetPieceFromBoard, finalPositions);
	// lower right
	AddFinalPositionsByDirections(currentPos, Position(1, 1), GetPieceFromBoard, finalPositions);	
	// down
	AddFinalPositionsByDirections(currentPos, Position(1, 0), GetPieceFromBoard, finalPositions);
	// lower left
	AddFinalPositionsByDirections(currentPos, Position(1, -1), GetPieceFromBoard, finalPositions);
	// left
	AddFinalPositionsByDirections(currentPos, Position(0, -1), GetPieceFromBoard, finalPositions);

	return finalPositions;
}