#include "gtest/gtest.h"

#include "ChessGame.h"
#include "ChessException.h"

#include <iostream>

TEST(MakeMove, MoveOnEmptySquare)
{
	ChessGame g;

	IPiecePtr toBeMoved = g.GetIPiecePtr(Position(6, 3));

	g.MakeMove(Position(6, 3), Position(4, 3));

	EXPECT_EQ(g.GetIPiecePtr(Position(6, 3)).get(), nullptr);
	EXPECT_EQ(g.GetIPiecePtr(Position(4, 3)), toBeMoved);
}

TEST(MakeMove, MoveAndCapture)
{
	ChessGame g({
		'R', ' ', 'B', 'Q', 'K', 'B', 'H', 'R',
		'P', ' ', 'P', 'P', ' ', 'P', 'P', 'P',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', 'P', ' ', ' ', 'H', ' ', ' ', ' ',
		' ', ' ', 'p', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', 'h', ' ', ' ', ' ', ' ', ' ',
		'p', 'p', ' ', 'p', 'p', ' ', 'p', 'p',
		'r', ' ', 'b', 'q', 'k', 'b', 'h', 'r'
		});

	IPiecePtr toBeCaptured = g.GetIPiecePtr(Position(3, 1));
	IPiecePtr toBeMoved = g.GetIPiecePtr(Position(5, 2));

	g.MakeMove(Position(5, 2), Position(3, 1));

	EXPECT_EQ(g.GetIPiecePtr(Position(3, 1)), toBeMoved);
	EXPECT_EQ(g.GetIPiecePtr(Position(5, 2)).get(), nullptr);
	EXPECT_EQ(g.GetCapturedPieces(EColor::Black).back(), toBeCaptured);
}

TEST(MakeMove, MoveOutOfBounds)
{
	ChessGame g({
		'R', ' ', 'B', 'Q', 'K', 'B', 'H', 'R',
		'P', ' ', 'P', 'P', ' ', 'P', 'P', 'P',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', 'P', ' ', ' ', 'H', ' ', ' ', ' ',
		' ', ' ', 'p', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', 'h', ' ', ' ', ' ', ' ', ' ',
		'p', 'p', ' ', 'p', 'p', ' ', 'p', 'p',
		'r', ' ', 'b', 'q', 'k', 'b', 'h', 'r'
		});

	IPiecePtr toBeMoved = g.GetIPiecePtr(Position(7, 6));

	EXPECT_THROW(g.MakeMove(Position(7, 6), Position(6, 8)), OutOfBoundsException);

	EXPECT_EQ(g.GetIPiecePtr(Position(7, 6)), toBeMoved);
}

TEST(MakeMove, MoveOnOccupiedByOwnPieceSquare)
{
	ChessGame g({
		'R', ' ', 'B', 'Q', 'K', 'B', 'H', 'R',
		'P', ' ', 'P', 'P', ' ', 'P', 'P', 'P',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', 'P', ' ', ' ', 'H', ' ', ' ', ' ',
		' ', ' ', 'p', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', 'h', ' ', ' ', ' ', ' ', ' ',
		'p', 'p', ' ', 'p', 'p', ' ', 'p', 'p',
		'r', ' ', 'b', 'q', 'k', 'b', 'h', 'r'
		});

	IPiecePtr toBeMoved = g.GetIPiecePtr(Position(7, 0));

	EXPECT_THROW(g.MakeMove(Position(7, 0), Position(6, 0)), OccupiedByOwnPieceException);

	EXPECT_EQ(g.GetIPiecePtr(Position(7, 0)), toBeMoved);
}

TEST(MakeMove, CastleMove)
{
	ChessGame g({
		'R', ' ', 'B', 'Q', 'K', 'B', 'H', 'R',
		'P', ' ', 'P', 'P', ' ', 'P', 'P', 'P',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', 'P', ' ', ' ', 'H', ' ', ' ', ' ',
		' ', ' ', 'p', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', 'h', ' ', ' ', ' ', ' ', ' ',
		'p', 'p', ' ', 'p', 'p', ' ', 'p', 'p',
		'r', ' ', 'b', 'q', 'k', ' ', ' ', 'r'
		});

	IPiecePtr king = g.GetIPiecePtr(Position(7, 4));
	IPiecePtr rook = g.GetIPiecePtr(Position(7, 7));

	g.MakeMove(Position(7, 4), Position(7, 6));

	EXPECT_EQ(g.GetIPiecePtr(Position(7, 6)), king);
	EXPECT_EQ(g.GetIPiecePtr(Position(7, 5)), rook);

	EXPECT_EQ(g.GetIPiecePtr(Position(7, 4)).get(), nullptr);
	EXPECT_EQ(g.GetIPiecePtr(Position(7, 7)).get(), nullptr);
}

TEST(MakeMove, CastleMoveByPassingCheckState)
{
	ChessGame g({
		'R', ' ', 'B', 'Q', 'K', 'B', 'H', 'R',
		'P', ' ', 'P', 'P', ' ', 'P', 'P', 'P',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', 'P', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', 'p', ' ', ' ', ' ', ' ', ' ',
		'p', 'p', 'h', ' ', 'H', ' ', ' ', ' ',
		' ', ' ', ' ', 'p', 'p', ' ', 'p', 'p',
		'r', ' ', 'b', 'q', 'k', ' ', ' ', 'r'
		});

	IPiecePtr king = g.GetIPiecePtr(Position(7, 4));
	IPiecePtr rook = g.GetIPiecePtr(Position(7, 7));

	EXPECT_THROW(g.MakeMove(Position(7, 4), Position(7, 6)), NotInPossibleMovesException);

	EXPECT_EQ(g.GetIPiecePtr(Position(7, 4)), king);
	EXPECT_EQ(g.GetIPiecePtr(Position(7, 7)), rook);

	EXPECT_EQ(g.GetIPiecePtr(Position(7, 5)).get(), nullptr);
	EXPECT_EQ(g.GetIPiecePtr(Position(7, 6)).get(), nullptr);
}