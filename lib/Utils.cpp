#include "Utils.h"

#include <iostream>

EventContainer::EventContainer() :
	m_timeTracker(),
	m_keys(),
	m_cursorTracker(),
	m_aspectRatio(0.0f)
{}

void EventContainer::reset()
{
	m_cursorTracker.clearPositionChange();
}

TimeTracker::TimeTracker() : 
	m_t(0.0f),
	m_dt(0.0f),
	m_canComputeTimeStep(false)
{}

void TimeTracker::setTime(GLfloat time)
{
	// We can only compute meaningful time step
	// if current time was submitted at least twice.
	if (m_canComputeTimeStep)
		m_dt = time - m_t;
	else
		m_canComputeTimeStep = true;
	m_t = time;
}

CursorTracker::CursorTracker() :
	m_x(0.0f),
	m_y(0.0f),
	m_dx(0.0f),
	m_dy(0.0f),
	m_canComputePositionChange(false)
{}

void CursorTracker::setPosition(GLfloat x, GLfloat y)
{
	// We can only compute meaningful position change
	// if cursor position was submitted at least twice.
	if (m_canComputePositionChange)
	{
		m_dx = x - m_x;
		// Inverted vertical movement
		m_dy = m_y - y;
	}
	else
		m_canComputePositionChange = true;

	m_x = x;
	m_y = y;
}

void CursorTracker::clearPositionChange()
{
	m_dx = 0.0f;
	m_dy = 0.0f;
}

void debugOutput(const std::string& message)
{
	std::cout << message << "\n";
}

StickyButton::StickyButton(bool state) :
	m_state(state),
	m_responsive(true)
{}

void StickyButton::push()
{
	if (m_responsive)
		m_state = !m_state;
	m_responsive = false;
}

void StickyButton::release()
{
	m_responsive = true;
}