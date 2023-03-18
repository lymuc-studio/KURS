#pragma once

#include "kurspch.h"

#include "Core/TypeName.hpp"

namespace kurs::detail
{
	enum TypeID : std::ptrdiff_t
	{
		Undefined = -1
	};

	TypeID generateTypeID();
}

namespace kurs
{
	using detail::TypeID;

	template<typename T>
	TypeID getTypeID()
	{
		static TypeID s_ID = detail::generateTypeID();
		return s_ID;
	}
}
