#include "GridButton.h"

void GridButton::mouseReleaseEvent(QMouseEvent* event)
{
	QPushButton::mouseReleaseEvent(event);
	if (isEnabled()) 
	{
		emit Clicked(m_Position.ToPair());
	}
}

void GridButton::updatePiece()
{
	std::string imagePath = "res/";

	switch (m_PieceColor)
	{
	case PieceColor::none:
		imagePath += "empty";
		break;
	case PieceColor::black:
		imagePath += "b";
		break;
	case PieceColor::white:
		imagePath += "w";
		break;
	}

	switch (m_PieceType)
	{
	case PieceType::none:
		break;
	case PieceType::king:
		imagePath += "k";
		break;
	case PieceType::rook:
		imagePath += "r";
		break;
	case PieceType::bishop:
		imagePath += "b";
		break;
	case PieceType::queen:
		imagePath += "q";
		break;
	case PieceType::knight:
		imagePath += "h";
		break;
	case PieceType::pawn:
		imagePath += "p";
		break;
	default:
		break;
	}

	imagePath += ".png";

	QPixmap pixmap(QString::fromStdString(imagePath));
	QIcon ButtonIcon(pixmap);
	setIcon(ButtonIcon);
	setIconSize(pixmap.rect().size());
}

void GridButton::updateBackgroundColor()
{
	bool defaultColorBlack = (m_Position.row + m_Position.col) % 2;
	QString backColor = "";

	if(m_Highlighted==1)
		backColor = "#6A6A6A"/*#D4E2D4*/;
	else if(m_Highlighted==2)
		backColor = "#E57373";
	else
	{
		if (defaultColorBlack)
			backColor = "#A77E48";
		else
			backColor = "#D3B887";
	}

	setStyleSheet("background-color: " + backColor + "; border: none;");
}

void GridButton::setPiece(std::pair<PieceType, PieceColor> newPiece)
{
	m_PieceType = newPiece.first;
	m_PieceColor = newPiece.second;

	updatePiece();
}

void GridButton::setHighlighted(int highlighted)
{
	m_Highlighted = highlighted;
	updateBackgroundColor();
}

void GridButton::setSelected(bool selected)
{
	m_Selected = selected;
	updateBackgroundColor();
}

PieceType GridButton::GetType() const
{
	return m_PieceType;
}

PieceColor GridButton::GetColor() const
{
	return m_PieceColor;
}

GridButton::GridButton(const std::pair<int, int>& boardPosition, PieceType pieceType, PieceColor pieceColor, QWidget* parent):
	m_Position(boardPosition), m_PieceType(pieceType), m_PieceColor(pieceColor), m_Highlighted(false), m_Selected(false)
{
	
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	updateBackgroundColor();

	updatePiece();
}

