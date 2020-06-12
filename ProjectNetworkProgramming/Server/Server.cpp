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
	test.CreateGroup("Hong_chat", "hue47");
	//test.AddUser("john1", "wick");
	//test.LockAccount("random");
	return EXIT_SUCCESS;
}