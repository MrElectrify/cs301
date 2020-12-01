#ifndef FINALPROJECT_DATABASE_H_
#define FINALPROJECT_DATABASE_H_

/// @file
/// Database
/// 11/30/20 20:02

// STL includes
#include <string>

namespace FinalProject
{
	/// @brief Database is a NoSQL document-based database
	class Database
	{
	public:
		/// @brief Imports data to the database
		/// @param dataPath The path of the data
		/// @return The success of the operation
		bool ImportData(const std::string& dataPath) noexcept;
		/// @brief Executes queries and outputs any necessary data
		/// @param queryPath The path of the query
		/// @return The success of the operation
		bool ExecuteQuery(const std::string& queryPath) noexcept;
	};
}

#endif