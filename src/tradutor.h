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
 * Cabeçalho do Analisador Sintático + Semântico + Geração de Código.
 * 
 * */

#ifndef TRADUTOR_H
#define TRADUTOR_H

#include "analisador-lexico.h"
#include <exception>
#include <fstream>

// Analisador Sintatico + Semantico Erros
struct ASError : public std::exception {
	const char * what () const throw () {
    	return "Erro";
    }
};

// Estrutura auxiliar para realizar ações semânticas
struct RegistroSemantico{
    unsigned char tipo; 
    int endereco;
    int tamanho;
    //bool negativo;
};

class AnalisadorSintatico {

public:
    void Compila();
    AnalisadorLexico analisadorLexico;
    string codigoAssembly;

    erros erro;
    int contadorDados = 65536; // 10000h
    int contadorTemps = 0; 
    int indiceRotulos = 0;

private:
    int NovoDado(int bytes);
    int NovoTemp(int bytes);
    string NovoRot();
    string EnderecoHex(int end);
    
    void INICIO();   
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