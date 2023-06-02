/**
 *  
 * Cabeçalho do Analisador Sintático + Semântico.
 * 
 * */

#ifndef ANALISADOR_SEMANTICO_H
#define ANALISADOR_SEMANTICO_H

#include "analisador-lexico.h"
#include <exception>

// Analisador Sintatico + Semantico Erros
struct ASError : public std::exception {
	const char * what () const throw () {
    	return "Erro";
    }
};

// Estrutura auxiliar para realizar a verificação de tipos
struct RegistroSemantico{
    unsigned char tipo; 
};

class AnalisadorSintatico {

public:

    void INICIO();
    AnalisadorLexico analisadorLexico;

    erros erro;

private:

    // Espaco para INICIO()
    void CasaToken(unsigned char TOKEN);
    void verificaTokenNaoEsp(int num_linhas);
    void verificaErroTipo(int num_linhas, int indice, string lexema);
    void F(RegistroSemantico* rsF);
    void T(RegistroSemantico* rsT);
    void EXPS(RegistroSemantico* rsEXPS);
    void EXP(RegistroSemantico* rsEXP);
    void ESCRITA();
    void LEITURA();
    void TESTE();
    void REPETICAO();
    void ATRIBUICAO();
    void COMANDOS();
    void DECLA_CONST();
    void TIPO(RegistroSemantico* rsTIPO);
    void DECLA_VAR();
    void DECLARACOES();

};

#endif