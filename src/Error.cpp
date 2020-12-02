#include <FinalProject/Error.h>

using FinalProject::DatabaseErrorCategory;

const char* DatabaseErrorCategory::name() const noexcept
{
	return "database";
}

std::string DatabaseErrorCategory::message(int ev) const
{
	switch (static_cast<DatabaseErrc>(ev))
	{
	case DatabaseErrc::FileNotFound:
		return "The file was not found";
	case DatabaseErrc::InvalidVariableName:
		return "The variable name was invalid";
	case DatabaseErrc::InvalidFormat:
		return "The format was invalid";
	case DatabaseErrc::InvalidData:
		return "The data was invalid";
	case DatabaseErrc::InvalidState:
		return "The state was invalid. Possible data corruption";
	case DatabaseErrc::InvalidQuery:
		return "The query name was invalid";
	case DatabaseErrc::InvalidOperator:
		return "The query contained an invalid operator";
	case DatabaseErrc::InvalidDirection:
		return "The sort had in invalid direction";
	case DatabaseErrc::IncompleteData:
		return "The input data was incomplete";
	case DatabaseErrc::ExpectedSemicolon:
		return "Expected ;";
	case DatabaseErrc::UnexpectedChar:
		return "Unexpected character";
	default:
		return "Unknown error";
	}
}

std::error_code FinalProject::make_error_code(DatabaseErrc errc)
{
	static DatabaseErrorCategory s_category;
	return { static_cast<int>(errc), s_category };
}