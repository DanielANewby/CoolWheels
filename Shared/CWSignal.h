#ifndef SIGNAL_H
#define SIGNAL_H
#pragma once

#include <memory>
#include <utility>
#include <vector>

template <class ...Args>
class Signal
{
public:
	Signal& operator=(const Signal&) = delete;
	Signal& operator=(Signal&&) = delete;

	template <class T, typename Fun>
	void AddListener(T* obj, Fun fun)
	{
		if (nullptr != fun)
			listeners.push_back(makeListener(obj, fun));
	}

	template <class T> void RemoveListener(T* obj)
	{
		auto pred = [=](std::unique_ptr<Listener>& p) {
			return static_cast<Holder<T>*>(p.get())->obj == obj;
		};

		auto iter = std::remove_if(listeners.begin(), listeners.end(), pred);
		listeners.erase(iter, listeners.end());
	}

	template <class ...Params>
	void operator()(Params... params) {
		for (auto& listener : listeners)
		{
			(*listener)(std::forward<Params>(params)...);
		}
	}

private:
	struct Listener
	{
	public:
		virtual ~Listener() = default;
		virtual void operator()(Args&&... args) = 0;
	};

	template <class T> struct Holder : public Listener
	{
		Holder(T* p) :obj(p) {}
		T* obj;
	};

	template <class T, typename Fun>
	struct ListenerImpl : public Holder<T>
	{
		ListenerImpl(T* p, Fun f) :Holder<T>(p), fun(f) {}
		~ListenerImpl() = default;

		void operator()(Args&&... args)
		{
			(Holder<T>::obj->*fun)(std::forward<Args>(args)...);
		}

	private:
		Fun fun;
	};

	template <class T, typename Fun>
	std::unique_ptr<Listener> makeListener(T* obj, Fun fun)
	{
		return std::make_unique<ListenerImpl<T, Fun>>(obj, fun);
	}

	std::vector<std::unique_ptr<Listener>> listeners;
};

struct SignalReceiverImplBase
{
	virtual ~SignalReceiverImplBase() = default;
	virtual void Attach() = 0;
	virtual void Detach() = 0;
};

template <class Receiver, typename ...Args>
struct SignalReceiverImpl : public SignalReceiverImplBase
{
	SignalReceiverImpl(Receiver* pRecv, Signal<Args...>& rEvent, void (Receiver::* fun)(Args...)) :
		_recv(pRecv),
		_event(rEvent),
		_fun(fun)
	{}

	void Attach() { _event.AddListener(_recv, _fun); }
	void Detach() { _event.RemoveListener(_recv); }

	Receiver* _recv;
	Signal<Args...>& _event;
	void (Receiver::* _fun)(Args...);
};

struct SignalReceiver
{
	template <class Receiver, typename ...Args>
	SignalReceiver(Signal<Args...>& rEvent, Receiver* pRecv, void (Receiver::* fun)(Args...))
	{
		static_assert(
			sizeof(SignalReceiverImpl<Receiver, Args...>) <= ListenerSize,
			"Static buffer size too small for EventListenerImpl"
			);
		new (pImpl) SignalReceiverImpl<Receiver, Args...>(pRecv, rEvent, fun);
		reinterpret_cast<SignalReceiverImplBase*>(pImpl)->Attach();
	}

	~SignalReceiver() { reinterpret_cast<SignalReceiverImplBase*>(pImpl)->Detach(); }

private:
	static constexpr size_t ListenerSize{ 32 };
	char pImpl[ListenerSize]{ 0 };
};

#endif // SIGNAL_H
