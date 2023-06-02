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
 * Tabela de Símbolos.
 * 
 * */

#include "tabela-simbolos.h"

// Construtor da Tabela de Simbolos
TabelaSimbolos::TabelaSimbolos(){
    inserir("const", TOKEN_CONST);
    inserir("int", TOKEN_INT);
    inserir("char", TOKEN_CHAR);
    inserir("while", TOKEN_WHILE);
    inserir("if", TOKEN_IF);
    inserir("float", TOKEN_FLOAT);
    inserir("else", TOKEN_ELSE);
    inserir("&&", TOKEN_AND);
    inserir("||", TOKEN_OR);
    inserir("!", TOKEN_NEGADO);
    inserir("<-", TOKEN_ATRIBUICAO);
    inserir("=", TOKEN_IGUAL);
    inserir("(", TOKEN_ABRE_PAREN);
    inserir(")", TOKEN_FECHA_PAREN);
    inserir("<", TOKEN_MENOR);
    inserir(">", TOKEN_MAIOR);
    inserir("!=", TOKEN_DIFERENTE);
    inserir(">=", TOKEN_MAIOR_IGUAL);
    inserir("<=", TOKEN_MENOR_IGUAL);
    inserir(",", TOKEN_VIRGULA);
    inserir("+", TOKEN_MAIS);
    inserir("-", TOKEN_MENOS);
    inserir("*", TOKEN_MULTIPLICACAO);
    inserir("/", TOKEN_DIVISAO);
    inserir(";", TOKEN_PONTO_VIRGULA);
    inserir("{", TOKEN_ABRE_CHAVES);
    inserir("}", TOKEN_FECHA_CHAVES);
    inserir("readln", TOKEN_READLN);
    inserir("div", TOKEN_DIV);
    inserir("write", TOKEN_WRITE);
    inserir("writeln", TOKEN_WRITELN);
    inserir("mod", TOKEN_MOD);
    inserir("[", TOKEN_ABRE_COLCH);
    inserir("]", TOKEN_FECHA_COLCH);
    inserir("string", TOKEN_STRING);
}

// Inserir identificador na tabela de símbolos.
// lexema - lexema a ser inserido na tabela de símbolos.
void TabelaSimbolos::inserir(string lexema, unsigned char token){
    tabelaSimbolo[lexema] = Simbolo{token,TIPO_NULL,CLASSE_VAZIA,0};
}

// Realizar pesquisa do token na tabela e inserir caso nao exista.
// Evitar que uma suposta variavel com mesmo nome de uma palavra reservada seja..
// ..possivel de armazenar e sobescrever o token ja existente.
// lexema - lexema do token a ser pesquisado na tabela
Simbolo TabelaSimbolos::pesquisar(string lexema, unsigned char token){
    Simbolo s;
    // token 0 significa que nao ha este token na tabela
    if ((int)tabelaSimbolo[lexema].token == 0 ) {
        inserir(lexema,token);
        s.token = token;
    } else {
        s = tabelaSimbolo[lexema];
    }

    return s;
}

// Método para devolver símbolo desejado
Simbolo TabelaSimbolos::recuperarSimbolo(string lexema){
    Simbolo s = tabelaSimbolo[lexema];
    return s;
}

// Método para atualizar símbolo na tabela
void TabelaSimbolos::atualizarTabela(string lexema, Simbolo s){
    tabelaSimbolo[lexema] = s;
}

// ver token da Tabela de Simbolos.
// criada pra testar.
void TabelaSimbolos::ver(string x){
    cout << "Token: " << (int)tabelaSimbolo[x].token << endl;
}

// Imprimir a Tabela de Simbolos.
void TabelaSimbolos::imprimir(){
    for(auto it = tabelaSimbolo.begin(); it != tabelaSimbolo.end(); ++it){
        cout << " Lexema: " << it->first << "\tToken: " << (int)it->second.token << endl;
    }
}