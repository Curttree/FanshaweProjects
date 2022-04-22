#include "GameEvents.h"

bool GameEvent_KeyPress::CompareData(GameEvent* gameEvent)
{
	GameEvent_KeyPress* keyPress = dynamic_cast<GameEvent_KeyPress*>(gameEvent);
	return keyPress == nullptr ? false : keyPress->GetKey() == m_Key && keyPress->m_Shift == m_Shift;
}
bool GameEvent_KeyRelease::CompareData(GameEvent* gameEvent)
{
	GameEvent_KeyRelease* keyRelease = dynamic_cast<GameEvent_KeyRelease*>(gameEvent);
	return keyRelease == nullptr ? false : keyRelease->GetKey() == m_Key;
}
bool GameEvent_MousePress::CompareData(GameEvent* gameEvent)
{
	GameEvent_MousePress* buttonPress = dynamic_cast<GameEvent_MousePress*>(gameEvent);
	return buttonPress == nullptr ? false : buttonPress->GetButton() == m_Button;
}
bool GameEvent_MouseRelease::CompareData(GameEvent* gameEvent)
{
	GameEvent_MouseRelease* buttonRelease = dynamic_cast<GameEvent_MouseRelease*>(gameEvent);
	return buttonRelease == nullptr ? false : buttonRelease->GetButton() == m_Button;
}
bool GameEvent_AFK::CompareData(GameEvent* gameEvent)
{
	//TODO: Verify if we need an extra piece of info to verify identity.
	GameEvent_AFK* animationExit = dynamic_cast<GameEvent_AFK*>(gameEvent);
	return animationExit == nullptr ? false : true;
}
bool GameEvent_BackInAction::CompareData(GameEvent* gameEvent)
{
	//TODO: Verify if we need an extra piece of info to verify identity.
	GameEvent_BackInAction* animationExit = dynamic_cast<GameEvent_BackInAction*>(gameEvent);
	return animationExit == nullptr ? false : true;
}
bool GameEvent_AnimationExit::CompareData(GameEvent* gameEvent)
{
	GameEvent_AnimationExit* animationExit = dynamic_cast<GameEvent_AnimationExit*>(gameEvent);
	return animationExit == nullptr ? false: animationExit->GetAnim() == animID;
}

