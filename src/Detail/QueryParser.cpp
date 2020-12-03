#include <FinalProject/Detail/QueryParser.h>

#include <FinalProject/Detail/Queries/Find.h>
#include <FinalProject/Detail/Queries/Sort.h>

using FinalProject::Detail::QueryParser;

bool QueryParser::Consume(QueryPtr_t& queryPtr, char input)
{
	switch (m_state)
	{
	case State::EXPECT_QUERYNAME:
		if (input == '\n')
		{
			// the query name is complete. normally we'd just use a
			// factory function to generate the query with the name,
			// but because there are only two types, we will keep it simple
			if (m_queryName == "FIND")
				queryPtr = std::make_unique<Queries::Find>();
			else if (m_queryName == "SORT")
				queryPtr = std::make_unique<Queries::Sort>();
			else
			{
				m_state = State::EXPECT_QUERYNAME;
				m_queryName.clear();
				throw make_error_code(DatabaseErrc::InvalidQuery);
			}
			m_state = State::EXPECT_QUERYDATA;
			m_queryName.clear();
			return false;
		}
		m_queryName.push_back(input);
		return false;
	case State::EXPECT_QUERYDATA:
		try
		{
			if (queryPtr->Consume(input) == true)
			{
				// the query is complete. prepare for the next one
				m_state = State::EXPECT_QUERYNAME;
				return true;
			}
		}
		catch (const std::error_code& ec)
		{
			m_state = State::EXPECT_QUERYNAME;
			throw ec;
		}
		return false;
	default:
		throw make_error_code(DatabaseErrc::InvalidState);
	}
}