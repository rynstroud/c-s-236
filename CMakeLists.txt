cmake_minimum_required(VERSION 3.17)
project(Lab2_Parser)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab2_Parser main.cpp Parser.h Parameter.h Predicate.h Rule.h DatalogProgram.h PlainParameter.h Expression.h Lexer.h Automaton.h MatcherAutomaton.h Token.h IDAutomaton.h Undefined.h StringAutomaton.h Comment.h EOFAutomaton.h IllegalComment.h IllegalString.h Database.h Relation.h Header.h Tuple.h Interpreter.h Node.h Graph.h)