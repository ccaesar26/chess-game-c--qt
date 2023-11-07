#include "ChessUIQt.h"
#include "ChessUIQt.h"

#include "IChessGame.h"
#include "ChessException.h"

#include <QInputDialog>
#include <QMessageBox>

#include <QClipboard>
#include <QGuiApplication>

#include <QLabel>
#include <QPalette>

#include <QFileInfo>
#include <QApplication>

#include <QHeaderView>
#include <QTime>

#include <QRegularExpression>
#include <QString>
#include <QStringList>


static EType ToETypeFromQString(const QString& s)
{
    if (s == "Rook")
    {
        return EType::Rook;
    } 
    else if (s == "Bishop")
    {
        return EType::Bishop;
    }
    else if (s == "Queen")
    {
        return EType::Queen;
    }
    else
    {
        return EType::Horse;
    }
}

static PieceType ToPieceTypeFromEType(const EType& t)
{
    switch (t)
    {
    case EType::Rook:
        return PieceType::rook;
    case EType::Horse:
        return PieceType::knight;
    case EType::King:
        return PieceType::king;
    case EType::Queen:
        return PieceType::queen;
    case EType::Bishop:
        return PieceType::bishop;
    case EType::Pawn:
        return PieceType::pawn;
	default:
        return PieceType::none;
    }
}

static std::pair<PieceType, PieceColor> ToPieceFromQChar(const QChar& c)
{
	PieceType type = PieceType::none;
	PieceColor color = PieceColor::none;
	if (!c.isLetter())
	{
		return std::make_pair(type, color);
	}
	if (c.isUpper())
	{
		color = PieceColor::black;
	}
	else
	{
		color = PieceColor::white;
	}
	switch (c.toLower().toLatin1())
	{
	case 'r':
		type = PieceType::rook;
		break;
	case 'h':
		type = PieceType::knight;
		break;
	case 'b':
		type = PieceType::bishop;
		break;
	case 'q':
		type = PieceType::queen;
		break;
	case 'k':
		type = PieceType::king;
		break;
	case 'p':
		type = PieceType::pawn;
		break;
	}
	return std::make_pair(type, color);
}

static QString FormatTime(int totalMilliseconds)
{
	int hours = totalMilliseconds / 3600000;             // 3600000 milliseconds in an hour
	int minutes = (totalMilliseconds % 3600000) / 60000; // 60000 milliseconds in a minute
	int seconds = (totalMilliseconds % 60000) / 1000;    // 1000 milliseconds in a second
	int milliseconds = totalMilliseconds % 1000;         // Remaining milliseconds

	QTime time(hours, minutes, seconds, milliseconds);
	return time.toString("hh:mm:ss.zzz");
}


ChessUIQt::ChessUIQt(QWidget *parent)
    : QMainWindow(parent)
{
    //Widget containing everything
    QWidget* mainWidget = new QWidget();
    QGridLayout* mainGridLayout = new QGridLayout();
    
    InitializeBoard(mainGridLayout);
    InitializeMessage(mainGridLayout);
    InitializeButtons(mainGridLayout);
    InitializeTimers(mainGridLayout);
    InitializeHistory(mainGridLayout);
	InitializeCapturedBoxes(mainGridLayout);

    mainWidget->setLayout(mainGridLayout);
    this->setCentralWidget(mainWidget);

	// Set a custom font for the application
	QFont customFont("Leelawadee UI", 11); // Replace "Arial" and 14 with your desired font family and size
	QApplication::setFont(customFont);

	QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor("#1A1A1A"));
	this->setPalette(palette);

	connect(this, SIGNAL(ClockUpdateSignal(QString)), this, SLOT(OnClockUpdate(QString)));
	connect(this, SIGNAL(ClockTimesUpSignal(EGameResult)) , this, SLOT(OnTimesUp(EGameResult)));
}

ChessUIQt::~ChessUIQt()
{
    //No delete?
    //https://doc.qt.io/qt-6/objecttrees.html
}

void ChessUIQt::SetGame(IChessGame* game)
{
    m_game = game;
}

void ChessUIQt::InitializeMessage(QGridLayout * mainGridLayout)
{
    m_MessageLabel = new QLabel();
    m_MessageLabel->setText("Waiting for white player\n");
    m_MessageLabel->setAlignment(Qt::AlignCenter);
    m_MessageLabel->setStyleSheet(
	"	font-size: 20px;"
	"	font-weight: bold;"
	"   color: #D3D3D3;"
	);

    mainGridLayout->addWidget(m_MessageLabel, 0, 1, 1, 3);
}

void ChessUIQt::InitializeButtons(QGridLayout* mainGridLayout)
{
    QPushButton* saveButton = new QPushButton("Save");
    QPushButton* loadButton = new QPushButton("Load");
    QPushButton* restartButton = new QPushButton("Restart");
    QPushButton* drawButton = new QPushButton("Draw");
    QPushButton* saveClipboardButton = new QPushButton("Save in Clipboard");

    QWidget* buttonContainer = new QWidget();
    QGridLayout* btnGrid = new QGridLayout();

    btnGrid->addWidget(saveButton, 0, 0);
    btnGrid->addWidget(loadButton, 0, 1);
    btnGrid->addWidget(restartButton, 0, 2);
    btnGrid->addWidget(drawButton, 1, 0);
    btnGrid->addWidget(saveClipboardButton, 1, 1, 1, 2);

    connect(saveButton, &QPushButton::pressed, this, &ChessUIQt::OnSaveButtonClicked);
    connect(loadButton, &QPushButton::pressed, this, &ChessUIQt::OnLoadButtonClicked);
    connect(restartButton, &QPushButton::pressed, this, &ChessUIQt::OnRestartButtonClicked);
    connect(drawButton, &QPushButton::pressed, this, &ChessUIQt::OnDrawButtonClicked);
    connect(saveClipboardButton, &QPushButton::pressed, this, &ChessUIQt::OnSaveInClipboardButtonClicked);

	QString buttonStyle = "QPushButton {"
		"    background-color: #7B7B7B;"      // Background color
		"    color: white;"                   // Text color
		"    border: 2px solid #343434;"      // Border style
		"    border-radius: 8px;"             // Border radius (rounded corners)
		"    padding: 4px 8px;"             // Padding
		"}"
		"QPushButton:hover {"
		"    background-color: #555555;"     // Color when hovered
		"}"
		"QPushButton:pressed {"
		"    background-color: #3F3F3F;"     // Color when pressed
		"    color: white;"
		"}";

	saveButton->setStyleSheet(buttonStyle);
	loadButton->setStyleSheet(buttonStyle);
	restartButton->setStyleSheet(buttonStyle);
	drawButton->setStyleSheet(buttonStyle);
	saveClipboardButton->setStyleSheet(buttonStyle);

    buttonContainer->setLayout(btnGrid);
	mainGridLayout->addWidget(buttonContainer, 0, 0, 1, 1);
}

void ChessUIQt::InitializeTimers(QGridLayout* mainGridLayout)
{
    QWidget* timerContainer = new QWidget();
    QGridLayout* timerGrid = new QGridLayout();

    QLabel* blackTimerLbl = new QLabel("Black timer:");
    m_BlackTimer = new QLabel("00:00:00");

    QPushButton* pauseTimerBtn = new QPushButton("Pause | Resume");
    //TODO Create slot and connect button

	QString buttonStyle = "QPushButton {"
		"    background-color: #65451F;"      // Background color
		"    color: white;"                   // Text color
		"    border: 2px solid #765827;"      // Border style
		"    border-radius: 8px;"             // Border radius (rounded corners)
		"    padding: 4px 8px;"             // Padding
		"}"
		"QPushButton:hover {"
		"    background-color: #C8AE7D;"     // Color when hovered
		"	 color: #65451F;"
		"}"
		"QPushButton:pressed {"
		"    background-color: #765827;"     // Color when pressed
		"    color: white;"
		"}";
	pauseTimerBtn->setStyleSheet(buttonStyle);

    QLabel* whiteTimerLbl = new QLabel("White timer:");
	whiteTimerLbl->setAlignment(Qt::AlignRight);
    m_WhiteTimer = new QLabel("00:00:00");
	m_WhiteTimer->setAlignment(Qt::AlignRight);

	QString labelStyle = "QLabel {"
		"	 color: white;"
		"}";
	m_BlackTimer->setStyleSheet(labelStyle);
	m_WhiteTimer->setStyleSheet(labelStyle);
	blackTimerLbl->setStyleSheet(labelStyle);
	whiteTimerLbl->setStyleSheet(labelStyle);

    timerContainer->setFixedWidth(680);

    timerGrid->addWidget(blackTimerLbl, 0, 0);
    timerGrid->addWidget(m_BlackTimer, 0, 1);
    timerGrid->addWidget(pauseTimerBtn, 0, 2);
    timerGrid->addWidget(whiteTimerLbl, 0, 3);
    timerGrid->addWidget(m_WhiteTimer, 0, 4);

    timerContainer->setLayout(timerGrid);
	mainGridLayout->addWidget(timerContainer, 2, 0, 1, 4, Qt::AlignCenter);

	connect(pauseTimerBtn, &QPushButton::pressed, this, &ChessUIQt::OnPauseButtonClicked);
}

void ChessUIQt::InitializeHistory(QGridLayout* mainGridLayout)
{
	m_MovesTable = new QTableWidget(); 
	m_MovesTable->setRowCount(0);    
	m_MovesTable->setColumnCount(2);  
	m_MovesTable->setFixedWidth(240);
	m_MovesTable->setFocusPolicy(Qt::NoFocus);

	m_MovesTable->horizontalHeader()->setVisible(false);
	m_MovesTable->verticalHeader()->setVisible(false);

	m_MovesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

	QString cellStyle = "QTableWidget {"
		"    border: 2px solid #343434;"
		"    border-radius: 8px;"
		"    margin: 8px;"
		"    background-color: #D3D3D3;"
		"    gridline-color: transparent;"
		"}"
		"QTableWidget::item {"
		"    padding: 2px;"
		"    margin: 2px;"
		"}"
		"QTableWidget::item:selected {"
		"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #5A5A5A, stop:0.4 rgba(90, 90, 90, 0.2), stop:0.8 rgba(0, 0, 0, 0));"
		"    color: white;"
		"    border: none;"
		"    border-radius: 4px;"
		"    outline: none;"
		"}";

	m_MovesTable->setStyleSheet(cellStyle);

	connect(m_MovesTable, &QTableWidget::itemActivated, this, &ChessUIQt::OnHistoryClicked);
	mainGridLayout->addWidget(m_MovesTable, 1, 0, 1, 1);
}

// Function to add a move to the history
void ChessUIQt::AddMoveToHistory(const QString& moveText)
{
	int rowCount = m_MovesTable->rowCount();
	QTableWidgetItem* moveItem = new QTableWidgetItem(moveText);

	bool isFirstCol = false;
	for (auto it : moveText)
	{
		if (it == '.')
		{
			isFirstCol = true;
			break;
		}
	}

	if (isFirstCol)
	{
		m_MovesTable->insertRow(rowCount);
		m_MovesTable->setItem(rowCount, 0, moveItem);
	}
	else
	{
		m_MovesTable->setItem(rowCount - 1, 1, moveItem);
	}
}


void ChessUIQt::InitializeBoard(QGridLayout* mainGridLayout)
{
	QGridLayout* chessGridLayout;
	chessGridLayout = new QGridLayout();
	QWidget* board = new QWidget();

	for (int i = 0; i < 8; i++)
	{
		QLabel* rowLabel = new QLabel(QString::number(8 - i) + "  ");
		rowLabel->setAlignment(Qt::AlignCenter);
		chessGridLayout->addWidget(rowLabel, i, 0);

		QFont rowFont = rowLabel->font();
		rowFont.setPointSize(10);
		rowLabel->setFont(rowFont);

		QPalette rowPalette = rowLabel->palette();
		rowPalette.setColor(QPalette::WindowText, QColor("#FFD89C"));
		rowLabel->setPalette(rowPalette);

		for (int j = 0; j < 8; j++)
		{
			m_grid[i][j] = new GridButton({ i,j }, PieceType::none, PieceColor::none);
			chessGridLayout->addWidget(m_grid[i][j], i, j + 1);
			chessGridLayout->setSpacing(2.5);
			connect(m_grid[i][j], &GridButton::Clicked, this, &ChessUIQt::OnButtonClicked);
		}
	}

	for (int j = 0; j < 8; j++)
	{
		QLabel* colLabel = new QLabel(QString(QChar('A' + j)));
		colLabel->setAlignment(Qt::AlignCenter);
		chessGridLayout->addWidget(colLabel, 8, j + 1);

        QFont colFont = colLabel->font();
        colFont.setPointSize(10);
        colLabel->setFont(colFont);

		QPalette colPalette = colLabel->palette();
		colPalette.setColor(QPalette::WindowText, QColor("#FFD89C"));
		colLabel->setPalette(colPalette);
	}

	board->setLayout(chessGridLayout);
	mainGridLayout->addWidget(board, 1, 2, 1, 1);
}

void ChessUIQt::InitializeCapturedBoxes(QGridLayout* mainGridLayout)
{
	QGridLayout* capturedGrid1;
	capturedGrid1 = new QGridLayout();
	QWidget* captured1 = new QWidget();

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			m_capturedGrid[i][j] = new GridButton({ j,i }, PieceType::none, PieceColor::none);
			m_capturedGrid[i][j]->setStyleSheet("background-color: #343434; border: none;");
			capturedGrid1->addWidget(m_capturedGrid[i][j], j, i);
			capturedGrid1->setSpacing(0);
		}
	}

	captured1->setLayout(capturedGrid1);
	mainGridLayout->addWidget(captured1, 1, 1, 1, 1);

	QGridLayout* capturedGrid2;
	capturedGrid2 = new QGridLayout();
	QWidget* captured2 = new QWidget();

	for (int i = 2; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			m_capturedGrid[i][j] = new GridButton({ j,i }, PieceType::none, PieceColor::none);
			m_capturedGrid[i][j]->setStyleSheet("background-color: #343434; border: none;");
			capturedGrid2->addWidget(m_capturedGrid[i][j], j, i);
			capturedGrid2->setSpacing(0);
		}
	}

	captured2->setLayout(capturedGrid2);
	mainGridLayout->addWidget(captured2, 1, 3, 1, 1);
}


void ChessUIQt::LoadHistory()
{
	QStringList moveList = QString::fromStdString(m_game->GetFormat(EFormat::Pgn)).split(" ");

	QString indexString;
	for (const QString& move : moveList) 
	{
		if (!move.trimmed().isEmpty()) 
		{
			if (move.trimmed().contains('.'))
			{
				indexString = move.trimmed() + ' ';
				continue;
			}
			std::string validMoveString = indexString.toStdString() + move.trimmed().toStdString();
			indexString = "";
			UpdateHistory(validMoveString);
		}
	}
}

void ChessUIQt::OnButtonClicked(const Position& position)
{
	//At second click
	if (m_selectedCell.has_value())
	{
		if (m_selectedCell.value() == position)
		{
			m_grid[m_selectedCell.value().row][m_selectedCell.value().col]->setSelected(false);
			m_selectedCell.reset();
			UnhighlightPossibleMoves(m_game->GetStatus()->GetPossibleMoves(position));
		}
		else
		{
			//TODO COMPLETE ME...
			try
			{
				UnhighlightPossibleMoves(m_game->GetStatus()->GetPossibleMoves(m_selectedCell.value()));
				m_game->MakeMove(m_selectedCell.value(), position);
			}
			catch (const OccupiedByOwnPieceException& e)
			{
				UnhighlightPossibleMoves(m_game->GetStatus()->GetPossibleMoves(m_selectedCell.value()));

				m_grid[m_selectedCell.value().row][m_selectedCell.value().col]->setSelected(false);
				m_selectedCell.reset();

				m_selectedCell = position;
				m_grid[position.row][position.col]->setSelected(true);

				AppendThrowMessage("");
				HighlightPossibleMoves(m_game->GetStatus()->GetPossibleMoves(m_selectedCell.value()));

				return;
			}
			catch (const ChessException& e)
			{
				HighlightPossibleMoves(m_game->GetStatus()->GetPossibleMoves(m_selectedCell.value()));
				AppendThrowMessage(e.what());
				return;
			}

			//Unselect prev. pressed button
			m_grid[m_selectedCell.value().row][m_selectedCell.value().col]->setSelected(false);
			m_selectedCell.reset();
		}
	}
	//At first click
	else
	{
		m_selectedCell = position;
		m_grid[position.row][position.col]->setSelected(true);

		//TODO Show possible moves here
		HighlightPossibleMoves(m_game->GetStatus()->GetPossibleMoves(position));
	}
}

void ChessUIQt::OnSaveButtonClicked()
{
	QString fileName = QFileDialog::getSaveFileName(
		this,
		"Save game",
		QDir::homePath(),
		tr("FEN File (*.fen);;PGN File (*.pgn);;All files (*.*)")
	);

	if (!fileName.isEmpty())
	{
		QFile file(fileName);
		QString fileExtension = QFileInfo(fileName).suffix();

		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QMessageBox::critical(this, "Error", "Failed to save the file.");
			return;
		}

		QTextStream out(&file);

		if (fileExtension == "fen")
		{
			out << FENStringFromBoard();
		}
		else if (fileExtension == "pgn")
		{
			out << PGNStringFromBoard();
		}
		else
		{
			QMessageBox::warning(this, "Warning", "Unsupported file format.");
		}

		if (out.status() != QTextStream::Ok)
		{
			QMessageBox::critical(this, "Error", "Failed to write to the file.");
		}

		file.close();
	}
}

void ChessUIQt::OnLoadButtonClicked()
{
	QString fileName = QFileDialog::getOpenFileName(
		this,
		"Load game",
		QDir::homePath(),
		tr("PGN File(*.pgn);;FEN File (*.fen);;All files (*.*)")
	);

	if (!fileName.isEmpty())
	{
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::critical(this, "Error", "Failed to open the file.");
			return;
		}

		QTextStream in(&file);
		QString fileContent = in.readAll();

		file.close();

		QString fileExtension = QFileInfo(fileName).suffix();

		if (fileExtension == "fen")
		{
			LoadFENString(fileContent);
		}
		else if (fileExtension == "pgn")
		{
			LoadPGNString(fileName);
		}
		else
		{
			QMessageBox::warning(this, "Warning", "Unsupported file format.");
		}
	}
}

void ChessUIQt::OnRestartButtonClicked()
{
	QMessageBox::StandardButton reply;
reply = QMessageBox::question(this, "Restart", "All progress will be lost. Are you sure?", QMessageBox::Yes | QMessageBox::No);

if (reply == QMessageBox::Yes)
{
	m_game->ResetGame();
}
}

void ChessUIQt::OnDrawButtonClicked()
{
	//TODO MODIFY ME
	m_game->DrawOperation(EDrawOperation::Request);

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "Draw proposal", "Do you accept a draw?", QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes)
	{
		m_game->DrawOperation(EDrawOperation::Accept);
		m_MessageLabel->setText("Game over!\nDraw.");
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "Game Over", "Draw.\nDo you want to play again?", QMessageBox::Yes | QMessageBox::Close);

		if (reply == QMessageBox::Yes)
		{
			m_game->ResetGame();
		}
		else
		{
			Exit();
		}
	}
	else
	{
		m_game->DrawOperation(EDrawOperation::Decline);
	}
}

void ChessUIQt::OnSaveInClipboardButtonClicked()
{
	QString textToCopy;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			PieceType type = m_grid[i][j]->GetType();
			PieceColor color = m_grid[i][j]->GetColor();

			char piece;

			switch (type) {
			case PieceType::none:
				piece = ' ';
				break;
			case PieceType::king:
				piece = 'k';
				break;
			case PieceType::rook:
				piece = 'r';
				break;
			case PieceType::bishop:
				piece = 'b';
				break;
			case PieceType::queen:
				piece = 'q';
				break;
			case PieceType::knight:
				piece = 'h';
				break;
			case PieceType::pawn:
				piece = 'p';
				break;
			default:
				piece = '*';
				break;
			}

			if (color == PieceColor::black)
				piece = std::toupper(piece);   // include to upper if it doesn t work //

			textToCopy.append("\'");
			textToCopy.append(piece);
			textToCopy.append("\', ");
		}
		textToCopy.append("\n");
	}
	textToCopy.chop(3);

	QClipboard* clipboard = QGuiApplication::clipboard();
	clipboard->setText(textToCopy);
}

void ChessUIQt::OnPauseButtonClicked()
{
	if (m_game->IsPaused())
	{
		m_game->Resume();

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				// Reconnect Clicked signal to m_grid if the last history item is selected
				connect(m_grid[i][j], &GridButton::Clicked, this, &ChessUIQt::OnButtonClicked);
			}
		}
	}
	else
	{
		m_game->Pause();

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				// Disconnect Clicked signal from m_grid if a history item is selected
				disconnect(m_grid[i][j], &GridButton::Clicked, this, &ChessUIQt::OnButtonClicked);
			}
		}
	}
}

void ChessUIQt::OnHistoryClicked(QTableWidgetItem* item)
{
	int selectedRow = m_MovesTable->currentRow();
	int selectedCol = m_MovesTable->currentColumn();

	bool isLastHistoryItem = (m_game->GetStatus()->GetNumberOfMoves() - 1 == 1 + 2 * selectedRow + selectedCol);

	for (int i = 0; i < 8; i++) 
	{
		for (int j = 0; j < 8; j++) 
		{
			if (!isLastHistoryItem)
			{
				// Disconnect Clicked signal from m_grid if a history item is selected
				disconnect(m_grid[i][j], &GridButton::Clicked, this, &ChessUIQt::OnButtonClicked);
			}
			else
			{
				// Reconnect Clicked signal to m_grid if the last history item is selected
				connect(m_grid[i][j], &GridButton::Clicked, this, &ChessUIQt::OnButtonClicked);
				item->setSelected(false);
			}
		}
	}

	CharBoard board = m_game->GetStatus()->GetBoardAtIndex(1 + 2 * selectedRow + selectedCol);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			m_grid[i][j]->setPiece(ToPieceFromQChar(board[i][j]));
			m_grid[i][j]->setSelected(false);
			m_grid[i][j]->setHighlighted(false);
		}
	}
}

QString ChessUIQt::FENStringFromBoard() const
{
	QString config;
	int numEmptySq = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			QChar letter(0);
			switch (m_grid[i][j]->GetType())
			{
			case PieceType::rook:
				letter = 'R';
				break;
			case PieceType::knight:
				letter = 'N';
				break;
			case PieceType::king:
				letter = 'K';
				break;
			case PieceType::queen:
				letter = 'Q';
				break;
			case PieceType::bishop:
				letter = 'B';
				break;
			case PieceType::pawn:
				letter = 'P';
				break;
			case PieceType::none:
				numEmptySq++;
				break;
			}
			if (letter != 0)
			{
				if (numEmptySq)
				{
					config.append(QChar(numEmptySq + '0'));
					numEmptySq = 0;
				}

				if (m_grid[i][j]->GetColor() == PieceColor::black)
				{
					letter = letter.toLower();
				}
				config.append(letter);
			}
		}
		if (numEmptySq)
		{
			config.append(QChar(numEmptySq + '0'));
			numEmptySq = 0;
		}
		config.append('/');
	}
	config[config.size() - 1] = ' ';

	switch (m_game->GetStatus()->GetCurrentPlayer())
	{
	case EColor::White:
		config.append("w ");
		break;
	case EColor::Black:
		config.append("b ");
		break;
	}

	bool castleFlag = false;
	if (m_game->GetStatus()->IsCastlingAvailable(EColor::White, ESide::Kingside))
	{
		config.append('K');
		castleFlag = true;
	}
	if (m_game->GetStatus()->IsCastlingAvailable(EColor::White, ESide::Queenside))
	{
		config.append('Q');
		castleFlag = true;
	}
	if (m_game->GetStatus()->IsCastlingAvailable(EColor::Black, ESide::Kingside))
	{
		config.append('k');
		castleFlag = true;
	}
	if (m_game->GetStatus()->IsCastlingAvailable(EColor::Black, ESide::Queenside))
	{
		config.append('q');
		castleFlag = true;
	}
	if (!castleFlag)
	{
		config.append('-');
	}

	config.append(" - 0 0");

	return config;
}

void ChessUIQt::LoadFENString(QString FENString)
{
	std::array<std::array<char, 8>, 8> config;
	
	int k = 0;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			QChar currP = FENString.at(k);
			
			if (currP.isDigit())
			{
				int numEmptySq = currP.digitValue();
				while (numEmptySq)
				{
					config[i][j] = ' ';
					numEmptySq--;
					j++;
				}
				k++;
				if (j == 8)
				{
					break;
				}
				currP = FENString.at(k);
			}

			char c = currP.cell();
			if (c >= 'a')
			{
				c = toupper(c);
			}
			else
			{
				c = tolower(c);
			}
			if (c == 'n')
			{
				c = 'h';
			}
			else if(c == 'N')
			{
				c = 'H';
			}

			config[i][j] = c;

			k++;
		}
		k++;
	}

	EColor turn = EColor::White;
	if (FENString.at(k) == 'b')
	{
		turn = EColor::Black;
	}

	k += 2;

	CastleValues cstVal = { false, false, false, false };
	while (FENString.at(k) != ' ')
	{
		if (FENString.at(k) == 'K')
		{
			cstVal[0][1] = true;
		}
		else if (FENString.at(k) == '0')
		{
			cstVal[0][1] = true;
		}
		else if (FENString.at(k) == 'k')
		{
			cstVal[1][1] = true;
		}
		else if (FENString.at(k) == 'q')
		{
			cstVal[1][0] = true;
		}
		k++;
	}

	m_game->RestoreGame(config, turn, cstVal);
}

QString ChessUIQt::PGNStringFromBoard() const
{
	QString PGNString;
	std::string PGNFormat = m_game->GetFormat(EFormat::Pgn);

	PGNString = QString::fromStdString(PGNFormat);
	PGNString.append("*");
	return PGNString;
}

void ChessUIQt::LoadPGNString(QString& filePath)
{
	std::string StringFilePath = filePath.toStdString();

	if (!m_game->LoadFromFile(EFormat::Pgn, StringFilePath))
	{
		m_MovesTable->clearContents();
		m_MovesTable->setRowCount(0);
		LoadHistory();
	}

	UpdateBoard();
	UpdateCaptures();

	if (m_game->GetStatus()->IsGameOver())
	{
		return;
	} 

	switch (m_game->GetStatus()->GetCurrentPlayer())
	{
	case EColor::Black:
		UpdateMessage("Waiting for black player");
		break;
	case EColor::White:
		UpdateMessage("Waiting for white player");
		break;
	default:
		break;
	}
	if (m_game->GetStatus()->IsCheckState())
	{
		QString s = m_MessageLabel->text();
		s.remove(s.size() - 1, 1);
		s.append(" - ");
		s.append("Solve check\n");
		m_MessageLabel->setText(s);
	}
}

void ChessUIQt::UpdateHistory(const std::string& move)
{
	AddMoveToHistory(QString::fromStdString(move));
}

void ChessUIQt::UpdateBoard()
{
    for (int i = 0; i < 8; i++) 
    {
        for (int j = 0; j < 8; j++) 
        {
            std::pair<PieceType, PieceColor> newPiece;

            if (m_game->GetStatus()->GetIPiecePtr(Position(i,j)))
            {
				switch (m_game->GetStatus()->GetIPiecePtr(Position(i, j))->GetType())
				{
				case EType::Rook:
					newPiece.first = PieceType::rook;
					break;
				case EType::Horse:
					newPiece.first = PieceType::knight;
					break;
				case EType::Bishop:
					newPiece.first = PieceType::bishop;
					break;
				case EType::Queen:
					newPiece.first = PieceType::queen;
					break;
				case EType::King:
					newPiece.first = PieceType::king;
					break;
				case EType::Pawn:
					newPiece.first = PieceType::pawn;
					break;
				default:
					break;
				}

                switch (m_game->GetStatus()->GetIPiecePtr(Position(i, j))->GetColor())
				{
				case EColor::Black:
					newPiece.second = PieceColor::black;
					break;
				case EColor::White:
					newPiece.second = PieceColor::white;
					break;
				default:
					break;
				}
            }
            else
            {
                newPiece.first = PieceType::none;
				newPiece.second = PieceColor::none;
            }
            
            m_grid[i][j]->setPiece(newPiece);
			m_grid[i][j]->setSelected(false);
			m_grid[i][j]->setHighlighted(false);
        }
    }

}

void ChessUIQt::UpdateCaptures()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			m_capturedGrid[i][j]->setPiece(std::make_pair(PieceType::none, PieceColor::none));
		}
	}

	IPieceList capturedByWhite = m_game->GetStatus()->GetCapturedPieces(EColor::Black);

	auto it = capturedByWhite.begin();

	for (int i = 0; i < 2 && it != capturedByWhite.end(); i++)
	{
		for (int j = 0; j < 8 && it != capturedByWhite.end(); j++)
		{
			std::pair<PieceType, PieceColor> newPiece;

			switch ((*it)->GetType())
			{
			case EType::Rook:
				newPiece.first = PieceType::rook;
				break;
			case EType::Horse:
				newPiece.first = PieceType::knight;
				break;
			case EType::Bishop:
				newPiece.first = PieceType::bishop;
				break;
			case EType::Queen:
				newPiece.first = PieceType::queen;
				break;
			case EType::King:
				newPiece.first = PieceType::king;
				break;
			case EType::Pawn:
				newPiece.first = PieceType::pawn;
				break;
			default:
				break;
			}

			switch ((*it)->GetColor())
			{
			case EColor::Black:
				newPiece.second = PieceColor::black;
				break;
			case EColor::White:
				newPiece.second = PieceColor::white;
				break;
			default:
				break;
			}

			m_capturedGrid[i][j]->setPiece(newPiece);
			m_capturedGrid[i][j]->setSelected(false);
			m_capturedGrid[i][j]->setHighlighted(false);
			m_capturedGrid[i][j]->setStyleSheet("background-color: #343434; border: none;");

			it++;
		}
	}

	IPieceList capturedByBlack = m_game->GetStatus()->GetCapturedPieces(EColor::White);

	it = capturedByBlack.begin();

	for (int i = 2; i < 4 && it != capturedByBlack.end(); i++)
	{
		for (int j = 0; j < 8 && it != capturedByBlack.end(); j++)
		{
			std::pair<PieceType, PieceColor> newPiece;

			switch ((*it)->GetType())
			{
			case EType::Rook:
				newPiece.first = PieceType::rook;
				break;
			case EType::Horse:
				newPiece.first = PieceType::knight;
				break;
			case EType::Bishop:
				newPiece.first = PieceType::bishop;
				break;
			case EType::Queen:
				newPiece.first = PieceType::queen;
				break;
			case EType::King:
				newPiece.first = PieceType::king;
				break;
			case EType::Pawn:
				newPiece.first = PieceType::pawn;
				break;
			default:
				break;
			}

			switch ((*it)->GetColor())
			{
			case EColor::Black:
				newPiece.second = PieceColor::black;
				break;
			case EColor::White:
				newPiece.second = PieceColor::white;
				break;
			default:
				break;
			}

			m_capturedGrid[i][j]->setPiece(newPiece);
			m_capturedGrid[i][j]->setSelected(false);
			m_capturedGrid[i][j]->setHighlighted(false);
			m_capturedGrid[i][j]->setStyleSheet("background-color: #343434; border: none;");

			it++;
		}
	}
}

void ChessUIQt::HighlightPossibleMoves(const PositionList& possibleMoves)
{
    for (const auto& position : possibleMoves) 
    {
        if (m_game->GetStatus()->GetIPiecePtr(position))
        {
            m_grid[position.row][position.col]->setHighlighted(2);
        } 
        else  m_grid[position.row][position.col]->setHighlighted(1);
    }
}

void ChessUIQt::UnhighlightPossibleMoves(const PositionList& possibleMoves)
{
	for (const auto& position : possibleMoves) {
		m_grid[position.row][position.col]->setHighlighted(false);
	}
}

void ChessUIQt::StartGame()
{
	GameModeDialog dialog(this);
	if (dialog.exec() == QDialog::Accepted)
	{
		int gameTimeInSeconds = dialog.getSelectedTimeInSeconds();
		if (gameTimeInSeconds != -1)
		{
			m_game->EnableTimedMode(gameTimeInSeconds);
			m_WhiteTimer->setText(FormatTime(m_game->GetRemainingTime(EColor::White)));
			m_BlackTimer->setText(FormatTime(m_game->GetRemainingTime(EColor::Black)));
		}
	}
	UpdateBoard();
}

QString ChessUIQt::ShowPromoteOptions()
{
	QInputDialog dialog;
	QList<QString> options;
	options.append("Rook");
	options.append("Bishop");
	options.append("Queen");
	options.append("Knight");

	dialog.setComboBoxItems(options);
	dialog.setModal(true);
	dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowCloseButtonHint); // Disable close button (X)

	bool ok;
	QString item = QInputDialog::getItem(this, tr("Pawn promote"),
		tr("Promote pawn to: "), options, 0, false, &ok);

	if (ok && !item.isEmpty())
	{
		// TODO: Perform the desired action here.
		return item;
	}

	// Prevent the dialog from being closed by "Cancel" button.
	while (true)
	{
		QString item = QInputDialog::getItem(this, tr("Pawn promote"),
			tr("Promote pawn to: "), options, 0, false, &ok);

		if (ok && !item.isEmpty())
		{
			// TODO: Perform the desired action here.
			return item;
		}
	}

	return QString();
}

void ChessUIQt::UpdateMessage(const QString& message)
{
    m_MessageLabel->setText(message + '\n');
}

void ChessUIQt::AppendThrowMessage(const QString& message)
{
	QString s = m_MessageLabel->text();
	for (auto it = s.begin(); it != s.end(); it++)
	{
		if (*it == '\n')
		{
			s.erase(it, s.end());
			break;
		}
	}
	s.append('\n');
	s.append(message);
	m_MessageLabel->setText(s);
}

void ChessUIQt::OnMoveMade(Position init, Position fin)
{
    std::pair<PieceType, PieceColor> initPosPiece;
    initPosPiece.first = m_grid[init.row][init.col]->GetType();
    initPosPiece.second = m_grid[init.row][init.col]->GetColor();

	m_grid[fin.row][fin.col]->setPiece(initPosPiece);
    //m_grid[fin.row][fin.col]->setSelected(false);
    //m_grid[fin.row][fin.col]->setHighlighted(false);

    std::pair<PieceType, PieceColor> voidPiece;
    voidPiece.first = PieceType::none;
    voidPiece.second = PieceColor::none;

    m_grid[init.row][init.col]->setPiece(voidPiece);

	//UpdateBoard();

	switch (m_game->GetStatus()->GetCurrentPlayer())
	{
	case EColor::Black:
		UpdateMessage("Waiting for black player");
		break;
	case EColor::White:
		UpdateMessage("Waiting for white player");
		break;
	default:
		break;
	}

	UpdateCaptures();
}

void ChessUIQt::OnGameOver(EGameResult result)
{
	if (result == EGameResult::Draw)
	{
		m_MessageLabel->setText("Game over!\nDraw.");
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "Game Over", "Draw.\nDo you want to play again?", QMessageBox::Yes | QMessageBox::Close);

		if (reply == QMessageBox::Yes)
		{
            m_game->ResetGame();
		}
		return;
	}
	
    //m_MessageLabel->setText("Game over!\nBlack player won");
	QMessageBox::StandardButton reply;

	if (result == EGameResult::BlackPlayerWon)
	{
        m_MessageLabel->setText("Game over!\nBlack player won");
		reply = QMessageBox::question(this, "Game Over", "Black player won.\nDo you want to play again?", QMessageBox::Yes | QMessageBox::Close);
	}
	else
	{
        m_MessageLabel->setText("Game over!\nWhite player won");
		reply = QMessageBox::question(this, "Game Over", "White player won.\nDo you want to play again?", QMessageBox::Yes | QMessageBox::Close);
	}

	if (reply == QMessageBox::Yes)
	{
		m_game->ResetGame();
	}
}

void ChessUIQt::OnPawnUpgrade(Position pos)
{
    QString item = ShowPromoteOptions();

    EType upgradeType = ToETypeFromQString(item);

    m_game->UpgradePawn(upgradeType);

	std::pair<PieceType, PieceColor> upgradePiece;
    if (pos.row == 0)
    {
        upgradePiece.second = PieceColor::white;
    }
    else
    {
        upgradePiece.second = PieceColor::black;
    }
    upgradePiece.first = ToPieceTypeFromEType(upgradeType);

	m_grid[pos.row][pos.col]->setPiece(upgradePiece);

	/*switch (m_game->GetStatus()->GetCurrentPlayer())
	{
	case EColor::Black:
		UpdateMessage("Waiting for black player");
		break;
	case EColor::White:
		UpdateMessage("Waiting for white player");
		break;
	default:
		break;
	}*/
}

void ChessUIQt::OnCheck()
{
	QString s = m_MessageLabel->text();
    s.remove(s.size() - 1, 1);
    s.append(" - ");
	s.append("Solve check\n");
	m_MessageLabel->setText(s);
    //AppendThrowMessage("Solve check state");
    //UpdateBoard();
}

void ChessUIQt::OnGameRestarted()
{
    //Widget containing everything
    QWidget* mainWidget = new QWidget();
    QGridLayout* mainGridLayout = new QGridLayout();

    InitializeBoard(mainGridLayout);
    InitializeMessage(mainGridLayout);
    InitializeButtons(mainGridLayout);
    InitializeTimers(mainGridLayout);
    InitializeHistory(mainGridLayout);
	InitializeCapturedBoxes(mainGridLayout);

    mainWidget->setLayout(mainGridLayout);
    this->setCentralWidget(mainWidget);

	StartGame();

	switch (m_game->GetStatus()->GetCurrentPlayer())
	{
	case EColor::White:
		m_MessageLabel->setText("Waiting for white player\n");
		break;
	case EColor::Black:
		m_MessageLabel->setText("Waiting for black player\n");
		break;
	}
}

void ChessUIQt::OnHistoryUpdate(std::string move)
{
	UpdateHistory(move);
}

void ChessUIQt::OnClockUpdate(const QString& time)
{
	switch (m_game->GetStatus()->GetCurrentPlayer())
	{
	case EColor::White:
		m_WhiteTimer->setText(time);
		break;
	case EColor::Black:
		m_BlackTimer->setText(time);
		break;
	}
}

void ChessUIQt::OnClockUpdate()
{
	if (!m_game)
	{
		return;
	}
	QString timeToDisplay;
	switch (m_game->GetStatus()->GetCurrentPlayer())
	{
	case EColor::White:
		timeToDisplay = FormatTime(m_game->GetRemainingTime(EColor::White));
		break;
	case EColor::Black:
		timeToDisplay = FormatTime(m_game->GetRemainingTime(EColor::Black));
		break;
	}

	// Emit the ClockUpdateSignal with the formatted time
	emit ClockUpdateSignal(timeToDisplay);
}

void ChessUIQt::OnTimesUp(const EGameResult& result)
{
	OnGameOver(result);
}

void ChessUIQt::OnTimesUp()
{
	EGameResult result;
	if (m_game->GetStatus()->IsWon(EColor::Black))
	{
		result = EGameResult::BlackPlayerWon;
	}
	else
	{
		result = EGameResult::WhitePlayerWon;
	}

	emit ClockTimesUpSignal(result);
}

