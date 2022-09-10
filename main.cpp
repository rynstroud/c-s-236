#include <fstream>
#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"

int main(int argc, const char * argv[])
{
    /* Writing to console instead of file */

    //Checking for correct number of arguments
    if (argc < 2)
    {
        std::cerr << "Please provide the name of input file, thanks\n";
        return 1;
    }

    //Checking if input file opens
    std::ifstream in(argv[1]);
    if (!in)
    {
        std::cerr << "Failure to open input file: " << argv[1] << std::endl;
        return 2;
    }

    std::string lexStr = "";
    if(in) {
        std::ostringstream os;
        os << in.rdbuf(); // reading data
        lexStr = os.str();
    }

    Lexer lexer;
    lexer.run(lexStr);

    std::vector<Token> tokens = lexer.getTokens();

    try
    {
        Parser data(tokens);
        DatalogProgram* d;
        d = data.datalogProgram();
        //std::cout << data;
        Interpreter interpreter(d);
        interpreter.run();
        //std::cout << interpreter;
        std::cout << interpreter.evaluateAllRules();
        std::cout << interpreter.evaluateAllQueries();
    } catch (std::string& errorString)
    {
        std::cout << errorString;
    }

    return 0;
}
