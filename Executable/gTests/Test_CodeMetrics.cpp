# include "stdafx.h"
# include "CurrentFileName.h"

# include "..\..\CodeMetrics\CodeMetricsLib\CodeMetrics_pimpl.h"
# include "..\..\CodeMetrics\PowerShell\PowerShell_CLR.h"
;
using std::string;
using boost::lexical_cast;
using testing::Test;
using ManagedCLR::PowerShell_CLR;
using Unmanaged_StatLib::CodeMetrics_pimpl;
;
class CodeMetrics_F : public Test
{
public:
	string command;
	int line;
	virtual void SetUp() override final;
	virtual void TearDown() override final {}
};
;
# define COMMENT << line << " non-blank lines in the current file"
;
TEST_F(CodeMetrics_F, CodeMetrics_Lines)
{
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	ASSERT_EQ(line, CodeMetrics_pimpl().SolutionLines_impl(command.c_str())) COMMENT;
}
;
TEST_F(CodeMetrics_F, PwrShell_Execute)
{
	PowerShell_CLR powerShell;
	powerShell.AppendScript("CD ..");
	powerShell.AppendScript(command.c_str());
	auto result(powerShell.Execute());
	;
	ASSERT_STREQ(lexical_cast<string>(line).c_str(), result.at(0).c_str()) COMMENT;
}
;
void CodeMetrics_F::SetUp()
{
	command = "(Get-ChildItem -Include ";
	command += CURRENT_FILE_NAME;
	command += " -Recurse | Select-String .).Pattern.Length";
	;
	line = __LINE__;
}