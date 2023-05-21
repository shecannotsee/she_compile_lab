//
// Created by shecannotsee on 23-5-22.
//

#ifndef SHE_COMPILE_LAB_M1_LLVM_LEXICAL_ANALYSIS_H
#define SHE_COMPILE_LAB_M1_LLVM_LEXICAL_ANALYSIS_H

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "make_unique.h"

namespace m1_llvm_lexical_analysis {

namespace lexical_analysis {
// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token {
  tok_eof = -1,// 文件结尾的枚举

  // commands
  tok_def = -2,
  tok_extern = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5
};

static std::string IdentifierStr; // Filled in if tok_identifier
static double NumVal;             // Filled in if tok_number

/// gettok - Return the next token from standard input.
static int gettok() {
  static int LastChar = ' ';

  // 跳过空格
  while (isspace(LastChar))
    LastChar = getchar();
  // 判断是否是字母
  if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    IdentifierStr = LastChar;
    // 判断是否为字母或数字
    while (isalnum((LastChar = getchar())))
      IdentifierStr += LastChar;

    if (IdentifierStr == "def")
      return tok_def;
    if (IdentifierStr == "extern")
      return tok_extern;
    return tok_identifier;
  }
  // 判断是否是数字
  if (isdigit(LastChar) || LastChar == '.') { // Number: [0-9.]+
    std::string NumStr;
    do {
      NumStr += LastChar;
      LastChar = getchar();
    } while (isdigit(LastChar) || LastChar == '.');

    NumVal = strtod(NumStr.c_str(), nullptr);
    return tok_number;
  }
  // 判断是否是'#'
  if (LastChar == '#') {
    // Comment until end of line.
    do
      LastChar = getchar();
    while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

    if (LastChar != EOF)
      return gettok();
  }

  // Check for end of file.  Don't eat the EOF.
  if (LastChar == EOF)
    return tok_eof;

  // 否则，只需返回字符作为其 ascii 值。
  int ThisChar = LastChar;
  LastChar = getchar();
  return ThisChar;
};

};// namespace lexical_analysis

namespace AST {


/// ExprAST - 所有表达式节点的基类。
class ExprAST {
 public:
  virtual ~ExprAST() = default;
};

/// NumberExprAST - 数字文本（如“1.0”）的表达式类。
class NumberExprAST : public ExprAST {
  double Val;

 public:
  NumberExprAST(double Val) : Val(Val) {}
};

/// VariableExprAST - 用于引用变量（如“a”）的表达式类。
class VariableExprAST : public ExprAST {
  std::string Name;

 public:
  VariableExprAST(const std::string &Name) : Name(Name) {}
};

/// BinaryExprAST - 二进制运算符的表达式类。
class BinaryExprAST : public ExprAST {
  char Op;
  std::unique_ptr <ExprAST> LHS, RHS;

 public:
  BinaryExprAST(char Op, std::unique_ptr <ExprAST> LHS,
                std::unique_ptr <ExprAST> RHS)
          : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

/// CallExprAST - 函数调用的表达式类。
class CallExprAST : public ExprAST {
  std::string Callee;
  std::vector <std::unique_ptr<ExprAST>> Args;

 public:
  CallExprAST(const std::string &Callee,
              std::vector <std::unique_ptr<ExprAST>> Args)
          : Callee(Callee), Args(std::move(Args)) {}
};

/// PrototypeAST - 此类表示函数的“原型”，它捕获其名称和参数名称（因此隐式表示函数采用的参数数）。
class PrototypeAST {
  std::string Name;
  std::vector <std::string> Args;

 public:
  PrototypeAST(const std::string &Name, std::vector <std::string> Args)
          : Name(Name), Args(std::move(Args)) {}

  const std::string &getName() const { return Name; }
};

/// FunctionAST - 此类表示函数定义本身。
class FunctionAST {
  std::unique_ptr <PrototypeAST> Proto;
  std::unique_ptr <ExprAST> Body;

 public:
  FunctionAST(std::unique_ptr <PrototypeAST> Proto,
              std::unique_ptr <ExprAST> Body)
          : Proto(std::move(Proto)), Body(std::move(Body)) {}
};

};// namespace AST

namespace parser {
/// CurTokgetNextToken - 提供一个简单的令牌缓冲区。CurTok 是解析器正在查看的当前令牌。getNextToken 从词法分析器读取另一个令牌，并用其结果更新 CurTok。
static int CurTok;

static int getNextToken() {
  return CurTok = lexical_analysis::gettok();
}

/// BinopPrecedence -  这保留定义的每个二进制运算符的优先级。
static std::map<char, int> BinopPrecedence;

/// GetTokPrecedence - 获取挂起的二进制运算符令牌的优先级。
static int GetTokPrecedence() {
  if (!isascii(CurTok))
    return -1;

  // Make sure it's a declared binop.
  int TokPrec = BinopPrecedence[CurTok];
  if (TokPrec <= 0)
    return -1;
  return TokPrec;
}

/// LogError* - 这些是用于错误处理的小帮助程序函数。
std::unique_ptr <AST::ExprAST> LogError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}

std::unique_ptr <AST::PrototypeAST> LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}

static std::unique_ptr <AST::ExprAST> ParseExpression();

/// numberexpr ::= number
static std::unique_ptr <AST::ExprAST> ParseNumberExpr() {
  auto Result = CPP11::make_unique<AST::NumberExprAST>(lexical_analysis::NumVal);
  getNextToken(); // consume the number
  return std::move(Result);
}

/// parenexpr ::= '(' expression ')'
static std::unique_ptr <AST::ExprAST> ParseParenExpr() {
  getNextToken(); // eat (.
  auto V = ParseExpression();
  if (!V)
    return nullptr;

  if (CurTok != ')')
    return LogError("expected ')'");
  getNextToken(); // eat ).
  return V;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
static std::unique_ptr <AST::ExprAST> ParseIdentifierExpr() {
  std::string IdName = lexical_analysis::IdentifierStr;

  getNextToken(); // eat identifier.

  if (CurTok != '(') // Simple variable ref.
    return CPP11::make_unique<AST::VariableExprAST>(IdName);

  // Call.
  getNextToken(); // eat (
  std::vector <std::unique_ptr<AST::ExprAST>> Args;
  if (CurTok != ')') {
    while (true) {
      if (auto Arg = ParseExpression())
        Args.push_back(std::move(Arg));
      else
        return nullptr;

      if (CurTok == ')')
        break;

      if (CurTok != ',')
        return LogError("Expected ')' or ',' in argument list");
      getNextToken();
    }
  }

  // Eat the ')'.
  getNextToken();

  return CPP11::make_unique<AST::CallExprAST>(IdName, std::move(Args));
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
static std::unique_ptr <AST::ExprAST> ParsePrimary() {
  switch (CurTok) {
    default:
      return LogError("unknown token when expecting an expression");
    case lexical_analysis::tok_identifier:
      return ParseIdentifierExpr();
    case lexical_analysis::tok_number:
      return ParseNumberExpr();
    case '(':
      return ParseParenExpr();
  }
}

/// binoprhs
///   ::= ('+' primary)*
static std::unique_ptr<AST::ExprAST> ParseBinOpRHS(int ExprPrec,std::unique_ptr <AST::ExprAST> LHS) {
  // If this is a binop, find its precedence.
  while (true) {
    int TokPrec = GetTokPrecedence();

    // If this is a binop that binds at least as tightly as the current binop,
    // consume it, otherwise we are done.
    if (TokPrec < ExprPrec)
      return LHS;

    // Okay, we know this is a binop.
    int BinOp = CurTok;
    getNextToken(); // eat binop

    // Parse the primary expression after the binary operator.
    auto RHS = ParsePrimary();
    if (!RHS)
      return nullptr;

    // If BinOp binds less tightly with RHS than the operator after RHS, let
    // the pending operator take RHS as its LHS.
    int NextPrec = GetTokPrecedence();
    if (TokPrec < NextPrec) {
      RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
      if (!RHS)
        return nullptr;
    }

    // Merge LHS/RHS.
    LHS = CPP11::make_unique<AST::BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
  }
};

/// expression
///   ::= primary binoprhs
///
static std::unique_ptr <AST::ExprAST> ParseExpression() {
  auto LHS = ParsePrimary();
  if (!LHS)
    return nullptr;

  return ParseBinOpRHS(0, std::move(LHS));
};

/// prototype
///   ::= id '(' id* ')'
static std::unique_ptr <AST::PrototypeAST> ParsePrototype() {
  if (CurTok != lexical_analysis::tok_identifier)
    return LogErrorP("Expected function name in prototype");

  std::string FnName = lexical_analysis::IdentifierStr;
  getNextToken();

  if (CurTok != '(')
    return LogErrorP("Expected '(' in prototype");

  std::vector <std::string> ArgNames;
  while (getNextToken() == lexical_analysis::tok_identifier)
    ArgNames.push_back(lexical_analysis::IdentifierStr);
  if (CurTok != ')')
    return LogErrorP("Expected ')' in prototype");

  // success.
  getNextToken(); // eat ')'.

  return CPP11::make_unique<AST::PrototypeAST>(FnName, std::move(ArgNames));
};

/// definition ::= 'def' prototype expression
static std::unique_ptr <AST::FunctionAST> ParseDefinition() {
  getNextToken(); // eat def.
  auto Proto = ParsePrototype();
  if (!Proto)
    return nullptr;

  if (auto E = ParseExpression())
    return CPP11::make_unique<AST::FunctionAST>(std::move(Proto), std::move(E));
  return nullptr;
};

/// toplevelexpr ::= expression
static std::unique_ptr <AST::FunctionAST> ParseTopLevelExpr() {
  if (auto E = ParseExpression()) {
    // Make an anonymous proto.
    auto Proto = CPP11::make_unique<AST::PrototypeAST>("__anon_expr",
                                                std::vector<std::string>());
    return CPP11::make_unique<AST::FunctionAST>(std::move(Proto), std::move(E));
  }
  return nullptr;
}

/// external ::= 'extern' prototype
static std::unique_ptr <AST::PrototypeAST> ParseExtern() {
  getNextToken(); // eat extern.
  return ParsePrototype();
}

};// namespace parser

namespace top_level_parsing {

static void HandleDefinition() {
  if (parser::ParseDefinition()) {
    fprintf(stderr, "Parsed a function definition.\n");
  } else {
    // Skip token for error recovery.
    parser::getNextToken();
  }
}

static void HandleExtern() {
  if (parser::ParseExtern()) {
    fprintf(stderr, "Parsed an extern\n");
  } else {
    // Skip token for error recovery.
    parser::getNextToken();
  }
}

static void HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  if (parser::ParseTopLevelExpr()) {
    fprintf(stderr, "Parsed a top-level expr\n");
  } else {
    // Skip token for error recovery.
    parser::getNextToken();
  }
}

/// top ::= definition | external | expression | ';'
static void MainLoop() {
  while (true) {
    fprintf(stderr, "ready> ");
    switch (parser::CurTok) {
      case lexical_analysis::tok_eof:
        return;
      case ';': // ignore top-level semicolons.
        parser::getNextToken();
        break;
      case lexical_analysis::tok_def:
        HandleDefinition();
        break;
      case lexical_analysis::tok_extern:
        HandleExtern();
        break;
      default:
        HandleTopLevelExpression();
        break;
    }
  }
}

};// namespace top_level_parsing

void main() {
  // Install standard binary operators.
  // 1 is lowest precedence.
  parser::BinopPrecedence['<'] = 10;
  parser::BinopPrecedence['+'] = 20;
  parser::BinopPrecedence['-'] = 20;
  parser::BinopPrecedence['*'] = 40; // highest.

  // Prime the first token.
  fprintf(stderr, "ready> ");
  parser::getNextToken();

  // Run the main "interpreter loop" now.
  top_level_parsing::MainLoop();

};

};// namespace m1_llvm_lexical_analysis

#endif //SHE_COMPILE_LAB_M1_LLVM_LEXICAL_ANALYSIS_H
