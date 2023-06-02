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
 * Cabeçalho da Tabela de Símbolos.
 * 
 * */

#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

#include <iostream>
#include <string>
#include <unordered_map>

#include "constantes.h"

using namespace std;

struct Simbolo{
    unsigned char token;
    // Novos atributos para o analisador semantico
    unsigned char tipo;
    unsigned char classe;
    int endereco;
    int tamanho;
};

class TabelaSimbolos{
    public:
        TabelaSimbolos();
        //~TabelaSimbolos();

        void inserir(string, unsigned char);
        Simbolo pesquisar(string, unsigned char);
        Simbolo recuperarSimbolo(string);
        void atualizarTabela(string,Simbolo);
        void imprimir();
        void ver(string); // teste
    protected:

    private:
        unordered_map<string, Simbolo> tabelaSimbolo;
        int final = -1;
};

#endif