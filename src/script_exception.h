/*
	Copyright (c) 2017 Willem Kemp

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#ifndef SCRIPT_EXCEPTION_H
#define SCRIPT_EXCEPTION_H

#include <exception>
#include <sstream>


class script_exception : public std::runtime_error
{

public:
	script_exception(const std::string& message) :
		std::runtime_error(message)
	{

	}

	const char* what() const noexcept
	{
		return std::runtime_error::what();
	}

};


class fmt
{
public:
	fmt() {}
	~fmt() {}

	template <typename Type>
	fmt & operator << (const Type & value)
	{
		stream_ << value;
		return *this;
	}

	std::string str() const { return stream_.str(); }
	operator std::string () const   { return stream_.str(); }


private:
	std::stringstream stream_;

	fmt(const fmt &) = delete;
	fmt & operator = (fmt &) = delete;
};

#endif
