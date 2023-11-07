#include "ChessTimer.h"

ChessTimer::ChessTimer() : m_paused(false)
, m_isTimerRunning(false)
, m_turn(EColor::White)
, m_refreshRate(std::chrono::milliseconds(1))
{

}

ChessTimer::~ChessTimer()
{
	Stop();
}

void ChessTimer::Start()
{
	Stop();
	m_timerThread = std::thread(&ChessTimer::StartTimer, this);
}

void ChessTimer::Stop()
{
	m_isTimerRunning = false;
	m_timerCV.notify_all();
	if (m_timerThread.joinable())
	{
		m_timerThread.join();
	}
}

void ChessTimer::SetTime(std::chrono::seconds time)
{
	m_whiteRemainingTime.store(time);
	m_blackRemainingTime.store(time);
}

void ChessTimer::SetRefreshRate(std::chrono::milliseconds rate)
{
	m_refreshRate = rate;
}

void ChessTimer::SetNotify(std::function<void()> notifyUpdate, std::function<void()> notifyTimesUp)
{
	NotifyUpdateTimer = notifyUpdate;
	NotifyTimesUp = notifyTimesUp;
}

int ChessTimer::GetRemainingTime(EColor color) const
{
	switch (m_turn)
	{
	case EColor::White:
		return m_whiteRemainingTime.load().count();
		break;
	case EColor::Black:
		return m_blackRemainingTime.load().count();
		break;
	}
}

void ChessTimer::SwitchTurn()
{
	m_turn.store(m_turn == EColor::White ? EColor::Black : EColor::White);
}

void ChessTimer::Pause()
{
	if (m_isTimerRunning && !m_paused)
	{
		m_paused = true;
		m_timerCV.notify_all();
	}
}

void ChessTimer::Resume()
{
	if (m_isTimerRunning && m_paused)
	{
		m_paused = false;
		m_timerCV.notify_all();
	}
}

bool ChessTimer::IsPaused() const
{
	return m_paused;
}

void ChessTimer::StartTimer()
{
	m_isTimerRunning = true;
	while (m_isTimerRunning)
	{
		NotifyUpdateTimer();

		auto initialTime = std::chrono::steady_clock::now();

		std::unique_lock<std::mutex> lock(m_timerMutex);
		m_timerCV.wait_for(lock, std::chrono::milliseconds(m_refreshRate), [&] { return !m_isTimerRunning || m_paused; });

		auto finalTime = std::chrono::steady_clock::now();

		if (m_paused)
		{
			m_timerCV.wait(lock, [&] { return !m_paused; });
		}

		auto& remainingTime = m_turn == EColor::White ? m_whiteRemainingTime : m_blackRemainingTime;

		
		remainingTime.store(remainingTime.load() - std::chrono::duration_cast<std::chrono::milliseconds>(finalTime - initialTime));
		if (remainingTime.load().count() <= 0)
		{
			m_isTimerRunning = false;
			NotifyUpdateTimer();
			NotifyTimesUp();
		}
	}
}
