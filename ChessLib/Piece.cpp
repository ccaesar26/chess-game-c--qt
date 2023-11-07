#include "Piece.h"

#include "Bishop.h"
#include "Horse.h"
#include "King.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"

#include <cctype>

PiecePtr Piece::Produce(EType type, EColor color)
{
	switch (type)
	{
	case EType::Rook:
		return std::make_shared<Rook>(color);
	case EType::Horse:
		return std::make_shared<Horse>(color);
	case EType::King:
		return std::make_shared<King>(color);
	case EType::Queen:
		return std::make_shared<Queen>(color);
	case EType::Bishop:
		return std::make_shared<Bishop>(color);
	case EType::Pawn:
		return std::make_shared<Pawn>(color);
	}
}

Piece::Piece(EColor color, EType name)
	: m_color(color)
	, m_type(name)
{
}

EColor Piece::GetColor() const
{
	return m_color;
}

EType Piece::GetType() const
{
	return m_type;
}

EType Piece::GetTypeFromLetter(char pieceLetter)
{	
	switch (pieceLetter) {
	case 'R':
		return EType::Rook;
	case 'N':
		return EType::Horse;
	case 'B':
		return EType::Bishop;
	case 'Q':
		return EType::Queen;
	case 'K':
		return EType::King;
	default:
		return EType::Pawn;
	}
}

bool Piece::Is(EType type) const
{
	return m_type == type;
}

bool Piece::Is(std::vector<EType> types) const
{
	return std::find(types.begin(), types.end(), m_type) != types.end();
}

char Piece::ToLetter() const
{
	// white pieces: p r h b q k
	// black pieces: P R H B Q K
	switch (GetType())
	{
	case EType::Rook:
		return GetColor() == EColor::White ? 'r' : 'R';
	case EType::Horse:
		return GetColor() == EColor::White ? 'h' : 'H';
	case EType::King:
		return GetColor() == EColor::White ? 'k' : 'K';
	case EType::Queen:
		return GetColor() == EColor::White ? 'q' : 'Q';
	case EType::Bishop:
		return GetColor() == EColor::White ? 'b' : 'B';
	case EType::Pawn:
		return GetColor() == EColor::White ? 'p' : 'P';
	}
	return ' ';
}

PositionList Piece::GetFinalPositionsFromMovementArray(const std::array<Position, 8>& possibleMovements,
	std::function<PiecePtr(Position)> GetPieceFromBoard) const
{
	PositionList finalPositions;

	for (auto toCheckPosition : possibleMovements)
	{
		if (toCheckPosition.row < 0 || toCheckPosition.row > 7 ||
			toCheckPosition.col < 0 || toCheckPosition.col > 7)
		{
			continue;
		}
		if (GetPieceFromBoard(toCheckPosition))
		{
			if (GetPieceFromBoard(toCheckPosition)->GetColor() != m_color)
			{
				finalPositions.push_back(toCheckPosition);
			}
			continue;
		}
		finalPositions.push_back(toCheckPosition);
	}

	return finalPositions;
}

void Piece::AddFinalPositionsByDirections(const Position& currentPos, Position movingWay, std::function<PiecePtr(Position)> GetPieceFromBoard, PositionList& finalPositions) const
{
	Position limit;
	limit.row = movingWay.row < 0 ? -1 : 8;
	limit.col = movingWay.col < 0 ? -1 : 8;

	for (int i = currentPos.row + movingWay.row, j = currentPos.col + movingWay.col
		;	 i != limit.row && j != limit.col
		;	 i += movingWay.row, j += movingWay.col)
	{
		Position toCheckPosition(i, j);
		if (GetPieceFromBoard(toCheckPosition))
		{
			if (GetPieceFromBoard(toCheckPosition)->GetColor() != m_color)
			{
				finalPositions.push_back(toCheckPosition);
			}
			break;
		}
		finalPositions.push_back(toCheckPosition);
	}
}