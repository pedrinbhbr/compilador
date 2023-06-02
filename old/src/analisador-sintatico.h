/**
 *  
 * Cabeçalho do Analisador Sintático.
 * 
 * */

#ifndef ANALISADOR_SINTATICO_H
#define ANALISADOR_SINTATICO_H

#include "analisador-lexico.h"
#include <exception>

// Analisador Sintatico Erro
struct ASError : public std::exception {
	const char * what () const throw () {
    	return "Erro";
    }
};

class AnalisadorSintatico {

public:
    void INICIO();
    AnalisadorLexico analisadorLexico;

    erros erro;

private:
    void CasaToken(unsigned char TOKEN);
    void verificaTokenNaoEsp(int num_linhas);
    void F();
    void T();
    void EXPS();
    void EXP();
    void ESCRITA();
    void LEITURA();
    void TESTE();
    void REPETICAO();
    void ATRIBUICAO();
    void COMANDOS();
    void DECLA_CONST();
    void TIPO();
    void DECLA_VAR();
    void DECLARACOES();

};

#endif