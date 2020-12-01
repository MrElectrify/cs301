#ifndef FINALPROJECT_DATABASE_H_
#define FINALPROJECT_DATABASE_H_

/// @file
/// Database
/// 11/30/20 20:02

// FinalProject includes
#include <FinalProject/Detail/DataParser.h>
#include <FinalProject/Detail/QueryParser.h>

// STL includes
#include <string>
#include <system_error>
#include <unordered_map>
#include <utility>
#include <vector>

namespace FinalProject
{
	namespace impl
	{
		struct hash_pair {
			template <class T1, class T2>
			size_t operator()(const std::pair<T1, T2>& p) const
			{
				auto hash1 = std::hash<T1>{}(p.first);
				auto hash2 = std::hash<T2>{}(p.second);
				return hash1 ^ hash2;
			}
		};
	}

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
	private:
		using Node_t = Detail::DataParser::Node_t;
		using Collection_t = Detail::DataParser::Collection_t;

		Detail::DataParser m_dataParser;
		Detail::QueryParser m_queryParser;

		std::vector<Collection_t> m_collections;
		std::unordered_multimap<Node_t, size_t, impl::hash_pair> m_nodeToCollectionIndexMap;

		friend class Query;
	};
}

#endif