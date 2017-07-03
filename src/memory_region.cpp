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

#include "memory_region.h"
#include "script_exception.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>



memory_region::memory_region(const std::string& name, uint64_t address, uint64_t size) :
	_name(name),
	_address(address),
	_size(size)
{
	uint64_t page_size = sysconf(_SC_PAGE_SIZE);
	uint64_t page_mask = (page_size - 1);

	// Align size to page size
	uint64_t aligned_address = address & ~page_mask;

	// Adjust size with the possible offset
	_mapped_size = size + (address - aligned_address);

	// Align size to page size
	if (_mapped_size & page_mask) {
		_mapped_size = (_mapped_size + page_size) & ~page_mask;
	}

	// Open /dev/mem
	_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (_fd == -1) {
		throw script_exception("Could not open /dev/mem\n");
	}

	// Map the base address
	_mapped_base = mmap(0, _mapped_size, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, aligned_address);
	if (_mapped_base == (void *) -1) {
		close(_fd);
		throw script_exception(fmt() << "Could not map memory at " << std::hex << address);
	}

	// Set the mapped address to the correct offset
	_mapped_address = (uint8_t*)_mapped_base + (address - aligned_address);
}

memory_region::~memory_region()
{
	munmap(_mapped_base, _mapped_size);
	close(_fd);
}
