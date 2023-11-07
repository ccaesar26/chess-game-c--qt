#include "gtest/gtest.h"

#include "ChessGame.h"

TEST(InitialPiecesPlacementOnBoard, BlackRook)
{
	ChessGame g;

	EXPECT_EQ(g.GetIPiecePtr(Position(0, 0))->GetType(), EType::Rook);
	EXPECT_EQ(g.GetIPiecePtr(Position(0, 0))->GetColor(), EColor::Black);
	EXPECT_EQ(g.GetIPiecePtr(Position(0, 7))->GetType(), EType::Rook);
	EXPECT_EQ(g.GetIPiecePtr(Position(0, 7))->GetColor(), EColor::Black);
}

TEST(InitialPiecesPlacementOnBoard, BlackHorse)
{
	ChessGame g;

	EXPECT_EQ(g.GetIPiecePtr(Position(0, 1))->GetType(), EType::Horse);
	EXPECT_EQ(g.GetIPiecePtr(Position(0, 1))->GetColor(), EColor::Black);
	EXPECT_EQ(g.GetIPiecePtr(Position(0, 6))->GetType(), EType::Horse);
	EXPECT_EQ(g.GetIPiecePtr(Position(0, 6))->GetColor(), EColor::Black);
}

TEST(InitialPiecesPlacementOnBoard, BlackBishop)
{
	ChessGame g;

	EXPECT_EQ(g.GetIPiecePtr(Position(0, 2))->GetType(), EType::Bishop);
	EXPECT_EQ(g.GetIPiecePtr(Position(0, 2))->GetColor(), EColor::Black);
	EXPECT_EQ(g.GetIPiecePtr(Position(0, 5))->GetType(), EType::Bishop);
	EXPECT_EQ(g.GetIPiecePtr(Position(0, 5))->GetColor(), EColor::Black);
}

TEST(InitialPiecesPlacementOnBoard, BlackQueen)
{
	ChessGame g;

	EXPECT_EQ(g.GetIPiecePtr(Position(0, 3))->GetType(), EType::Queen);
	EXPECT_EQ(g.GetIPiecePtr(Position(0, 3))->GetColor(), EColor::Black);
}

TEST(InitialPiecesPlacementOnBoard, BlackKing)
{
	ChessGame g;

	EXPECT_EQ(g.GetIPiecePtr(Position(0, 4))->GetType(), EType::King);
	EXPECT_EQ(g.GetIPiecePtr(Position(0, 4))->GetColor(), EColor::Black);
}

TEST(InitialPiecesPlacementOnBoard, BlackPawn)
{
	ChessGame g;

	for (int i = 0; i < 7; i++)
	{
		EXPECT_EQ(g.GetIPiecePtr(Position(1, i))->GetType(), EType::Pawn);
		EXPECT_EQ(g.GetIPiecePtr(Position(1, i))->GetColor(), EColor::Black);
	}
}

TEST(InitialPiecesPlacementOnBoard, WhiteRook)
{
	ChessGame g;

	EXPECT_EQ(g.GetIPiecePtr(Position(7, 0))->GetType(), EType::Rook);
	EXPECT_EQ(g.GetIPiecePtr(Position(7, 0))->GetColor(), EColor::White);
	EXPECT_EQ(g.GetIPiecePtr(Position(7, 7))->GetType(), EType::Rook);
	EXPECT_EQ(g.GetIPiecePtr(Position(7, 7))->GetColor(), EColor::White);
}

TEST(InitialPiecesPlacementOnBoard, WhiteHorse)
{
	ChessGame g;

	EXPECT_EQ(g.GetIPiecePtr(Position(7, 1))->GetType(), EType::Horse);
	EXPECT_EQ(g.GetIPiecePtr(Position(7, 1))->GetColor(), EColor::White);
	EXPECT_EQ(g.GetIPiecePtr(Position(7, 6))->GetType(), EType::Horse);
	EXPECT_EQ(g.GetIPiecePtr(Position(7, 6))->GetColor(), EColor::White);
}

TEST(InitialPiecesPlacementOnBoard, WhiteBishop)
{
	ChessGame g;

	EXPECT_EQ(g.GetIPiecePtr(Position(7, 2))->GetType(), EType::Bishop);
	EXPECT_EQ(g.GetIPiecePtr(Position(7, 2))->GetColor(), EColor::White);
	EXPECT_EQ(g.GetIPiecePtr(Position(7, 5))->GetType(), EType::Bishop);
	EXPECT_EQ(g.GetIPiecePtr(Position(7, 5))->GetColor(), EColor::White);
}

TEST(InitialPiecesPlacementOnBoard, WhiteQueen)
{
	ChessGame g;

	EXPECT_EQ(g.GetIPiecePtr(Position(7, 3))->GetType(), EType::Queen);
	EXPECT_EQ(g.GetIPiecePtr(Position(7, 3))->GetColor(), EColor::White);
}

TEST(InitialPiecesPlacementOnBoard, WhiteKing)
{
	ChessGame g;

	EXPECT_EQ(g.GetIPiecePtr(Position(7, 4))->GetType(), EType::King);
	EXPECT_EQ(g.GetIPiecePtr(Position(7, 4))->GetColor(), EColor::White);
}

TEST(InitialPiecesPlacementOnBoard, WhitePawn)
{
	ChessGame g;
	
	for (int i = 0; i < 7; i++)
	{
		EXPECT_EQ(g.GetIPiecePtr(Position(6, i))->GetType(), EType::Pawn);
		EXPECT_EQ(g.GetIPiecePtr(Position(6, i))->GetColor(), EColor::White);
	}
}
