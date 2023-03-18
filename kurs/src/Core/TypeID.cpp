#include "Core/TypeID.hpp"

namespace kurs::detail
{
	TypeID generateTypeID()
	{
		static std::ptrdiff_t s_ID;
		return static_cast<TypeID>(s_ID++);
	}
}
