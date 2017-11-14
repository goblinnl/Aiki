#ifndef SINGLETON_H
#define SINGLETON_H

// External
#include <iostream>

template<typename T>
class Singleton {
private:
	Singleton();
	~Singleton();
	Singleton(Singleton const&);
	Singleton& operator=(Singleton &const);
	static Holder<t> mInstance;
		

public:
	template <class H>
	class Holder {
	private:
		H* mPointer;

	protected:
		friend class Singleton<H>;

		bool Valid() { return mPointer; }
		void Create() { mPointer = new H(); }
		void Destroy() { if(Valid()) { delete mPointer; mPointer = nullptr; } }

	public:
		Holder() : mPointer(nullptr) {}
		Holder(const Holder& rCopy) : mPointer(rCopy.mPointer) {}
		~Holder() {}

		H* operator->() { return mPointer; }
		const H* operator->() const{ return mPointer; }
		H& operator = (const Holder& rCopy) { mPointer = rCopy.mPointer; return *this; }
	};

	static void Create() {
		if(!mInstance.Valid()) {
			mInstance.Create();
		}
	}

	static Holder<T> Instance() {
		return mInstance.Valid() ? mInstance : (mInstance.Create(), mInstance);
	}

	static void Destroy() {
		if(mInstance.Valid()) {
			mInstance.Destroy();
		}
	}
};

template<class T>typename Singleton<T>::template Holder<T> Singleton<T>::mInstance = Holder<T>();

#endif // SINGLETON_H
