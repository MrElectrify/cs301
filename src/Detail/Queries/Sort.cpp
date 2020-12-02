#include <FinalProject/Detail/Queries/Sort.h>

#include <FinalProject/Database.h>

#include <algorithm>
#include <iostream>
#include <vector>

using FinalProject::Detail::Queries::Sort;

void Sort::Execute(const Database& database)
{
	Query::Execute(database);
	// populate a vector of collections that contain the variable
	CollectionItVec_t collectionIts;
	for (size_t i = 0; i < database.GetCollectionVec().size(); ++i)
	{
		if (database.GetIndexToContainsSet()[i].find(m_sortVar) ==
			database.GetIndexToContainsSet()[i].cend())
			continue;
		// the collection contains the variable. add the iterator to the list
		collectionIts.push_back(database.GetCollectionVec().cbegin() + i);
	}
	MergeSort(collectionIts);
	// print all of the collections
	for (const auto& collectionIt : collectionIts)
		PrintCollection(*collectionIt);
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
			m_sortDescending = true;
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

void Sort::PrintCollection(const Database::Collection_t& collection) const noexcept
{
	for (const Database::Node_t& node : collection)
		std::cout << node.first << ": " << node.second << ' ';
	std::cout << '\n';
}

void Sort::MergeSort(CollectionItVec_t& vec) const noexcept
{
	CollectionItVec_t workVec = vec;
	TopDownSplitMerge(workVec, 0, vec.size(), vec);
}

void Sort::TopDownSplitMerge(CollectionItVec_t& b, size_t iBegin,
	size_t iEnd, CollectionItVec_t& a) const noexcept
{
	if (iEnd - iBegin <= 1)
		return;
	size_t iMiddle = (iBegin + iEnd) / 2;
	TopDownSplitMerge(a, iBegin, iMiddle, b);
	TopDownSplitMerge(a, iMiddle, iEnd, b);
	TopDownMerge(b, iBegin, iMiddle, iEnd, a);
}

void Sort::TopDownMerge(CollectionItVec_t& a, size_t iBegin,
	size_t iMiddle, size_t iEnd, CollectionItVec_t& b) const noexcept
{
	size_t i = iBegin, j = iMiddle;
	for (size_t k = iBegin; k < iEnd; ++k)
	{
		if (i < iMiddle && (j >= iEnd ||
			((m_sortDescending) ?
				a[i]->at(m_sortVar) > a[j]->at(m_sortVar) :
				a[i]->at(m_sortVar) <= a[j]->at(m_sortVar))))
			b[k] = a[i++];
		else
			b[k] = a[j++];
	}
}