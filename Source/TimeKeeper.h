#ifndef _SDLGL_TIMEKEEPER_H
#define _SDLGL_TIMEKEEPER_H

#include <sdl.h>

class TimeKeeper
{
public:
	TimeKeeper(double minFrameTime = 0.01, double maxFrameTime = 0.1);
	~TimeKeeper(void);

	bool Initialize();

	bool Update();

	double GetDeltaTime() const { return m_current_time - m_previous_time; }

	double GetCurrentTime() const;

	double GetFrameRate() const { return m_FrameRate; }
	int GetMissedFrames() const { return m_MissedFrames;  }

	void SetMinFrameTime(double minFrameTime) { m_minFrameTime = minFrameTime; }
	void SetMaxFrameTime(double maxFrameTime) { m_maxFrameTime = maxFrameTime; }

private:
	void CalculateFPS();

	Uint64 m_freq = 0;
	Uint64 m_init_time = 0;

	double m_previous_time = 0.0;
	double m_current_time = 0.0;

	double	m_minFrameTime;
	double	m_maxFrameTime;

	int		m_MissedFrames = 0;

	double  m_FrameRate = 0;
};

#endif // _TIMEKEEPER_H
