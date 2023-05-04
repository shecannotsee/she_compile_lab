//
// Created by shecannotsee on 23-5-4.
//

#ifndef SHE_COMPILE_LAB_SRC_LEXICAL_ANALYSIS_SYMBOL_TABLE_H_
#define SHE_COMPILE_LAB_SRC_LEXICAL_ANALYSIS_SYMBOL_TABLE_H_

#include <string>
#include <unordered_map>

enum class SYMBOL_TABLE {
  SEMICOLON
};
static std::unordered_map<std::string,KEYWORDS> keywords = {
    {";",    SYMBOL_TABLE::SEMICOLON},

};

#endif //SHE_COMPILE_LAB_SRC_LEXICAL_ANALYSIS_SYMBOL_TABLE_H_
