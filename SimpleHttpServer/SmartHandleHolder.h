#pragma once
#include <Windows.h>
#include <memory>

class SmartHandleHolder
{

public:
	// CR: add explicit to constructors with only one argument in order to avoid unintended implicit casts
	SmartHandleHolder(HANDLE handle);
	// CR: always make descutors in base classes virtual, otherwise you will have memory leaks and
	// weird bugs. To see why read: https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
	// As a rule of thumb, for every single class you write, either the class should be final, or the 
	// destructor should be virtual!
	~SmartHandleHolder();

	// CR: For Coder:
	// there arent any use cases where you want to hold a SmartHandleHolder without inheriting from it 
	// This isnt really an object but a utility for closing handles. Therefore any usage of this clas
	// will be by inheritance in some resource class and if you want composition of multiple resources
	// that are HANDLEs, than compose several classes that inherit from handle. You want to avoid
	// exposing of handles at all cost, and some would even recommend private inheritance from 
	// this class, and a protected constructor!!!
	// for CR maker: what if someone would like to use it with composition and hold muliple "Handlers" for instance ?
	//doesnt is should be public?
public: 
	HANDLE get_handle()const;

public:
	SmartHandleHolder(const SmartHandleHolder& other) = delete;
	SmartHandleHolder(SmartHandleHolder&& other) = delete;
	SmartHandleHolder& operator=(SmartHandleHolder&& other) = delete;
	SmartHandleHolder& operator=(SmartHandleHolder& other) = delete;


private:
	const HANDLE _handle;
};

