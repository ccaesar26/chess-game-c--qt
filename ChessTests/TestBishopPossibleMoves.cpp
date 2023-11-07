#include "Utility.h"

#include "gtest/gtest.h"

TEST(TestBishopPossibleMoves, Test_Moves_On_Empty_Squares)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			' ', 'K', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', 'b', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			' ', 'k', ' ', ' ', ' ', ' ', ' ', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList bishopPossibleMoves = game.GetPossibleMoves(Position(4, 3));
	PositionList bishopExpectedMoves = { Position(3,2),Position(2, 1) ,Position(1, 0) ,Position(3,4),Position(2, 5) ,Position(1, 6) , Position(0, 7)
	,Position(5, 2),Position(6, 1),Position(7, 0),Position(5, 4),Position(6, 5),Position(7, 6) };

	EXPECT_EQ(ComparePositionLists(bishopPossibleMoves, bishopExpectedMoves),true);
}

TEST(TestBishopPossibleMoves, Test_Moves_On_Ocupied_Squares_With_Same_Color)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			' ', 'K', ' ', ' ', ' ', ' ', ' ', 'p',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', 'p', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', 'b', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', 'p', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			' ', 'k', ' ', ' ', ' ', ' ', 'p', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList bishopPossibleMoves = game.GetPossibleMoves(Position(4, 3));
	PositionList bishopExpectedMoves = { Position(3,4),Position(2,5),Position(1,6), Position(5,2),Position(5,4),Position(6,5) };

	EXPECT_EQ(ComparePositionLists(bishopPossibleMoves, bishopExpectedMoves), true);
}

TEST(TestBishopPossibleMoves, Test_Moves_On_Ocupied_Squares_With_Enemy_Color)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			' ', 'K', ' ', ' ', ' ', ' ', ' ', 'H',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', 'H', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', 'b', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', 'H', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			' ', 'k', ' ', ' ', ' ', ' ', 'H', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList bishopPossibleMoves = game.GetPossibleMoves(Position(4, 3));
	PositionList bishopExpectedMoves = { Position(3,2),Position(3,4),Position(2,5),Position(1,6),Position(0,7),
		Position(5,2),Position(6,1),Position(5,4),Position(6,5),Position(7,6) };

	EXPECT_EQ(ComparePositionLists(bishopPossibleMoves, bishopExpectedMoves), true);
}

TEST(TestBishopPossibleMoves, Test_Moves_While_King_In_Check_From_Rook)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			' ', 'K', ' ', 'R', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', 'b', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			' ', ' ', ' ', 'k', ' ', ' ', ' ', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList bishopPossibleMoves = game.GetPossibleMoves(Position(3, 6));
	PositionList bishopExpectedMoves = { Position(0,3), Position(6,3) };

	EXPECT_EQ(ComparePositionLists(bishopPossibleMoves, bishopExpectedMoves), true);
}

TEST(TestBishopPossibleMoves, Test_Moves_While_King_In_Check_From_Bishop_1)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', 'B',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', ' ', ' ', ' ', 'b', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList bishopPossibleMoves = game.GetPossibleMoves(Position(7, 6));
	PositionList bishopExpectedMoves = { Position(4,3) };

	EXPECT_EQ(ComparePositionLists(bishopPossibleMoves, bishopExpectedMoves), true);
}

TEST(TestBishopPossibleMoves, Test_Moves_While_King_In_Check_From_Bishop_2)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', 'B', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', ' ', ' ', ' ', 'b', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList bishopPossibleMoves = game.GetPossibleMoves(Position(7, 6));
	PositionList bishopExpectedMoves = { Position(4,3) };

	EXPECT_EQ(ComparePositionLists(bishopPossibleMoves, bishopExpectedMoves), true);
}

TEST(TestBishopPossibleMoves, Test_Moves_While_King_In_Check_From_Horse_1)
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
			' ', 'H', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', 'b', ' ', ' ', ' ', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList bishopPossibleMoves = game.GetPossibleMoves(Position(7, 3));
	PositionList bishopExpectedMoves = { Position(5,1) };

	EXPECT_EQ(ComparePositionLists(bishopPossibleMoves, bishopExpectedMoves), true);
}

TEST(TestBishopPossibleMoves, Test_Moves_While_King_In_Check_From_Horse_2)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', 'b', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', 'H', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', ' ', ' ', ' ', ' ', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList bishopPossibleMoves = game.GetPossibleMoves(Position(4, 1));

	EXPECT_EQ(bishopPossibleMoves.size(), 0);
}

TEST(TestBishopPossibleMoves, Test_Moves_While_King_Is_Pinned_By_Rook)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', 'R', ' ', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', 'b', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			' ', ' ', 'k', ' ', ' ', ' ', ' ', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList bishopPossibleMoves = game.GetPossibleMoves(Position(4, 2));
	
	EXPECT_EQ(bishopPossibleMoves.size(), 0);
}

TEST(TestBishopPossibleMoves, Test_Moves_While_King_Is_Pinned_By_Bishop)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', 'B',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', 'b', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', ' ', ' ', ' ', ' ', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList bishopPossibleMoves = game.GetPossibleMoves(Position(4, 3));
	PositionList bishopExpectedMoves = { Position(3,4), Position(2,5), Position(1,6),Position(0,7),Position(5,2),Position(6,1) };

	EXPECT_EQ(ComparePositionLists(bishopPossibleMoves, bishopExpectedMoves), true);
}
