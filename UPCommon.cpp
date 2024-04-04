#include "../skse64/PapyrusVM.h"
#include "../skse64/GameRTTI.h"
#include "../skse64/GameData.h"

#include "UPCommon.h"

namespace UPCommon
{
	// 0x009B49C0
	// unk148 is 0x248 see skse64 TESQuest, left over from skse probs
	inline bool TESQuestIsRunning(TESQuest* quest)
	{
		return ((quest->unk0D8.flags & 1) != 0 && ((quest->unk0D8.flags >> 7) & 1) == 0 && quest->unk148 == 0);
	}

	UInt64 GetVMHandleForQuest(TESQuest* quest)
	{
		T(_DMESSAGE("[TRACE] GetVMHandleForQuest(TESQuest ptr 0x%016" PRIXPTR ")", quest));

		VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();

		UInt64 handle = 0;

		if (registry && quest && TESQuestIsRunning(quest))
		{
			T(_DMESSAGE("[TRACE] Quest exists and is running"));

			BGSBaseAlias* baseAlias = nullptr;
			if (quest->aliases.GetNthItem(0, baseAlias))
			{
				const auto refAlias = DYNAMIC_CAST(baseAlias, BGSBaseAlias, BGSRefAlias);
				if (refAlias)
				{
					IObjectHandlePolicy* policy = registry->GetHandlePolicy();
					handle = policy->Create(refAlias->kTypeID, refAlias);

					T(_DMESSAGE("[TRACE] Quest handle is %d", handle));
				}
			}
		}

		return handle;
	}

	const char* GetActorName(Actor* akActor)
	{
		//T(_DMESSAGE("GetActorName(Actor ptr 0x%016" PRIXPTR ")", akActor));

		static const char unkName[] = "unknown";
		const char* result = unkName;

		if (akActor && akActor->formType == kFormType_Character)
		{
			//T(_DMESSAGE("Actor exists and is a character"));

			const auto actorBase = dynamic_cast<TESActorBase*>(akActor->baseForm);
			if (actorBase)
			{
				auto pFullName = dynamic_cast<TESFullName*>(actorBase);
				if (pFullName)
					result = pFullName->GetName();
			}
		}

		return result;
	}

	UInt32 GetFormId(const char* espName, UInt32 localFormId)
	{
		T(_DMESSAGE("[TRACE] GetFormID(%s, %d)", espName, localFormId));
		UInt32 formID = 0;

		if (formID == 0)
		{
			const UInt32 idx = DataHandler::GetSingleton()->LookupModByName(espName)->GetPartialIndex();
			if (idx != 0xFF)
			{
				formID = (idx << 24) | localFormId;
				T(_DMESSAGE("[TRACE] FormID %d found", formID));
			}
		}

		return formID;
	}

	bool ActorHasEffect(Actor* actor, UInt32 formId)
	{
		T(_DMESSAGE("[TRACE] ActorHasEffect(0x%016" PRIXPTR ", %d)", actor, formId));

		const auto list = actor->magicTarget.GetActiveEffects();

		bool bHasEffect = false;

		for (auto it = list->Begin(); !it.End(); ++it)
		{
			if (it.Get())
				if (it->effect)
					if (it->effect->mgef)
						if (it->effect->mgef->formID == formId)
						{
							T(_DMESSAGE("[TRACE] Effect found"));
							bHasEffect = true;
							break;
						}
		}

		return bHasEffect;
	}

	bool FormHasKeyword(TESForm* thisForm, UInt32 keywordFormId)
	{
		T(_DMESSAGE("[TRACE] FormHasKeyword(0x%016" PRIXPTR ", %d)", thisForm, keywordFormId));

		BGSKeyword* keyword = nullptr;

		if (keywordFormId)
			keyword = dynamic_cast<BGSKeyword*>(LookupFormByID(keywordFormId));

		auto keywords = DYNAMIC_CAST(thisForm, TESForm, BGSKeywordForm);

		if (keyword && keywords)
			return keywords->HasKeyword(keyword);

		return false;
	}
}
