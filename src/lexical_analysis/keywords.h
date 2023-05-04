//
// Created by shecannotsee on 23-5-4.
//

#ifndef SHE_COMPILE_LAB_SRC_LEXICAL_ANALYSIS_KEYWORDS_H_
#define SHE_COMPILE_LAB_SRC_LEXICAL_ANALYSIS_KEYWORDS_H_

#include <string>
#include <unordered_map>

enum class KEYWORDS {
  BOOL,   CHAR,     DOUBLE, FLOAT,  INT,
  CONST,  UNSIGNED,
  IF,     ELSE,     SWITCH, CASE,   DEFAULT,
  DO,     WHILE,    FOR,    GOTO,
  BREAK,  CONTINUE,
  RETURN, STRUCT,
  STATIC,
  VOID,   ENUM
};
static std::unordered_map<std::string,KEYWORDS> keywords = {
    {"bool",    KEYWORDS::BOOL},
    {"char",    KEYWORDS::CHAR},
    {"double",  KEYWORDS::DOUBLE},
    {"float",   KEYWORDS::FLOAT},
    {"int",     KEYWORDS::INT},
    {"const",   KEYWORDS::CONST},
    {"unsigned",KEYWORDS::UNSIGNED},
    {"if",      KEYWORDS::IF},
    {"else",    KEYWORDS::ELSE},
    {"switch",  KEYWORDS::SWITCH},
    {"case",    KEYWORDS::CASE},
    {"default", KEYWORDS::DEFAULT},
    {"do",      KEYWORDS::DO},
    {"while",   KEYWORDS::WHILE},
    {"for",     KEYWORDS::FOR},
    {"goto",    KEYWORDS::GOTO},
    {"break",   KEYWORDS::BREAK},
    {"continue",KEYWORDS::CONTINUE},
    {"return",  KEYWORDS::RETURN},
    {"struct",  KEYWORDS::STRUCT},
    {"static",  KEYWORDS::STATIC},
    {"void",    KEYWORDS::VOID},
    {"enum",    KEYWORDS::ENUM},
};


#endif //SHE_COMPILE_LAB_SRC_LEXICAL_ANALYSIS_KEYWORDS_H_
