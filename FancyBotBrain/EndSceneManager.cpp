#include "stdafx.h"
#include "EndSceneManager.h"


EndSceneManager::EndSceneManager()
{
}


EndSceneManager::~EndSceneManager()
{
}

concurrency::task<void> EndSceneManager::Execute(std::function<void()> fn)
{
	PendingFunction pending(fn);
	mPendingFunctions.push_back(pending);
	return concurrency::task<void>(pending.mTaskEvent);
}

void EndSceneManager::EvaluateNextFunction()
{
	auto& pending = mPendingFunctions.front();
	pending.mFunction();
	pending.mTaskEvent.set();
	mPendingFunctions.pop_front();
}

bool EndSceneManager::Empty() const
{
	return mPendingFunctions.empty();
}

PendingFunction::PendingFunction(std::function<void()> fn)
	: mFunction(fn)
	, mTaskEvent()
{
}

EndSceneManager& EndSceneManager::Instance()
{
	static EndSceneManager manager;
	return manager;
}