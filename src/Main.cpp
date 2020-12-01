#include <FinalProject/Database.h>

#include <iostream>

using FinalProject::Database;

int main(int argc, char* argv[])
{
	std::string dataPath = (argc == 2) ? argv[1] : "data.txt";
	std::string queryPath = (argc == 3) ? argv[2] : "final.txt";
	Database database;
	if (database.ImportData(dataPath) == false)
	{
		std::cerr << "Failed to import data from " << dataPath << '\n';
		return 1;
	}
	if (database.ExecuteQuery(queryPath) == false)
	{
		std::cerr << "Failed to execute queries from " << queryPath << '\n';
		return 1;
	}
	return 0;
}