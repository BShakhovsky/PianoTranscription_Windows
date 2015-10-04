# pragma once

namespace gTest
{
	class CostTable abstract : private boost::noncopyable
	{
		CostTable() = delete;
		~CostTable() = delete;
	public:
		typedef void(*funcCostTable)(size_t, size_t, char, char, int16_t, int16_t);
		static void CheckTableCells(funcCostTable, int16_t note1, int16_t note2);
		static void CheckAllTableCells(funcCostTable, int16_t randNote);
	};
}