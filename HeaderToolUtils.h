#pragma once

#include <array>
#include <string>

namespace HeaderToolUtils
{
	constexpr int NUM_FLOAT_TYPES = 4;
	constexpr int NUM_INT_TYPES = 9;
	constexpr int NUM_BOOL_TYPES = 1;
	constexpr int NUM_STRING_TYPES = 2;
	constexpr int NUM_BASIC_TYPES = NUM_FLOAT_TYPES + NUM_INT_TYPES + NUM_BOOL_TYPES + NUM_STRING_TYPES;
	
	static std::string floatTypes[NUM_FLOAT_TYPES] = {"float", "double", "f32", "f64"};
	static std::string intTypes[NUM_INT_TYPES] = {"int", "i8", "u8", "i16", "u16", "i32", "u32", "i64", "u64"};
	static std::string boolTypes[NUM_BOOL_TYPES] = {"bool"};
	static std::string stringTypes[NUM_STRING_TYPES] = {"std::string", "string"};
	
	typedef std::array<std::string, NUM_BASIC_TYPES> BasicTypeArray;
	BasicTypeArray GetBasicTypes();
}