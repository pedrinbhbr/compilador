/**
 * Ciência da Computação - PUC Coração Eucarístico - Compiladores - 2/2021 
 * 
 * @author Igor Machado Seixas
 * @author Matheus Mendes Tavares
 * @author Pedro Henrique Ferreira de Souza
 * @version 1.0
 * 
 * */

#include <string>
#include <iostream>

using namespace std;

class erros {
    public:
        // Erros léxicos;
        void caractereInvalido(int nn){ cout << nn << endl << "caractere invalido." << endl; }
        void lexemaNaoIdentificado(int nn, string lex){ cout << nn << endl << "lexema nao identificado [" << lex << "]." << endl; }
        void eofNaoEsperado(int nn){ cout << nn << endl << "fim de arquivo nao esperado." << endl; }

        // Erros sintáticos;
        void tokenNaoEsperado(int nn, string lex){ cout << nn << endl << "token nao esperado [" << lex << "]." << endl; }

        // Erros semânticos;
        void idNaoDeclarado(int nn, string lex){ cout << nn << endl << "identificador nao declarado [" << lex << "]." << endl; }
        void idJaDeclarado(int nn, string lex){ cout << nn << endl << "identificador ja declarado [" << lex << "]." << endl; }
        void classeIdIncompativel(int nn, string lex){ cout << nn << endl << "classe de identificador incompativel [" << lex << "]." << endl; }
        void tipoIncompativel(int nn){ cout << nn << endl << "tipos incompativeis." << endl; }
};