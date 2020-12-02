#include <FinalProject/Detail/Queries/Find.h>

#include <FinalProject/Database.h>

#include <iostream>

using FinalProject::Detail::Queries::Find;

void Find::Execute(const Database& database)
{
	Query::Execute(database);
	if (m_conditions.empty() == true)
	{
		// print everything, there are no conditions
		for (const Database::Collection_t& collection : database.GetCollectionVec())
			PrintCollection(collection);
		return;
	}
	// search for the smallest list of equivalent conditions
	auto bestCondIt = m_conditions.cend();
	size_t bestSize = SIZE_MAX;
	for (auto condIt = m_conditions.cbegin(); 
		condIt != m_conditions.cend(); ++condIt)
	{
		if (condIt->GetOperator() == Condition::Operator::EQUALS)
		{
			const size_t count = database.GetNodeToCollectionIndexMap().count(
				{ condIt->GetVar(), condIt->GetLiteral() });
			if (count < bestSize)
			{
				bestCondIt = condIt;
				bestSize = count;
			}
		}
	}
	if (bestCondIt != m_conditions.cend())
	{
		// we can optimize by searching only a smaller list
		auto range = database.GetNodeToCollectionIndexMap().equal_range(
			{ bestCondIt->GetVar(), bestCondIt->GetLiteral() });
		for (auto it = range.first; it != range.second; ++it)
		{
			bool isMet = true;
			for (auto condIt2 = m_conditions.cbegin();
				condIt2 != m_conditions.cend(); ++condIt2)
			{
				// we already know this is true
				if (condIt2 == bestCondIt)
					continue;
				if (condIt2->IsMet(database.GetCollectionVec()[it->second]) == false)
				{
					isMet = false;
					break;
				}
			}
			if (isMet == true)
				PrintCollection(database.GetCollectionVec()[it->second]);
		}
		return;
	}
	// no optimizations. go through every single collection and check
	for (const Database::Collection_t& collection : database.GetCollectionVec())
	{
		bool isMet = true;
		for (const Condition& condition : m_conditions)
		{
			// check all conditions
			if (condition.IsMet(collection) == false)
			{
				isMet = false;
				break;
			}
		}
		if (isMet == true)
			PrintCollection(collection);
	}
}

bool Find::Consume(char input)
{
	switch (m_state)
	{
	case State::EXPECT_CONDITION_VAR:
		if (input < 'A' || input > 'W')
		{
			if (input == 'Y')
			{
				// no condition. skip to projections
				m_state = State::EXPECT_NEWLINE_Y;
				return false;
			}
			else if (input == 'Z')
			{
				// we project all variables
				m_projectAllVariables = true;
				m_state = State::EXPECT_PROJECTION_VAR;
				return false;
			}
			throw make_error_code(DatabaseErrc::InvalidVariableName);
		}
		m_tmpVar = input;
		m_state = State::EXPECT_CONDITION_OP;
		return false;
	case State::EXPECT_CONDITION_OP:
		// at this stage, we can either be parsing a
		// comparison, or projection.
		switch (input)
		{
		case ' ':
			// ignore spaces
			return false;
		case '=':
		case '<':
		case '>':
			m_tmpCondition.SetVar(m_tmpVar);
			switch (input)
			{
			case '=':
				m_tmpCondition.SetOperator(Condition::Operator::EQUALS);
				break;
			case '<':
				m_tmpCondition.SetOperator(Condition::Operator::LESS_THAN);
				break;
			case '>':
				m_tmpCondition.SetOperator(Condition::Operator::GREATER_THAN);
				break;
			}
			m_state = State::EXPECT_CONDITION_LIT;
			return false;
		case ';':
			// the end of a query was reached, and this was
			// a projection. add the variable to the list.
			// the query is parsed. wait for the newline
			m_projectionVariables.emplace(m_tmpVar);
			m_state = State::EXPECT_NEWLINE_END;
			return false;
		default:
			if (input < 'A' || input > 'W')
				throw make_error_code(DatabaseErrc::InvalidVariableName);
			// this is a variable, and therefore we are parsing
			// a projection. add the variable to the list
			m_projectionVariables.emplace(m_tmpVar);
			m_projectionVariables.emplace(input);
			m_state = State::EXPECT_PROJECTION_VAR;
			return false;
		}
	case State::EXPECT_CONDITION_LIT:
	{
		// ignore spaces
		if (input == ' ')
			return false;
		if (input == '\n')
		{
			// the end of the condition has been reached.
			// add it to the condition list and clear out
			// the literal for future parsing
			m_conditions.push_back(m_tmpCondition);
			m_tmpCondition.SetLiteral(0);
			m_state = State::EXPECT_CONDITION_VAR;
			return false;
		}
		char i = input - '0';
		if (i < 0 || i > 9)
			throw make_error_code(DatabaseErrc::InvalidData);
		m_tmpCondition.SetLiteral(m_tmpCondition.GetLiteral() * 10 + i);
		return false;
	}
	case State::EXPECT_PROJECTION_VAR:
		switch (input)
		{
		case ' ':
			// ignore spaces
			return false;
		case ';':
			// we reached the end of the query. wait for the newline
			m_state = State::EXPECT_NEWLINE_END;
			return false;
		default:
			if (input < 'A' || input > 'W')
			{
				if (input == 'Z')
				{
					// we project all variables
					m_projectAllVariables = true;
					return false;
				}
				throw make_error_code(DatabaseErrc::InvalidVariableName);
			}
			m_projectionVariables.emplace(input);
			return false;
		}
	case State::EXPECT_NEWLINE_Y:
		switch (input)
		{
		case ' ':
			return false;
		case '\n':
			m_state = State::EXPECT_PROJECTION_VAR;
			return false;
		default:
			throw make_error_code(DatabaseErrc::UnexpectedChar);
		}
	case State::EXPECT_NEWLINE_END:
		switch (input)
		{
		case ' ':
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

void Find::PrintCollection(const Database::Collection_t& collection) const noexcept
{
	size_t numPrinted = 0;
	for (const Database::Node_t& node : collection)
	{
		if (m_projectAllVariables == true ||
			m_projectionVariables.find(node.first) != m_projectionVariables.cend())
		{
			++numPrinted;
			std::cout << node.first << ": " << node.second << ' ';
		}
	}
	// if it doesn't have any of the fields, print A
	if (numPrinted == 0)
		std::cout << "A: " << collection.at('A') << ' ';
	std::cout << '\n';
}

bool Find::Condition::IsMet(const Database::Collection_t& collection) const noexcept
{
	for (const Database::Node_t& node : collection)
	{
		if (m_var == node.first)
		{
			switch (m_op)
			{
			case Operator::EQUALS:
				return node.second == m_lit;
			case Operator::GREATER_THAN:
				return node.second > m_lit;
			case Operator::LESS_THAN:
				return node.second < m_lit;
			}
		}
	}
	return false;
}