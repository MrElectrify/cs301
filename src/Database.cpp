#include <FinalProject/Database.h>

#include <fstream>
#include <iterator>

using FinalProject::Database;

void Database::ImportData(const std::string& dataPath)
{
	std::ifstream inData(dataPath);
	if (inData.good() == false)
		throw make_error_code(DatabaseErrc::FileNotFound);
	// read the data
	std::string data((std::istreambuf_iterator<char>(inData)),
		std::istreambuf_iterator<char>());
	// parse the data
	ParseData(data);
}

void Database::ImportData(const std::string& dataPath, std::error_code& ec) noexcept
{
	try
	{
		ImportData(dataPath);
	}
	catch (const std::error_code& e)
	{
		ec = e;
	}
}

void Database::ExecuteQuery(const std::string& queryPath)
{
	std::ifstream inQuery(queryPath);
	if (inQuery.good() == false)
		throw make_error_code(DatabaseErrc::FileNotFound);
}

void Database::ExecuteQuery(const std::string& queryPath, std::error_code& ec) noexcept
{
	try
	{
		ExecuteQuery(queryPath);
	}
	catch (const std::error_code& e)
	{
		ec = e;
	}
}

void Database::ParseData(const std::string& data)
{

}