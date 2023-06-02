/**
 * Ciência da Computação - PUC Coração Eucarístico - Compiladores - 2/2021 
 * 
 * @author Igor Machado Seixas
 * @author Matheus Mendes Tavares
 * @author Pedro Henrique Ferreira de Souza
 * @version 1.0
 * 
 * */

// Definição dos Identificadores dos Simbolos.
const unsigned char TOKEN_ID = 1;
const unsigned char TOKEN_CONSTANTE = 2;
const unsigned char TOKEN_AND = 3;
const unsigned char TOKEN_OR = 4;
const unsigned char TOKEN_NEGADO = 5;
const unsigned char TOKEN_ATRIBUICAO = 6;
const unsigned char TOKEN_IGUAL = 7;
const unsigned char TOKEN_ABRE_PAREN = 8;
const unsigned char TOKEN_FECHA_PAREN = 9;
const unsigned char TOKEN_MENOR = 10;
const unsigned char TOKEN_MAIOR = 11;
const unsigned char TOKEN_DIFERENTE = 12;
const unsigned char TOKEN_MAIOR_IGUAL = 13;
const unsigned char TOKEN_MENOR_IGUAL = 14;
const unsigned char TOKEN_VIRGULA = 15;
const unsigned char TOKEN_MAIS = 16;
const unsigned char TOKEN_MENOS = 17;
const unsigned char TOKEN_MULTIPLICACAO = 18;
const unsigned char TOKEN_DIVISAO = 19;
const unsigned char TOKEN_PONTO_VIRGULA = 20;
const unsigned char TOKEN_ABRE_CHAVES = 21;
const unsigned char TOKEN_FECHA_CHAVES = 22;
const unsigned char TOKEN_ABRE_COLCH = 23;
const unsigned char TOKEN_FECHA_COLCH = 24;
const unsigned char TOKEN_CONST = 25;
const unsigned char TOKEN_INT = 26;
const unsigned char TOKEN_CHAR = 27;
const unsigned char TOKEN_WHILE = 28;
const unsigned char TOKEN_IF = 29;
const unsigned char TOKEN_FLOAT = 30;
const unsigned char TOKEN_ELSE = 31;
const unsigned char TOKEN_READLN = 32;
const unsigned char TOKEN_DIV = 33;
const unsigned char TOKEN_WRITE = 34;
const unsigned char TOKEN_WRITELN = 35;
const unsigned char TOKEN_MOD = 36;
const unsigned char TOKEN_STRING = 37;
const unsigned char TOKEN_EOF = 38;

// Definição dos tipos e quantidade de tipos.
const unsigned char TIPO_INT = 50;
const unsigned char TIPO_CHAR = 51;
const unsigned char TIPO_FLOAT = 52;
const unsigned char TIPO_STRING = 53;
// Acrescentando novos tipos para o analisador semantico
const unsigned char TIPO_BOOL = 54;
const unsigned char TIPO_NULL = 55;

// Definição dos valores de classe
const unsigned char CLASSE_VAZIA = 70;
const unsigned char CLASSE_VAR = 71;
const unsigned char CLASSE_CONST = 72;