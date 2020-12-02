#include <FinalProject/Database.h>
#include <FinalProject/Detail/Query.h>

#include <iostream>

using FinalProject::Database;
using FinalProject::Detail::Query;

int main(int argc, char* argv[])
{
	std::string dataPath = (argc == 2) ? argv[1] : "data.txt";
	std::string queryPath = (argc == 3) ? argv[2] : "final.txt";
	Database database;
	std::error_code ec{};
	database.ImportData(dataPath, ec);
	if (ec)
	{
		std::cerr << "Failed to import data from " << dataPath << ": " << ec.message() << '\n';
		return 1;
	}
	database.ImportQueries(queryPath, ec);
	if (ec)
	{
		std::cerr << "Failed to import queries from " << queryPath << ": " << ec.message() << " (query " << Query::GetQueryNum() << ")\n";
		return 1;
	}
	return 0;
}