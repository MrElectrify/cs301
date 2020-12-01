#include <FinalProject/Database.h>

#include <FinalProject/Error.h>

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
	std::string::const_iterator it = data.cbegin();
	while (it != data.cend())
	{
		Collection_t collection;
		std::tie(collection, it) = 
			m_dataParser.ParseCollection(it, data.cend());
		// add the collection
		m_collections.push_back(std::move(collection));
		// map all of the variables
		for (const Node_t& node : m_collections.back())
			m_nodeToCollectionIndexMap.emplace(node, m_collections.size() - 1);
	}
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