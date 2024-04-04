#pragma once

#include "../skse64/GameForms.h"

#include <cinttypes>

#ifdef _DEBUG
#define D(x) x
#else
#define D(x) 
#endif

#ifdef _DEBUG
#define _FULLTRACE
#endif

#ifdef _FULLTRACE
#define T(x) x
#else
#define T(x)
#endif

namespace UPCommon
{
	inline bool TESQuestIsRunning(TESQuest* quest);
	const char* GetActorName(Actor* akActor);
	UInt64 GetVMHandleForQuest(TESQuest* quest);
	UInt32 GetFormId(const char* espName, UInt32 formId);
	bool ActorHasEffect(Actor* actor, UInt32 formId);
	bool FormHasKeyword(TESForm* thisForm, UInt32 keywordFormId);
}
