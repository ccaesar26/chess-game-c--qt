#include "gtest/gtest.h"

#include "ChessGame.h"
#include "Utility.h"

// Tests for White // 

TEST(TestCastle, Test_White_Can_Castle)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',   // 6
			'r', ' ', ' ', ' ', 'k', ' ', ' ', 'r'    // 7
	};

	ChessGame game(alternativeBoard, EColor::White);

	// Tests //

	PositionList kingPossbileMoves = game.GetPossibleMoves(Position(7, 4));
	PositionList kingExpectedMoves = {Position(7,5),Position(7,3), Position(7,2), Position(7,6)};

	EXPECT_EQ(ComparePositionLists(kingPossbileMoves, kingExpectedMoves), true);
}

TEST(TestCastle, Test_White_Can_Not_Castle_1)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',   // 6
			'r', ' ', ' ', ' ', 'k', ' ', ' ', 'r'    // 7
	};


	CastleValues Castle = { false,false,true,true };
	ChessGame game(alternativeBoard, EColor::White, Castle);

	// Tests //

	PositionList kingPossbileMoves = game.GetPossibleMoves(Position(7, 4));
	PositionList kingExpectedMoves = { Position(7,5),Position(7,3) };

	EXPECT_EQ(ComparePositionLists(kingPossbileMoves, kingExpectedMoves), true);
}

TEST(TestCastle, Test_White_Can_Not_Castle_2)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', 'p', 'p', 'p', ' ', ' ',   // 6
			'B', ' ', ' ', ' ', 'k', ' ', ' ', 'B'    // 7
	};

	ChessGame game(alternativeBoard, EColor::White);

	// Tests //

	PositionList kingPossbileMoves = game.GetPossibleMoves(Position(7, 4));
	PositionList kingExpectedMoves = { Position(7,5),Position(7,3) };

	EXPECT_EQ(ComparePositionLists(kingPossbileMoves, kingExpectedMoves), true);
}

TEST(TestCastle, Test_White_Can_Not_Castle_3)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', 'p', 'p', 'p', ' ', ' ',   // 6
			'r', 'h', ' ', ' ', 'k', ' ', 'h', 'r'    // 7
	};

	ChessGame game(alternativeBoard, EColor::White);

	// Tests //

	PositionList kingPossbileMoves = game.GetPossibleMoves(Position(7, 4));
	PositionList kingExpectedMoves = { Position(7,5),Position(7,3) };

	EXPECT_EQ(ComparePositionLists(kingPossbileMoves, kingExpectedMoves), true);
}

TEST(TestCastle, Test_White_Can_Not_Castle_4)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', 'R', ' ', ' ', ' ', 'R', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', 'p', 'p', 'p', ' ', ' ',   // 6
			'r', ' ', ' ', ' ', 'k', ' ', ' ', 'r'    // 7
	};

	ChessGame game(alternativeBoard, EColor::White);

	// Tests //

	PositionList kingPossbileMoves = game.GetPossibleMoves(Position(7, 4));
	PositionList kingExpectedMoves = { Position(7,5),Position(7,3) };

	EXPECT_EQ(ComparePositionLists(kingPossbileMoves, kingExpectedMoves), true);
}

TEST(TestCastle, Test_White_Can_Not_Castle_5)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', 'R', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', 'p', ' ', 'p', ' ', ' ',   // 6
			'r', ' ', ' ', ' ', 'k', ' ', ' ', 'r'    // 7
	};

	ChessGame game(alternativeBoard, EColor::White);

	// Tests //

	PositionList kingPossbileMoves = game.GetPossibleMoves(Position(7, 4));
	PositionList kingExpectedMoves = { Position(7,5),Position(7,3) };

	EXPECT_EQ(ComparePositionLists(kingPossbileMoves, kingExpectedMoves), true);
}

// Tests for Black // 

TEST(TestCastle, Test_Black_Castle_1)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'R', ' ', ' ', ' ', 'K', ' ', ' ', 'R',   // 0
			'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 7
	};

	ChessGame game(alternativeBoard, EColor::Black);

	// Tests //

	PositionList kingPossbileMoves = game.GetPossibleMoves(Position(0, 4));
	PositionList kingExpectedMoves = { Position(0,5),Position(0,3), Position(0,2), Position(0,6) };

	EXPECT_EQ(ComparePositionLists(kingPossbileMoves, kingExpectedMoves), true);
}

TEST(TestCastle, Test_Black_Can_Not_Castle_1)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'R', ' ', ' ', ' ', 'K', ' ', ' ', 'R',   // 0
			'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 7
	};


	CastleValues Castle = { true,true,false,false };
	ChessGame game(alternativeBoard, EColor::Black, Castle);

	// Tests //

	PositionList kingPossbileMoves = game.GetPossibleMoves(Position(0, 4));
	PositionList kingExpectedMoves = { Position(0,5),Position(0,3) };

	EXPECT_EQ(ComparePositionLists(kingPossbileMoves, kingExpectedMoves), true);
}

TEST(TestCastle, Test_Black_Can_Not_Castle_2)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'R', ' ', 'B', ' ', 'K', ' ', 'H', 'R',   // 0
			'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 7
	};

	ChessGame game(alternativeBoard, EColor::Black);

	// Tests //

	PositionList kingPossbileMoves = game.GetPossibleMoves(Position(0, 4));
	PositionList kingExpectedMoves = { Position(0,5),Position(0,3) };

	EXPECT_EQ(ComparePositionLists(kingPossbileMoves, kingExpectedMoves), true);
}

TEST(TestCastle, Test_Black_Can_Not_Castle_3)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'R', ' ', ' ', ' ', 'K', ' ', ' ', 'R',   // 0
			' ', ' ', ' ', 'P', 'P', 'P', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			'k', ' ', 'r', ' ', ' ', ' ', 'r', ' ',   // 7
	};

	ChessGame game(alternativeBoard, EColor::Black);

	// Tests //

	PositionList kingPossbileMoves = game.GetPossibleMoves(Position(0, 4));
	PositionList kingExpectedMoves = { Position(0,5),Position(0,3) };

	EXPECT_EQ(ComparePositionLists(kingPossbileMoves, kingExpectedMoves), true);
}

TEST(TestCastle, Test_Black_Can_Not_Castle_4)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'R', ' ', ' ', ' ', 'K', ' ', ' ', 'R',   // 0
			' ', ' ', ' ', 'P', ' ', 'P', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', ' ', 'r', ' ', ' ', ' ',   // 7
	};

	ChessGame game(alternativeBoard, EColor::Black);

	// Tests //

	PositionList kingPossbileMoves = game.GetPossibleMoves(Position(0, 4));
	PositionList kingExpectedMoves = { Position(0,5),Position(0,3) };

	EXPECT_EQ(ComparePositionLists(kingPossbileMoves, kingExpectedMoves), true);
}

TEST(TestCastle, Test_Black_Can_Not_Castle_5)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'h', ' ', ' ', ' ', 'K', ' ', ' ', 'h',   // 0
			' ', ' ', ' ', 'P', 'P', 'P', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 7
	};

	ChessGame game(alternativeBoard, EColor::Black);

	// Tests //

	PositionList kingPossbileMoves = game.GetPossibleMoves(Position(0, 4));
	PositionList kingExpectedMoves = { Position(0,5),Position(0,3) };

	EXPECT_EQ(ComparePositionLists(kingPossbileMoves, kingExpectedMoves), true);
}
