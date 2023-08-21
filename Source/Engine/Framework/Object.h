#pragma once
#include "Factory.h"
#include "Core/Json.h"
#include <string>

#define CLASS_DECLARATION(classname) \
	virtual const char* GetClassName() { return #classname; } \
	virtual void Read(const json_t& value); \
	virtual std::unique_ptr<Object> Clone() { return std::make_unique<classname>(*this); } \
	class Register \
	{ \
	public: \
		Register() \
		{ \
			Factory::Instance().Register<classname>(#classname); \
		} \
	}; 

#define CLASS_DEFINITION(classname) \
	classname::Register register_class;


namespace lola 
{
	class Object 
	{
	public:
		Object() = default;
		Object(const std::string name) : name{ name } {};
		virtual ~Object() { OnDestroy(); } // The destructor should be virtual, as then all children will call their own destructor before the parent's.

		CLASS_DECLARATION(Object);

		virtual bool Initialize() { return true; } // = 0 would force children classes to implement the function.
		virtual void OnDestroy() {}

	public:
		std::string name;
		bool active = true;
	};
}