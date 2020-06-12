// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "query_database.h"

using namespace std;

int main()
{
	QueryDatabase test;
	test.LockAccount("ranom");
	return EXIT_SUCCESS;
}