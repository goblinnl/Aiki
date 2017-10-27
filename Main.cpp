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
	if (argc == 1) {
		printf("No file specified.\n");
		return 1;
	} else {
		string file = argv[1];
		Parser parser(file, true);
		
		if (parser.parseFile()) {
			try {
				if (parser.compileTokens()) {
					try {
						OperationCodeText tostr;
						tostr.parse(parser.getOpcodes());
						
						Environment env(parser.getOpcodes());
						env.Execute();
					} catch (exception &e) {
						printf("Runtime error:\n%s\n", e.what());
					}
				} else {
					printf("Compilation failed.\n");
				}
			} catch (exception &e) {
				printf("Compilation failed.\n%s\n", e.what());
			}
		}
	}
	std::cin.get();
	return 0;
}