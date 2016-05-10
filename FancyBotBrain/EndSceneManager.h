#pragma once

#include <deque>
#include <mutex>
#include <ppltasks.h>
#include <type_traits>

template <typename T>
struct TaskFnRet
{
    typedef typename concurrency::details::_UnwrapTaskType<std::result_of_t<T()>>::_Type Type;
};

template <typename T>
struct TaskFromFn
{
    typedef typename concurrency::task<typename TaskFnRet<T>::Type> Type;
};

template <typename T>
struct TceFromFn
{
    typedef typename concurrency::task_completion_event<typename TaskFnRet<T>::Type> Type;
};

class IPendingFunction
{
public:
    virtual ~IPendingFunction() {};
    virtual void Evaluate() = 0;
};

template <typename T>
class Finalizer
{
public:
    template <typename F>
    static void Finalize(
        F fn,
        concurrency::task_completion_event<T> tce)
    {
        tce.set(fn());
    }
};

template <>
class Finalizer<void>
{
public:
    template <typename F>
    static void Finalize(
        F fn,
        concurrency::task_completion_event<void> tce)
    {
        fn();
        tce.set();
    }
};

template <typename T>
class PendingFunction : public IPendingFunction
{
public:
	PendingFunction(T fn)
        : IPendingFunction()
        , mFunction(fn)
        , mTaskEvent()
    {
    }

    virtual ~PendingFunction()
    {
    }

    virtual void Evaluate()
    {
        Finalizer<typename TaskFnRet<T>::Type>::Finalize(mFunction, mTaskEvent);
    }

public:
	T mFunction;
	typename TceFromFn<T>::Type mTaskEvent;
};

class EndSceneManager
{
public:
	static EndSceneManager& Instance();

	~EndSceneManager();

    template <typename T>
    typename TaskFromFn<T>::Type
    Execute(T fn)
    {
    	auto pPending = std::make_unique<PendingFunction<T>>(fn);
        auto event = pPending->mTaskEvent;
        std::unique_lock<std::recursive_mutex> lock(mMutex);
    	mPendingFunctions.emplace_back(std::move(pPending));
        typedef typename TaskFromFn<T>::Type RetTask;
    	return RetTask(event);
    }

	void EvaluateNextFunction();

	bool Empty() const;

private:
	EndSceneManager();

    std::recursive_mutex mMutex;
	std::deque<std::unique_ptr<IPendingFunction>> mPendingFunctions;
};
