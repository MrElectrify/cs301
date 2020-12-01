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
	default:
		return "Unknown error";
	}
}

std::error_code FinalProject::make_error_code(DatabaseErrc errc)
{
	static DatabaseErrorCategory s_category;
	return { static_cast<int>(errc), s_category };
}