#ifndef FINALPROJECT_ERROR_H_
#define FINALPROJECT_ERROR_H_

/// @file
/// Error
/// 11/30/20 20:18

// STL includes
#include <string>
#include <system_error>
#include <type_traits>

namespace FinalProject
{
	enum class DatabaseErrc
	{
		FileNotFound = 1,		// the file was not found
		InvalidVariableName,	// the variable name was invalid
		InvalidFormat,			// the format was invalid
		InvalidData,			// the data was invalid
		InvalidState,			// the state was invalid (data corruption)
		IncompleteData,			// the data was incomplete
	};
	std::error_code make_error_code(DatabaseErrc errc);
	class DatabaseErrorCategory :
		public std::error_category
	{
	public:
		const char* name() const noexcept override;
		std::string message(int ev) const override;
	};
}

namespace std
{
	template<>
	struct is_error_code_enum<FinalProject::DatabaseErrc> : std::true_type {};
}

#endif