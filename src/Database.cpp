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
		// map all of the variables and add them to the set
		m_indexToContainsSet.emplace_back();
		for (const auto& node : m_collections.back())
		{
			m_nodeToCollectionIndexMap.emplace(node, m_collections.size() - 1);
			m_indexToContainsSet.back().emplace(node.first);
		}
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

void Database::ImportQueries(const std::string& queryPath)
{
	std::ifstream inQuery(queryPath);
	if (inQuery.good() == false)
		throw make_error_code(DatabaseErrc::FileNotFound);
	// read the data
	std::string queries((std::istreambuf_iterator<char>(inQuery)),
		std::istreambuf_iterator<char>());
	// parse the data
	std::string::const_iterator it = queries.cbegin();
	while (it != queries.cend())
	{
		Detail::QueryParser::QueryPtr_t queryPtr;
		// parse the query
		std::tie(queryPtr, it) =
			m_queryParser.ParseQuery(it, queries.cend());
		// import the query
		m_queries.push_back(std::move(queryPtr));
	}
}

void Database::ImportQueries(const std::string& queryPath, std::error_code& ec) noexcept
{
	try
	{
		ImportQueries(queryPath);
	}
	catch (const std::error_code& e)
	{
		ec = e;
	}
}

void Database::ExecuteQueries()
{
	for (const Detail::QueryParser::QueryPtr_t& queryPtr : m_queries)
		queryPtr->Execute(*this);
	m_queries.clear();
}

void Database::ExecuteQueries(std::error_code& ec) noexcept
{
	try
	{
		ExecuteQueries();
	}
	catch (const std::error_code& e)
	{
		ec = e;
	}
}