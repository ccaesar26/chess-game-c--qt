#pragma once

#include <stdexcept>

// Generic exception class //

/**
 * @brief Base exception class for chess-related errors.
 *
 * This exception class is derived from `std::logic_error` and serves as the base class for all
 * chess-related exception classes. It provides constructors for creating exceptions with or
 * without custom error messages.
 */
class ChessException : public std::logic_error
{
public:
	/**
	 * @brief Default constructor for the base exception class.
	 *
	 * Constructs a `ChessException` with a default error message ("Unknown exception").
	 */
	ChessException();

	/**
	 * @brief Constructor for the base exception class with a custom message.
	 *
	 * @param message A custom error message describing the exception.
	 */
	ChessException(const std::string& message);
};

/**
 * @brief Inline constructor implementation for the default ChessException constructor.
 */
inline ChessException::ChessException()
	: std::logic_error("Unknown exception")
{
}

/**
 * @brief Inline constructor implementation for the ChessException constructor with a custom message.
 *
 * @param message A custom error message describing the exception.
 */
inline ChessException::ChessException(const std::string& message)
	: std::logic_error(message)
{
}


 // Specialized exception classes //

 // 1.	Invalid move:
 //		a.	Out of bounds
 //		b.	Occupied square
 //			i.	By enemy piece
 //			ii.	By own piece
 //		c.	NotInPossibleMovesException	
 //		d.	InitialSquareIsEmptyException

/**
 * @brief Exception class for invalid chess moves.
 *
 * This exception class is derived from the `ChessException` base class and is used to represent
 * errors related to invalid chess moves. It provides constructors for creating exceptions with
 * custom error messages.
 */
class InvalidMoveException : public ChessException
{
public:
	/**
	 * @brief Constructor for the InvalidMoveException class with a custom message.
	 *
	 * @param message A custom error message describing the invalid move error.
	 */
	InvalidMoveException(const std::string& message);
};

/**
 * @brief Inline constructor implementation for the InvalidMoveException constructor with a custom message.
 *
 * @param message A custom error message describing the invalid move error.
 */
inline InvalidMoveException::InvalidMoveException(const std::string& message)
	: ChessException(message)
{
}

/**
 * @brief Exception class for moves that are out of bounds on the chessboard.
 *
 * This exception class is derived from the `InvalidMoveException` class and is used to represent
 * errors related to moves that are outside the bounds of the chessboard. It provides constructors
 * for creating exceptions with custom error messages.
 */
class OutOfBoundsException : public InvalidMoveException
{
public:
	/**
	 * @brief Constructor for the OutOfBoundsException class with a custom message.
	 *
	 * @param message A custom error message describing the out-of-bounds error.
	 */
	OutOfBoundsException(const std::string& message);

private:
	// No additional members at the moment.
};

/**
 * @brief Inline constructor implementation for the OutOfBoundsException constructor with a custom message.
 *
 * @param message A custom error message describing the out-of-bounds error.
 */
inline OutOfBoundsException::OutOfBoundsException(const std::string& message)
	: InvalidMoveException(message)
{
}

/**
 * @brief Exception class for moves to occupied squares on the chessboard.
 *
 * This exception class is derived from the `InvalidMoveException` class and is used to represent
 * errors related to moves to squares that are already occupied on the chessboard. It provides
 * constructors for creating exceptions with custom error messages.
 */
class OccupiedSquareException : public InvalidMoveException
{
public:
	/**
	 * @brief Constructor for the OccupiedSquareException class with a custom message.
	 *
	 * @param message A custom error message describing the occupied square error.
	 */
	OccupiedSquareException(const std::string& message);

private:
	// No additional members at the moment.
};

/**
 * @brief Inline constructor implementation for the OccupiedSquareException constructor with a custom message.
 *
 * @param message A custom error message describing the occupied square error.
 */
inline OccupiedSquareException::OccupiedSquareException(const std::string& message)
	: InvalidMoveException(message)
{
}

/**
 * @brief Exception class for moves to squares occupied by enemy pieces on the chessboard.
 *
 * This exception class is derived from the `OccupiedSquareException` class and is used to represent
 * errors related to moves to squares that are already occupied by enemy pieces on the chessboard. It
 * provides constructors for creating exceptions with custom error messages.
 */
class OccupiedByEnemyPieceException : public OccupiedSquareException
{
public:
	/**
	 * @brief Constructor for the OccupiedByEnemyPieceException class with a custom message.
	 *
	 * @param message A custom error message describing the occupied by enemy piece error.
	 */
	OccupiedByEnemyPieceException(const std::string& message);
};

/**
 * @brief Inline constructor implementation for the OccupiedByEnemyPieceException constructor with a custom message.
 *
 * @param message A custom error message describing the occupied by enemy piece error.
 */
inline OccupiedByEnemyPieceException::OccupiedByEnemyPieceException(const std::string& message)
	: OccupiedSquareException(message)
{
}

/**
 * @brief Exception class for moves to squares occupied by own pieces on the chessboard.
 *
 * This exception class is derived from the `OccupiedSquareException` class and is used to represent
 * errors related to moves to squares that are already occupied by own pieces on the chessboard. It
 * provides constructors for creating exceptions with custom error messages.
 */
class OccupiedByOwnPieceException : public OccupiedSquareException
{
public:
	/**
	 * @brief Constructor for the OccupiedByOwnPieceException class with a custom message.
	 *
	 * @param message A custom error message describing the occupied by own piece error.
	 */
	OccupiedByOwnPieceException(const std::string& message);
};

/**
 * @brief Inline constructor implementation for the OccupiedByOwnPieceException constructor with a custom message.
 *
 * @param message A custom error message describing the occupied by own piece error.
 */
inline OccupiedByOwnPieceException::OccupiedByOwnPieceException(const std::string& message)
	: OccupiedSquareException(message)
{
}

/**
 * @brief Exception class for moves that are not within the list of possible moves.
 *
 * This exception class is derived from the `InvalidMoveException` class and is used to represent
 * errors related to moves that are not included in the list of possible moves for a chess piece.
 * It provides constructors for creating exceptions with custom error messages.
 */
class NotInPossibleMovesException : public InvalidMoveException
{
public:
	/**
	 * @brief Constructor for the NotInPossibleMovesException class with a custom message.
	 *
	 * @param message A custom error message describing the not in possible moves error.
	 */
	NotInPossibleMovesException(const std::string& message);

private:
	// No additional members at the moment.
};

/**
 * @brief Inline constructor implementation for the NotInPossibleMovesException constructor with a custom message.
 *
 * @param message A custom error message describing the not in possible moves error.
 */
inline NotInPossibleMovesException::NotInPossibleMovesException(const std::string& message)
	: InvalidMoveException(message)
{
}

/**
 * @brief Exception class for moves from empty initial squares on the chessboard.
 *
 * This exception class is derived from the `InvalidMoveException` class and is used to represent
 * errors related to moves from empty initial squares on the chessboard. It provides constructors
 * for creating exceptions with custom error messages.
 */
class InitialSquareIsEmptyException : public InvalidMoveException
{
public:
	/**
	 * @brief Constructor for the InitialSquareIsEmptyException class with a custom message.
	 *
	 * @param message A custom error message describing the initial square is empty error.
	 */
	InitialSquareIsEmptyException(const std::string& message);
};

/**
 * @brief Inline constructor implementation for the InitialSquareIsEmptyException constructor with a custom message.
 *
 * @param message A custom error message describing the initial square is empty error.
 */
inline InitialSquareIsEmptyException::InitialSquareIsEmptyException(const std::string& message)
	: InvalidMoveException(message)
{
}

// 2.	Invalid input:
//		a.	Invalid answer
//		b.	Invalid upgrade
//		c.	Invalid position
//			i.	Invalid BoardPosition
//			ii.	Invalid MemoryPosition

/**
 * @brief Exception class for invalid input errors.
 *
 * This exception class is derived from the `ChessException` base class and is used to represent
 * errors related to invalid input in the chess game. It provides constructors for creating
 * exceptions with custom error messages.
 */
class InvalidInputException : public ChessException
{
public:
	/**
	 * @brief Constructor for the InvalidInputException class with a custom message.
	 *
	 * @param message A custom error message describing the invalid input error.
	 */
	InvalidInputException(const std::string& message);
};

/**
 * @brief Inline constructor implementation for the InvalidInputException constructor with a custom message.
 *
 * @param message A custom error message describing the invalid input error.
 */
inline InvalidInputException::InvalidInputException(const std::string& message)
	: ChessException(message)
{
}

/**
 * @brief Exception class for invalid answers to prompts or questions.
 *
 * This exception class is derived from the `InvalidInputException` class and is used to represent
 * errors related to invalid answers given in response to prompts or questions in the chess game.
 * It provides constructors for creating exceptions with custom error messages.
 */
class InvalidAnswerException : public InvalidInputException
{
public:
	/**
	 * @brief Constructor for the InvalidAnswerException class with a custom message.
	 *
	 * @param message A custom error message describing the invalid answer error.
	 */
	InvalidAnswerException(const std::string& message);
};

/**
 * @brief Inline constructor implementation for the InvalidAnswerException constructor with a custom message.
 *
 * @param message A custom error message describing the invalid answer error.
 */
inline InvalidAnswerException::InvalidAnswerException(const std::string& message)
	: InvalidInputException(message)
{
}

/**
 * @brief Exception class for invalid upgrades during pawn promotion.
 *
 * This exception class is derived from the `InvalidInputException` class and is used to represent
 * errors related to invalid upgrades during pawn promotion in the chess game. It provides constructors
 * for creating exceptions with custom error messages.
 */
class InvalidUpgradeException : public InvalidInputException
{
public:
	/**
	 * @brief Constructor for the InvalidUpgradeException class with a custom message.
	 *
	 * @param message A custom error message describing the invalid upgrade error.
	 */
	InvalidUpgradeException(const std::string& message);
};

/**
 * @brief Inline constructor implementation for the InvalidUpgradeException constructor with a custom message.
 *
 * @param message A custom error message describing the invalid upgrade error.
 */
inline InvalidUpgradeException::InvalidUpgradeException(const std::string& message)
	: InvalidInputException(message)
{
}

/**
 * @brief Exception class for invalid positions on the chessboard.
 *
 * This exception class is derived from the `InvalidInputException` class and is used to represent
 * errors related to invalid positions on the chessboard. It provides constructors for creating
 * exceptions with custom error messages.
 */
class InvalidPositionException : public InvalidInputException
{
public:
	/**
	 * @brief Constructor for the InvalidPositionException class with a custom message.
	 *
	 * @param message A custom error message describing the invalid position error.
	 */
	InvalidPositionException(const std::string& message);
};

/**
 * @brief Inline constructor implementation for the InvalidPositionException constructor with a custom message.
 *
 * @param message A custom error message describing the invalid position error.
 */
inline InvalidPositionException::InvalidPositionException(const std::string& message)
	: InvalidInputException(message)
{
}

/**
 * @brief Exception class for invalid positions on the chessboard grid.
 *
 * This exception class is derived from the `InvalidPositionException` class and is used to represent
 * errors related to invalid positions on the chessboard grid. It provides constructors for creating
 * exceptions with custom error messages.
 */
class InvalidBoardPositionException : public InvalidPositionException
{
public:
	/**
	 * @brief Constructor for the InvalidBoardPositionException class with a custom message.
	 *
	 * @param message A custom error message describing the invalid board position error.
	 */
	InvalidBoardPositionException(const std::string& message);
};

/**
 * @brief Inline constructor implementation for the InvalidBoardPositionException constructor with a custom message.
 *
 * @param message A custom error message describing the invalid board position error.
 */
inline InvalidBoardPositionException::InvalidBoardPositionException(const std::string& message)
	: InvalidPositionException(message)
{
}

/**
 * @brief Exception class for invalid positions in memory-related operations.
 *
 * This exception class is derived from the `InvalidPositionException` class and is used to represent
 * errors related to invalid positions in memory-related operations. It provides constructors for creating
 * exceptions with custom error messages.
 */
class InvalidMemoryPositionException : public InvalidPositionException
{
public:
	/**
	 * @brief Constructor for the InvalidMemoryPositionException class with a custom message.
	 *
	 * @param message A custom error message describing the invalid memory position error.
	 */
	InvalidMemoryPositionException(const std::string& message);
};

/**
 * @brief Inline constructor implementation for the InvalidMemoryPositionException constructor with a custom message.
 *
 * @param message A custom error message describing the invalid memory position error.
 */
inline InvalidMemoryPositionException::InvalidMemoryPositionException(const std::string& message)
	: InvalidPositionException(message)
{
}

// 3.	Invalid state 

/**
 * @brief Exception class for invalid game states.
 *
 * This exception class is derived from the `ChessException` base class and is used to represent
 * errors related to invalid game states in the chess game. It provides constructors for creating
 * exceptions with custom error messages.
 */
class InvalidStateException : public ChessException
{
public:
	/**
	 * @brief Constructor for the InvalidStateException class with a custom message.
	 *
	 * @param message A custom error message describing the invalid state error.
	 */
	InvalidStateException(const std::string& message);

private:
	// No additional members at the moment.
};

/**
 * @brief Inline constructor implementation for the InvalidStateException constructor with a custom message.
 *
 * @param message A custom error message describing the invalid state error.
 */
inline InvalidStateException::InvalidStateException(const std::string& message)
	: ChessException(message)
{
}