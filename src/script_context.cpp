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

#include "script_context.h"
#include "script_exception.h"


script_context::~script_context()
{
	for (auto&& i = _memory_regions.begin(); i != _memory_regions.end(); ++i) {
		memory_region* memory_region((*i).second);
		delete memory_region;
	}
}

void script_context::add_memory_region(memory_region* region)
{
	_memory_regions[region->name()] = region;
}

memory_region* script_context::get_memory_region(const std::string& name) const
{
	auto i = _memory_regions.find(name);
	if (i == _memory_regions.end()) {
		return nullptr;
	} else {
		return (*i).second;
	}
}

void script_context::set_variable(const variable& variable)
{
	_variables[variable.name()] = variable;
}

uint64_t script_context::resolve_value(const std::string& value) const
{
	uint64_t result = 0;

	if (value.length() == 0) {
		return 0;
	}

	// Anything that starts with an alphabetic character is considered an identifier
	if (isalpha(value[0])) {

		auto i = _variables.find(value);

		if (i != _variables.end()) {
			result = (*i).second.value();

		} else {
			throw script_exception(fmt() << "could not find identifier: " << value);
		}

	// Else parse as a number
	} else {
		size_t pos;
		result = std::stoull(value, &pos, 0);
	}

	return result;
}
