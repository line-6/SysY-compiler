%option noyywrap
%option nounput
%option noinput

%{
/* 头文件及全局声明 */
#include <string>
#include <cstdlib>
#include <iostream>
#include "sysy.tab.hpp"

using namespace std;
%}

/* flex/Bison 定义：正则表达式规则/(非)终结符规则 */
WhiteSpace      [ \n\t\r]+                   
LineComment     "//".*
BlockCommet     "/*"([^*]|\*+[^*/])*\*+"/"

Identifier      [a-zA-Z_][a-zA-Z_0-9]*

Dec_num         [1-9][0-9]*
Oct_num         0[0-7]*
Hex_num         0[xX][0-9a-fA-F]+

%%

{WhiteSpace}    { /* 忽略, 不做任何操作 */ }
{LineComment}   { /* 忽略, 不做任何操作 */ }
{BlockCommet}   { /* 忽略, 不做任何操作 */ }

"int"           {   /* std::cout << "this is int" << std::endl; */ 
                    return INT; }

"return"        {   /* std::cout << "this is return" << std::endl; */
                    return RETURN; }

{Identifier}    {   yylval.str_val = new string(yytext); 
                    /* std::cout << "this is ident: " + *yylval.str_val<< std::endl; */
                    return IDENT; }

{Dec_num}       {   yylval.int_val = strtol(yytext, nullptr, 0); 
                    /* std::cout << "this is dec num: " + yylval.int_val<< std::endl; */ 
                    return INT_CONST; }

{Oct_num}       {   yylval.int_val = strtol(yytext, nullptr, 0); 
                    return INT_CONST; }

{Hex_num}       {   yylval.int_val = strtol(yytext, nullptr, 0); 
                    return INT_CONST; }

.               {   /* std::cout << "this is .:" + yytext[0]<< std::endl; */
                    return yytext[0]; }
%%
