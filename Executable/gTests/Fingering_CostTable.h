# pragma once

namespace gTest
{
	class CostTable abstract : private boost::noncopyable
	{
		CostTable() = delete;
		~CostTable() = delete;
	public:
		typedef boost::function<void(size_t, size_t, char, char, int16_t, int16_t)> FUNC_COST_TABLE;
		static void CheckTableCells(FUNC_COST_TABLE, int16_t note1, int16_t note2);
		static void CheckAllTableCells(FUNC_COST_TABLE, int16_t randNote);
	};
}