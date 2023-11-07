#pragma once

/**
 * @brief Interface for managing timed modes in a chess game.
 *
 * This interface defines methods to enable, pause, resume, and query timed modes within a chess game.
 */
class IChessGameTimedMode
{
public:
    /**
     * @brief Virtual destructor for the interface.
     */
    virtual ~IChessGameTimedMode() = default;

    /**
     * @brief Enables timed mode with the specified time limit.
     *
     * @param seconds The time limit in seconds for each player's turn.
     */
    virtual void EnableTimedMode(int seconds) = 0;

    /**
     * @brief Pauses the timed mode, suspending the countdown.
     */
    virtual void Pause() = 0;

    /**
     * @brief Resumes the timed mode after it has been paused.
     */
    virtual void Resume() = 0;

    /**
     * @brief Sets refresh rate with the specified time rate.
     *
     * @param milliseconds The time rate in milliseconds for clock's refresh rate.
     */
    virtual void SetRefreshRate(int milliseconds) = 0;

    /**
     * @brief Retrieves the remaining time for a player's turn.
     *
     * @param color The color of the player for whom to retrieve the remaining time.
     * @return The remaining time in milliseconds for the specified player's turn.
     */
    virtual int GetRemainingTime(EColor color) const = 0;

    /**
     * @brief Checks if the timed mode is currently paused.
     *
     * @return `true` if the timed mode is paused, otherwise `false`.
     */
    virtual bool IsPaused() const = 0;
};
