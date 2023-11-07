#include "ChessUIQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::shared_ptr<ChessUIQt> w = std::make_shared<ChessUIQt>();
    std::shared_ptr<IChessGame> game = IChessGame::CreateGame();
    game->AddListener(w);
    w->SetGame(game.get());
    w->show();
    w->StartGame();

    QObject::connect(w.get(), &ChessUIQt::Exit, &a, &QApplication::quit);
    return a.exec();
}
