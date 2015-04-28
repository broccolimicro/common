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
	if (location != "")
		printf("%s:%d:%s: internal failure: %s\n", debug_file.c_str(), debug_line, location.c_str(), internal.c_str());
	else
		printf("%s:%d: internal failure: %s\n", debug_file.c_str(), debug_line, internal.c_str());
	num_internal++;
}

void error(string location, string error, string debug_file, int debug_line)
{
	if (debug && location != "")
		printf("%s:%d:%s: error: %s\n", debug_file.c_str(), debug_line, location.c_str(), error.c_str());
	else if (debug && location == "")
		printf("%s:%d: error: %s\n", debug_file.c_str(), debug_line, error.c_str());
	else if (!debug && location != "")
		printf("%s: error: %s\n", location.c_str(), error.c_str());
	else if (!debug && location == "")
		printf("error: %s\n", error.c_str());
	num_errors++;
}

void warning(string location, string warning, string debug_file, int debug_line)
{
	if (debug && location != "")
		printf("%s:%d:%s: warning: %s\n", debug_file.c_str(), debug_line, location.c_str(), warning.c_str());
	else if (debug && location == "")
		printf("%s:%d: warning: %s\n", debug_file.c_str(), debug_line, warning.c_str());
	else if (!debug && location != "")
		printf("%s: warning: %s\n", location.c_str(), warning.c_str());
	else if (!debug && location == "")
		printf("warning: %s\n", warning.c_str());
	num_warnings++;
}

void note(string location, string note, string debug_file, int debug_line)
{
	if (debug && location != "")
		printf("%s:%d:%s: note: %s\n", debug_file.c_str(), debug_line, location.c_str(), note.c_str());
	else if (debug && location == "")
		printf("%s:%d: note: %s\n", debug_file.c_str(), debug_line, note.c_str());
	else if (!debug && location != "")
		printf("%s: note: %s\n", location.c_str(), note.c_str());
	else if (!debug && location == "")
		printf("note: %s\n", note.c_str());
	num_notes++;
}

void log(string location, string log, string debug_file, int debug_line)
{
	if (verbose)
	{
		if (debug && location != "")
			printf("%s:%d:%s: log: %s\n", debug_file.c_str(), debug_line, location.c_str(), log.c_str());
		else if (debug && location == "")
			printf("%s:%d: log: %s\n", debug_file.c_str(), debug_line, log.c_str());
		else if (!debug && location != "")
			printf("%s: log: %s\n", location.c_str(), log.c_str());
		else if (!debug && location == "")
			printf("log: %s\n", log.c_str());
		num_log++;
	}
}

void progress(string location, string log, string debug_file, int debug_line)
{
	if (debug && location != "")
		printf("\r%s:%d:%s: %s                              ", debug_file.c_str(), debug_line, location.c_str(), log.c_str());
	else if (debug && location == "")
		printf("\r%s:%d: %s                              ", debug_file.c_str(), debug_line, log.c_str());
	else if (!debug && location != "")
		printf("\r%s: %s                              ", location.c_str(), log.c_str());
	else if (!debug && location == "")
		printf("\r%s                              ", log.c_str());
	fflush(stdout);
}

void message(string message, string debug_file, int debug_line)
{
	if (debug)
		printf("%s:%d: %s\n", debug_file.c_str(), debug_line, message.c_str());
	else
		printf("%s\n", message.c_str());
}

void done_progress()
{
	printf("\r");
	fflush(stdout);
}

void complete()
{
	if (num_internal > 0)
		printf("completed with %d internal failures, %d errors, and %d warnings\n", num_internal, num_errors, num_warnings);
	else
		printf("completed with %d errors, and %d warnings\n", num_errors, num_warnings);
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
