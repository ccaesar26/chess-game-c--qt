#pragma once
#include <QPushButton>
#include "Enums.h"
#include "IPiece.h"

class GridButton : public QPushButton
{
	Q_OBJECT

public:
	GridButton(const std::pair<int, int>& boardPosition, PieceType pieceType = PieceType::none, PieceColor pieceColor = PieceColor::none, QWidget* parent = nullptr);
	~GridButton() = default;

	void mouseReleaseEvent(QMouseEvent* event) override;

	void updatePiece();
	void updateBackgroundColor();

	void setPiece(std::pair<PieceType, PieceColor> newPiece);
	void setHighlighted(int highlighted);
	void setSelected(bool selected);

	// Getters // 

	PieceType GetType() const;
	PieceColor GetColor() const;

signals:
	void Clicked(const std::pair<int, int>& position);

private:
	Position m_Position;
	PieceType m_PieceType;
	PieceColor m_PieceColor;
	int m_Highlighted;
	bool m_Selected;
};
