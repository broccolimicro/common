/*
 * message.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: nbingham
 */

#include "message.h"

int num_errors = 0;
int num_warnings = 0;
int num_internal = 0;
int num_notes = 0;
int num_log = 0;
bool verbose = true;
bool debug = true;

void internal(string location, string internal, string debug_file, int debug_line)
{
	cout << debug_file << ":" << debug_line << ":";
	if (location != "")
		cout << location << ": ";
	cout << "internal failure: " << internal << endl;
	num_internal++;
}

void error(string location, string error, string debug_file, int debug_line)
{
	if (debug)
		cout << debug_file << ":" << debug_line << ":";

	if (location != "")
		cout << location << ": ";
	cout << "error: " << error << endl;
	num_errors++;
}

void warning(string location, string warning, string debug_file, int debug_line)
{
	if (debug)
		cout << debug_file << ":" << debug_line << ":";

	if (location != "")
		cout << location << ": ";
	cout << "warning: " << warning << endl;
	num_warnings++;
}

void note(string location, string note, string debug_file, int debug_line)
{
	if (debug)
		cout << debug_file << ":" << debug_line << ":";

	if (location != "")
		cout << location << ": ";
	cout << "note: " << note << endl;

	num_notes++;
}

void log(string location, string log, string debug_file, int debug_line)
{
	if (verbose)
	{
		if (debug)
			cout << debug_file << ":" << debug_line << ":";

		if (location != "")
			cout << location << ":";
		cout << " " << log << endl;
		num_log++;
	}
}

void progress(string location, string log, string debug_file, int debug_line)
{
	cout << "\r";
	if (debug)
		cout << debug_file << ":" << debug_line << ":";
	if (location != "")
		cout << location << ":";
	cout << " " << log << "                              ";
	cout.flush();
}

void message(string message, string debug_file, int debug_line)
{
	if (debug)
		cout << debug_file << ":" << debug_line << ":";
	cout << " " << message << endl;
}

void done_progress()
{
	cout << "\r";
	cout.flush();
}

void complete()
{
	cout << "completed with ";
	if (num_internal > 0)
		cout << num_internal << " internal failures, ";
	cout << num_errors << " errors and " << num_warnings << " warnings" << endl;
}

bool is_clean()
{
	return (num_internal == 0 && num_errors == 0);
}

void set_debug(bool value)
{
	debug = value;
}

void set_verbose(bool value)
{
	verbose = value;
}
