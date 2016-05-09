#include "stdafx.h"
#include "EndSceneManager.h"

#include "hadesmem/detail/trace.hpp"

EndSceneManager::EndSceneManager()
{
}


EndSceneManager::~EndSceneManager()
{
}

void EndSceneManager::EvaluateNextFunction()
{
	HADESMEM_DETAIL_TRACE_A("Enter Evaluate");
	auto& pPending = mPendingFunctions.front();
    pPending->Evaluate();
	mPendingFunctions.pop_front();
	HADESMEM_DETAIL_TRACE_A("Exit Evaluate");
}

bool EndSceneManager::Empty() const
{
	return mPendingFunctions.empty();
}

EndSceneManager& EndSceneManager::Instance()
{
	static EndSceneManager manager;
	return manager;
}
