/*
 * Cppcheck - A tool for static C/C++ code analysis
 * Copyright (C) 2007-2013 Daniel Marjamäki and Cppcheck team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//---------------------------------------------------------------------------
#ifndef optimiseH
#define optimiseH
//---------------------------------------------------------------------------

#if defined(_MSC_VER) && _MSC_VER >= 1600 && _MSC_VER <= 1800
//This has only been tested in VS 2010 & 2013
//Other compilers/STL implementations may already have better comparisons to char arrays (i.e. string literals)
#define OPTIMISE_STD_STRING_COMPARE
#endif

#ifdef OPTIMISE_STD_STRING_COMPARE

//The implementation of this can't tell the difference between a string literal and a char[x] variable, which could have a different size to length.
//Assert when this happens. It is the best that can be done.
#if defined( _DEBUG ) || defined( DEBUG ) || defined (__DEBUG__)
#   if defined(_MSC_VER)
#       define ASSERT(x) if (!(x)) { __debugbreak(); }
#   else
#       include <assert.h>
#       define ASSERT assert
#   endif
#   else
#       define ASSERT(x) {}
#endif

#include <string>

inline bool operator==(const std::string& left, const char (&right)[1])
{
	ASSERT(right[0] == 0);
	(void)right;
	return left.empty();
}

inline bool operator==(const std::string& left, const char(&right)[2])
{
	ASSERT(right[0] != 0 && right[1] == 0);
	return left.size() == 1 && *reinterpret_cast<const unsigned short*>(left.c_str()) == *reinterpret_cast<const unsigned short*>(right);
}

inline bool operator==(const std::string& left, const char(&right)[3])
{
	ASSERT(right[0] != 0 && right[1] != 0 && right[2] == 0);
	return left.size() == 2 && *reinterpret_cast<const unsigned short*>(left.c_str()) == *reinterpret_cast<const unsigned short*>(right);
}

inline bool operator==(const std::string& left, const char(&right)[4])
{
	ASSERT(right[0] != 0 && right[1] != 0 && right[2] != 0 && right[3] == 0);
	return left.size() == 3 && *reinterpret_cast<const unsigned int*>(left.c_str()) == *reinterpret_cast<const unsigned int*>(right);
}

inline bool operator==(const std::string& left, const char(&right)[5])
{
	ASSERT(right[0] != 0 && right[1] != 0 && right[2] != 0 && right[3] != 0 && right[4] == 0);
	return left.size() == 4 && *reinterpret_cast<const unsigned int*>(left.c_str()) == *reinterpret_cast<const unsigned int*>(right);
}

inline bool operator!=(const std::string& left, const char(&)[1])
{
	return !left.empty();
}

inline bool operator!=(const std::string& left, const char(&right)[2])
{
	return !operator==(left, right);
}

inline bool operator!=(const std::string& left, const char(&right)[3])
{
	return !operator==(left, right);
}

inline bool operator!=(const std::string& left, const char(&right)[4])
{
	return !operator==(left, right);
}

inline bool operator!=(const std::string& left, const char(&right)[5])
{
	return !operator==(left, right);
}

#endif // OPTIMISE_STD_STRING_COMPARE

#endif // optimiseH