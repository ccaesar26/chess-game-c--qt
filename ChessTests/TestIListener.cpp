#include "IChessGameListener.h"
#include "ChessGame.h"
#include "ChessException.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::InSequence;
using ::testing::_;

class MockListener : public IChessGameListener
{

public:

	MOCK_METHOD(void, OnMoveMade, (Position init, Position fin), (override));

	MOCK_METHOD(void, OnGameOver, (EGameResult result), (override));

	MOCK_METHOD(void, OnPawnUpgrade, (Position pos), (override));

	MOCK_METHOD(void, OnCheck, (), (override));

	MOCK_METHOD(void, OnGameRestarted, (), (override));

	MOCK_METHOD(void, OnHistoryUpdate, (std::string move), (override));

	MOCK_METHOD(void, OnClockUpdate, (), (override));

	MOCK_METHOD(void, OnTimesUp, (), (override));
};

TEST(OnMoveMadeIsCalled, LegalMove1) 
{
	ChessGame game;

	auto listener = std::make_shared<MockListener>();

	game.AddListener(listener);

	EXPECT_CALL(*listener, OnHistoryUpdate(::testing::_))
		.Times(::testing::AnyNumber());

	EXPECT_CALL(*listener, OnMoveMade(Position(6, 4), Position(4, 4)))
		.Times(1);

	game.MakeMove(Position(6, 4), Position(4, 4));
}

TEST(OnMoveMadeIsCalled, IllegalMove1)
{
	ChessGame game;

	auto listener = std::make_shared<MockListener>();

	game.AddListener(listener);


	EXPECT_CALL(*listener, OnMoveMade(::testing::_, ::testing::_))
		.Times(0);

	try
	{
		EXPECT_THROW(game.MakeMove(Position(7, 6), Position(6, 4)), InvalidMoveException);
	}
	catch (const std::exception&)
	{}
}

TEST(OnMoveMadeIsCalled, IllegalMove2)
{
	ChessGame game;

	auto listener = std::make_shared<MockListener>();

	game.AddListener(listener);


	EXPECT_CALL(*listener, OnMoveMade(::testing::_, ::testing::_))
		.Times(0);

	try
	{
		EXPECT_THROW(game.MakeMove(Position(8, 3), Position(3, 9)), OutOfBoundsException);
	}
	catch (const std::exception&)
	{}
}

TEST(OnMoveMadeIsCalled, IllegalMove3)
{
	ChessGame game;

	auto listener = std::make_shared<MockListener>();

	game.AddListener(listener);


	EXPECT_CALL(*listener, OnMoveMade(::testing::_, ::testing::_))
		.Times(0);

	try
	{
		EXPECT_THROW(game.MakeMove(Position(6, 0), Position(5, 1)), InvalidMoveException);
	}
	catch (const std::exception&)
	{}
}

TEST(OnMoveMadeIsCalled, IllegalMove4)
{

	std::array<std::array<char, 8>, 8> alternativeBoard =
	{
		//   0    1    2    3    4    5    6    7

			'K', 'R', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 1		
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
			' ', 'r', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
			' ', 'k', ' ', ' ', ' ', ' ', ' ', ' '    // 7
	};

	ChessGame game(alternativeBoard, EColor::White);

	auto listener = std::make_shared<MockListener>();

	game.AddListener(listener);


	EXPECT_CALL(*listener, OnMoveMade(::testing::_, ::testing::_))
		.Times(0);

	try
	{
		EXPECT_THROW(game.MakeMove(Position(6, 1), Position(6, 7)), InvalidMoveException);
	}
	catch (const std::exception&)
	{}
}

TEST(OnMoveMadeIsCalled, IllegalMove5)
{
	ChessGame game;

	auto listener = std::make_shared<MockListener>();

	game.AddListener(listener);


	EXPECT_CALL(*listener, OnMoveMade(::testing::_, ::testing::_))
		.Times(0);

	try
	{
		EXPECT_THROW(game.MakeMove(Position(1, 0), Position(2, 0)), InvalidMoveException);
	}
	catch (const std::exception&)
	{
	}
}

TEST(ListenersFunction, GameSimulation_1)
{
	ChessGame game;
	auto listener = std::make_shared<MockListener>();
	game.AddListener(listener);

	EXPECT_CALL(*listener, OnHistoryUpdate(::testing::_))
		.Times(::testing::AnyNumber());

	try
	{
		{
			InSequence sequence;

			EXPECT_CALL(*listener, OnMoveMade(Position(6, 3), Position(4, 3)))
				.Times(1)
				.RetiresOnSaturation();

			EXPECT_CALL(*listener, OnMoveMade(Position(1, 2), Position(3, 2)))
				.Times(1)
				.RetiresOnSaturation();

			EXPECT_CALL(*listener, OnMoveMade(Position(4, 3), Position(3, 2)))
				.Times(1)
				.RetiresOnSaturation();

			EXPECT_CALL(*listener, OnMoveMade(Position(0, 3), Position(1, 2)))
				.Times(1)
				.RetiresOnSaturation();

		}

		game.MakeMove(Position(6, 3), Position(4, 3));  // White
		game.MakeMove(Position(1, 2), Position(3, 2));  // Black
		game.MakeMove(Position(4, 3), Position(3, 2));  // White
		game.MakeMove(Position(0, 3), Position(1, 2));	// Black

		{
			InSequence sequence;

			EXPECT_CALL(*listener, OnMoveMade(Position(7, 3), Position(6, 3)))
				.Times(1)
				.RetiresOnSaturation();

			EXPECT_CALL(*listener, OnMoveMade(Position(1, 2), Position(3, 2)))
				.Times(1)
				.RetiresOnSaturation();

			EXPECT_CALL(*listener, OnMoveMade(Position(6, 3), Position(1, 3)))
				.Times(1)
				.RetiresOnSaturation();

			EXPECT_CALL(*listener, OnCheck())
				.Times(1)
				.RetiresOnSaturation();

		}

		game.MakeMove(Position(7, 3), Position(6, 3));	// White
		game.MakeMove(Position(1, 2), Position(3, 2));	// Black
		game.MakeMove(Position(6, 3), Position(1, 3));	// White
		EXPECT_THROW(game.MakeMove(Position(0, 0), Position(1, 3)), InvalidMoveException);	// Black 

		{
			InSequence sequence;

			EXPECT_CALL(*listener, OnMoveMade(Position(0, 2), Position(1, 3)))
				.Times(1)
				.RetiresOnSaturation();

			EXPECT_CALL(*listener, OnMoveMade(Position(6, 1), Position(4, 1)))
				.Times(1)
				.RetiresOnSaturation();

			EXPECT_CALL(*listener, OnMoveMade(Position(3, 2), Position(4, 1)))
				.Times(1)
				.RetiresOnSaturation();

			EXPECT_CALL(*listener, OnCheck())
				.Times(1)
				.RetiresOnSaturation();
		}

		game.MakeMove(Position(0, 2), Position(1, 3));	// Black
		game.MakeMove(Position(6, 1), Position(4, 1));	// White
		game.MakeMove(Position(3, 2), Position(4, 1));	// Black
		EXPECT_THROW(game.MakeMove(Position(7, 4), Position(6, 3)), InvalidMoveException);	// White

		{
			InSequence seq;

			EXPECT_CALL(*listener, OnMoveMade(Position(6, 2), Position(5, 2)))
				.Times(1)
				.RetiresOnSaturation();

			EXPECT_CALL(*listener, OnMoveMade(Position(4, 1), Position(2, 3)))
				.Times(1)
				.RetiresOnSaturation();

			EXPECT_CALL(*listener, OnMoveMade(Position(7, 6), Position(5, 5)))
				.Times(1)
				.RetiresOnSaturation();

			EXPECT_CALL(*listener, OnMoveMade(Position(0, 1), Position(2, 0)))
				.Times(1)
				.RetiresOnSaturation();
		}

		game.MakeMove(Position(6, 2), Position(5, 2));	// White
		EXPECT_THROW(game.MakeMove(Position(4, 1), Position(1, 4)), InvalidMoveException);	// Black
		game.MakeMove(Position(4, 1), Position(2, 3)); // black
		game.MakeMove(Position(7, 6), Position(5, 5)); // White
		game.MakeMove(Position(0, 1), Position(2, 0)); // black

		{
			InSequence seq;

			EXPECT_CALL(*listener, OnMoveMade(Position(6, 6), Position(5, 6)))
				.Times(1)
				.RetiresOnSaturation();

			// ! IMPORTANT ! AT CASTLE OnMoveMade is called twice : first for Rook, second for King

			EXPECT_CALL(*listener, OnMoveMade(Position(0, 0), Position(0, 3)))
				.Times(1)
				.RetiresOnSaturation();

			EXPECT_CALL(*listener, OnMoveMade(Position(0, 4), Position(0, 2)))
				.Times(1)
				.RetiresOnSaturation(); 
			//

			EXPECT_CALL(*listener, OnMoveMade(Position(7, 5), Position(6, 6)))
				.Times(1)
				.RetiresOnSaturation();

			EXPECT_CALL(*listener, OnMoveMade(Position(1, 3), Position(2, 4)))
				.Times(1)
				.RetiresOnSaturation();

			EXPECT_CALL(*listener, OnMoveMade(Position(6, 0), Position(5, 0)))
				.Times(1)
				.RetiresOnSaturation();
		}

		game.MakeMove(Position(6, 6), Position(5, 6)); // White	
		game.MakeMove(Position(0, 4), Position(0, 2)); // black
		game.MakeMove(Position(7, 5), Position(6, 6)); // White	
		game.MakeMove(Position(1, 3), Position(2, 4)); // black
		game.MakeMove(Position(6, 0), Position(5, 0)); // White	

		{
			InSequence seq;

			EXPECT_CALL(*listener, OnMoveMade(Position(2, 3), Position(7, 3)))
				.Times(1)
				.RetiresOnSaturation();

			// ! IMPORTANT ! Even though the game is over after a move, the state is still changed to "check" before ending the game

			EXPECT_CALL(*listener, OnCheck())
				.Times(1)
				.RetiresOnSaturation();

			EXPECT_CALL(*listener, OnGameOver(EGameResult::BlackPlayerWon))
				.Times(1)
				.RetiresOnSaturation();

		}
		
		game.MakeMove(Position(2, 3), Position(7, 3)); // black

	}
	catch (const std::exception&)
	{
	}	
}

//TEST(OnGameOver, AfterPawnUpgrade_IsStealMate)
//{
//	std::array<std::array<char, 8>, 8> alternativeBoard =
//	{
//		   //0    1    2    3    4    5    6    7
//
//			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 0
//			' ', ' ', ' ', ' ', ' ', 'p', ' ', 'K',   // 1		
//			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 2
//			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 3
//			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 4
//			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 5
//			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // 6
//			'k', ' ', ' ', ' ', ' ', ' ', 'r', ' '    // 7
//	};
//
//	ChessGame game(alternativeBoard, EColor::White);
//	auto listener = std::make_shared<MockListener>();
//	game.AddListener(listener);
//
//	EXPECT_CALL(*listener, OnMoveMade(Position(1, 5), Position(0, 5)))
//		.Times(1);
//
//	EXPECT_CALL(*listener, OnPawnUpgrade(Position(0, 5)))
//		.Times(1);
//
//	EXPECT_CALL(*listener, OnGameOver(EGameResult::Draw))
//		.Times(1);
//	
//	game.MakeMove(Position(1, 5), Position(0, 5));
//	game.UpgradePawn(EType::Queen);
//
//}
