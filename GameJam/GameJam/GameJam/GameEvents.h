#pragma once

struct Animation;

enum class GameEventType
{
	NONE,
	KEY_PRESS,
	KEY_RELEASE,
	MOUSE_PRESS,
	MOUSE_RELEASE,
	AFK,
	BACK_IN_ACTION,
	ANIMATION_EXIT
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
		, m_Key(key), m_Shift(false) { }
	GameEvent_KeyPress(unsigned char key, bool shift)
		: GameEvent(GameEventType::KEY_PRESS)
		, m_Key(key), m_Shift(shift) { }
	virtual ~GameEvent_KeyPress() { }

	unsigned char GetKey() const { return m_Key; }
	virtual bool CompareData(GameEvent* gameEvent);
private:

	unsigned char m_Key;
	bool m_Shift;
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

class GameEvent_MousePress : public GameEvent
{
public:
	GameEvent_MousePress(unsigned int button)
		: GameEvent(GameEventType::MOUSE_PRESS)
		, m_Button(button) { }
	virtual ~GameEvent_MousePress() { }

	unsigned char GetButton() const { return m_Button; }
	virtual bool CompareData(GameEvent* gameEvent);
private:

	unsigned int m_Button;
};

class GameEvent_MouseRelease : public GameEvent
{
public:
	GameEvent_MouseRelease(unsigned int button)
		: GameEvent(GameEventType::MOUSE_RELEASE)
		, m_Button(button) { }
	virtual ~GameEvent_MouseRelease() { }

	unsigned char GetButton() const { return m_Button; }
	virtual bool CompareData(GameEvent* gameEvent);
private:

	unsigned int m_Button;
};

class GameEvent_AFK : public GameEvent
{
public:
	GameEvent_AFK()
		: GameEvent(GameEventType::AFK){ }
	virtual ~GameEvent_AFK() { }
	virtual bool CompareData(GameEvent* gameEvent);
};

class GameEvent_BackInAction : public GameEvent
{
public:
	GameEvent_BackInAction()
		: GameEvent(GameEventType::BACK_IN_ACTION) { }
	virtual ~GameEvent_BackInAction() { }
	virtual bool CompareData(GameEvent* gameEvent);
};

class GameEvent_AnimationExit : public GameEvent
{
public:
	GameEvent_AnimationExit(unsigned int _anim)
		: GameEvent(GameEventType::ANIMATION_EXIT),
	animID(_anim) { }
	virtual ~GameEvent_AnimationExit() { }
	unsigned int GetAnim() const {return animID;}
	virtual bool CompareData(GameEvent* gameEvent);
private:
	unsigned int animID;
};


class iGameEventListener
{
public:
	virtual void Notify(GameEventType type, void* data) = 0;
};