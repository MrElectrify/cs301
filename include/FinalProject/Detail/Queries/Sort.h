#ifndef FINALPROJECT_DETAIL_QUERIES_SORT_H_
#define FINALPROJECT_DETAIL_QUERIES_SORT_H_

/// @file
/// Sort Query
/// 12/1/20 15:18

// FinalProject includes
#include <FinalProject/Detail/Query.h>

namespace FinalProject
{
	namespace Detail
	{
		namespace Queries
		{
			class Sort : public Query
			{
			public:
				void Execute(const Database& database);

				~Sort() {}
			private:
				enum class State
				{

				};

				bool Consume(char input);
			};
		}
	}
}

#endif