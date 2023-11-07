#pragma once

#include "IChessGame.h"
#include "Piece.h"
#include "PGNBuilder.h"
#include "ChessTimer.h"

#include <array>
#include <unordered_map>
#include <string>

using ArrayBoard = std::array<std::array<PiecePtr, 8>, 8>;
using ChessMap = std::unordered_map<std::array<std::array<char, 8>, 8>, int, struct HashFunctor>;
using ChessVector = std::vector<std::array<std::array<char, 8>, 8>>;
using CastleValues = std::array<std::array<bool, 2>, 2>;
using BoardPosition = std::pair<char, char>;

enum class EGameState
{
	MovingPiece,
	Draw,
	WonByWhitePlayer,
	WonByBlackPlayer,
	UpgradePawn,
	CheckState,
	WaitingForDrawResponse
};

enum class ENotification
{
	GameOver,
	Check,
	Reset,
	ClockUpdate,
	TimesUp,
	Pause,
	Resume
};

struct HashFunctor {
	std::size_t operator()(const std::array<std::array<char, 8>, 8>& data) const
	{
		std::size_t seed = 0;
		for (const auto& row : data) 
		{
			for (char element : row) 
			{
				// Combine the hash of each element in the data structure
				seed ^= std::hash<char>{}(element)+0x9e3779b9 + (seed << 6) + (seed >> 2);
			}
		}
		return seed;
	}
};

struct ChessData
{
	ArrayBoard board;
	EColor turn;
	int turnCount;

	PositionList kingPositions;

	IPieceList whitePiecesCaptured;
	IPieceList blackPiecesCaptured;

	EGameState state;

	CastleValues castle;

	ChessMap boardConfigFrequency;
	ChessVector boardConfigurations;

	PGNBuilder PGNFormat;

	std::vector<IChessGameListenerWeakPtr> listeners;
};

class ChessGame 
	: public IChessGame
	, public IChessGameStatus
{
public:

	// --- IChessGame Virtual Implementations						--- //

	void AddListener(IChessGameListenerPtr listener) override;
	void RemoveListener(IChessGameListener* listener) override;

	const IChessGameStatus* GetStatus() const override;

	// --- Status Virtual Implementations							--- //

	IPiecePtr GetIPiecePtr(Position pos) const override;
	PositionList GetPossibleMoves(Position currentPos) const override;
	IPieceList GetCapturedPieces(EColor color) const override;
	EColor GetCurrentPlayer() const override;
	CharBoard GetBoardAtIndex(int index) const override;
	int GetNumberOfMoves() const override;

	bool IsGameOver() const override;
	bool IsDraw() const override;
	bool IsWaitingForDrawResponse() const override;
	bool IsWon(EColor player) const override;
	bool IsWaitingForUpgrade() const override;
	bool IsCheckState() const override;
	bool IsCastlingAvailable(EColor color, ESide side) const override;

	// --- Control Virtual Implementations							--- //

	void ResetGame() override;
	void RestoreGame(const CharBoard& inputConfig, EColor turn = EColor::White, CastleValues castle = { true, true, true, true }) override;

	void MakeMove(Position initialPos, Position finalPos, bool EnableNotification = true, EType upgradeType = EType::Pawn) override;
	void UpgradePawn(EType upgradeType) override;
	void DrawOperation(EDrawOperation op) override;
		
	// --- Storage Virtual Implementations							--- //

	bool LoadFromFile(EFormat format, const std::string& fileName) override;
	bool SaveFormat(EFormat format, const std::string& fileName) const override;

	std::string GetFormat(EFormat format) const override;

	// --- Timed Mode Virtual Implementations						--- //

	void EnableTimedMode(int seconds) override;

	void Pause() override;
	void Resume() override;

	void SetRefreshRate(int milliseconds) override;

	int GetRemainingTime(EColor color) const;

	bool IsPaused() const override;

	// --- Constructors												--- //

	ChessGame();
	ChessGame(const CharBoard& inputConfig, EColor turn = EColor::White, CastleValues castle = { true, true, true, true });

	// ---------------------------------------------------------------- //

	bool CheckCheckMate() const ;
	PiecePtr GetPieceFromBoard(Position pos) const;

private:

	void InitializeChessGame();
	void InitializeChessGame(const CharBoard& inputConfig, EColor turn = EColor::White, CastleValues castle = {true, true, true, true});

	void ResetBoard();

	PiecePtr GetPiece(Position pos, const ArrayBoard& board) const;
	PieceList GetCheckPieces(Position& checkPos) const;
	Position GetMovingDirections(const Position& checkPiecePos) const;
	PositionList GetToBlockPositions(const Position& checkPiecePos) const;
	Position GetPiecePositionWithSameTypeThatCanMoveToFinalPosition(Position initialPos, Position finalPos, EType currentPieceType);
	ChessData GetData() const;
		
	void SetData(const ChessData& data);
	void SetCastleValues(const CastleValues& Castle);

	void MakeMoveFromString(std::string& move);
	void SwitchTurn();
	void UpdateState(EGameState);
	void SaveConfiguration();

	bool CheckStaleMate() const;
	bool CheckThreeFoldRepetition();
	bool CheckPieceCanBeCaptured(const Position& checkPiecePos) const;
	bool CanBeCaptured(const ArrayBoard& board, Position toCapturePos) const;
	bool KingsWayCanBeBlocked(const PositionList& toBlockPositions) const;
	
	void AddCastle(Position kingPosition, PositionList& kingPossibleMoves) const;
	void ConvertMoveToPositions(std::string& move, Position& initialPos, Position& finalPos);
	static BoardPosition ConvertToBoardPosition(Position pos);

	void NotifyMoveMade(Position init, Position fin);
	void NotifyPawnUpgrade(Position pos);
	void NotifyHistoryUpdate(std::string move);
	void Notify(ENotification notif);

	static bool IsInMatrix(Position piecePosition);

private:

	ArrayBoard m_board;
	EColor m_turn;
	int m_turnCount;
	EGameState m_state;
	CastleValues m_castle;    // Row 1 is for White and Row 2 is for Black ! Column 1 is for left castle and Column 2 is for right castle

	PositionList m_kingPositions;

	IPieceList m_whitePiecesCaptured;
	IPieceList m_blackPiecesCaptured;

	ChessMap m_boardConfigFrequency;
	ChessVector m_boardConfigurations;
	std::vector<IChessGameListenerWeakPtr> m_listeners;

	PGNBuilder m_PGNFormat;
	ChessTimer m_timer;
};
