#include <FinalProject/Detail/Query.h>

#include <iostream>

using FinalProject::Detail::Query;

uint32_t Query::s_queryNum = 1;

void Query::Execute(const Database& database)
{
	// print the query number
	std::cout << "//Query " << s_queryNum++ << '\n';
}