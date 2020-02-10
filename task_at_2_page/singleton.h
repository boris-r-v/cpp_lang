#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class SingletonBase
{

    protected:
	SingletonBase()=default;
	~SingletonBase()=default;
	SingletonBase( SingletonBase const& )=delete;
	SingletonBase( SingletonBase const&& )=delete;
	SingletonBase& operator=( SingletonBase const& )=delete;
	SingletonBase&& operator=( SingletonBase const&& )=delete;

    public:
	static T* instance()
	{
	    static T instance_;
	    return &instance_;
	}
};


#endif // SINGLETON_H

