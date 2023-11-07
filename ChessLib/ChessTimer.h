#pragma once

#include "IPiece.h"

#include <chrono>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

class ChessTimer
{
public:
	ChessTimer();
	~ChessTimer();

	void Start();
	void Stop();

	void SetTime(std::chrono::seconds time);
	void SetRefreshRate(std::chrono::milliseconds rate);
	void SetNotify(std::function<void()> notifyUpdate, std::function<void()> notifyTimesUp);

	int GetRemainingTime(EColor color) const;

	void SwitchTurn();

	void Pause();
	void Resume();

	bool IsPaused() const;

private:

	void StartTimer();

	std::thread m_timerThread;
	std::mutex m_timerMutex;
	std::condition_variable m_timerCV;

	std::atomic_bool m_isTimerRunning;
	std::atomic_bool m_paused;

	std::atomic<std::chrono::milliseconds> m_whiteRemainingTime;
	std::atomic<std::chrono::milliseconds> m_blackRemainingTime;
	std::atomic<std::chrono::milliseconds> m_refreshRate;

	std::atomic<EColor> m_turn;
	std::function<void()> NotifyUpdateTimer;
	std::function<void()> NotifyTimesUp;
};