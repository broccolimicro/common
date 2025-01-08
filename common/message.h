#pragma once

#include "standard.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"

void internal(string location, string internal, string debug_file, int debug_line);
void error(string location, string error, string debug_file, int debug_line);
void warning(string location, string warning, string debug_file, int debug_line);
void note(string location, string note, string debug_file, int debug_line);
void log(string location, string log, string debug_file, int debug_line);
void progress(string location, string log, string debug_file, int debug_line);
void message(string message, string debug_file, int debug_line);
void done_progress();

void complete();
bool is_clean();

void flag_errors(int num);
void flag_warnings(int num);
void flag_internal(int num);
void flag_notes(int num);
void flag_log(int num);

void set_debug(bool value);
bool get_debug();
void set_verbose(bool value);
bool get_verbose();

