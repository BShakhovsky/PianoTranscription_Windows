# include "stdafx.h"
# include "..\..\Model\MidiPlayback_Lib\MidiPlayback.h"
# include "..\..\CodeMetrics\CodeMetrics.h"

using namespace std;
using Model::MidiPlayback::MidiPlayback;

# ifdef CODE_METRICS
#	define SOLUTION_LINES																						{\
		cout << "Solution total non-blank lines of code: "	<< CODE_METRICS.SolutionLines("../..")		<< '\n'	\
				<< "\t Model: "								<< CODE_METRICS.SolutionLines("../../Model")		\
				<< "   Interface: "							<< CODE_METRICS.SolutionLines("../../Interface")	\
				<< "   CodeMetrics: "						<< CODE_METRICS.SolutionLines("../../CodeMetrics")	\
				<< "   Unit tests: "						<< CODE_METRICS.SolutionLines("../gTests")			\
			<< endl;																							}
# else
#	define SOLUTION_LINES
# endif

int main()
{
# ifdef _DEBUG
	SOLUTION_LINES;
# endif
	try
	{
		MidiPlayback::Execute("../../../Test.mid");
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
		system("Pause");
		throw;
	}

	return NULL;
}