#pragma once

enum class GameEventType
{
	NONE,
	KEY_PRESS,
	KEY_RELEASE
};

class GameEvent
{
public:
	virtual ~GameEvent() { }

	GameEventType GetType() const { return m_EventType; }
	virtual bool CompareData(GameEvent* gameEvent) = 0;

protected:
	GameEvent(GameEventType type)
		: m_EventType(type) { }

private:
	GameEvent() : m_EventType(GameEventType::NONE) { }
	GameEventType m_EventType;
};

class GameEvent_KeyPress : public GameEvent
{
public:
	GameEvent_KeyPress(unsigned char key)
		: GameEvent(GameEventType::KEY_PRESS)
		, m_Key(key) { }
	virtual ~GameEvent_KeyPress() { }

	unsigned char GetKey() const { return m_Key; }
	virtual bool CompareData(GameEvent* gameEvent);
private:

	unsigned char m_Key;
};

class GameEvent_KeyRelease : public GameEvent
{
public:
	GameEvent_KeyRelease(unsigned char key)
		: GameEvent(GameEventType::KEY_RELEASE)
		, m_Key(key) { }
	virtual ~GameEvent_KeyRelease() { }

	unsigned char GetKey() const { return m_Key; }
	virtual bool CompareData(GameEvent* gameEvent);
private:

	unsigned char m_Key;
};


class iGameEventListener
{
public:
	virtual void Notify(GameEventType type, void* data) = 0;
};