#include "TimeKeeper.h"

#include <assert.h> 


TimeKeeper::TimeKeeper(double minFrameTime /*= 0.01*/, double maxFrameTime /*= 0.1*/)
	:
	m_minFrameTime(minFrameTime),
	m_maxFrameTime(maxFrameTime)
{

}

TimeKeeper::~TimeKeeper()
{
}

//-------------------------------------------------------------
// Get (current) time since initialize()

double TimeKeeper::GetCurrentTime() const
{
	assert( m_freq );

	static double dFreq = static_cast<double>( m_freq );

	Uint64 t = SDL_GetPerformanceCounter() - m_init_time;

	return static_cast<double>(t)/dFreq;
}

//-------------------------------------------------------------
// Calculate the average frames per second
void TimeKeeper::CalculateFPS()
{
	// Make static so the variables persist even after
	// the function returns.
	static int		frameCnt = 0;
	static double	timeElapsed = 0.0f;

	// Function called implies a new frame, so increment
	// the frame count.
	++frameCnt;

	// Also increment how much time has passed since the
	// last frame.  
	timeElapsed += GetDeltaTime();

	// Has a second passed?
	if (timeElapsed >= 1.0f)
	{
		// Yes, so compute the frames per second.
		m_FrameRate = frameCnt / timeElapsed;

		// Reset the counters to prepare for the next time
		// we compute the frames per second.
		frameCnt = 0;
		timeElapsed = 0.0f;
	}
}

//-------------------------------------------------------------

bool TimeKeeper::Initialize()
{
	m_freq = SDL_GetPerformanceFrequency();
	
	m_init_time = SDL_GetPerformanceCounter();

	m_current_time = m_previous_time = GetCurrentTime();

	return true;
}

//-------------------------------------------------------------
bool TimeKeeper::Update()
{
	double new_current_time = GetCurrentTime();
	double deltaTime = new_current_time - m_current_time;

	// Only update if we have waited long enough
	if (deltaTime >= m_minFrameTime)
	{
		m_previous_time = m_current_time;
		m_current_time = new_current_time;

		// Prevents the deltatime going beyond a given value
		// Necessary to stop physics errors from long timesteps
		if (deltaTime > m_maxFrameTime)
		{
			m_previous_time = m_current_time - m_maxFrameTime;
			m_MissedFrames++;
		}

		CalculateFPS();

		return true;
	}

	return false;
}

