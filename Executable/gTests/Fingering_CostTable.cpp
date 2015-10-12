# include "stdafx.h"
# include "Fingering_CostTable.h"

using gTest::CostTable;

void CostTable::CheckTableCells(const FUNC_COST_TABLE func, const int16_t note1, const int16_t note2)
{
	func(0, static_cast<size_t>(note2 - note1 + 5), '\1', '\2', note1, note2);
	func(1, static_cast<size_t>(note2 - note1 + 5), '\1', '\3', note1, note2);
	func(2, static_cast<size_t>(note2 - note1 + 5), '\1', '\4', note1, note2);
	func(3, static_cast<size_t>(note2 - note1 + 5), '\1', '\5', note1, note2);
	func(4, static_cast<size_t>(note2 - note1 + 5), '\2', '\3', note1, note2);
	func(5, static_cast<size_t>(note2 - note1 + 5), '\2', '\4', note1, note2);
	func(6, static_cast<size_t>(note2 - note1 + 5), '\2', '\5', note1, note2);
	func(7, static_cast<size_t>(note2 - note1 + 5), '\3', '\4', note1, note2);
	func(8, static_cast<size_t>(note2 - note1 + 5), '\3', '\5', note1, note2);
	func(9, static_cast<size_t>(note2 - note1 + 5), '\4', '\5', note1, note2);

	func(0, static_cast<size_t>(note2 - note1 + 5), '\2', '\1', note2, note1);
	func(1, static_cast<size_t>(note2 - note1 + 5), '\3', '\1', note2, note1);
	func(2, static_cast<size_t>(note2 - note1 + 5), '\4', '\1', note2, note1);
	func(3, static_cast<size_t>(note2 - note1 + 5), '\5', '\1', note2, note1);
	func(4, static_cast<size_t>(note2 - note1 + 5), '\3', '\2', note2, note1);
	func(5, static_cast<size_t>(note2 - note1 + 5), '\4', '\2', note2, note1);
	func(6, static_cast<size_t>(note2 - note1 + 5), '\5', '\2', note2, note1);
	func(7, static_cast<size_t>(note2 - note1 + 5), '\4', '\3', note2, note1);
	func(8, static_cast<size_t>(note2 - note1 + 5), '\5', '\3', note2, note1);
	func(9, static_cast<size_t>(note2 - note1 + 5), '\5', '\4', note2, note1);
}

# pragma warning(push)
# ifdef NDEBUG
#	pragma warning(disable:4711)	// automatic inline expansion
# endif
void CostTable::CheckAllTableCells(const FUNC_COST_TABLE func, const int16_t randNote)
{
# ifdef _DEBUG
	ASSERT_DEBUG_DEATH(func(0, 0, 1, 1 - rand() % (CHAR_MAX - 2) - 2, randNote, randNote), "WRONG FINGER");
	ASSERT_DEBUG_DEATH(func(0, 0, 2,							NULL, randNote, randNote), "WRONG FINGER");
	ASSERT_DEBUG_DEATH(func(0, 0, 3,								6, randNote, randNote), "WRONG FINGER");
	ASSERT_DEBUG_DEATH(func(0, 0, 1 - rand() % (CHAR_MAX - 2) - 2,	3, randNote, randNote), "WRONG FINGER");
	ASSERT_DEBUG_DEATH(func(0, 0, NULL,								4, randNote, randNote), "WRONG FINGER");
	ASSERT_DEBUG_DEATH(func(0, 0, rand() % (CHAR_MAX - 6) + 6,		5, randNote, randNote), "WRONG FINGER");
# endif
	for (int16_t i(-5); i <= 15; ++i) CheckTableCells(func, randNote, randNote + i);
}
# pragma warning(pop)