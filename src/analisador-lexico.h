/**
 * Ciência da Computação - PUC Coração Eucarístico - Compiladores - 2/2021 
 * 
 * @author Igor Machado Seixas
 * @author Matheus Mendes Tavares
 * @author Pedro Henrique Ferreira de Souza
 * @version 1.0
 * 
 * */

/**
 *  
 * Cabeçalho do Analisador Léxico.
 * 
 * */

#ifndef ANALISADOR_LEXICO_H
#define ANALISADOR_LEXICO_H

/* caracteres validos: 
letras, digitos, espaço, sublinhado, ponto, vírgula, 
ponto-e-vírgula, dois-pontos, parênteses, colchetes, 
chaves, mais, menos, aspas, apóstrofo, barra, barra em pé, 
barra invertida, e-comercial, asterisco, porcentagem, exclamação, 
interrogação, maior, menor e igual, além da quebra de linha (bytes 0Dh e 0Ah).
*/
#define CARACTERE_VALIDO(c) (c == '!' || c == '\"' || c == '_' || (37 <= c && c <= 47) || (58 <= c && c <= 63) || (91 <= c && c <= 93) || (123 <= c && c <= 125)) 

#include <string>
#include "tabela-simbolos.h"
#include "erros.h"

using namespace std;

struct Token{
    unsigned char token;
    string lexema;
    int endereco;
    unsigned char tipo;
    unsigned char tamanho;    
};

class AnalisadorLexico {

public:
    //AnalisadorLexico();
    //~AnalisadorLexico();
    string analisar();
    unsigned char getToken();
    string getLexema();
    Token getRegistroLexico();

    erros erro;
    bool abortar = false;
    int num_linhas = 1;
    TabelaSimbolos tabelaSimbolos;

private:
    Token token;

    void devolveCaractere(char c);
    void verificaCaracNaoIdent(int num_linhas, string lexema);

};

#endif