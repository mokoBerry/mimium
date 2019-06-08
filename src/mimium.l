%{
  #include "scanner.hpp"
  #undef  YY_DECL
  #define YY_DECL int mmmpsr::MimiumScanner::yylex( mmmpsr::MimiumParser::semantic_type * const lval, mmmpsr::MimiumParser::location_type *loc )
  
  using token = mmmpsr::MimiumParser::token;
  #define yyterminate() return ( token::END )
  #define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}
%option c++ noyywrap nounput noinput nodefault debug
%option yyclass="mmmpsr::MimiumParser"

NONZERO  [1-9]
DIGIT    [[:digit:]]
FLOAT    ({NONZERO}{DIGIT}*\.?|0\.|\.{DIGIT}){DIGIT}*|0
EXPONENT [eE](-|\+?){DIGIT}+
NUMBER   {FLOAT}{EXPONENT}?
DELIMITER "{"|"}"|"("|")"|"["|"]"|","
WSPACE   [[:blank:]]+
BREAK  "\r\n" | "\n" | "\r"
NEWLINE {BREAK} | ";"
%%

%{
   yylval = lval;
%}

{NUMBER} {
    yylval->emplace<int>(std::stoi(yytext));
    return token::NUM;
};

{WSPACE} ;

"/"  return token::DIV; 
"*"  return token::MUL; 
"%"  return token::MOD; 
"+"  return token::ADD; 
"-"  return token::SUB; 
"^"  return token::EXPONENT;
"&" return token::AND;
"|" return token::OR;
"&&" return token::BITAND;
"||" return token::BITOR;
"!=" return token::NEQ;
"==" return token::EQ;
"!"  return token::NOT;

"@" return token::AT;

\r\n|\r|\n	{ loc->lines(); loc->step(); return token::NEWLINE;}
{DELIMITER} { return yytext[0];};
. {
std::cerr << "Lexer: Unrecognized char \"" << yytext[0] << "\"" << std::endl;
		loc->step();
}
%%
