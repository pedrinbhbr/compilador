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
 * Analisador Léxico.
 * 
 * */

#include <iostream>
#include <ctype.h>
#include <stdexcept>
#include "analisador-lexico.h"

string AnalisadorLexico::analisar(){
    int estado = 0;
    int estadoFinal = 20;
    string lexema = "";
    char c;
    abortar = false;

    // cout << "Teste da Tabela Simbolo" << endl;
    // tabelaSimbolos.imprimir();
    
    while(estado != estadoFinal && !abortar){
        
        c = cin.get();

        // Verificar se o caractere é válido.
        if(!(isspace(c) || isalnum(c) || CARACTERE_VALIDO(c) || cin.eof())){
            erro.caractereInvalido(num_linhas);
            abortar = true;
            estado = 99;
        }

        // if (cin.eof() && estado != 0){
        //     erro.eofNaoEsperado(num_linhas);
        //     abortar = true;
        // }//fim if

        switch (estado)
        {
        case 0:
            // Quebra de linha ou espaço em branco.
            if(isspace(c)){
                if(c == '\n'){ // Verificação quebra de linha (bytes 0Dh e 0Ah). || c == '\r'
                    //cout << "Numero de Linhas -> " << num_linhas << endl;
                    num_linhas++;
                }
                estado = 0;       
            } 
            // ID ou palavra reservada
            else if(isalpha(c) || c == '_'){ // (Letra | _).
                lexema += c;
                estado = 1;
            } 
            // Constante numerica inteira ou real
            else if(isdigit(c) && c != '0'){
                lexema += c;
                estado = 2; 
            } 
            // Constante numerica real
            else if(c == '.'){
                lexema += c;
                estado = 3;
            } 
            // Constante numerica inteira, real ou char hexadecimal
            else if(c == '0'){
                lexema += c;
                estado = 4;
            } 
            // Constante char
            else if(c == '\''){
                lexema += c;
                estado = 6;
            } 
            // Constante string
            else if(c == '\"'){
                lexema += c;
                estado = 8;
            } 
            // Token &&
            else if(c == '&'){
                lexema += c;
                estado = 9;
            } 
            // Token ||
            else if(c == '|'){
                lexema += c;
                estado = 10;
            } 
            // Tokens > ou >=
            else if(c == '>'){
                lexema += c;
                estado = 11;
            } 
            // Tokens <, <= ou <-
            else if(c == '<'){
                lexema += c;
                estado = 12;
            } 
            // Tokens ! ou !=
            else if(c == '!'){
                lexema += c;
                estado = 13;
            } 
            // Token / ou comentario
            else if(c == '/'){
                lexema += c;
                estado = 14;
            } 
            // Token =
            else if(c == '='){
                lexema += c;
                token.token = TOKEN_IGUAL;
                token.tipo = -1;
                token.tamanho = 0;

                estado = estadoFinal;
            } 
            // Token (
            else if(c == '('){
                lexema += c;
                token.token = TOKEN_ABRE_PAREN;
                token.tipo = -1;
                token.tamanho = 0;

                estado = estadoFinal;
            } 
            // Token )
            else if(c == ')'){
                lexema += c;
                token.token = TOKEN_FECHA_PAREN;
                token.tipo = -1;
                token.tamanho = 0;

                estado = estadoFinal;
            } 
            // Token ,
            else if(c == ','){
                lexema += c;
                token.token = TOKEN_VIRGULA;
                token.tipo = -1;
                token.tamanho = 0;

                estado = estadoFinal;
            } 
            // Token +
            else if(c == '+'){
                lexema += c;
                token.token = TOKEN_MAIS;
                token.tipo = -1;
                token.tamanho = 0;

                estado = estadoFinal;
            } 
            // Token -
            else if(c == '-'){
                lexema += c;
                token.token = TOKEN_MENOS;
                token.tipo = -1;
                token.tamanho = 0;

                estado = estadoFinal;
            } 
            // Token *
            else if(c == '*'){
                lexema += c;
                token.token = TOKEN_MULTIPLICACAO;
                token.tipo = -1;
                token.tamanho = 0;

                estado = estadoFinal;
            } 
            // Token ;
            else if(c == ';'){
                lexema += c;
                token.token = TOKEN_PONTO_VIRGULA;
                token.tipo = -1;
                token.tamanho = 0;

                estado = estadoFinal;
            } 
            // Token {
            else if(c == '{'){
                lexema += c;
                token.token = TOKEN_ABRE_CHAVES;
                token.tipo = -1;
                token.tamanho = 0;

                estado = estadoFinal;
            } 
            // Token }
            else if(c == '}'){
                lexema += c;
                token.token = TOKEN_FECHA_CHAVES;
                token.tipo = -1;
                token.tamanho = 0;

                estado = estadoFinal;
            } 
            // Token [
            else if(c == '['){
                lexema += c;
                token.token = TOKEN_ABRE_COLCH;
                token.tipo = -1;
                token.tamanho = 0;

                estado = estadoFinal;
            } 
            // Token ]
            else if(c == ']'){
                lexema += c;
                token.token = TOKEN_FECHA_COLCH;
                token.tipo = -1;
                token.tamanho = 0;

                estado = estadoFinal;
            } 
            // Token EOF
            else if(cin.eof()){
                lexema = "EOF";
                token.token = TOKEN_EOF;
                token.tipo = -1;
                token.tamanho = 0;

                estado = estadoFinal;
            } 
            // Erro de lexema invalido
            else{
                lexema += c;
                erro.lexemaNaoIdentificado(num_linhas, lexema);
                abortar = true;
            }
            break;
        
        // Lexema ID (Dígito | Letra | _ | .)* ou palavra reservada
        case 1: 
            while (isalpha(c) || isdigit(c) || c == '.' || c == '_') {
                lexema += c;
                c = cin.get();
            }// fim while
            // Tamanho maximo do ID
            if(lexema.length() > 32){
                erro.lexemaNaoIdentificado(num_linhas, lexema);
                abortar = true;
            } else{
                devolveCaractere(c);
                token.token = TOKEN_ID;
                token.tipo = -1;
                token.tamanho = 0;
                
                // pesquisa e insere na tabela o ID caso nao exista
                Simbolo s = tabelaSimbolos.pesquisar(lexema,token.token);
                if (s.token != token.token) {
                    // caso ja exista, atualiza o registro lexico
                    token.token = s.token;
                }//fim if

                estado = estadoFinal;
            }
            break;
        // Lexema Digito inteiro
        case 2:
            while(isdigit(c)) {
                lexema += c;
                c = cin.get();
            }//fim while
            // Pode ser uma constante numerica real
            if(c == '.'){
                lexema += c;
                estado = 3;
            }
            else{
                try { // verifica se o inteiro e um escalar de 4bytes
                    stoi(lexema); // converte string em inteiro
                    devolveCaractere(c);
                    token.token = TOKEN_CONSTANTE;
                    token.tipo = TIPO_INT;
                    token.tamanho = 4;

                    estado = estadoFinal;
                } 
                // Caso estoure o tamanho maximo de um int
                catch (const out_of_range& oor) {
                    erro.lexemaNaoIdentificado(num_linhas, lexema);
                    abortar = true;
                }// fim try catch
            }
            break;
        // Lexema ponto flutuante
        case 3:
            // Verifica se o primeiro caractere apos o ponto eh numero
            if (isdigit(c)) {
                while(isdigit(c)) {
                    lexema += c;
                    c = cin.get();
                }// fim while
                // Verifica as 6 casas de precisao
                if((lexema[0] == '.' && lexema.length() > 6) || (lexema[0] != '.' && lexema.length() > 7) ){
                    erro.lexemaNaoIdentificado(num_linhas, lexema);
                    abortar = true;
                } else{
                    devolveCaractere(c);
                    token.token = TOKEN_CONSTANTE;
                    token.tipo = TIPO_FLOAT;
                    token.tamanho = 4; // 4 bytes mesmo?

                    estado = estadoFinal;
                }
            } else {
                verificaCaracNaoIdent(num_linhas, lexema);
                abortar = true;

            }// fim if else
            break;
        // Lexema inteiro, ponto flutuante ou char hexadecimal
        case 4:
            if(isdigit(c)){
                lexema += c;
                estado = 2;
            } else if(c == '.'){
                lexema += c;
                estado = 3;
            } else if (c == 'x'){
                lexema += c;
                estado = 5;
            } else {
                // Devolve o caractere e monta registro lexico do token constante tipo inteiro.
                devolveCaractere(c);
                token.token = TOKEN_CONSTANTE;
                token.tipo = TIPO_INT;
                token.tamanho = 4;

                estado = estadoFinal;
            }
            break;
        // Lexema char hexadecimal
        case 5:
            // Primeiro digito hexadecimal
            if(isdigit(c) || ('A' <= c && c <= 'F')){
                lexema += c;
                c = cin.get();
                // Segundo digito hexadecimal
                if(isdigit(c) || ('A' <= c && c <= 'F')){
                    lexema += c;
                    token.token = TOKEN_CONSTANTE;
                    token.tipo = TIPO_CHAR;
                    token.tamanho = 1;

                    estado = estadoFinal;
                } 
                // Caso caractere lido nao seja valido, printa erro de caractere invalido
                else if ( !(isspace(c) || isalpha(c) || CARACTERE_VALIDO(c) || cin.eof())) {
                    erro.caractereInvalido(num_linhas); 
                    abortar = true;
                }
                // Printa erro de lexema invalido
                else {
                    verificaCaracNaoIdent(num_linhas, lexema); 
                    abortar = true;
                }// fim if else
            } 
            // Printa erro de lexema invalido
            else{
                verificaCaracNaoIdent(num_linhas, lexema); 
                abortar = true;
            }//fim if else
            break;
        // Lexema char 1
        case 6:
            lexema += c;
            estado = 7;
            break;
        // Lexema char 2
        case 7:
            if(c == '\''){
                lexema += c;
                token.token = TOKEN_CONSTANTE;
                token.tipo = TIPO_CHAR;
                token.tamanho = 1;
                
                estado = estadoFinal;          
            } else{
                verificaCaracNaoIdent(num_linhas, lexema);
                abortar = true;
            }
            break;
        // Lexema string
        case 8:
            // Nao pode haver quebra de linha dentro da string
            while (c == ' ' || isalnum(c) || (CARACTERE_VALIDO(c) && c != '\"' )) {
                lexema += c;
                c = cin.get();
            }// fim while
            if (c != '\"' || lexema.length() > 256) { // 255 + aspas inicial                
                // Se o caractere for invalido, diferente de quebra de linha e diferente de EOF, printa erro de caractere invalido
                if (!isspace(c) && !CARACTERE_VALIDO(c) && !cin.eof()) {
                    erro.caractereInvalido(num_linhas);
                }
                // Caso tamanho da string seja maior que 255 caracteres uteis ou lexema errado, printa erro
                else {
                    verificaCaracNaoIdent(num_linhas, lexema);
                } //fim if else
                
                abortar = true;
            } else {
                lexema += c; // inclui aspas final
                token.token = TOKEN_CONSTANTE;
                token.tipo = TIPO_STRING;
                token.tamanho = lexema.length() - 2; // tira 2 bytes das aspas
                
                estado = estadoFinal; 
            }//fim if else
            break;
        // Lexema &&
        case 9:
            if (c != '&' ) { 
                verificaCaracNaoIdent(num_linhas, lexema);
                abortar = true;
            } else {
                lexema += c;
                token.token = TOKEN_AND;
                token.tipo = -1;
                token.tamanho = 0;
                
                estado = estadoFinal; 
            }//fim if else
            break;
        // Lexema ||
        case 10:
            if (c != '|' ) { 
                verificaCaracNaoIdent(num_linhas, lexema);
                abortar = true;
            } else {
                lexema += c;
                token.token = TOKEN_OR;
                token.tipo = -1;
                token.tamanho = 0;
                
                estado = estadoFinal; 
            }//fim if else
            break;
        // Lexema > ou >=
        case 11:
            if (c == '=' ) { 
                lexema += c;
                token.token = TOKEN_MAIOR_IGUAL;
                token.tipo = -1;
                token.tamanho = 0;
            } else {
                devolveCaractere(c);
                token.token = TOKEN_MAIOR;
                token.tipo = -1;
                token.tamanho = 0;
            }//fim if else
            estado = estadoFinal; 
            
            break;
        // Lexema <, <- ou <=
        case 12:
            if (c == '-' ) { 
                lexema += c;
                token.token = TOKEN_ATRIBUICAO;
                token.tipo = -1;
                token.tamanho = 0;
            } else if (c == '=' ) { 
                lexema += c;
                token.token = TOKEN_MENOR_IGUAL;
                token.tipo = -1;
                token.tamanho = 0;
            } else {
                devolveCaractere(c);
                token.token = TOKEN_MENOR;
                token.tipo = -1;
                token.tamanho = 0;
            }//fim if else
            estado = estadoFinal; 
            
            break;
        // Lexema ! ou !=
        case 13:
            if (c == '=' ) { 
                lexema += c;
                token.token = TOKEN_DIFERENTE;
                token.tipo = -1;
                token.tamanho = 0;
            } else {
                devolveCaractere(c);
                token.token = TOKEN_NEGADO;
                token.tipo = -1;
                token.tamanho = 0;
            }//fim if else
            estado = estadoFinal; 
            
            break;
        // Lexema / ou comentario
        case 14:
            if (c == '*' ) { 
                lexema += c;
                estado = 15;
            } else {
                devolveCaractere(c);
                token.token = TOKEN_DIVISAO;
                token.tipo = -1;
                token.tamanho = 0;

                estado = estadoFinal; 
            }//fim if else
            break;
        // Trata comentario 1
        case 15:
            while (isspace(c) || isalnum(c) || (CARACTERE_VALIDO(c) && c != '*')) {
                // // Checa se pulou linha.
                if (c == '\n'){
                    num_linhas++;
                }//fim if
                lexema += c;
                c = cin.get();
            }// fim while
            // Continua a tratar comentario
            if (c == '*' ) {
                lexema += c;
                estado = 16;
            } 
            // Se o caractere for invalido e diferente de EOF, printa erro de caractere invalido
            else if (!CARACTERE_VALIDO(c) && !cin.eof()) {
                erro.caractereInvalido(num_linhas);
                abortar = true;
            } 
            // Imprime erro de EOF nao esperado
            else {
                erro.eofNaoEsperado(num_linhas);
                abortar = true;
            }//fim if else
            break;
        // Trata comentario 2
        case 16:
            while (c == '*') {
                lexema += c;
                c = cin.get();
            }// fim while
            // Se encontrar / finaliza o comentario
            if (c == '/' ) {
                lexema = "";
                estado = 0;
            } 
            // Verifica se eh caractere valido e continua a tratar comentario
            else if (isspace(c) || isalnum(c) || CARACTERE_VALIDO(c)) {
                // // Checa se pulou linha.
                if (c == '\n'){
                    num_linhas++;
                }//fim if
                lexema += c;
                estado = 15;
            } 
            // Se for diferente de EOF, imprime erro de caractere invalido
            else if (!cin.eof()) {
                erro.caractereInvalido(num_linhas);
                abortar = true;
            }
            // Imprime erro de EOF nao esperado
            else {
                erro.eofNaoEsperado(num_linhas);
                abortar = true;
            } //fim if else
            break;
        default:
            break;
        }
    }

    // Adiciona o lexema no registro lexico caso nao houve erro
    if ( !abortar ) {
        token.lexema = lexema;
        //cout << "token: " << (int)token.token << " lexema: " << token.lexema << " tipo: " << token.tipo << " tamanho: " << (int)token.tamanho << endl;
    }// fim if

    return lexema;
}

// Devolve caractere para o buffer
void AnalisadorLexico::devolveCaractere(char c) {
    // Apenas de for diferente de EOF
    if (!cin.eof()) {      
        cin.putback(c);
    }// fim if
}// fim devolveCaractere()

// Imprime erro de acordo com os parametros recebidos
void AnalisadorLexico::verificaCaracNaoIdent(int num_linhas, string lexema) {
    // EOF
    if (cin.eof()) {
        erro.eofNaoEsperado(num_linhas);
    } 
    // envia lexema para printar o lexema invalido
    else {
        erro.lexemaNaoIdentificado(num_linhas, lexema);
    } // fim if else
} // verificaCaracNaoIdent()

// Devolver o token guardado no registro lexico
unsigned char AnalisadorLexico::getToken() {
    return token.token;
}// fim getToken()

// Devolver o lexema guardado no registro lexico
string AnalisadorLexico::getLexema() {
    return token.lexema;
}// fim getToken()

// Devolver o registro lexico montado
Token AnalisadorLexico::getRegistroLexico() {
    return token;
}// fim getToken()