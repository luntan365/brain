#pragma once

#include <deque>
#include <ppltasks.h>

struct PendingFunction
{
	PendingFunction(std::function<void ()> fn);

	std::function<void()> mFunction;
	concurrency::task_completion_event<void> mTaskEvent;
};

class EndSceneManager
{
public:
	static EndSceneManager& Instance();

	~EndSceneManager();

	concurrency::task<void> Execute(std::function<void ()> fn);

	void EvaluateNextFunction();

	bool Empty() const;

private:
	EndSceneManager();

	std::deque<PendingFunction> mPendingFunctions;
};