#include <Windows.h>	
#include <process.h>	// Actual thread calls

class cAnimal;

struct sAnimalThreadDesc
{
	cAnimal* pAnimal;
	HANDLE hWorkerThread;
	DWORD workerThreadID;
	// Pointer to the common bool for all the threads
	bool* pRunThreads;
	bool* pKillThreads;
	// Set to false right before the thread exits
	bool bIsAlive;
};