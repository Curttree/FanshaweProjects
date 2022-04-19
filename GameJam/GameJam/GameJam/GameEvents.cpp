#include "GameEvents.h"

bool GameEvent_KeyPress::CompareData(GameEvent* gameEvent)
{
	GameEvent_KeyPress* keyPress = dynamic_cast<GameEvent_KeyPress*>(gameEvent);
	return keyPress == nullptr ? false : keyPress->GetKey() == m_Key;
}
bool GameEvent_KeyRelease::CompareData(GameEvent* gameEvent)
{
	GameEvent_KeyRelease* keyPress = dynamic_cast<GameEvent_KeyRelease*>(gameEvent);
	return keyPress == nullptr ? false : keyPress->GetKey() == m_Key;
}
