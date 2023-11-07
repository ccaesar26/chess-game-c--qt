#pragma once

#include "IPiece.h"
#include "IChessGameListener.h"

#include "IChessGameControl.h"
#include "IChessGameStorage.h"
#include "IChessGameStatus.h"
#include "IChessGameTimedMode.h"

#include <vector>
#include <string>

using IChessGamePtr = std::shared_ptr<class IChessGame>;
using MoveList = std::vector<std::string>;

/**
 * @class IChessGame
 * @brief Interface for a chess game.
 *
 * This interface defines methods to control a chess game, manage its storage, and handle timed modes.
 * It also provides methods for adding and removing listeners for game events.
 */
class IChessGame
    : public IChessGameControl
    , public IChessGameStorage
    , public IChessGameTimedMode
{
public:
    /**
     * @brief Creates a new instance of a chess game.
     * @return A shared pointer to the created game instance.
     */
    static IChessGamePtr CreateGame();

    /**
     * @brief Virtual destructor for the IChessGame interface.
     */
    virtual ~IChessGame() = default;

    /**
     * @brief Adds a listener to receive game events.
     * @param listener The listener to be added.
     */
    virtual void AddListener(IChessGameListenerPtr listener) = 0;

    /**
     * @brief Removes a listener from receiving game events.
     * @param listener The listener to be removed.
     */
    virtual void RemoveListener(IChessGameListener* listener) = 0;

    /**
     * @brief Gets the current status of the chess game.
     * @return A pointer to the current game status.
     */
    virtual const IChessGameStatus* GetStatus() const = 0;
};
