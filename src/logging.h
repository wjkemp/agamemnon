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

#ifndef LOGGING_H
#define LOGGING_H

#include <sstream>



enum loglevel
{
	ll_none,
	ll_v,
	ll_vv,
	ll_vvv
};


class log
{
public:
	log();
	virtual ~log();
	std::ostringstream& get(int loglevel);

public:
	static int current_loglevel;

protected:
	std::ostringstream _os;

private:
	log(const log&) = delete;
	log& operator =(const log&) = delete;

private:
	int _loglevel;
};


#define LOG(loglevel) log().get(loglevel)


#endif
