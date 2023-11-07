#include "Utility.h"

#include "gtest/gtest.h"

TEST(TestHorsePossibleMoves, Test_Moves_On_Empty_Squares)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', 'h', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', ' ', ' ', ' ', ' ', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList horsepossibleMoves = game.GetPossibleMoves(Position(4,3));
	PositionList expectedHorseMoves = { Position(2,2),Position(2, 4) ,Position(3, 1) ,Position(3, 5), Position(5, 1) ,Position(5, 5),Position(6, 2),Position(6, 4) };

	EXPECT_EQ(ComparePositionLists(horsepossibleMoves, expectedHorseMoves), true);
}

TEST(TestHorsePossibleMoves, Test_Moves_On_Ocupied_Squares_With_Same_Color)
{
	// Declare board // 

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', 'p', ' ', 'p', ' ', ' ', ' ',   // 2
			' ', 'p', ' ', ' ', ' ', 'p', ' ', ' ',   // 3
			' ', ' ', ' ', 'h', ' ', ' ', ' ', ' ',   // 4
			' ', 'p', ' ', ' ', ' ', 'p', ' ', ' ',   // 5
			' ', ' ', 'p', ' ', 'p', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', ' ', ' ', ' ', ' ', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList horsepossibleMoves = game.GetPossibleMoves(Position(4, 3));
	EXPECT_EQ(horsepossibleMoves.size(), 0);
}

TEST(TestHorsePossibleMoves, Test_Moves_On_Ocupied_Squares_With_Enemy_Color)
{
	// Declare board // 

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', 'P', ' ', 'P', ' ', ' ', ' ',   // 2
			' ', 'P', ' ', ' ', ' ', 'P', ' ', ' ',   // 3
			' ', ' ', ' ', 'h', ' ', ' ', ' ', ' ',   // 4
			' ', 'P', ' ', ' ', ' ', 'P', ' ', ' ',   // 5
			' ', ' ', 'P', ' ', 'P', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', ' ', ' ', ' ', ' ', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList horsepossibleMoves = game.GetPossibleMoves(Position(4, 3));
	PositionList expectedHorseMoves = { Position(2,2),Position(2, 4) ,Position(3, 1) ,Position(3, 5), Position(5, 1) ,Position(5, 5),Position(6, 2),Position(6, 4) };

	EXPECT_EQ(ComparePositionLists(horsepossibleMoves, expectedHorseMoves), true);
}

TEST(TestHorsePossibleMoves, Test_Moves_From_Right_Down_Corner)
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
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', ' ', ' ', ' ', ' ', 'h'    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList horsepossibleMoves = game.GetPossibleMoves(Position(7, 7));
	PositionList expectedHorseMoves = { Position(5,6),Position(6, 5) };

	EXPECT_EQ(ComparePositionLists(horsepossibleMoves, expectedHorseMoves), true);
}

TEST(TestHorsePossibleMoves, Test_Moves_From_Left_Down_Corner)
{
	// Declare board // 

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			' ', ' ', ' ', ' ', ' ', ' ', ' ', 'K',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			'h', ' ', ' ', ' ', ' ', ' ', ' ', 'k'    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList horsepossibleMoves = game.GetPossibleMoves(Position(7, 0));
	PositionList expectedHorseMoves = { Position(5,1),Position(6, 2) };

	EXPECT_EQ(ComparePositionLists(horsepossibleMoves, expectedHorseMoves), true);
}

TEST(TestHorsePossibleMoves, Test_Moves_From_Left_Top_Corner)
{
	// Declare board // 

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'h', ' ', ' ', ' ', ' ', ' ', ' ', 'K',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList horsepossibleMoves = game.GetPossibleMoves(Position(0, 0));
	PositionList expectedHorseMoves = { Position(1,2),Position(2, 1) };

	EXPECT_EQ(ComparePositionLists(horsepossibleMoves, expectedHorseMoves), true);
}

TEST(TestHorsePossibleMoves, Test_Moves_From_Right_Top_Corner)
{
	// Declare board // 

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', 'h',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', ' ', ' ', ' ', ' ', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList horsepossibleMoves = game.GetPossibleMoves(Position(0, 7));
	PositionList expectedHorseMoves = { Position(1,5),Position(2, 6) };

	EXPECT_EQ(ComparePositionLists(horsepossibleMoves, expectedHorseMoves), true);
}

TEST(TestHorsePossibleMoves, Test_Moves_While_King_In_Check_From_Rook)
{
	// Declare board // 

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', 'R', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', 'h', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			' ', ' ', 'k', ' ', ' ', ' ', ' ', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList horsepossibleMoves = game.GetPossibleMoves(Position(3, 4));
	PositionList expectedHorseMoves = { Position(2,2),Position(4, 2) };

	EXPECT_EQ(ComparePositionLists(horsepossibleMoves, expectedHorseMoves), true);
}

TEST(TestHorsePossibleMoves, Test_Moves_While_King_In_Check_From_Bishop)
{
	// Declare board // 

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', 'h', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', 'B',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			' ', ' ', 'k', ' ', ' ', ' ', ' ', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList horsepossibleMoves = game.GetPossibleMoves(Position(1, 5));
	PositionList expectedHorseMoves = { Position(2,7),Position(3, 6) };

	EXPECT_EQ(ComparePositionLists(horsepossibleMoves, expectedHorseMoves), true);
}

TEST(TestHorsePossibleMoves, Test_Moves_While_King_In_Check_From_Horse)
{
	// Declare board // 

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', 'h', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', 'H', ' ', ' ', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', ' ', ' ', ' ', ' ', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList horsepossibleMoves = game.GetPossibleMoves(Position(4, 1));
	PositionList expectedHorseMoves = { Position(6,2) };

	EXPECT_EQ(ComparePositionLists(horsepossibleMoves, expectedHorseMoves), true);                       
}

TEST(TestHorsePossibleMoves, Test_Can_Not_Move_Because_King_Is_Pinned_By_Rook)
{
	// Declare board // 

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', 'R', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', 'h', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			' ', ' ', ' ', 'k', ' ', ' ', ' ', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList horsepossibleMoves = game.GetPossibleMoves(Position(4, 3));
	EXPECT_EQ(horsepossibleMoves.size(), 0);
}

TEST(TestHorsePossibleMoves, Test_Can_Not_Move_Because_King_Is_Pinned_By_Bishop)
{
	// Declare board // 

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', 'B',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', 'h', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', ' ', ' ', ' ', ' ', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList horsepossibleMoves = game.GetPossibleMoves(Position(4, 3));
	EXPECT_EQ(horsepossibleMoves.size(), 0);
}

TEST(TestHorsePossibleMoves, Test_Can_Not_Move_Because_King_Is_In_Check_From_Horse)
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
			' ', ' ', 'h', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', 'H', ' ', ' ', ' ', ' ', ' ',   // 6
			'k', ' ', ' ', ' ', ' ', ' ', ' ', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList horsepossibleMoves = game.GetPossibleMoves(Position(5, 2));
	EXPECT_EQ(horsepossibleMoves.size(), 0);
}

TEST(TestHorsePossibleMoves, Test_Can_Not_Move)
{
	// Declare board // 

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', 'R', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', 'B',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', 'h', ' ', ' ', ' ', ' ', ' ',   // 6
			' ', 'k', ' ', ' ', ' ', ' ', ' ', ' '    // 7
	};

	// Tests //

	ChessGame game(alternativeBoard, EColor::White);

	PositionList horsepossibleMoves = game.GetPossibleMoves(Position(6, 2));
	EXPECT_EQ(horsepossibleMoves.size(), 0);
}