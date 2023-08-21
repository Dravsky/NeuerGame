#include "Object.h"

namespace lola 
{
	void Object::Read(const rapidjson::Value& value) 
	{
		READ_DATA(value, name);
	}
}