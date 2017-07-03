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

#ifndef MEMORY_REGION_H
#define MEMORY_REGION_H

#include <string>
#include <cstdint>

class memory_region
{
public:
	memory_region(const std::string& name, uint64_t address, uint64_t size);
	~memory_region();

	std::string name() const { return _name; }
	uint64_t address() const { return _address; }
	uint64_t size() const { return _size; }
	void* mapped_address() const { return _mapped_address; }

private:
	std::string _name;
	uint64_t _address;
	uint64_t _size;
	void* _mapped_base;
	void* _mapped_address;
	uint64_t _mapped_size;
	int _fd;
};

#endif
