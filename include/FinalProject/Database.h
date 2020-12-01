#ifndef FINALPROJECT_DATABASE_H_
#define FINALPROJECT_DATABASE_H_

/// @file
/// Database
/// 11/30/20 20:02

// STL includes
#include <string>

namespace FinalProject
{
	class Database
	{
	public:
		/// @brief Imports data to the database
		/// @param path The path of the data
		/// @return The success of the operation
		bool ImportData(const std::string path = "data.txt") noexcept;
	};
}

#endif