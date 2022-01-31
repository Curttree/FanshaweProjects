#include "cCommand_PlaySound.h"
#include "globals.h"

cCommand_PlaySound::cCommand_PlaySound(int _index)  {
	index = _index;
}

void cCommand_PlaySound::Execute() {
	if (::g_pGameEngine->isMuted) {
		return;
	}
	::g_pGameEngine->audioManager.PlayAudio(index);
}