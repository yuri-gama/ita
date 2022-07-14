#ifndef MYSIGNAL
#define MYSIGNAL

#include <stdbool.h>
#include <signal.h>
#include <stdio.h>

bool termination_prog = false;

void stop_handler(int );

#endif