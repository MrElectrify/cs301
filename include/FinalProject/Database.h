#ifndef FINALPROJECT_DATABASE_H_
#define FINALPROJECT_DATABASE_H_

/// @file
/// Database
/// 11/30/20 20:02

// FinalProject includes
#include <FinalProject/Error.h>

// STL includes
#include <string>

namespace FinalProject
{
	/// @brief Database is a NoSQL document-based database
	class Database
	{
	public:
		/// @brief Imports data to the database
		/// @param dataPath The path to the data
		/// @throws std::error_code
		void ImportData(const std::string& dataPath);
		/// @brief Imports data to the database
		/// @param dataPath The path to the data
		/// @param ec The error code
		void ImportData(const std::string& dataPath, std::error_code& ec) noexcept;
		/// @brief Executes queries and outputs any necessary data
		/// @param queryPath The path to the query
		/// @throws std::error_code
		void ExecuteQuery(const std::string& queryPath);
		/// @brief Executes queries and outputs any necessary data
		/// @param queryPath The path to the query
		/// @param ec The error code
		void ExecuteQuery(const std::string& queryPath, std::error_code& ec) noexcept;
	};
}

#endif