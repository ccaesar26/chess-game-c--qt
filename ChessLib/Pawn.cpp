#include "Pawn.h"

Pawn::Pawn(EColor color)
	: Piece(color, EType::Pawn)
{
}

PositionList Pawn::GetPattern(Position currentPos, std::function<PiecePtr(Position)> GetPieceFromBoard) const
{
	PositionList finalPositions;

	int movingWay = 1, initialLine = 1;
	if (m_color == EColor::White)
	{
		movingWay = -1;
		initialLine = 6;
	}
	
	Position oneSquareForwardPos = Position(currentPos.row + movingWay, currentPos.col);
	Position twoSquaresForwardPos = Position(currentPos.row + 2 * movingWay, currentPos.col);
	Position leftDiagonal = Position(currentPos.row + movingWay, currentPos.col - 1);
	Position rightDiagonal = Position(currentPos.row + movingWay, currentPos.col + 1);

	// One square forward
	if (!GetPieceFromBoard(oneSquareForwardPos))
	{
		finalPositions.push_back(oneSquareForwardPos);

		// Two squares forward
		if (currentPos.row == initialLine)
		{
			if (!GetPieceFromBoard(twoSquaresForwardPos))
			{
				finalPositions.push_back(twoSquaresForwardPos);
			}
		}
	}

	// Left capture
	if (currentPos.col - 1 >= 0)
	{
		if (GetPieceFromBoard(leftDiagonal))
		{
			if (GetPieceFromBoard(leftDiagonal)->GetColor() != m_color)
			{
				finalPositions.push_back(leftDiagonal);
			}
		}
	}

	// Right capture
	if (currentPos.col + 1 <= 7)
	{
		if (GetPieceFromBoard(rightDiagonal))
		{
			if (GetPieceFromBoard(rightDiagonal)->GetColor() != m_color)
			{
				finalPositions.push_back(rightDiagonal);
			}
		}
	}

	return finalPositions;
}