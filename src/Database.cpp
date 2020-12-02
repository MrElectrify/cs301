#include <FinalProject/Database.h>

#include <FinalProject/Error.h>

#include <fstream>
#include <iostream>
#include <sstream>

using FinalProject::Database;

void Database::ImportData(const std::string& dataPath)
{
	std::ifstream inData(dataPath, std::ios_base::binary);
	
	if (inData.good() == false)
		throw make_error_code(DatabaseErrc::FileNotFound);
	// read the data
	std::string data, tmp;
	while (GetLineCP(inData, tmp).eof() == false)
	{
		data.append(tmp);
		// standardize line endings to \n
		data.push_back('\n');
	}
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
		for (const Node_t& node : m_collections.back().second)
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

void Database::ImportQueries(const std::string& queryPath)
{
	std::ifstream inQueries(queryPath, std::ios_base::binary);
	if (inQueries.good() == false)
		throw make_error_code(DatabaseErrc::FileNotFound);
	// read the data
	std::string queries, tmp;
	while (GetLineCP(inQueries, tmp).eof() == false)
	{
		queries.append(tmp);
		// standardize line endings to \n
		queries.push_back('\n');
	}
	// parse each query
	std::stringstream queryStream(queries);
	std::string query;
	while (GetQuery(queryStream, query).eof() == false)
	{
		std::string::const_iterator it = query.cbegin();
		Detail::QueryParser::QueryPtr_t queryPtr;
		// parse the query
		try
		{
			std::tie(queryPtr, it) =
				m_queryParser.ParseQuery(it, query.cend());
		}
		catch (const std::error_code& ec)
		{
			std::cout << "Error parsing query " << Detail::Query::GetQueryNum()++ << ": " << ec.message() << '\n';
			continue;
		}
		// import the query
		queryPtr->Execute(*this);
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

// by user763305 from https://stackoverflow.com/a/6089413/6815521.
// I don't know which line endings there will be, so I need to standardize it to \n
std::istream& Database::GetLineCP(std::istream& is, std::string& t) noexcept
{
	t.clear();

	// The characters in the stream are read one-by-one using a std::streambuf.
	// That is faster than reading them one-by-one using the std::istream.
	// Code that uses streambuf this way must be guarded by a sentry object.
	// The sentry object performs various tasks,
	// such as thread synchronization and updating the stream state.

	std::istream::sentry se(is, true);
	std::streambuf* sb = is.rdbuf();

	for (;;) {
		int c = sb->sbumpc();
		switch (c) {
		case '\n':
			return is;
		case '\r':
			if (sb->sgetc() == '\n')
				sb->sbumpc();
			return is;
		case std::streambuf::traits_type::eof():
			// Also handle the case when the last line has no line ending
			if (t.empty())
				is.setstate(std::ios::eofbit);
			return is;
		default:
			t += (char)c;
		}
	}
}

std::istream& Database::GetQuery(std::istream& is, std::string& t)
{
	// get the query until the ';'
	t.clear();
	if (std::getline(is, t, ';').eof() == true)
	{
		// end of file
		if (t.empty() == true)
			return is;
		throw make_error_code(DatabaseErrc::IncompleteData);
	}
	t += ';';
	// bump the istream until the newline
	std::streambuf* sb = is.rdbuf();
	while (true)
	{
		int c = sb->sbumpc();
		switch (c)
		{
		case ' ':
			continue;
		case '\n':
			return is;
		case std::streambuf::traits_type::eof():
			if (t.empty())
				is.setstate(std::ios::eofbit);
			return is;
		default:
			throw make_error_code(DatabaseErrc::UnexpectedChar);
		}
	}
}