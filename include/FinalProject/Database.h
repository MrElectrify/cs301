#ifndef FINALPROJECT_DATABASE_H_
#define FINALPROJECT_DATABASE_H_

/// @file
/// Database
/// 11/30/20 20:02

// FinalProject includes
#include <FinalProject/Detail/DataParser.h>
#include <FinalProject/Detail/QueryParser.h>

// STL includes
#include <istream>
#include <string>
#include <system_error>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace FinalProject
{
	namespace impl
	{
		struct hash_pair {
			template <class T1>
			size_t operator()(const std::pair<const char, T1>& p) const
			{
				auto hash = std::hash<T1>{}(p.second);
				return hash ^ p.first;
			}
		};
	}

	/// @brief Database is a NoSQL document-based database
	class Database
	{
	public:
		using Collection_t = Detail::DataParser::Collection_t;
		using Node_t = Collection_t::value_type;
		using CollectionVec_t = std::vector<Collection_t>;
		using NodeToCollectionIndexMap_t = std::unordered_multimap<Node_t, size_t, impl::hash_pair>;
		using IndexToContainsSet_t = std::vector<std::unordered_set<char>>;
		
		/// @brief Imports data to the database
		/// @param dataPath The path to the data
		/// @throws std::error_code
		void ImportData(const std::string& dataPath);
		/// @brief Imports data to the database
		/// @param dataPath The path to the data
		/// @param ec The error code
		void ImportData(const std::string& dataPath, std::error_code& ec) noexcept;
		/// @brief Imports queries to the database
		/// @param queryPath The path to the query
		/// @throws std::error_code
		void ImportQueries(const std::string& queryPath);
		/// @brief Imports queries to the database
		/// @param queryPath The path to the query
		/// @param ec The error code
		void ImportQueries(const std::string& queryPath, std::error_code& ec) noexcept;
		/// @brief Executes imported queries and clears them
		/// @throws std::error_code
		void ExecuteQueries();
		/// @brief Executes imported queries and clears them
		void ExecuteQueries(std::error_code& ec) noexcept;

		const CollectionVec_t& GetCollectionVec() const noexcept { return m_collections; }
		const NodeToCollectionIndexMap_t& GetNodeToCollectionIndexMap() const noexcept { return m_nodeToCollectionIndexMap; }
		const IndexToContainsSet_t& GetIndexToContainsSet() const noexcept { return m_indexToContainsSet; }
	private:
		/// @brief Getline, but cross-platform line endings
		/// @param is The input stream
		/// @param t The output string
		/// @return The input stream
		static std::istream& GetLineCP(std::istream& is, std::string& t) noexcept;

		Detail::DataParser m_dataParser;
		Detail::QueryParser m_queryParser;

		CollectionVec_t m_collections;
		NodeToCollectionIndexMap_t m_nodeToCollectionIndexMap;
		IndexToContainsSet_t m_indexToContainsSet;
		std::vector<Detail::QueryParser::QueryPtr_t> m_queries;
	};
}

#endif