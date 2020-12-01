#ifndef FINALPROJECT_DETAIL_DATAPARSER_H_
#define FINALPROJECT_DETAIL_DATAPARSER_H_

/// @file
/// DataParser
/// 12/1/20 13:41

// FinalProject includes
#include <FinalProject/Error.h>

// STL includes
#include <cstdint>
#include <system_error>
#include <utility>
#include <vector>

namespace FinalProject
{
	namespace Detail
	{
		/// @brief DataParser parses input data and returns database objects
		class DataParser
		{
		public:
			using Node_t = std::pair<char, uint32_t>;
			using Collection_t = std::vector<Node_t>;

			/// @brief Parses a collection from an input
			/// @tparam InputIterator_t The type of input iterator
			/// @param begin The beginning iterator
			/// @param end The end iterator
			/// @throws std::error_code
			/// @return The resulting collection, and the last parsed data
			template<typename InputIterator_t>
			std::pair<Collection_t, InputIterator_t> ParseCollection(
				InputIterator_t begin, InputIterator_t end)
			{
				// A stores the ID
				Collection_t collection = { { 'A', m_lastId++ } };
				while (begin != end)
				{
					if (Consume(collection, *begin++) == true)
						return std::make_pair(std::move(collection), begin);
				}
				throw make_error_code(DatabaseErrc::IncompleteData);
			}

			/// @brief Parses a collection from an input
			/// @tparam InputIterator_t The type of input iterator
			/// @param begin The beginning iterator
			/// @param end The end iterator
			/// @throws std::error_code
			/// @return The resulting collection, and the last parsed data
			template<typename InputIterator_t>
			std::pair<Collection_t, InputIterator_t> ParseCollection(
				InputIterator_t begin, InputIterator_t end, std::error_code& ec)
			{
				try
				{
					return ParseCollection(begin, end);
				}
				catch (const std::error_code& e)
				{
					ec = e;
				}
			}
		private:
			enum class State
			{
				EXPECT_VARNAME,	// expect a variable name
				EXPECT_COLON,	// expect a colon
				EXPECT_SPACE,	// expect a space
				EXPECT_VARDATA,	// expect variable data
			};

			/// @brief Consumes a single byte of input
			/// @param collection The output collection
			/// @param input The input byte
			/// @throws std::error_code
			/// @return Whether or not the collection is complete
			bool Consume(Collection_t& collection, char input);

			uint32_t m_lastId = 1;
			State m_state = State::EXPECT_VARNAME;
			char m_varName = 0;
			uint32_t m_varData = 0;
		};
	}
}

#endif