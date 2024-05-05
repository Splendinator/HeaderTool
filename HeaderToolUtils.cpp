#include "HeaderToolUtils.h"

namespace HeaderToolUtils
{
	BasicTypeArray GetBasicTypes()
	{
		BasicTypeArray array;
		int insertIndex = 0;

		// floats
		for (int i = 0; i < NUM_FLOAT_TYPES; ++i)
		{
			array[insertIndex++] = floatTypes[i];
		}

		// ints
		for (int i = 0; i < NUM_INT_TYPES; ++i)
		{
			array[insertIndex++] = intTypes[i];
		}

		// bools
		for (int i = 0; i < NUM_BOOL_TYPES; ++i)
		{
			array[insertIndex++] = boolTypes[i];
		}

		// string
		for (int i = 0; i < NUM_STRING_TYPES; ++i)
		{
			array[insertIndex++] = stringTypes[i];
		}

		return array;
	}
}
