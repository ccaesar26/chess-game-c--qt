#include "ChessGame.h"
#include "Piece.h"
#include "ChessException.h"
#include "PGNReader.h"

#include <cctype>

// ---		Local Static Functions													--- //

static const CharBoard DEFAULT_CHAR_BOARD = {
	'R', 'H', 'B', 'Q', 'K', 'B', 'H', 'R',
	'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
	'r', 'h', 'b', 'q', 'k', 'b', 'h', 'r'
};

static const std::string TABLE_COLUMNS("abcdefgh");
static const std::string TABLE_ROWS("87654321");

static EType GetType(char c)
{
	// white pieces: p r h b q k
	// black pieces: P R H B Q K
	// empty tile: ' '

	static const EType types[] = { EType::Pawn,EType::Rook, EType::Horse, EType::Bishop, EType::Queen, EType::King };

	std::string str = "prhbqk";
	int pos = str.find_first_of(tolower(c));

	return types[pos];
}

static EColor GetColor(char c)
{
	return islower(c) ? EColor::White : EColor::Black;
}

// ------------------------------------------------------------------------------------ //

// --- IChessGame Virtual Implementations											--- //

IChessGamePtr IChessGame::CreateGame()
{
	return std::make_shared<ChessGame>();
}

void ChessGame::AddListener(IChessGameListenerPtr listener)
{
	m_listeners.push_back(listener);

	m_timer.SetNotify(std::bind(&ChessGame::Notify, this, ENotification::ClockUpdate)
		, std::bind(&ChessGame::Notify, this, ENotification::TimesUp));
}

void ChessGame::RemoveListener(IChessGameListener* listener)
{
	/*
	for (auto it = m_listeners.begin(); it != m_listeners.end();)
	{
		if (auto sp = it->lock())
		{
			if (sp.get())
			{
				it = m_listeners.erase(it);
			}
			else
			{
				it++;
			}
		}
		else
		{
			m_listeners.erase(it);
		}
	}
	*/
	auto f = [listener](IChessGameListenerWeakPtr& weak) {
		auto sp = weak.lock();

		return !sp || sp.get() == listener;
	};

	m_listeners.erase(std::remove_if(m_listeners.begin(), m_listeners.end(), f));
}

const IChessGameStatus* ChessGame::GetStatus() const
{
	return this;
}

// ------------------------------------------------------------------------------------ //

// --- Status Virtual Implementations												--- //

IPiecePtr ChessGame::GetIPiecePtr(Position pos) const
{
	if(IsInMatrix(pos))
		return m_board[pos.row][pos.col];

	throw InvalidBoardPositionException("Position out of range");
}

PositionList ChessGame::GetPossibleMoves(Position currentPos) const
{
	PositionList possibleMoves;
	PiecePtr currentPiece = m_board[currentPos.row][currentPos.col];

	if (currentPiece)
	{
		if (currentPiece->GetColor() == m_turn)
		{
			possibleMoves = currentPiece->GetPattern(currentPos,
				std::bind(&ChessGame::GetPiece, this, std::placeholders::_1, m_board));
		}
	}

	for (int i = 0; i < possibleMoves.size(); i++)
	{
		Position pos = possibleMoves[i];  // The pos variable is for iterating the possible moves //

		PiecePtr currentPieceCopy = m_board[currentPos.row][currentPos.col];
		PiecePtr finalPieceCopy = m_board[pos.row][pos.col];

		// Create alternative board  My code //

		ArrayBoard boardAfterMove = m_board;
		Position kingPosition = m_kingPositions[(int)m_turn];

		boardAfterMove[pos.row][pos.col] = boardAfterMove[currentPos.row][currentPos.col];
		boardAfterMove[currentPos.row][currentPos.col].reset();

		if (boardAfterMove[pos.row][pos.col]->GetType() == EType::King)
		{
			kingPosition = pos;
		}

		if (CanBeCaptured(boardAfterMove, kingPosition))
		{
			possibleMoves.erase(std::find(possibleMoves.begin(), possibleMoves.end(), pos));
			i--;
		}
	}

	if (currentPiece && currentPiece->GetType() == EType::King)
	{
		AddCastle(currentPos, possibleMoves);
	}

	return possibleMoves;
}

IPieceList ChessGame::GetCapturedPieces(EColor color) const
{
	return color == EColor::White ? m_whitePiecesCaptured : m_blackPiecesCaptured;
}

EColor ChessGame::GetCurrentPlayer() const
{
	return m_turn;
}

CharBoard ChessGame::GetBoardAtIndex(int index) const
{
	return m_boardConfigurations.at(index);
}


int ChessGame::GetNumberOfMoves() const
{
	return m_boardConfigurations.size();
}

bool ChessGame::IsGameOver() const
{
	if (m_state == EGameState::Draw || m_state == EGameState::WonByWhitePlayer || m_state == EGameState::WonByBlackPlayer)
		return true;
	return false;
}

bool ChessGame::IsDraw() const
{
	return m_state == EGameState::Draw;
}

bool ChessGame::IsWaitingForUpgrade() const
{
	return m_state == EGameState::UpgradePawn;
}

bool ChessGame::IsWon(EColor player) const
{
	switch (player)
	{
	case EColor::White:
		return m_state == EGameState::WonByWhitePlayer;
	case EColor::Black:
		return m_state == EGameState::WonByBlackPlayer;
	}
}

bool ChessGame::IsWaitingForDrawResponse() const
{
	return m_state == EGameState::WaitingForDrawResponse;
}

bool ChessGame::IsCheckState() const
{
	return m_state == EGameState::CheckState;
}

bool ChessGame::IsCastlingAvailable(EColor color, ESide side) const
{
	return m_castle[(int)color][(int)side];
}

// ------------------------------------------------------------------------------------ //

// --- Control Virtual Implementations												--- //

void ChessGame::ResetGame()
{
	ResetBoard();

	m_whitePiecesCaptured.clear();
	m_blackPiecesCaptured.clear();
	m_boardConfigFrequency.clear();

	InitializeChessGame();

	Notify(ENotification::Reset);
}

void ChessGame::RestoreGame(const CharBoard& inputConfig, EColor turn /*= EColor::White*/, CastleValues castle /*= { true, true, true, true }*/)
{
	ResetBoard();

	m_whitePiecesCaptured.clear();
	m_blackPiecesCaptured.clear();
	m_boardConfigFrequency.clear();

	InitializeChessGame(inputConfig, turn, castle);

	Notify(ENotification::Reset);
}

void ChessGame::MakeMove(Position initialPos, Position finalPos, bool EnableNotification /*= true*/, EType upgradeType /*= EType::Pawn*/)
{
	if (!IsInMatrix(initialPos))
	{
		throw OutOfBoundsException("Initial position is not a valid position");
	}
	if (!IsInMatrix(finalPos))
	{
		throw OutOfBoundsException("Final position is not a valid position");
	}

	if (m_board[finalPos.row][finalPos.col] && m_board[finalPos.row][finalPos.col]->GetColor() == m_turn)
	{
		throw OccupiedByOwnPieceException("The final square is occupied by own piece");
	}

	PositionList possibleMoves = GetPossibleMoves(initialPos);
	if (std::find(possibleMoves.begin(), possibleMoves.end(), finalPos) == possibleMoves.end())
	{
		throw NotInPossibleMovesException("Your move is not possible");
	}

	UpdateState(EGameState::MovingPiece);

	// For PGN Begin // 

	std::string move;

	if (m_turn == EColor::White)
		move = std::to_string(m_turnCount + 1) + ". ";

	char pieceLetter = std::toupper(m_board[initialPos.row][initialPos.col]->ToLetter());
	if (pieceLetter == 'H')
	{
		pieceLetter = 'N';
	}

	if (pieceLetter != 'P')
	{
		move += pieceLetter;
	}

	Position pos = GetPiecePositionWithSameTypeThatCanMoveToFinalPosition(initialPos, finalPos,
		m_board[initialPos.row][initialPos.col]->GetType());

	if (pos.col != -1 && pieceLetter != 'P')
	{
		move += TABLE_COLUMNS[initialPos.col];
	}
	if (pos.row != -1 && pieceLetter != 'P')
	{
		move = move + std::to_string(8 - initialPos.row);
	}

	// For PGN End // 

	if (m_board[finalPos.row][finalPos.col])
	{
		if (pieceLetter == 'P')
		{
			move += TABLE_COLUMNS[initialPos.col];
		}
		move += "x";	// For PGN // 
		if (m_turn == EColor::White)
		{
			m_blackPiecesCaptured.push_back(m_board[finalPos.row][finalPos.col]);
		}
		else
		{
			m_whitePiecesCaptured.push_back(m_board[finalPos.row][finalPos.col]);
		}
	}

	m_board[finalPos.row][finalPos.col] = m_board[initialPos.row][initialPos.col];
	m_board[initialPos.row][initialPos.col].reset();

	if (m_board[finalPos.row][finalPos.col]->GetType() == EType::Rook)
	{
		// Make Castle Inaccessible if Rook moved
		m_castle[(int)m_turn][initialPos.col % 2] = false;
	}
	else if (m_board[finalPos.row][finalPos.col]->GetType() == EType::King)
	{
		m_kingPositions[(int)m_turn] = finalPos;
		// Make Castle Inaccessible if King moved
		m_castle[(int)m_turn][0] = false;
		m_castle[(int)m_turn][1] = false;
		if (initialPos.col - finalPos.col == 2)
		{
			move.resize(move.length() - 1);
			move += "0-0-0";  // For PGN // 
			m_board[finalPos.row][finalPos.col + 1] = m_board[finalPos.row][0];
			m_board[finalPos.row][0].reset();
			if (EnableNotification)
				NotifyMoveMade(Position(finalPos.row, 0), Position(finalPos.row, finalPos.col + 1));
		}
		else if (initialPos.col - finalPos.col == -2)
		{
			move.resize(move.length() - 1);
			move += "0-0";	// For PGN // 
			m_board[finalPos.row][finalPos.col - 1] = m_board[finalPos.row][7];
			m_board[finalPos.row][7].reset();
			if (EnableNotification)
				NotifyMoveMade(Position(finalPos.row, 7), Position(finalPos.row, finalPos.col - 1));
		}
	}

	// For PGN  Begin // 

	if (move.length() == 0 || move[move.length() - 1] != '0')
	{
		BoardPosition boardPos = ConvertToBoardPosition(finalPos);
		move += boardPos.second;
		move += boardPos.first;
	}

	// For PGN  End // 

	//StopPlayerTimer(); // Stop the timer for the current player
	SwitchTurn();      // Switch to the next player's turn
	//StartPlayerTimer(); // Start the timer for the next player

	if (EnableNotification)
		NotifyMoveMade(initialPos, finalPos);

	if (m_board[finalPos.row][finalPos.col]->GetType() == EType::Pawn)
	{
		if (m_board[finalPos.row][finalPos.col]->GetColor() == EColor::White && finalPos.row == 0)
		{
			if (EnableNotification)
			{
				UpdateState(EGameState::UpgradePawn);
				NotifyPawnUpgrade(finalPos);
			}
			else
				UpgradePawn(upgradeType);

			// For PGN // 
			pieceLetter = std::toupper(m_board[finalPos.row][finalPos.col]->ToLetter());
			if (pieceLetter == 'H')
			{
				pieceLetter = 'N';
			}

			move += "=";
			move += pieceLetter;
		}
		if (m_board[finalPos.row][finalPos.col]->GetColor() == EColor::Black && finalPos.row == 7)
		{
			if (EnableNotification)
			{
				UpdateState(EGameState::UpgradePawn);
				NotifyPawnUpgrade(finalPos);
			}
			else
				UpgradePawn(upgradeType);

			// For PGN //
			pieceLetter = std::toupper(m_board[finalPos.row][finalPos.col]->ToLetter());
			if (pieceLetter == 'H')
			{
				pieceLetter = 'N';
			}
			move += "=";
			move += pieceLetter;
		}
	}

	SaveConfiguration();

	UpdateState(EGameState::MovingPiece);

	if (CheckThreeFoldRepetition())
	{
		move += " 1/2-1/2";		// For PGN //

		UpdateState(EGameState::Draw);
		Notify(ENotification::GameOver);
	}

	if (CanBeCaptured(m_board, m_kingPositions[(int)m_turn]) == true)
	{
		move += "+";		// For PGN //

		UpdateState(EGameState::CheckState);
		Notify(ENotification::Check);
	}

	if (CheckCheckMate())
	{
		move[move.length() - 1] = '#';	// For PGN //

		UpdateState(m_turn == EColor::White ? EGameState::WonByBlackPlayer : EGameState::WonByWhitePlayer);
		Notify(ENotification::GameOver);
	}
	else if (CheckStaleMate())
	{
		move += "1/2-1/2";	// For PGN // 

		UpdateState(EGameState::Draw);
		Notify(ENotification::GameOver);
	}

	m_PGNFormat.AddMove(move);
	if (m_turn == EColor::Black)
	{
		m_turnCount++;
	}
	NotifyHistoryUpdate(move);
}

void ChessGame::UpgradePawn(EType upgradeType)
{
	for (int i = 0; i < 8; i++)
	{
		if (m_board[0][i] && m_board[0][i]->GetType() == EType::Pawn)
		{
			m_board[0][i] = Piece::Produce(upgradeType, EColor::White);
			return;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		if (m_board[7][i] && m_board[7][i]->GetType() == EType::Pawn)
		{
			m_board[7][i] = Piece::Produce(upgradeType, EColor::Black);
			return;
		}
	}
}

void ChessGame::DrawOperation(EDrawOperation op)
{
	switch (op)

	{
	case EDrawOperation::Request:
		m_state = EGameState::WaitingForDrawResponse;
		break;
	case EDrawOperation::Accept:
		m_state = EGameState::Draw;
		break;
	case EDrawOperation::Decline:
		m_state = EGameState::MovingPiece;
		break;
	default:
		break;
	}
}

// ------------------------------------------------------------------------------------ //

// --- Storage Virtual Implementations												--- //

bool ChessGame::LoadFromFile(EFormat format, const std::string& fileName)
{
	PGNReader reader;

	if (!reader.LoadFromFile(fileName))
		return false;

	ChessData gameData = GetData();
	ResetGame();

	auto moves = reader.GetMoves();
	for (auto& move : moves)
	{
		EType upgradeType = EType::Pawn;

		int evolvePos = move.find('=');
		if (evolvePos != -1)
		{
			upgradeType = Piece::GetTypeFromLetter(move[evolvePos + 1]);
			move.erase(evolvePos, 2);
		}

		Position initialPosition;
		Position finalPosition;

		ConvertMoveToPositions(move, initialPosition, finalPosition);

		try
		{
			MakeMove(initialPosition, finalPosition, false, upgradeType);
		}
		catch (const ChessException& e)
		{
			//ResetGame();
			SetData(gameData);
			return false;
		}
	}
	return true;
}

bool ChessGame::SaveFormat(EFormat format, const std::string& fileName) const
{
	return m_PGNFormat.SaveFormat(fileName);
}

std::string ChessGame::GetFormat(EFormat format) const
{
	return m_PGNFormat.GetPGNFormat();
}

// ------------------------------------------------------------------------------------ //

// --- TImed Mode Virtual Implementations											--- //

void ChessGame::EnableTimedMode(int seconds)
{
	m_timer.SetTime(std::chrono::seconds(seconds));
	if (seconds > 0)
	{
		m_timer.Start();
	}
}

void ChessGame::Pause()
{
	m_timer.Pause();
}

void ChessGame::Resume()
{
	m_timer.Resume();
}

void ChessGame::SetRefreshRate(int milliseconds)
{
	throw std::logic_error("The method or operation is not implemented.");
}

int ChessGame::GetRemainingTime(EColor color) const
{
	return m_timer.GetRemainingTime(color);
}

bool ChessGame::IsPaused() const
{
	return m_timer.IsPaused();
}

// ------------------------------------------------------------------------------------ //

// --- Constructors																	--- //

ChessGame::ChessGame()
{
	InitializeChessGame();
}

ChessGame::ChessGame(const CharBoard& inputConfig, EColor turn, CastleValues castle)
	: m_turn(turn)
	, m_state(EGameState::MovingPiece)
	, m_castle(castle)
{
	InitializeChessGame(inputConfig, turn, castle);
}

// ------------------------------------------------------------------------------------ //

// --- Game Logic & Private Methods Implementations									--- //

void ChessGame::InitializeChessGame()
{
	m_boardConfigurations.clear();
	m_boardConfigFrequency.clear();
	m_whitePiecesCaptured.clear();
	m_blackPiecesCaptured.clear();
	m_turnCount = 0; 

	m_turn = EColor::White;
	m_kingPositions = { Position(7 ,4), Position(0, 4) };
	UpdateState(EGameState::MovingPiece);

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			m_castle[i][j] = true;

	for (int j = 0; j < 8; j++)
	{
		m_board[6][j] = Piece::Produce(EType::Pawn, EColor::White);
		m_board[1][j] = Piece::Produce(EType::Pawn, EColor::Black);
	}

	const std::vector<EType> TYPES = { EType::Rook, EType::Horse, EType::Bishop, EType::Queen, EType::King, EType::Bishop, EType::Horse, EType::Rook };

	for (int i = 0; i < TYPES.size(); i++)
	{
		m_board[0][i] = Piece::Produce(TYPES[i], EColor::Black);
		m_board[7][i] = Piece::Produce(TYPES[i], EColor::White);
	}

	m_boardConfigFrequency[DEFAULT_CHAR_BOARD] = 1;

	m_boardConfigurations.push_back(DEFAULT_CHAR_BOARD);
}

void ChessGame::InitializeChessGame(const CharBoard& inputConfig, EColor turn, CastleValues castle)
{
	m_turn = turn;
	UpdateState(EGameState::MovingPiece);
	m_boardConfigurations.clear();
	m_boardConfigFrequency.clear();

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			m_castle[i][j] = castle[i][j];

	m_kingPositions.resize(2);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (inputConfig[i][j] == ' ')
			{
				continue;
			}

			EType type = GetType(inputConfig[i][j]);
			EColor color = GetColor(inputConfig[i][j]);

			m_board[i][j] = Piece::Produce(type, color);
			if (type == EType::King)
			{
				m_kingPositions[(int)color] = Position(i, j);
			}
		}
	}
	if (CanBeCaptured(m_board, m_kingPositions[(int)turn]))
	{
		UpdateState(EGameState::CheckState);
	}

	m_boardConfigFrequency[inputConfig] = 1;
	m_boardConfigurations.push_back(inputConfig);
}

void ChessGame::ResetBoard()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			m_board[i][j].reset();
		}
	}
}

PiecePtr ChessGame::GetPiece(Position pos, const ArrayBoard& board) const
{
	return board[pos.row][pos.col];
}

PieceList ChessGame::GetCheckPieces(Position& checkPos) const
{
	PieceList checkPieces;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (m_board[i][j] && m_board[i][j]->GetColor() != m_turn)
			{
				PositionList enemyPiecePositions = m_board[i][j]->GetPattern(Position(i, j),
					std::bind(&ChessGame::GetPiece, this, std::placeholders::_1, m_board));

				for (auto pos : enemyPiecePositions)
				{
					if (pos == m_kingPositions[(int)m_turn])
					{
						if (checkPieces.empty())
						{
							checkPieces.push_back(m_board[i][j]);
							checkPos = Position(i, j);
						}
						else
						{
							checkPieces.push_back(m_board[i][j]);
							break;
						}
					}
				}
			}
		}
	}
	return checkPieces;
}

Position ChessGame::GetMovingDirections(const Position& checkPiecePos) const
{
	int movingRow;
	int movingCol;

	Position kingPosition = m_kingPositions[(int)m_turn];

	if (kingPosition.row < checkPiecePos.row)
	{
		movingRow = -1;
	}
	else if (kingPosition.row > checkPiecePos.row)
	{
		movingRow = 1;
	}
	else
	{
		movingRow = 0;
	}

	if (kingPosition.col < checkPiecePos.col)
	{
		movingCol = -1;
	}
	else if (kingPosition.col > checkPiecePos.col)
	{
		movingCol = 1;
	}
	else
	{
		movingCol = 0;
	}

	return Position(movingRow, movingCol);
}

PositionList ChessGame::GetToBlockPositions(const Position& checkPiecePos) const
{
	Position kingPosition = m_kingPositions[(int)m_turn];
	PositionList toBlockPositions;
	Position movingDirections = GetMovingDirections(checkPiecePos);

	Position iterator_pos(checkPiecePos.row + movingDirections.row, checkPiecePos.col + movingDirections.col);

	while (iterator_pos != kingPosition)
	{
		toBlockPositions.push_back(iterator_pos);
		iterator_pos.row += movingDirections.row;
		iterator_pos.col += movingDirections.col;
	}

	return toBlockPositions;
}

Position ChessGame::GetPiecePositionWithSameTypeThatCanMoveToFinalPosition(Position initialPos, Position finalPos, EType currentPieceType)
{
	bool sameRow = false;
	bool sameCol = false;

	PositionList sameTypePos;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (m_board[i][j] && m_board[i][j]->GetColor() == m_turn && m_board[i][j]->GetType() == currentPieceType && Position(i, j) != initialPos)
			{
				PositionList possibleMoves = GetPossibleMoves(Position(i, j));
				if (std::find(possibleMoves.begin(), possibleMoves.end(), finalPos) != possibleMoves.end())
				{
					sameTypePos.push_back(Position(i, j));
				}
			}
		}
	}

	for (auto& pos : sameTypePos)
	{
		if (initialPos.row == pos.row)
			sameRow = true;
		if (initialPos.col == pos.col)
			sameCol = true;
	}

	if (sameTypePos.empty())
		return Position(-1, -1);

	if (sameRow && sameCol)
		return Position(1, 1);

	if (!sameRow && !sameCol)
		return Position(-1, 1);

	if (sameRow && !sameCol)
		return Position(-1, 1);

	if (!sameRow && sameCol)
		return Position(1, -1);
}

ChessData ChessGame::GetData() const
{
	ChessData data;

	data.board = m_board;
	data.turn = m_turn;
	data.turnCount = m_turnCount;


	data.kingPositions = m_kingPositions;

	data.whitePiecesCaptured = m_whitePiecesCaptured;
	data.blackPiecesCaptured = m_blackPiecesCaptured;

	data.state = m_state;

	data.castle = m_castle;

	data.boardConfigFrequency = m_boardConfigFrequency;
	data.boardConfigurations = m_boardConfigurations;

	data.PGNFormat = m_PGNFormat;

	data.listeners = m_listeners;

	return data;
}

PiecePtr ChessGame::GetPieceFromBoard(Position pos) const
{
	return m_board[pos.row][pos.col];
}

void ChessGame::SetData(const ChessData& data)
{
	ResetBoard();

	m_board = data.board;
	m_turn = data.turn;
	m_turnCount = data.turnCount;

	m_kingPositions = data.kingPositions;

	m_whitePiecesCaptured = data.whitePiecesCaptured;
	m_blackPiecesCaptured = data.blackPiecesCaptured;

	m_state = data.state;

	m_castle = data.castle;

	m_boardConfigFrequency = data.boardConfigFrequency;
	m_boardConfigurations = data.boardConfigurations;

	m_PGNFormat = data.PGNFormat;

	m_listeners = data.listeners;
}

void ChessGame::SetCastleValues(const CastleValues& Castle)
{
	m_castle = Castle;
}

void ChessGame::MakeMoveFromString(std::string& move)
{
	EType upgradeType;

	int evolvePos = move.find('=');
	if (evolvePos != -1)
	{
		upgradeType = Piece::GetTypeFromLetter(move[evolvePos + 1]);
		move.erase(evolvePos, 2);
	}

	Position initialPosition;
	Position finalPosition;

	ConvertMoveToPositions(move, initialPosition, finalPosition);

	// Make the actual move //

	if (!IsInMatrix(initialPosition))
	{
		throw OutOfBoundsException("Initial position is not a valid position");
	}
	if (!IsInMatrix(finalPosition))
	{
		throw OutOfBoundsException("Final position is not a valid position");
	}

	if (m_board[finalPosition.row][finalPosition.col] && m_board[finalPosition.row][finalPosition.col]->GetColor() == m_turn)
	{
		throw OccupiedByOwnPieceException("The final square is occupied by own piece");
	}

	PositionList possibleMoves = GetPossibleMoves(initialPosition);
	if (std::find(possibleMoves.begin(), possibleMoves.end(), finalPosition) == possibleMoves.end())
	{
		throw NotInPossibleMovesException("Your move is not possible");
	}

	UpdateState(EGameState::MovingPiece);

	// For PGN Begin // 

	move = "";

	if (m_turn == EColor::White)
		move = std::to_string(m_turnCount + 1) + ". ";

	char pieceLetter = std::toupper(m_board[initialPosition.row][initialPosition.col]->ToLetter());
	if (pieceLetter == 'H')
	{
		pieceLetter = 'N';
	}

	if (pieceLetter != 'P')
	{
		move += pieceLetter;
	}

	Position pos = GetPiecePositionWithSameTypeThatCanMoveToFinalPosition(initialPosition, finalPosition,
		m_board[initialPosition.row][initialPosition.col]->GetType());

	if (pos.col != -1 && pieceLetter != 'P')
	{
		move += TABLE_COLUMNS[initialPosition.col];
	}
	if (pos.row != -1 && pieceLetter != 'P')
	{
		move = move + std::to_string(8 - initialPosition.row);
	}

	// For PGN End // 

	if (m_board[finalPosition.row][finalPosition.col])
	{
		if (pieceLetter == 'P')
		{
			move += TABLE_COLUMNS[initialPosition.col];
		}
		move += "x";	// For PGN // 
		if (m_turn == EColor::White)
		{
			m_blackPiecesCaptured.push_back(m_board[finalPosition.row][finalPosition.col]);
		}
		else
		{
			m_whitePiecesCaptured.push_back(m_board[finalPosition.row][finalPosition.col]);
		}
	}

	m_board[finalPosition.row][finalPosition.col] = m_board[initialPosition.row][initialPosition.col];
	m_board[initialPosition.row][initialPosition.col].reset();

	if (m_board[finalPosition.row][finalPosition.col]->GetType() == EType::Rook)
	{
		// Make Castle Inaccessible if Rook moved
		m_castle[(int)m_turn][initialPosition.col % 2] = false;
	}
	else if (m_board[finalPosition.row][finalPosition.col]->GetType() == EType::King)
	{
		m_kingPositions[(int)m_turn] = finalPosition;
		// Make Castle Inaccessible if King moved
		m_castle[(int)m_turn][0] = false;
		m_castle[(int)m_turn][1] = false;
		if (initialPosition.col - finalPosition.col == 2)
		{
			move.resize(move.length() - 1);
			move += "0-0-0";  // For PGN // 
			m_board[finalPosition.row][finalPosition.col + 1] = m_board[finalPosition.row][0];
			m_board[finalPosition.row][0].reset();
			NotifyMoveMade(Position(finalPosition.row, 0), Position(finalPosition.row, finalPosition.col + 1));
		}
		else if (initialPosition.col - finalPosition.col == -2)
		{
			move.resize(move.length() - 1);
			move += "0-0";	// For PGN // 
			m_board[finalPosition.row][finalPosition.col - 1] = m_board[finalPosition.row][7];
			m_board[finalPosition.row][7].reset();
			//Notify(ENotification::MoveMade, Position(finalPosition.row, 7), Position(finalPosition.row, finalPosition.col - 1));
		}
		//
	}

	// For PGN  Begin // 

	if (move.length() == 0 || move[move.length() - 1] != '0')// If the move is not Castle //
	{
		BoardPosition boardPos = ConvertToBoardPosition(finalPosition);
		move += boardPos.second;
		move += boardPos.first;
	}

	// For PGN  End // 

	SwitchTurn();

	if (m_board[finalPosition.row][finalPosition.col]->GetType() == EType::Pawn)
	{
		if (m_board[finalPosition.row][finalPosition.col]->GetColor() == EColor::White && finalPosition.row == 0)
		{
			UpgradePawn(upgradeType);

			// For PGN // 
			pieceLetter = std::toupper(m_board[finalPosition.row][finalPosition.col]->ToLetter());
			if (pieceLetter == 'H')
			{
				pieceLetter = 'N';
			}
			move += "=";
			move += pieceLetter;
		}
		if (m_board[finalPosition.row][finalPosition.col]->GetColor() == EColor::Black && finalPosition.row == 7)
		{
			UpgradePawn(upgradeType);

			// For PGN //
			pieceLetter = std::toupper(m_board[finalPosition.row][finalPosition.col]->ToLetter());
			if (pieceLetter == 'H')
			{
				pieceLetter = 'N';
			}
			move += "=";
			move += pieceLetter;
		}
	}

	SaveConfiguration();

	if (CheckThreeFoldRepetition())
	{
		move += " 1/2-1/2";		// For PGN //

		UpdateState(EGameState::Draw);
	}

	if (CanBeCaptured(m_board, m_kingPositions[(int)m_turn]) == true)
	{
		move += "+";		// For PGN //

		UpdateState(EGameState::CheckState);
	}

	if (CheckCheckMate())
	{
		// For PGN //
		move[move.length() - 1] = '#';

		UpdateState(m_turn == EColor::White ? EGameState::WonByBlackPlayer : EGameState::WonByWhitePlayer);
	}
	else if (CheckStaleMate())
	{
		move += "1/2-1/2";	// For PGN // 

		UpdateState(EGameState::Draw);
	}

	m_PGNFormat.AddMove(move);
	if (m_turn == EColor::Black)
	{
		m_turnCount++;
	}
	NotifyHistoryUpdate(move);
}

void ChessGame::SwitchTurn()
{
	m_turn = (m_turn == EColor::White) ? EColor::Black : EColor::White;
	m_timer.SwitchTurn();
}

void ChessGame::UpdateState(EGameState state)
{
	switch (state)
	{
	case EGameState::MovingPiece:
		m_state = EGameState::MovingPiece;
		break;
	case EGameState::Draw:
		m_state = EGameState::Draw;
		break;
	case EGameState::WonByWhitePlayer:
		m_state = EGameState::WonByWhitePlayer;
		break;
	case EGameState::WonByBlackPlayer:
		m_state = EGameState::WonByBlackPlayer;
		break;
	case EGameState::UpgradePawn:
		m_state = EGameState::UpgradePawn;
		break;
	case EGameState::CheckState:
		m_state = EGameState::CheckState;
		break;
	case EGameState::WaitingForDrawResponse:
		m_state = EGameState::WaitingForDrawResponse;
		break;
	default:
		break;
	}
}

void ChessGame::SaveConfiguration()
{
	std::array<std::array<char, 8>, 8> currConfig;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (m_board[i][j])
			{
				currConfig[i][j] = m_board[i][j]->ToLetter();
			}
			else
			{
				currConfig[i][j] = ' ';
			}
		}
	}
	m_boardConfigFrequency[currConfig] ++;
	m_boardConfigurations.push_back(currConfig);
}

bool ChessGame::CheckStaleMate() const
{
	if (m_state != EGameState::MovingPiece)
	{
		return false;
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (m_board[i][j] && m_board[i][j]->GetColor() == m_turn)
			{
				PositionList possibleMoves = GetPossibleMoves(Position(i, j));
				if (!possibleMoves.empty())
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool ChessGame::CheckCheckMate() const
{
	if (m_state == EGameState::Draw)
		return false;

	if (m_state != EGameState::CheckState)
	{
		return false;
	}

	Position checkPiecePos;
	PieceList checkPieces = GetCheckPieces(checkPiecePos);

	PositionList kingPossibleMoves = GetPossibleMoves(m_kingPositions[(int)m_turn]);

	if (!kingPossibleMoves.empty())
	{
		return false;
	}
	if (checkPieces.size() > 1)
	{
		//Notify(ENotification::GameOver);
		return true;
	}

	if (CheckPieceCanBeCaptured(checkPiecePos))
	{
		return false;
	}

	if (checkPieces.front()->Is({ EType::King, EType::Pawn, EType::Horse }))
	{
		return true;
	}

	PositionList toBlockPositions = GetToBlockPositions(checkPiecePos);

	if (toBlockPositions.empty())
	{
		//Notify(ENotification::GameOver);
		return true;
	}
	if (KingsWayCanBeBlocked(toBlockPositions))
	{
		return false;
	}

	//Notify(ENotification::GameOver);
	return true;
}

bool ChessGame::CheckThreeFoldRepetition()
{
	for (auto it : m_boardConfigFrequency)
	{
		if (it.second >= 3)
		{
			return true;
		}
	}
	return false;
}

bool ChessGame::CheckPieceCanBeCaptured(const Position& checkPiecePos) const
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (m_board[i][j] && m_board[i][j]->GetColor() == m_turn)
			{
				PositionList ownPiecePositions = GetPossibleMoves(Position(i, j));
				for (auto pos : ownPiecePositions)
				{
					if (pos == checkPiecePos)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool ChessGame::CanBeCaptured(const ArrayBoard& board, Position toCapturePos) const
{
	EColor pieceColor = board[toCapturePos.row][toCapturePos.col]->GetColor();

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] && board[i][j]->GetColor() != pieceColor)
			{
				Position atackingPiecePosition(i, j);
				PositionList enemyPiecePositions = board[i][j]->GetPattern(atackingPiecePosition,
					std::bind(&ChessGame::GetPiece, this, std::placeholders::_1, board));

				for (auto pos : enemyPiecePositions)
				{
					if (pos == toCapturePos)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool ChessGame::KingsWayCanBeBlocked(const PositionList& toBlockPositions) const
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (m_board[i][j] && m_board[i][j]->GetColor() == m_turn)
			{
				PositionList possibleMoves = GetPossibleMoves(Position(i, j));
				for (auto pos : toBlockPositions)
				{
					if (std::find(possibleMoves.begin(), possibleMoves.end(), pos) != possibleMoves.end())
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

void ChessGame::AddCastle(Position kingPosition, PositionList& kingPossibleMoves) const
{
	PiecePtr king = m_board[kingPosition.row][kingPosition.col];

	ArrayBoard boardAfterMove = m_board;

	// Add Left Castle //

	PositionList toCheckPositions = { kingPosition,Position(kingPosition.row,kingPosition.col - 1)
		, Position(kingPosition.row,kingPosition.col - 2) };

	if (m_castle[(int)king->GetColor()][0] == true)
	{
		if (m_board[kingPosition.row][0] && m_board[kingPosition.row][0]->Is(EType::Rook)
			&& m_board[kingPosition.row][0]->GetColor() == m_turn)
		{
			if (!m_board[kingPosition.row][kingPosition.col - 1] && !m_board[kingPosition.row][kingPosition.col - 2] && !m_board[kingPosition.row][kingPosition.col - 3])
			{
				bool leftCastle = true;
				for (auto pos : toCheckPositions)
				{
					if (pos != kingPosition)
					{
						boardAfterMove[pos.row][pos.col] = boardAfterMove[pos.row][pos.col + 1];
						boardAfterMove[pos.row][pos.col + 1].reset();
					}

					if (CanBeCaptured(boardAfterMove, pos))
					{
						leftCastle = false;
						break;
					}
				}

				if (leftCastle)
					kingPossibleMoves.push_back(Position(kingPosition.row, kingPosition.col - 2));
			}
		}
	}

	// Add Right Castle //

	boardAfterMove = m_board;

	toCheckPositions = { kingPosition,Position(kingPosition.row,kingPosition.col + 1)
		, Position(kingPosition.row,kingPosition.col + 2) };

	if (m_castle[(int)king->GetColor()][1] == true)
	{
		if (m_board[kingPosition.row][7] && m_board[kingPosition.row][7]->GetType() == EType::Rook
			&& m_board[kingPosition.row][7]->GetColor() == m_turn)
		{
			if (!m_board[kingPosition.row][kingPosition.col + 1] && !m_board[kingPosition.row][kingPosition.col + 2])
			{
				bool rightCastle = true;
				for (auto pos : toCheckPositions)
				{
					if (pos != kingPosition)
					{
						boardAfterMove[pos.row][pos.col] = boardAfterMove[pos.row][pos.col - 1];
						boardAfterMove[pos.row][pos.col - 1].reset();
					}

					if (CanBeCaptured(boardAfterMove, pos))
					{
						rightCastle = false;
						break;
					}
				}

				if (rightCastle)
					kingPossibleMoves.push_back(Position(kingPosition.row, kingPosition.col + 2));
			}
		}
	}
}

void ChessGame::ConvertMoveToPositions(std::string& move, Position& initialPos, Position& finalPos)
{
	// Verify Castle //

	if (move == "0-0" || move == "0-0-0")
	{
		if (m_turn == EColor::White)
			initialPos.row = finalPos.row = 7;
		else
			initialPos.row = finalPos.row = 0;

		initialPos.col = 4;

		finalPos.col = (move == "0-0") ? 6 : 2;
		return;
	}

	// End of Verify Castle //

	int pos = move.length();

	char finalRow = move[pos - 1];
	char finalCol = move[pos - 2];

	finalPos.row = 8 - (finalRow - '0');
	finalPos.col = TABLE_COLUMNS.find(finalCol);

	char pieceLetter = move[0];
	EType pieceType = Piece::GetTypeFromLetter(pieceLetter);

	initialPos = Position(-1, -1);

	// New Code //

	if (move.length() == 5)
	{
		initialPos.col = TABLE_COLUMNS.find(move[1]);
		initialPos.row = 8 - (move[2] - '0');
		return;
	}
	else if (move.length() == 4)
	{
		if (move[1] >= '0' && move[1] <= '8')   // If is row // 
		{
			initialPos.row = 8 - (move[1] - '0');
		}
		else  // If is col // 
		{
			initialPos.col = TABLE_COLUMNS.find(move[1]);
		}
	}
	else if (move.length() == 3)
	{
		if (move[0] < 'A' || move[0]>'H')
		{
			if (move[0] >= '0' && move[0] <= '8')   // If is row // 
			{
				initialPos.row = 8 - (move[0] - '0');
			}
			else  // If is col // 
			{
				initialPos.col = TABLE_COLUMNS.find(move[0]);
			}
		}
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (m_board[i][j] && m_board[i][j]->GetColor() == m_turn && m_board[i][j]->GetType() == pieceType)
			{
				PositionList possibleMoves = GetPossibleMoves(Position(i, j));
				if (std::find(possibleMoves.begin(), possibleMoves.end(), finalPos) != possibleMoves.end())
				{
					if (initialPos.row == -1 && initialPos.col == -1)
					{
						initialPos = Position(i, j);
						return;
					}
					else if (i == initialPos.row || j == initialPos.col)
					{
						initialPos = Position(i, j);
						return;
					}
				}
			}
		}
	}
}

BoardPosition ChessGame::ConvertToBoardPosition(Position pos)
{
	BoardPosition boardPos;
	boardPos.first = TABLE_ROWS[pos.row];;
	boardPos.second = TABLE_COLUMNS[pos.col];
	return boardPos;
}

void ChessGame::NotifyMoveMade(Position init, Position fin)
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); it++)
	{
		if (auto sp = it->lock())
		{
			sp->OnMoveMade(init, fin);
		}
	}
}

void ChessGame::NotifyPawnUpgrade(Position pos)
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); it++)
	{
		if (auto sp = it->lock())
		{
			sp->OnPawnUpgrade(pos);
		}
	}
}

void ChessGame::NotifyHistoryUpdate(std::string move)
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); it++)
	{
		if (auto sp = it->lock())
		{
			sp->OnHistoryUpdate(move);
		}
	}
}

void ChessGame::Notify(ENotification notif)
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); it++)
	{
		if (auto sp = it->lock())
		{
			switch (notif)
			{
			case ENotification::GameOver:
			{
				auto result = EGameResult::Draw;
				if (IsWon(EColor::White))
					result = EGameResult::WhitePlayerWon;
				if (IsWon(EColor::Black))
					result = EGameResult::BlackPlayerWon;
				sp->OnGameOver(result);
				break;
			}
			case ENotification::Check:
				sp->OnCheck();
				break;
			case ENotification::Reset:
				sp->OnGameRestarted();
				break;
			case ENotification::ClockUpdate:
				{
					sp->OnClockUpdate();
				}
				break;
			case ENotification::TimesUp:
				switch (m_turn)
				{
				case EColor::White:
					m_state = EGameState::WonByBlackPlayer;
					break;
				case EColor::Black:
					m_state = EGameState::WonByWhitePlayer;
					break;
				}
				//m_timer.Stop();
				sp->OnTimesUp();
				break;
			}
		}
	}
}

bool ChessGame::IsInMatrix(Position pos)
{
	return pos.row >= 0 && pos.row < 8 
		&& pos.col >= 0 && pos.col < 8;
}