#pragma once

#include "kurspch.hpp"

#include "Core/TypeName.hpp"

namespace kurs::detail
{
	enum TypeID : std::ptrdiff_t
	{
		TypeID_Undefined = -1
	};

	TypeID generateTypeID();
}

namespace kurs
{
	using detail::TypeID;
	using detail::TypeID_Undefined;

	template<typename T>
	TypeID getTypeID()
	{
		static TypeID s_ID = detail::generateTypeID();
		return s_ID;
	}
}
