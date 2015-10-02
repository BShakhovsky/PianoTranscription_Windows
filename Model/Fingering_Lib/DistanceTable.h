# pragma once

namespace Model
{
	namespace Fingering
	{
		class DistanceTable abstract : private boost::noncopyable
		{
			static const char distanceMatrix_[10][6];
		public:
			static char MinPrac(char finger1, char finger2)
			{
				return distanceMatrix_[TableRow(finger1, finger2)][0];
			}
			static char MinComf(char finger1, char finger2)
			{
				return distanceMatrix_[TableRow(finger1, finger2)][1];
			}
			static char MinRel(char finger1, char finger2)
			{
				return distanceMatrix_[TableRow(finger1, finger2)][2];
			}
			static char MaxRel(char finger1, char finger2)
			{
				return distanceMatrix_[TableRow(finger1, finger2)][3];
			}
			static char MaxComf(char finger1, char finger2)
			{
				return distanceMatrix_[TableRow(finger1, finger2)][4];
			}
			static char MaxPrac(char finger1, char finger2)
			{
				return distanceMatrix_[TableRow(finger1, finger2)][5];
			}
		private:
			static int TableRow(char finger1, char finger2);

			DistanceTable() = delete;
			~DistanceTable() = delete;
		};
	}
}