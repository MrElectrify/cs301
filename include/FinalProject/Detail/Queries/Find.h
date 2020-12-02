#ifndef FINALPROJECT_DETAIL_QUERIES_FIND_H_
#define FINALPROJECT_DETAIL_QUERIES_FIND_H_

/// @file
/// Find Query
/// 12/1/20 15:18

// FinalProject includes
#include <FinalProject/Detail/Query.h>
#include <FinalProject/Database.h>

// STL includes
#include <cstdint>
#include <unordered_set>
#include <vector>

namespace FinalProject
{
	namespace Detail
	{
		namespace Queries
		{
			class Find : public Query
			{
			public:
				void Execute(const Database& database);

				~Find() {}
			private:
				enum class State
				{
					EXPECT_CONDITION_VAR,		// expect the variable for a condition
					EXPECT_CONDITION_OP,		// expect the operator for a condition
					EXPECT_CONDITION_LIT,		// expect the literal for a condition
					EXPECT_PROJECTION_VAR,		// expect a projection variable name
					EXPECT_NEWLINE_Y,			// expect a newline after a 'Y' condition
				};

				class Condition
				{
				public:
					enum class Operator
					{
						EQUALS,
						LESS_THAN,
						GREATER_THAN
					};

					void SetVar(char var) noexcept { m_var = var; }
					char GetVar() const noexcept { return m_var; }

					void SetOperator(Operator op) noexcept { m_op = op; }
					Operator GetOperator() const noexcept { return m_op; }
					
					void SetLiteral(uint32_t lit) noexcept { m_lit = lit; }
					uint32_t GetLiteral() const noexcept { return m_lit; }

					/// @brief Evaluates a collection to see if it meets the conditions
					/// @param collection The collection to test
					/// @return Whether or not the collection meets the conditions
					bool IsMet(const Database::Collection_t& collection) const noexcept;
				private:
					char m_var;
					Operator m_op;
					uint32_t m_lit;
				};

				bool Consume(char input);

				/// @brief Prints a collection with projection constraints
				/// @param collection The collection to print
				void PrintCollection(const Database::Collection_t& collection) const noexcept;

				bool m_projectAllVariables = false;
				char m_tmpVar = '\0';
				State m_state = State::EXPECT_CONDITION_VAR;
				Condition m_tmpCondition;
				std::vector<Condition> m_conditions;
				std::unordered_set<char> m_projectionVariables;
			};
		}
	}
}

#endif