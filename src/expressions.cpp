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

#include "expressions.h"
#include "script_exception.h"

static uint64_t script_operator_and(const Json::Value& opr, script_context& script_context);
static uint64_t script_operator_or(const Json::Value& opr, script_context& script_context);
static uint64_t script_operator_not(const Json::Value& opr, script_context& script_context);
static uint64_t script_operator_shl(const Json::Value& opr, script_context& script_context);
static uint64_t script_operator_shr(const Json::Value& opr, script_context& script_context);



typedef uint64_t (*operator_callback_t)(const Json::Value& opr, script_context& script_context);
static std::map<std::string, operator_callback_t> operator_map;


void expressions_init()
{
	operator_map["and"] = script_operator_and;
	operator_map["or"] = script_operator_or;
	operator_map["not"] = script_operator_not;
	operator_map["shl"] = script_operator_shl;
	operator_map["shr"] = script_operator_shr;
}

uint64_t expression_process(const Json::Value& value, script_context& script_context)
{
	if (value.isString()) {
		return script_context.resolve_value(value.asString());

	} else if (value.isObject()) {
		std::string opr = value["operator"].asString();
		auto i = operator_map.find(opr);
		if (i == operator_map.end()) {
			throw script_exception(fmt() << "unknown operator: " << opr);
		} else {
			return ((*i).second)(value, script_context);
		}

	} else {
		throw script_exception("invalid value type");
	}
}

static uint64_t script_operator_and(const Json::Value& opr, script_context& script_context)
{
	const Json::Value& left_obj(opr["left"]);
	const Json::Value& right_obj(opr["right"]);

	uint64_t left = expression_process(left_obj, script_context);
	uint64_t right = expression_process(right_obj, script_context);

	return (left & right);
}

static uint64_t script_operator_or(const Json::Value& opr, script_context& script_context)
{
	const Json::Value& left_obj(opr["left"]);
	const Json::Value& right_obj(opr["right"]);

	uint64_t left = expression_process(left_obj, script_context);
	uint64_t right = expression_process(right_obj, script_context);

	return (left | right);
}

static uint64_t script_operator_not(const Json::Value& opr, script_context& script_context)
{
	const Json::Value& value_obj(opr["right"]);

	uint64_t value = expression_process(value_obj, script_context);

	return (~value);
}

static uint64_t script_operator_shl(const Json::Value& opr, script_context& script_context)
{
	const Json::Value& left_obj(opr["left"]);
	const Json::Value& right_obj(opr["right"]);

	uint64_t left = expression_process(left_obj, script_context);
	uint64_t right = expression_process(right_obj, script_context);

	return (left << right);
}

static uint64_t script_operator_shr(const Json::Value& opr, script_context& script_context)
{
	const Json::Value& left_obj(opr["left"]);
	const Json::Value& right_obj(opr["right"]);

	uint64_t left = expression_process(left_obj, script_context);
	uint64_t right = expression_process(right_obj, script_context);

	return (left >> right);
}
