#include <FinalProject/Detail/Queries/Sort.h>

#include <FinalProject/Database.h>

#include <iostream>
#include <vector>

using FinalProject::Detail::Queries::Sort;

void Sort::Execute(const Database& database)
{
	// populate a vector of collections that contain the variable
	std::vector<Database::CollectionVec_t::const_iterator> collectionIts;
	for (size_t i = 0; i < database.GetCollectionVec().size(); ++i)
	{
		if (database.GetIndexToContainsSet()[i].contains(m_sortVar) == false)
			continue;
		// the collection contains the variable. add the iterator to the list
		collectionIts.push_back(database.GetCollectionVec().cbegin() + i);
	}
	
}

bool Sort::Consume(char input)
{
	switch (m_state)
	{
	case State::EXPECT_VAR:
		if (input < 'A' || input > 'W')
			throw make_error_code(DatabaseErrc::InvalidVariableName);
		m_sortVar = input;
		m_state = State::EXPECT_EQUALS;
		return false;
	case State::EXPECT_EQUALS:
		switch (input)
		{
		case ' ':
			// ignore spaces
			return false;
		case '=':
			m_state = State::EXPECT_DIRECTION;
			return false;
		default:
			throw make_error_code(DatabaseErrc::InvalidFormat);
		}
	case State::EXPECT_DIRECTION:
		switch (input)
		{
		case ' ':
			// ignore spaces
			return false;
		case '-':
			// it is negative, wait for the 1
			m_isNegative = true;
			return false;
		case '1':
			m_state = State::EXPECT_SEMICOLON;
			return false;
		default:
			throw make_error_code(DatabaseErrc::InvalidDirection);
		}
	case State::EXPECT_SEMICOLON:
		switch (input)
		{
		case ' ':
			// ignore spaces
			return false;
		case ';':
			m_state = State::EXPECT_NEWLINE;
			return false;
		default:
			throw make_error_code(DatabaseErrc::ExpectedSemicolon);
		}
	case State::EXPECT_NEWLINE:
		switch (input)
		{
		case ' ':
			// ignore spaces
			return false;
		case '\n':
			return true;
		default:
			throw make_error_code(DatabaseErrc::UnexpectedChar);
		}
	default:
		throw make_error_code(DatabaseErrc::InvalidState);
	}
}