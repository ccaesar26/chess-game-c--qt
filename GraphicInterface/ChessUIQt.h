#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qgridlayout.h>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QTableWidget>
#include <QFileDialog>
#include <QFile>

#include "GridButton.h"
#include "GameModeDialog.h"
#include "IChessGame.h"
#include "IChessGameListener.h"

class ChessUIQt : public QMainWindow, public IChessGameListener
{
    Q_OBJECT

public:
    ChessUIQt(QWidget *parent = nullptr);
    ~ChessUIQt() override;

    void SetGame(IChessGame* game);

    void InitializeMessage(QGridLayout* mainGridLayout);
    void InitializeButtons(QGridLayout* mainGridLayout);
    void InitializeTimers(QGridLayout* mainGridLayout);
    void InitializeHistory(QGridLayout* mainGridLayout);
    void InitializeBoard(QGridLayout* mainGridLayout);
    void InitializeCapturedBoxes(QGridLayout* mainGridLayout);

    void LoadHistory();

    void UpdateHistory(const std::string& move);
    void UpdateBoard();
    void UpdateCaptures();

    void UpdateMessage(const QString& message);
    void AppendThrowMessage(const QString& message);

    void HighlightPossibleMoves(const PositionList& possibleMoves);
    void UnhighlightPossibleMoves(const PositionList& possibleMoves);

    void StartGame();

    QString ShowPromoteOptions();


    void OnMoveMade(Position init, Position fin) override;
    void OnGameOver(EGameResult result) override;
    void OnPawnUpgrade(Position pos) override;
    void OnCheck() override;
    void OnGameRestarted() override;
    void OnHistoryUpdate(std::string move) override;

    void OnClockUpdate() override;
    void OnTimesUp() override;

public slots:
    void OnButtonClicked(const Position& position);

    void OnSaveButtonClicked();
    void OnLoadButtonClicked();
    void OnRestartButtonClicked();
    void OnDrawButtonClicked();
    void OnSaveInClipboardButtonClicked();
    void OnPauseButtonClicked();
    void OnHistoryClicked(QTableWidgetItem* item);
    void OnClockUpdate(const QString& time);
    void OnTimesUp(const EGameResult& result);

signals:
    void Exit();
    void ClockUpdateSignal(QString time);
    void ClockTimesUpSignal(EGameResult result);

private:
    // TODO: fen save/load methods
    QString FENStringFromBoard() const;
    void LoadFENString(QString FENString);
    // TODO: pgn save/load methods
    QString PGNStringFromBoard() const;
    void LoadPGNString(QString& filePath);

    void AddMoveToHistory(const QString& moveText);

private:
    std::array<std::array<GridButton*, 8>, 8> m_grid;
    std::array<std::array<GridButton*, 8>, 4> m_capturedGrid;

    std::optional<Position> m_selectedCell;
    QLabel* m_MessageLabel;
    QTableWidget* m_MovesTable;
    QLabel* m_BlackTimer, *m_WhiteTimer;

    IChessGame* m_game;
};