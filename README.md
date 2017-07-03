# Introduction

agamemnon is a scriptable device memory access utility.

# Usage

An agamemnon script consists of an array of JSON command objects. Commands are used to

- Set configuration parameters
- Declare physical memory regions
- Read and write values to physical memory
- Declare and update variables

The array can also contain strings as comments.


# Basic concepts

## Data representation

All data is stored in 64 bit unsigned integers, and all arithmetic operations are performed as
64 bit operations.


## Expressions

Any field that expects an expression can be set to either a numerical constant, a variable identifier, or
an expression object. Expression objects are JSON objects that have an operator and a left and optional right operand.
The operands can in turn be constants, identifiers, or expression objects. This allows the user to build complex expressions.

The following operators are implemented:
- 'and' - Binary AND
- 'or' - Binary OR
- 'not' - Binary NOT (only takes a 'right' parameter)
- 'shl' - Shift left
- 'shr' - Shift right

Example:

```
[
	{ "command": "set_variable", "name": "var_a", "value": "0x0000000a" },
	{ "command": "set_variable", "name": "var_b", "value": "0x00000005" },
	{ "command": "set_variable", "name": "result",
		"value": { "operator": "or", "left": "var_a", "right": "var_b" } },
	{ "command": "assert", "variable": "result", "value": "0x0000000f", "expression": true }
]
```


# Command reference

## set_config

Updates configuration variables. The following configuration variables are currently supported:

| name | type | value | description
| --- | --- | --- | ---
| loglevel | integer | 0-3 | Sets the log level (0 = none, 3 = verbose)


## set_variable

Sets a new variable or updates an existing variable.

| Field | Type | Description
| --- | --- | ---
| name | string | The name of the variable
| value | expression | The value to set


## declare_memory_region

Declares a physical memory region.

| Field | Type | Description
| --- | --- | ---
| name | string | The name of the region
| value | string | A numeric constant


## write_value

Writes a single value to a memory address.

| Field | Type | Description
| --- | --- | ---
| memory_region | string | The name of the region to write to
| offset | expression | The byte offset at which to write the value
| width | integer | The bit width of the access (8, 16, 32, 64)
| value | expression | The value to write


## read_value

Reads a single value from a memory address.

| Field | Type | Description
| --- | --- | ---
| memory_region | string | The name of the region to read from
| offset | expression | The byte offset to read from
| width | integer | The bit width of the access (8, 16, 32, 64)
| variable_name | string | The name of the variable to store the result in (optional)
| display_prefix | string | A string to print before printing the variable (optional)


## print

Prints a formatted string

| Field | Type | Description
| --- | --- | ---
| message | string | The format string
| arguments | expression array| An array of parameters

The print command will replace the placeholders in the message string with values from the arguments array.
Placeholders are in the form of {n}, where n indicates the number of the argument, indexed from 1.


## poll_value

Polls a memory location until a condition is met

| Field | Type | Description
| --- | --- | ---
| memory_region | string | The name of the region to read from
| offset | expression | The byte offset to read from
| width | integer | The bit width of the access (8, 16, 32, 64)
| condition | boolean | The condition to test for
| mask | expression | A mask that is binary ANDed to the value read back
| timeout | integer | A timeout in milliseconds


## delay

Delays for the specified time

| Field | Type | Description
| --- | --- | ---
| s | integer | The amount of seconds to delay (optional)
| ms | integer | The amount of milliseconds to delay (optional)
| us | integer | The amount of microseconds to delay (optional)


## assert

Checks a variable against a value

| Field | Type | Description
| --- | --- | ---
| variable | string | The name of the variable
| value | expression | The value to test for
| condition | boolean | The expected condition
