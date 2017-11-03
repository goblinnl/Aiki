// External
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

// Internal
#include "Parser/Environment.h"
#include "OperationCode.h"
#include "compiler/parser.h"
#include "OperationCodeText.h"

int main(int argc, const char *argv[])
{
	if(argc == 1) {
		printf("No file specified.\n");
		return 1;
	}
	else {
		MCString file = argv[1];
		Parser parser(file, true);

		if(parser.ParseFile()) {
			try {
				if(parser.CompileTokens()) {
					try {
						OperationCodeText tostr;
						tostr.Parse(parser.GetOpcodes());

						Environment env(parser.GetOpcodes());
						env.Execute();
					}
					catch(std::exception &e) {
						printf("Runtime error:\n%s\n", e.what());
					}
				}
				else {
					printf("Compilation failed.\n");
				}
			}
			catch(std::exception &e) {
				printf("Compilation failed.\n%s\n", e.what());
			}
		}
	}
	std::cin.get();
	return 0;
}