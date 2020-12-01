#ifndef FINALPROJECT_DETAIL_QUERY_H_
#define FINALPROJECT_DETAIL_QUERY_H_

/// @file
/// Query
/// 12/1/20 15:04

// STL includes
#include <cstdint>

namespace FinalProject
{
	class Database;

	namespace Detail
	{
		class QueryParser;

		/// @brief Query is an abstract query
		class Query
		{
		public:
			/// @brief Executes the query
			/// @param database The database to use
			/// @throws std::error_code
			virtual void Execute(const Database& database) = 0;

			virtual ~Query() {}
		protected:
			static uint32_t s_queryNum;
		private:
			/// @brief Consumes a byte of input
			/// @param input The input byte
			/// @throws std::error_code
			/// @return Whether or not the query is complete
			virtual bool Consume(char input) = 0;

			friend class QueryParser;
		};
	}
}

#endif