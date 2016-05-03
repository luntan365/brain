#include "stdafx.h"
#include "EndSceneManager.h"


EndSceneManager::EndSceneManager()
{
}


EndSceneManager::~EndSceneManager()
{
}

void EndSceneManager::EvaluateNextFunction()
{
	auto& pPending = mPendingFunctions.front();
    pPending->Evaluate();
	mPendingFunctions.pop_front();
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
