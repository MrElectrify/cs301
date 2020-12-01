#include <FinalProject/DataParser.h>

using FinalProject::DataParser;

bool DataParser::Consume(Collection_t& collection, char input)
{
	switch (m_state)
	{
	case State::EXPECT_VARNAME:
		// the collection is complete
		if (input == '\n')
			return true;
		// variables must be B <= var <= W
		if (input < 'B' || input > 'W')
			throw make_error_code(DatabaseErrc::InvalidVariableName);
		m_varName = input;
		m_state = State::EXPECT_COLON;
		return false;
	case State::EXPECT_COLON:
		if (input != ':')
			throw make_error_code(DatabaseErrc::InvalidFormat);
		m_state = State::EXPECT_SPACE;
		return false;
	case State::EXPECT_SPACE:
		if (input != ' ')
			throw make_error_code(DatabaseErrc::InvalidFormat);
		m_state = State::EXPECT_VARDATA;
		return false;
	case State::EXPECT_VARDATA:
	{
		if (input == ' ' || input == '\n')
		{
			// we got a complete data point. add it
			collection.emplace_back(m_varName, m_varData);
			// clear varData
			m_varData = 0;
			m_state = State::EXPECT_VARNAME;
			return input == '\n';
		}
		// get the raw number
		char i = input - '0';
		if (i < 0 || i > 9)
			throw make_error_code(DatabaseErrc::InvalidData);
		m_varData = (m_varData * 10) + i;
		return false;
	}
	default:
		throw make_error_code(DatabaseErrc::InvalidState);
	}
}