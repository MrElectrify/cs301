#ifndef FINALPROJECT_DETAIL_QUERYPARSER_H_
#define FINALPROJECT_DETAIL_QUERYPARSER_H_

/// @file
/// QueryParser
/// 12/1/20 14:52

// FinalProject includes
#include <FinalProject/Error.h>

// STL includes
#include <memory>
#include <string>

namespace FinalProject
{
	namespace Detail
	{
		class Query;

		/// @brief QueryParser parses raw data and returns a query
		class QueryParser
		{
		public:
			using QueryPtr_t = std::unique_ptr<Query>;

			/// @brief Parses a query from an input
			/// @tparam InputIterator_t The type of input iterator
			/// @param begin The beginning iterator
			/// @param end The end iterator
			/// @throws std::error_code
			/// @return The query pointer and the last parsed data
			template<typename InputIterator_t>
			std::pair<QueryPtr_t, InputIterator_t> ParseQuery(
				InputIterator_t begin, InputIterator_t end)
			{
				QueryPtr_t query;
				while (begin != end)
				{
					if (Consume(query, *begin++) == true)
						return std::make_pair(query, begin);
				}
				throw make_error_code(DatabaseErrc::IncompleteData);
			}
		private:
			enum class State
			{
				EXPECT_QUERYNAME,	// expect the name of the query
				EXPECT_QUERYDATA,	// expect the query data
			};

			/// @brief Consumes a byte of input
			/// @param queryPtr The query pointer
			/// @param input The input character
			/// @return Whether or not the query is complete
			bool Consume(QueryPtr_t& queryPtr, char input);

			std::string m_queryName;
			State m_state = State::EXPECT_QUERYNAME;
		};
	}
}

#endif