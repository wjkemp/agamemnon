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

#include "commands.h"


#include "memory_region.h"
#include "script_exception.h"
#include "expressions.h"
#include "textutils.h"
#include "logging.h"

#include <iostream>
#include <chrono>
#include <thread>



typedef void (*dispatch_script_command_t)(const Json::Value& script, script_context& script_context);

static std::map<std::string, dispatch_script_command_t> command_dispatch_map;


static void cmd_set_config(const Json::Value& script, script_context& script_context);
static void cmd_declare_memory_region(const Json::Value& script, script_context& script_context);
static void cmd_set_variable(const Json::Value& script, script_context& script_context);
static void cmd_write_value(const Json::Value& script, script_context& script_context);
static void cmd_read_value(const Json::Value& script, script_context& script_context);
static void cmd_poll_value(const Json::Value& script, script_context& script_context);
static void cmd_delay(const Json::Value& script, script_context& script_context);
static void cmd_print(const Json::Value& script, script_context& script_context);
static void cmd_assert(const Json::Value& script, script_context& script_context);



void commands_init()
{
	command_dispatch_map["set_config"] = cmd_set_config;
	command_dispatch_map["declare_memory_region"] = cmd_declare_memory_region;
	command_dispatch_map["set_variable"] = cmd_set_variable;
	command_dispatch_map["write_value"] = cmd_write_value;
	command_dispatch_map["read_value"] = cmd_read_value;
	command_dispatch_map["poll_value"] = cmd_poll_value;
	command_dispatch_map["delay"] = cmd_delay;
	command_dispatch_map["print"] = cmd_print;
	command_dispatch_map["assert"] = cmd_assert;
}

void command_process(const Json::Value& script, script_context& script_context)
{
	// Strings are allowed for comments
	if (!script.isObject() && !script.isArray() && !script.isString()) {
		throw script_exception("Invalid script object");
	}

	if (script.isObject()) {
		std::string command = script["command"].asString();
		auto i = command_dispatch_map.find(command);
		if (i == command_dispatch_map.end()) {
			throw script_exception("Command not found");
		} else {
			((*i).second)(script, script_context);
		}

	} else {
		for (Json::ArrayIndex i=0; i < script.size(); ++i) {
			const Json::Value& child(script[i]);
			command_process(child, script_context);
		}
	}
}


static void cmd_set_config(const Json::Value& script, script_context& script_context)
{
	std::string name = script["name"].asString();

	if (name == "loglevel") {
		log::current_loglevel = script["value"].asInt();
	}

}

static void cmd_declare_memory_region(const Json::Value& script, script_context& script_context)
{
	std::string name = script["name"].asString();
	uint64_t address = tu::parse_hex(script["address"].asString());
	uint64_t size = tu::parse_hex(script["size"].asString());

	LOG(ll_vvv) << "declare_memory_region: name=" << name << ", address=" << std::hex << address << ", size=" << size;

	memory_region* region = new memory_region(name, address, size);
	script_context.add_memory_region(region);
}

static void cmd_set_variable(const Json::Value& script, script_context& script_context)
{
	std::string name = script["name"].asString();
	uint64_t value = expression_process(script["value"], script_context);

	LOG(ll_vvv) << "set_variable: name=" << name << ", value=" << std::hex << value;

	script_context.set_variable(variable(name, value));
}

static void cmd_write_value(const Json::Value& script, script_context& script_context)
{
	memory_region* memory_region = script_context.get_memory_region(script["memory_region"].asString());
	uint64_t offset = expression_process(script["offset"].asString(), script_context);
	int width = script["width"].asInt();
	uint64_t value = expression_process(script["value"], script_context);

	if (memory_region == nullptr) {
		throw script_exception(fmt() << "memory region " << script["memory_region"].asString() << " not found");
	}

	LOG(ll_vvv) << "write_value: memory_region=" << memory_region->name() << ", offset=" << std::hex <<  offset << ", value=" << std::hex << value;

	switch (width) {
		case 8: {
			uint8_t* ptr = (uint8_t*)((uint8_t*)memory_region->mapped_address() + offset);
			*ptr = value;
		} break;
		case 16: {
			uint16_t* ptr = (uint16_t*)((uint8_t*)memory_region->mapped_address() + offset);
			*ptr = value;
		} break;
		case 32: {
			uint32_t* ptr = (uint32_t*)((uint8_t*)memory_region->mapped_address() + offset);
			*ptr = value;
		} break;
		case 64: {
			uint64_t* ptr = (uint64_t*)((uint8_t*)memory_region->mapped_address() + offset);
			*ptr = value;
		} break;
		default: {
			throw script_exception(fmt() << "invalid data width: " << width);
		}
	}
}

static void cmd_read_value(const Json::Value& script, script_context& script_context)
{
	memory_region* memory_region = script_context.get_memory_region(script["memory_region"].asString());
	uint64_t offset = expression_process(script["offset"].asString(), script_context);
	int width = script["width"].asInt();
	std::string variable_name = script["variable_name"].asString();
	std::string display_prefix = script["display_prefix"].asString();

	if (memory_region == nullptr) {
		throw script_exception(fmt() << "memory region " << script["memory_region"].asString() << " not found");
	}

	LOG(ll_vvv) << "read_value: memory_region=" << memory_region->name() << ", offset=" << std::hex << offset;

	uint64_t value = 0;

	switch (width) {
		case 8: {
			uint8_t* ptr = (uint8_t*)((uint8_t*)memory_region->mapped_address() + offset);
			value = *ptr;
		} break;
		case 16: {
			uint16_t* ptr = (uint16_t*)((uint8_t*)memory_region->mapped_address() + offset);
			value = *ptr;
		} break;
		case 32: {
			uint32_t* ptr = (uint32_t*)((uint8_t*)memory_region->mapped_address() + offset);
			value = *ptr;
		} break;
		case 64: {
			uint64_t* ptr = (uint64_t*)((uint8_t*)memory_region->mapped_address() + offset);
			value = *ptr;
		} break;
		default: {
			throw script_exception(fmt() << "invalid data width: " << width);
		}
	}

	if (!variable_name.empty()) {
		script_context.set_variable(variable(variable_name, value));
	}

	std::cout << display_prefix << std::hex << value << std::endl;
}

static void cmd_poll_value(const Json::Value& script, script_context& script_context)
{

	memory_region* memory_region = script_context.get_memory_region(script["memory_region"].asString());
	uint64_t offset = expression_process(script["offset"].asString(), script_context);
	int width = script["width"].asInt();
	bool condition = script["condition"].asBool();
	uint64_t mask = expression_process(script["mask"], script_context);
	int timeout = script["timeout"].asInt();

	if (memory_region == nullptr) {
		throw script_exception(fmt() << "memory region " << script["memory_region"].asString() << " not found");
	}

	LOG(ll_vvv) << "poll_value: memory_region=" << memory_region->name() << ", offset=" << std::hex << offset;

	uint64_t value = 0;

	auto mark = std::chrono::high_resolution_clock::now();

	do {
		switch (width) {
			case 8: {
				uint8_t* ptr = (uint8_t*)((uint8_t*)memory_region->mapped_address() + offset);
				value = *ptr;
			} break;
			case 16: {
				uint16_t* ptr = (uint16_t*)((uint8_t*)memory_region->mapped_address() + offset);
				value = *ptr;
			} break;
			case 32: {
				uint32_t* ptr = (uint32_t*)((uint8_t*)memory_region->mapped_address() + offset);
				value = *ptr;
			} break;
			case 64: {
				uint64_t* ptr = (uint64_t*)((uint8_t*)memory_region->mapped_address() + offset);
				value = *ptr;
			} break;
			default: {
				throw script_exception(fmt() << "invalid data width: " << width);
			}
		}

		auto now = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - mark);

		if (elapsed.count() > timeout) {
			LOG(ll_v) << "timeout reached while polling value";
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	} while (((value & mask) > 0) != condition);
}

static void cmd_delay(const Json::Value& script, script_context& script_context)
{
	int s = script["s"].asInt();
	int ms = script["ms"].asInt();
	int us = script["us"].asInt();

	us += ms * 1000;
	us += s * 1000 * 1000;

	LOG(ll_vvv) << "delay: us=" << us;

	std::this_thread::sleep_for(std::chrono::microseconds(us));
}

static void cmd_print(const Json::Value& script, script_context& script_context)
{
	std::string message = script["message"].asString();
	Json::Value arguments(script["arguments"]);

	if (!arguments.isArray()) {
		throw script_exception("print arguments must be an array");
	}

	for (Json::ArrayIndex i=0; i < arguments.size(); ++i) {
		uint64_t value = expression_process(arguments[i], script_context);
		std::string text = (fmt() << std::hex << value);
		std::string placeholder = (fmt() << "{" << i + 1 << "}");

		size_t start_pos = message.find(placeholder);
		if(start_pos == std::string::npos) {
			throw script_exception("could not find string placeholder");
		}

		message.replace(start_pos, placeholder.length(), text);
	}

	std::cout << message << std::endl;
}

static void cmd_assert(const Json::Value& script, script_context& script_context)
{
	uint64_t variable = expression_process(script["variable"], script_context);
	uint64_t value = expression_process(script["value"], script_context);
	bool condition = script["condition"].asBool();

	LOG(ll_vvv) << "assert: variable="<<  std::hex << variable << ", value=" << std::hex << value;

	if ((variable == value) != condition) {
		throw script_exception("assertion failed");
	}
}
