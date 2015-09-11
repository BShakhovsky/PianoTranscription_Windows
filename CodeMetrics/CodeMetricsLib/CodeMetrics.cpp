# include "stdafx.h"
# include "..\CodeMetrics.h"
# include "CodeMetrics_pimpl.h"

using namespace std;
using Unmanaged_StatLib::CodeMetrics;

# pragma warning(push)
# pragma warning(disable:4711)	// automatic inline expansion
CodeMetrics::CodeMetrics() :
	pimpl_(new CodeMetrics_pimpl)
{}
# pragma warning(pop)

CodeMetrics::~CodeMetrics()
{
	delete pimpl_;
}

int CodeMetrics::SolutionLines(const char* directory, const bool openPSwindow) const
{
	string	command1("CD "),
		command2("(Get-ChildItem -Include *.H, *.CPP -Recurse | Select-String .)"),
		command3(".Pattern.Length");
	command1 += directory;
	command1 += ';';

	if (openPSwindow)
	{
		string command("Start PowerShell ");
		command += command1;
		command += "Get-ChildItem;";	// Dir
		command += R"%(''; 'Solution total number of non-blank lines of code:'; '';)%";
		command += '\"' + command2 + '\"' + command3;
		command += "; ''; 'You may close this PowerShell window at any time'; Pause";
		system(command.c_str());
		system("Pause");
	}

	return pimpl_->SolutionLines_impl((command1 + command2 + command3).c_str());
}