#pragma once

#include "IPiece.h"

using PiecePtr = std::shared_ptr<class Piece>;
using PieceList = std::vector<PiecePtr>;

class Piece : public IPiece
{
public:
	static PiecePtr Produce(EType type, EColor color);

	Piece(EColor color, EType name);

	EColor GetColor() const override;
	EType GetType() const override;
	static EType GetTypeFromLetter(char pieceLetter);

	bool Is(EType type) const;
	bool Is(std::vector<EType> types) const;

	char ToLetter() const;

	virtual PositionList GetPattern(Position currentPos, std::function<PiecePtr(Position)> GetPieceFromBoard) const = 0;

protected:
	PositionList GetFinalPositionsFromMovementArray(const std::array<Position, 8>& possibleMovements,
		std::function<PiecePtr(Position)> GetPieceFromBoard) const;
	void AddFinalPositionsByDirections(const Position& currentPos, Position movingWay, 
		std::function<PiecePtr(Position)> GetPieceFromBoard, PositionList& finalPositions) const;

	EColor m_color;
	EType m_type;
};