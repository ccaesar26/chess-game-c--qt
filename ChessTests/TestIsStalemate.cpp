#include "gtest/gtest.h"

#include "ChessGame.h"
#include "Utility.h"

TEST(TestIsStaleMate, Test_Stale_Mate_1)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', 'R', ' ', ' ', ' ', ' ', 'R', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'    // 7
	};

	ChessGame game(alternativeBoard, EColor::Black);

	// Tests //

	game.MakeMove(Position(0, 1), Position(6, 1));

	EXPECT_EQ(game.IsDraw(),true);
}

TEST(TestIsStaleMate, Test_Stale_Mate_2)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
			'Q', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'    // 7
	};

	ChessGame game(alternativeBoard, EColor::Black);

	// Tests //

	game.MakeMove(Position(1, 0), Position(6, 5));

	EXPECT_EQ(game.IsDraw(), true);
}

TEST(TestIsStaleMate, Test_Stale_Mate_3)
{
	// Declare board //

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
			'Q', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			'p', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			' ', ' ', ' ', ' ', ' ', ' ', ' ', 'k'    // 7
	};

	ChessGame game(alternativeBoard, EColor::Black);

	// Tests //

	game.MakeMove(Position(1, 0), Position(6, 5));

	EXPECT_EQ(game.IsDraw(), false);
}
