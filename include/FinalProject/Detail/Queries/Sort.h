#ifndef FINALPROJECT_DETAIL_QUERIES_SORT_H_
#define FINALPROJECT_DETAIL_QUERIES_SORT_H_

/// @file
/// Sort Query
/// 12/1/20 15:18

// FinalProject includes
#include <FinalProject/Detail/Query.h>
#include <FinalProject/Database.h>

namespace FinalProject
{
	namespace Detail
	{
		namespace Queries
		{
			class Sort : public Query
			{
			public:
				void Execute(const Database& database);

				~Sort() {}
			private:
				using CollectionItVec_t = std::vector<Database::CollectionVec_t::const_iterator>;

				enum class State
				{
					EXPECT_VAR,			// expect a variable name
					EXPECT_EQUALS,		// expect the equals sign
					EXPECT_DIRECTION,	// expect a direction
					EXPECT_SEMICOLON,	// expect a semicolon
					EXPECT_NEWLINE,		// expect a newline
				};

				bool Consume(char input);

				/// @brief Prints all of the values in a collection
				/// @param collection The collection to print
				void PrintCollection(const Database::Collection_t& collection) const noexcept;

				void MergeSort(CollectionItVec_t& vec) const noexcept;
				void TopDownSplitMerge(CollectionItVec_t& b, size_t iBegin, 
					size_t iEnd, CollectionItVec_t& a) const noexcept;
				void TopDownMerge(CollectionItVec_t& a, size_t iBegin,
					size_t iMiddle, size_t iEnd, CollectionItVec_t& b) const noexcept;

				char m_sortVar;
				bool m_sortDescending = false;
				State m_state = State::EXPECT_VAR;
			};
		}
	}
}

#endif