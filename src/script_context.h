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

#ifndef SCRIPT_CONTEXT_H
#define SCRIPT_CONTEXT_H


#include "memory_region.h"
#include "variable.h"
#include <map>


class script_context
{
public:
	~script_context();

	void add_memory_region(memory_region* region);
	memory_region* get_memory_region(const std::string& name) const;

	void set_variable(const variable& variable);

	uint64_t resolve_value(const std::string& value) const;

private:
	std::map<std::string, memory_region*> _memory_regions;
	std::map<std::string, variable> _variables;
};


#endif
