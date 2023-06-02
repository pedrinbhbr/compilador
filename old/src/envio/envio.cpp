/**
 * Ciência da Computação - PUC Coração Eucarístico - Compiladores - 2/2021
 *  
 * Função Principal.
 * @author Igor Machado Seixas
 * @author Matheus Mendes Tavares
 * @author Pedro Henrique Ferreira de Souza
 * @version 0.2
 * 
 * */

//constantes.h
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

/**
 * 
 * Cabeçalho da Tabela de Símbolos.
 * 
 * */

#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

struct Simbolo{
    unsigned char token;
    // Novos atributos para o analisador semantico
    unsigned char tipo;
    unsigned char classe;
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

/**
 * 
 * Tabela de Símbolos.
 * 
 * */

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
    tabelaSimbolo[lexema] = Simbolo{token,TIPO_NULL,CLASSE_VAZIA};
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

//erros.h

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

/**
 *  
 * Cabeçalho do Analisador Léxico.
 * 
 * */

/* caracteres validos: 
letras, digitos, espaço, sublinhado, ponto, vírgula, 
ponto-e-vírgula, dois-pontos, parênteses, colchetes, 
chaves, mais, menos, aspas, apóstrofo, barra, barra em pé, 
barra invertida, e-comercial, asterisco, porcentagem, exclamação, 
interrogação, maior, menor e igual, além da quebra de linha (bytes 0Dh e 0Ah).
*/
#define CARACTERE_VALIDO(c) (c == '!' || c == '\"' || c == '_' || (37 <= c && c <= 47) || (58 <= c && c <= 63) || (91 <= c && c <= 93) || (123 <= c && c <= 125)) 



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

/**
 *  
 * Analisador Léxico.
 * 
 * */


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

/**
 *  
 * Cabeçalho do Analisador Sintático + Semântico.
 * 
 * */

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

/**
 *  
 * Analisador Sintático + Semântico.
 * 
 * */


// Metodo aborta programa caso analisador lexico encontre erro
// Caso contrario, verifica se o token lido eh o mesmo do token esperado
// Se nenhuma das duas, reporta erro
void AnalisadorSintatico::CasaToken(unsigned char TOKEN) {
    // Verifica se o analisador lexico encontrou erro
    if (analisadorLexico.abortar) {
        throw ASError();
    }
    // Se o token recebido for igual ao token lido, chama analisador lexico
    else if (TOKEN == analisadorLexico.getToken()) {
        // Chama analisador lexico apenas se nao EOF
        if (TOKEN != TOKEN_EOF) {
            analisadorLexico.analisar(); // Le proximo token e atualiza
        }// fim if        
    } 
    // Envia erro caso contrario
    else {
        verificaTokenNaoEsp(analisadorLexico.num_linhas);
    }// fim if else

}// fim CasaToken()

// Procedimento INICIO()
// INICIO -> {DECLARACOES | COMANDOS} EOF
// Parte semantica: sem acao semantica
void AnalisadorSintatico::INICIO() {
    // Verifica se eh algum token first dos procedimentos DECLARACOES ou COMANDOS, que pode ser chamado 0 ou mais vezes
    while (analisadorLexico.getToken() == TOKEN_ID || analisadorLexico.getToken() == TOKEN_READLN ||
           analisadorLexico.getToken() == TOKEN_WRITE || analisadorLexico.getToken() == TOKEN_WRITELN ||
           analisadorLexico.getToken() == TOKEN_PONTO_VIRGULA || analisadorLexico.getToken() == TOKEN_WHILE ||
           analisadorLexico.getToken() == TOKEN_IF || analisadorLexico.getToken() == TOKEN_INT ||
           analisadorLexico.getToken() == TOKEN_FLOAT || analisadorLexico.getToken() == TOKEN_STRING ||
           analisadorLexico.getToken() == TOKEN_CHAR || analisadorLexico.getToken() == TOKEN_CONST ) {
        
        //Se for algum token first da DECLARACOES, chama o procedimento
        if (analisadorLexico.getToken() == TOKEN_INT || analisadorLexico.getToken() == TOKEN_FLOAT || 
            analisadorLexico.getToken() == TOKEN_STRING || analisadorLexico.getToken() == TOKEN_CHAR ||
            analisadorLexico.getToken() == TOKEN_CONST) {

            DECLARACOES();
        } else { // Caso contrario, chama COMANDOS
            COMANDOS();
        }//fim if else
    }// fim while

    // Fim do programa fonte
    CasaToken(TOKEN_EOF);
}// fim INICIO()

// Procedimento DECLARACOES()
// DECLARACOES -> (DECLA_VAR | DECLA_CONST) ;
// Parte semantica: sem acao semantica
void AnalisadorSintatico::DECLARACOES() {
    if (analisadorLexico.getToken() == TOKEN_CONST) {
        DECLA_CONST();
    } else {
        DECLA_VAR();
    }//fim if else

    CasaToken(TOKEN_PONTO_VIRGULA);
}// fim DECLARACOES()

// Procedimento DECLA_VAR()
// DECLA_VAR -> TIPO id [<- [-] constante] {, id [<- [-] constante]}
// Parte semantica: DECLA_VAR -> TIPO id 32 [<- [- 33] constante 34] {, id 32 [<- [- 33] constante 34]}
void AnalisadorSintatico::DECLA_VAR() {
    RegistroSemantico rsTIPO; // registro semantico auxiliar para TIPO
    TIPO(&rsTIPO);

    string lexemaID = analisadorLexico.getLexema(); // guarda o lexema do ID para buscar na tabela
    CasaToken(TOKEN_ID);   

    Simbolo sID = analisadorLexico.tabelaSimbolos.recuperarSimbolo(lexemaID); // recupera simbolo na tabela de simbolos
    // verifica se id nao foi declarado ainda
    if (sID.classe != CLASSE_VAZIA) {
        verificaErroTipo(analisadorLexico.num_linhas,2,lexemaID); // id ja declarado
    } else {
        sID.classe = CLASSE_VAR; // atribui classe do id
        sID.tipo = rsTIPO.tipo; // atribui tipo do id
        analisadorLexico.tabelaSimbolos.atualizarTabela(lexemaID,sID); // atualiza a tabela de simbolos
    }// fim if else

    // variaveis auxiliares para as analises semanticas
    bool tokenNegativo = false; 
    Token constanteTK; 
    if (analisadorLexico.getToken() == TOKEN_ATRIBUICAO) {
        CasaToken(TOKEN_ATRIBUICAO);

        if (analisadorLexico.getToken() == TOKEN_MENOS) {
            CasaToken(TOKEN_MENOS);

            // verifica se o tipo do id eh numerico
            if (sID.tipo != TIPO_INT && sID.tipo != TIPO_FLOAT) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            }// fim if

            tokenNegativo = true;
        }//fim if

        constanteTK = analisadorLexico.getRegistroLexico(); // salva uma copia do registro lexico para salvar o tipo da constante
        CasaToken(TOKEN_CONSTANTE);

        // verifica se tem o token negativo e se o tipo da constante eh numerico
        if (tokenNegativo && (constanteTK.tipo != TIPO_INT && constanteTK.tipo != TIPO_FLOAT)) {
            verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        }// fim if

        // se tipo do id for real, constante deve ser numerico
        if (sID.tipo == TIPO_FLOAT && (constanteTK.tipo != TIPO_INT && constanteTK.tipo != TIPO_FLOAT)) {
            verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        } // caso contrario, tipo do id e da constante devem ser identicos
        else if (sID.tipo != TIPO_FLOAT && sID.tipo != constanteTK.tipo) {
            verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        }// fim if else
    }// fim if

    while (analisadorLexico.getToken() == TOKEN_VIRGULA) {
        CasaToken(TOKEN_VIRGULA);

        lexemaID = analisadorLexico.getLexema(); // guarda o lexema do ID para buscar na tabela
        CasaToken(TOKEN_ID);

        sID = analisadorLexico.tabelaSimbolos.recuperarSimbolo(lexemaID); // recupera simbolo na tabela de simbolos
        // verifica se id nao foi declarado ainda
        if (sID.classe != CLASSE_VAZIA) {
            verificaErroTipo(analisadorLexico.num_linhas,2,lexemaID); // id ja declarado
        } else {
            sID.classe = CLASSE_VAR; // atribui classe do id
            sID.tipo = rsTIPO.tipo; // atribui tipo do id
            analisadorLexico.tabelaSimbolos.atualizarTabela(lexemaID,sID); // atualiza a tabela de simbolos
        }// fim if else

        tokenNegativo = false; // variavel auxiliar para as analises semanticas
        if (analisadorLexico.getToken() == TOKEN_ATRIBUICAO) {
            CasaToken(TOKEN_ATRIBUICAO);

            if (analisadorLexico.getToken() == TOKEN_MENOS) {
                CasaToken(TOKEN_MENOS);

                // verifica se o tipo do id eh numerico
                if (sID.tipo != TIPO_INT && sID.tipo != TIPO_FLOAT) {
                    verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
                }// fim if

                tokenNegativo = true;
            }//fim if

            constanteTK = analisadorLexico.getRegistroLexico(); // salva uma copia do registro lexico para salvar o tipo da constante
            CasaToken(TOKEN_CONSTANTE);

            // verifica se tem o token negativo e se o tipo da constante eh numerico
            if (tokenNegativo && (constanteTK.tipo != TIPO_INT && constanteTK.tipo != TIPO_FLOAT)) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            }// fim if
    
            // se tipo do id for real, constante deve ser numerico
            if (sID.tipo == TIPO_FLOAT && (constanteTK.tipo != TIPO_INT && constanteTK.tipo != TIPO_FLOAT)) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // caso contrario, tipo do id e da constante devem ser identicos
            else if (sID.tipo != constanteTK.tipo) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            }// fim if else
        }// fim if
    }//fim while

}// fim DECLA_VAR()

// Procedimento TIPO()
// TIPO -> int | float | string | char
// Parte semantica: TIPO -> int 28 | float 29 | string 30 | char 31
void AnalisadorSintatico::TIPO(RegistroSemantico* rsTIPO) {
    if (analisadorLexico.getToken() == TOKEN_INT) {
        CasaToken(TOKEN_INT);

        rsTIPO->tipo = TIPO_INT;
    } else if(analisadorLexico.getToken() == TOKEN_FLOAT) {
        CasaToken(TOKEN_FLOAT);

        rsTIPO->tipo = TIPO_FLOAT;
    } else if (analisadorLexico.getToken() == TOKEN_STRING) {
        CasaToken(TOKEN_STRING);
        
        rsTIPO->tipo = TIPO_STRING;
    } else { // tem que ser token char
        CasaToken(TOKEN_CHAR);
        
        rsTIPO->tipo = TIPO_CHAR;
    }//fim if else
}// fim TIPO()

// Procedimento DECLA_CONST()
// DECLA_CONST -> const id = [-] constante
// Parte semantica: DECLA_CONST -> const id 26 = [-] constante 27
void AnalisadorSintatico::DECLA_CONST() {
    CasaToken(TOKEN_CONST);

    string lexemaID = analisadorLexico.getLexema(); // guarda o lexema do ID para buscar na tabela
    CasaToken(TOKEN_ID);    

    Simbolo sID = analisadorLexico.tabelaSimbolos.recuperarSimbolo(lexemaID); // recupera simbolo na tabela de simbolos
    // verifica se id nao foi declarado ainda
    if (sID.classe != CLASSE_VAZIA) {
        verificaErroTipo(analisadorLexico.num_linhas,2,lexemaID); // id ja declarado
    } else {
        sID.classe = CLASSE_CONST; // atribui classe do id
    }// fim if else

    CasaToken(TOKEN_IGUAL);

    bool tokenNegativo = false; // variavel auxiliar para as analises semanticas
    if (analisadorLexico.getToken() == TOKEN_MENOS) {
        CasaToken(TOKEN_MENOS);
        tokenNegativo = true;
    }//fim if

    Token constanteTK = analisadorLexico.getRegistroLexico(); // salva uma copia do registro lexico para salvar o tipo da constante
    CasaToken(TOKEN_CONSTANTE);   

    // verifica se teve o token negativo
    if (tokenNegativo) {
        // constante deve ser do tipo numero
        if (constanteTK.tipo != TIPO_INT && constanteTK.tipo != TIPO_FLOAT) {
            verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        } // atribui o tipo do id constante de acordo com o tipo da constante
        else if (constanteTK.tipo == TIPO_INT) {
            sID.tipo = TIPO_INT;
        } else {
            sID.tipo = TIPO_FLOAT;
        }// fim if else
    } // verifica o tipo da constante para atribuir ao id constante 
    else if (constanteTK.tipo == TIPO_INT) {
        sID.tipo = TIPO_INT;
    } else if (constanteTK.tipo == TIPO_FLOAT) {
        sID.tipo = TIPO_FLOAT;
    } else if (constanteTK.tipo == TIPO_CHAR) {
        sID.tipo = TIPO_CHAR;
    } else if (constanteTK.tipo == TIPO_STRING) {
        sID.tipo = TIPO_STRING;
    } else {
        verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
    }// fim if else

    analisadorLexico.tabelaSimbolos.atualizarTabela(lexemaID,sID); // atualiza a tabela de simbolos
}// fim DECLA_CONST()

// Procedimento COMANDOS()
// COMANDOS -> [ATRIBUICAO | LEITURA | ESCRITA] ; | (REPETICAO | TESTE)
// Parte semantica: sem acoes semanticas
void AnalisadorSintatico::COMANDOS() {
    // Verifica se eh algum token first de cada procedimento para decidir qual das duas regras entrar
    if (analisadorLexico.getToken() == TOKEN_ID || analisadorLexico.getToken() == TOKEN_READLN ||
        analisadorLexico.getToken() == TOKEN_WRITE || analisadorLexico.getToken() == TOKEN_WRITELN ||
        analisadorLexico.getToken() == TOKEN_PONTO_VIRGULA) {

        // Verifica se eh alguma das producoes opcionais
        if (analisadorLexico.getToken() == TOKEN_ID) {
            ATRIBUICAO();
        } else if (analisadorLexico.getToken() == TOKEN_READLN) {
            LEITURA();
        } else if (analisadorLexico.getToken() == TOKEN_WRITE || analisadorLexico.getToken() == TOKEN_WRITELN) {
            ESCRITA();
        }//fim if else

        CasaToken(TOKEN_PONTO_VIRGULA);
    } 
    // Entra na segunda regra
    else {
        if (analisadorLexico.getToken() == TOKEN_WHILE) {
            REPETICAO();
        } else {
            TESTE();
        }//fim if else
    }// fim if else
    
}// fim COMANDOS()

// Procedimento ATRIBUICAO()
// ATRIBUICAO -> id [ "[" EXP "]" ] <- EXP
// Parte semantica: ATRIBUICAO -> id 22 [ "[" 6 EXP 24 "]" ] <- EXP 25
void AnalisadorSintatico::ATRIBUICAO() {
    string lexemaID = analisadorLexico.getLexema(); // guarda o lexema do ID para buscar na tabela
    CasaToken(TOKEN_ID);

    Simbolo sID = analisadorLexico.tabelaSimbolos.recuperarSimbolo(lexemaID); // recupera simbolo na tabela de simbolos
    // verifica se id eh variavel
    if (sID.classe == CLASSE_VAZIA) {
        verificaErroTipo(analisadorLexico.num_linhas,3,lexemaID); // id nao declarado
    } else if (sID.classe == CLASSE_CONST) {
        verificaErroTipo(analisadorLexico.num_linhas,4,lexemaID); // classe de id incompativel
    }// fim if else

    bool stringArranjo = false; // variavel auxiliar para as analises semanticas
    if (analisadorLexico.getToken() == TOKEN_ABRE_COLCH) {
        CasaToken(TOKEN_ABRE_COLCH);

        // variavel precisa ser do tipo string
        if (sID.tipo != TIPO_STRING) {
            verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        } // fim if
        
        RegistroSemantico rsEXP; // registro semantico auxiliar para EXP
        EXP(&rsEXP);

        // indice do arranjo precisa ser inteiro
        if (rsEXP.tipo != TIPO_INT) {
            verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        } // fim if

        CasaToken(TOKEN_FECHA_COLCH);

        stringArranjo = true;
    }//fim if

    CasaToken(TOKEN_ATRIBUICAO);

    RegistroSemantico rsEXP; // registro semantico auxiliar para EXP
    EXP(&rsEXP);

    // verifica se o tipo da expressao eh igual ao do id
    // id inteiro e exp inteira
    if (sID.tipo == TIPO_INT && rsEXP.tipo != TIPO_INT) {
        verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
    } // id real e exp numerica
    else if (sID.tipo == TIPO_FLOAT && (rsEXP.tipo != TIPO_INT && rsEXP.tipo != TIPO_FLOAT)) {
        verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
    } // id caractere e exp caractere
    else if (sID.tipo == TIPO_CHAR && rsEXP.tipo != TIPO_CHAR) {
        verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
    } // id string
    else if (sID.tipo == TIPO_STRING) {
        // se for posicao da string, exp precisa ser caractere
        if (stringArranjo && rsEXP.tipo != TIPO_CHAR) {
            verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        } // se nao for posicao da string, exp precisa ser string
        else if (!stringArranjo && rsEXP.tipo != TIPO_STRING) {
            verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        }// fim if else
    }// fim if else
}// fim ATRIBUICAO()

// Procedimento REPETICAO()
// REPETICAO -> while EXP ( COMANDOS | "{" {COMANDOS} "}" )
// Parte semantica: REPETICAO -> while EXP 23 ( COMANDOS | "{" {COMANDOS} "}" )
void AnalisadorSintatico::REPETICAO() {
    CasaToken(TOKEN_WHILE);

    RegistroSemantico rsEXP; // registro semantico auxiliar para EXP
    EXP(&rsEXP);

    // tipo da EXP deve ser logico
    if (rsEXP.tipo != TIPO_BOOL) {
        verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
    }//fim if 

    if (analisadorLexico.getToken() == TOKEN_ABRE_CHAVES) {
        CasaToken(TOKEN_ABRE_CHAVES);
        // Verifica se eh algum token first do procedimento COMANDOS, que pode ser chamado 0 ou mais vezes
        while (analisadorLexico.getToken() == TOKEN_ID || analisadorLexico.getToken() == TOKEN_READLN ||
               analisadorLexico.getToken() == TOKEN_WRITE || analisadorLexico.getToken() == TOKEN_WRITELN ||
               analisadorLexico.getToken() == TOKEN_PONTO_VIRGULA || analisadorLexico.getToken() == TOKEN_WHILE ||
               analisadorLexico.getToken() == TOKEN_IF) {

            COMANDOS();
        }// fim while
        CasaToken(TOKEN_FECHA_CHAVES);
    } else {
        COMANDOS();
    }// fim if else
}// fim REPETICAO()

// Procedimento TESTE()
// TESTE -> if EXP ( COMANDOS | "{" {COMANDOS} "}" ) [ else ( COMANDOS | "{" {COMANDOS} "}" ) ]
// Parte semantica: TESTE -> if EXP 23 ( COMANDOS | "{" {COMANDOS} "}" ) [ else ( COMANDOS | "{" {COMANDOS} "}" ) ]
void AnalisadorSintatico::TESTE() {
    CasaToken(TOKEN_IF);

    RegistroSemantico rsEXP; // registro semantico auxiliar para EXP
    EXP(&rsEXP);

    // tipo da EXP deve ser logico
    if (rsEXP.tipo != TIPO_BOOL) {
        verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
    }//fim if 

    if (analisadorLexico.getToken() == TOKEN_ABRE_CHAVES) {
        CasaToken(TOKEN_ABRE_CHAVES);
        // Verifica se eh algum token first do procedimento COMANDOS, que pode ser chamado 0 ou mais vezes
        while (analisadorLexico.getToken() == TOKEN_ID || analisadorLexico.getToken() == TOKEN_READLN ||
               analisadorLexico.getToken() == TOKEN_WRITE || analisadorLexico.getToken() == TOKEN_WRITELN ||
               analisadorLexico.getToken() == TOKEN_PONTO_VIRGULA || analisadorLexico.getToken() == TOKEN_WHILE ||
               analisadorLexico.getToken() == TOKEN_IF) {

            COMANDOS();
        }// fim while
        CasaToken(TOKEN_FECHA_CHAVES);
    } else {
        COMANDOS();
    }// fim if else

    if (analisadorLexico.getToken() == TOKEN_ELSE) {
        CasaToken(TOKEN_ELSE);

        if (analisadorLexico.getToken() == TOKEN_ABRE_CHAVES) {
            CasaToken(TOKEN_ABRE_CHAVES);
            // Verifica se eh algum token first do procedimento COMANDOS, que pode ser chamado 0 ou mais vezes
            while (analisadorLexico.getToken() == TOKEN_ID || analisadorLexico.getToken() == TOKEN_READLN ||
                   analisadorLexico.getToken() == TOKEN_WRITE || analisadorLexico.getToken() == TOKEN_WRITELN ||
                   analisadorLexico.getToken() == TOKEN_PONTO_VIRGULA || analisadorLexico.getToken() == TOKEN_WHILE ||
                   analisadorLexico.getToken() == TOKEN_IF) {

                COMANDOS();
            }// fim while
            CasaToken(TOKEN_FECHA_CHAVES);
        } else {
            COMANDOS();
        }// fim if else
    }// fim if
}// fim TESTE()

// Procedimento LEITURA()
// LEITURA -> readln "(" id ")"
// Parte semantica: LEITURA -> readln "(" id 22 ")"
void AnalisadorSintatico::LEITURA() {
    CasaToken(TOKEN_READLN);
    CasaToken(TOKEN_ABRE_PAREN);

    string lexemaID = analisadorLexico.getLexema(); // guarda o lexema do ID para buscar na tabela
    CasaToken(TOKEN_ID);

    Simbolo sID = analisadorLexico.tabelaSimbolos.recuperarSimbolo(lexemaID); // recupera simbolo na tabela de simbolos
    // verifica se id eh variavel
    if (sID.classe == CLASSE_VAZIA) {
        verificaErroTipo(analisadorLexico.num_linhas,3,lexemaID); // id nao declarado
    } else if (sID.classe == CLASSE_CONST) {
        verificaErroTipo(analisadorLexico.num_linhas,4,lexemaID); // classe de id incompativel
    }// fim if else

    CasaToken(TOKEN_FECHA_PAREN);
}// fim LEITURA()

// Procedimento ESCRITA()
// ESCRITA -> (write | writeln) "(" EXP {, EXP} ")"
// Parte semantica: ESCRITA -> (write | writeln) "(" EXP 21 {, EXP 21} ")"
void AnalisadorSintatico::ESCRITA() {
    if (analisadorLexico.getToken() == TOKEN_WRITE) {
        CasaToken(TOKEN_WRITE);
    } else {
        CasaToken(TOKEN_WRITELN);
    }//fim if else

    CasaToken(TOKEN_ABRE_PAREN);

    RegistroSemantico rsEXP; // registro semantico auxiliar para EXP
    EXP(&rsEXP);

    // tipo da EXP nao pode ser logico
    if (rsEXP.tipo == TIPO_BOOL) {
        verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
    }//fim if 

    while (analisadorLexico.getToken() == TOKEN_VIRGULA) {
        CasaToken(TOKEN_VIRGULA);
        EXP(&rsEXP);

        // tipo da EXP nao pode ser logico
        if (rsEXP.tipo == TIPO_BOOL) {
            verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        }//fim if 
    }// fim while

    CasaToken(TOKEN_FECHA_PAREN);
}// fim ESCRITA()

// Procedimento EXP()
// EXP -> EXPS [ (= | != | < | > | <= | >=) EXPS]
// Parte semantica: EXP -> EXPS [ (= | != | < | > | <= | >=) EXPS1 19] 20
void AnalisadorSintatico::EXP(RegistroSemantico* rsEXP) {
    RegistroSemantico rsEXPS; // registro semantico auxiliar para EXPS
    EXPS(&rsEXPS);

    RegistroSemantico rsEXPS1; // registro semantico auxiliar para EXPS1

    switch(analisadorLexico.getToken()) {
        case TOKEN_IGUAL:
            CasaToken(TOKEN_IGUAL);
            EXPS(&rsEXPS1);
            
            // verifica se a comparacao eh entre tipos numericos
            if ((rsEXPS.tipo == TIPO_INT || rsEXPS.tipo == TIPO_FLOAT) && (rsEXPS1.tipo != TIPO_INT && rsEXPS1.tipo != TIPO_FLOAT)) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // verifica se a comparacao eh entre caracteres
            else if (rsEXPS.tipo == TIPO_CHAR && rsEXPS1.tipo != TIPO_CHAR) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // verifica se a comparacao eh entre strings para o token igual
            else if (rsEXPS.tipo == TIPO_STRING && rsEXPS1.tipo != TIPO_STRING) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // atribui tipo final da comparacao caso nao encontrado nenhum erro
            else {
                rsEXPS.tipo = TIPO_BOOL;
            }//fim if else
            break;

        case TOKEN_DIFERENTE:
            CasaToken(TOKEN_DIFERENTE);
            EXPS(&rsEXPS1);

            // verifica se a comparacao eh entre tipos numericos
            if ((rsEXPS.tipo == TIPO_INT || rsEXPS.tipo == TIPO_FLOAT) && (rsEXPS1.tipo != TIPO_INT && rsEXPS1.tipo != TIPO_FLOAT)) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // verifica se a comparacao eh entre caracteres
            else if (rsEXPS.tipo == TIPO_CHAR && rsEXPS1.tipo != TIPO_CHAR) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // verifica se a comparacao eh com strings fora do token igual
            else if (rsEXPS.tipo == TIPO_STRING || rsEXPS1.tipo == TIPO_STRING) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // atribui tipo final da comparacao caso nao encontrado nenhum erro
            else {
                rsEXPS.tipo = TIPO_BOOL;
            }//fim if else
            break;

        case TOKEN_MENOR:
            CasaToken(TOKEN_MENOR);
            EXPS(&rsEXPS1);

            // verifica se a comparacao eh entre tipos numericos
            if ((rsEXPS.tipo == TIPO_INT || rsEXPS.tipo == TIPO_FLOAT) && (rsEXPS1.tipo != TIPO_INT && rsEXPS1.tipo != TIPO_FLOAT)) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // verifica se a comparacao eh entre caracteres
            else if (rsEXPS.tipo == TIPO_CHAR && rsEXPS1.tipo != TIPO_CHAR) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // verifica se a comparacao eh com strings fora do token igual
            else if (rsEXPS.tipo == TIPO_STRING || rsEXPS1.tipo == TIPO_STRING) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // atribui tipo final da comparacao caso nao encontrado nenhum erro
            else {
                rsEXPS.tipo = TIPO_BOOL;
            }//fim if else
            break;

        case TOKEN_MAIOR:
            CasaToken(TOKEN_MAIOR);
            EXPS(&rsEXPS1);

            // verifica se a comparacao eh entre tipos numericos
            if ((rsEXPS.tipo == TIPO_INT || rsEXPS.tipo == TIPO_FLOAT) && (rsEXPS1.tipo != TIPO_INT && rsEXPS1.tipo != TIPO_FLOAT)) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // verifica se a comparacao eh entre caracteres
            else if (rsEXPS.tipo == TIPO_CHAR && rsEXPS1.tipo != TIPO_CHAR) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // verifica se a comparacao eh com strings fora do token igual
            else if (rsEXPS.tipo == TIPO_STRING || rsEXPS1.tipo == TIPO_STRING) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // atribui tipo final da comparacao caso nao encontrado nenhum erro
            else {
                rsEXPS.tipo = TIPO_BOOL;
            }//fim if else
            break;

        case TOKEN_MENOR_IGUAL:
            CasaToken(TOKEN_MENOR_IGUAL);
            EXPS(&rsEXPS1);

            // verifica se a comparacao eh entre tipos numericos
            if ((rsEXPS.tipo == TIPO_INT || rsEXPS.tipo == TIPO_FLOAT) && (rsEXPS1.tipo != TIPO_INT && rsEXPS1.tipo != TIPO_FLOAT)) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // verifica se a comparacao eh entre caracteres
            else if (rsEXPS.tipo == TIPO_CHAR && rsEXPS1.tipo != TIPO_CHAR) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // verifica se a comparacao eh com strings fora do token igual
            else if (rsEXPS.tipo == TIPO_STRING || rsEXPS1.tipo == TIPO_STRING) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // atribui tipo final da comparacao caso nao encontrado nenhum erro
            else {
                rsEXPS.tipo = TIPO_BOOL;
            }//fim if else
            break;

        case TOKEN_MAIOR_IGUAL:
            CasaToken(TOKEN_MAIOR_IGUAL);
            EXPS(&rsEXPS1);

            // verifica se a comparacao eh entre tipos numericos
            if ((rsEXPS.tipo == TIPO_INT || rsEXPS.tipo == TIPO_FLOAT) && (rsEXPS1.tipo != TIPO_INT && rsEXPS1.tipo != TIPO_FLOAT)) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // verifica se a comparacao eh entre caracteres
            else if (rsEXPS.tipo == TIPO_CHAR && rsEXPS1.tipo != TIPO_CHAR) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // verifica se a comparacao eh com strings fora do token igual
            else if (rsEXPS.tipo == TIPO_STRING || rsEXPS1.tipo == TIPO_STRING) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } // atribui tipo final da comparacao caso nao encontrado nenhum erro
            else {
                rsEXPS.tipo = TIPO_BOOL;
            }//fim if else
            break;

        default:
            break;
            // Faz nada (lambda)
    }//fim switch

    rsEXP->tipo = rsEXPS.tipo; // devolve tipo final da expressao
}// fim EXP()

// Procedimento EXPS()
// EXPS -> [-] T { (+ | - | "||") T}
// Parte semantica: EXPS -> [-] T 14 { (+ 15 | - 15 | "||" 16) T1 17} 18
void AnalisadorSintatico::EXPS(RegistroSemantico* rsEXPS) {
    bool tokenNegativo = false; // variavel auxiliar para as analises semanticas

    if (analisadorLexico.getToken() == TOKEN_MENOS) {
        CasaToken(TOKEN_MENOS);
        tokenNegativo = true;
    }// fim if

    RegistroSemantico rsT; // registro semantico auxiliar para T
    T(&rsT);

    // se tem um - antes, o tipo de T deve ser numerico
    if (tokenNegativo && (rsT.tipo != TIPO_INT && rsT.tipo != TIPO_FLOAT)) {
        verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
    }//fim if

    bool tokenPositivo; // variavel auxiliar para as analises semanticas

    while (analisadorLexico.getToken() == TOKEN_MAIS || analisadorLexico.getToken() == TOKEN_MENOS ||
           analisadorLexico.getToken() == TOKEN_OR) {

        tokenNegativo = false;
        tokenPositivo = false;

        switch(analisadorLexico.getToken()) {
            case TOKEN_MAIS:
                CasaToken(TOKEN_MAIS);

                // se tem um + antes, o tipo de T deve ser numerico
                if (rsT.tipo != TIPO_INT && rsT.tipo != TIPO_FLOAT) {
                    verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
                }//fim if
                tokenPositivo = true;
                break;

            case TOKEN_MENOS:
                CasaToken(TOKEN_MENOS);

                // se tem um - antes, o tipo de T deve ser numerico
                if (rsT.tipo != TIPO_INT && rsT.tipo != TIPO_FLOAT) {
                    verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
                }//fim if
                tokenNegativo = true;
                break;

            default:
                CasaToken(TOKEN_OR);

                // se tem um OR antes, o tipo de T deve ser logico
                if (rsT.tipo != TIPO_BOOL) {
                    verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
                }//fim if
        }//fim switch

        RegistroSemantico rsT1; // registro semantico auxiliar para T1
        T(&rsT1);

        if (tokenNegativo || tokenPositivo) {
            // o tipo de T1 deve ser numerico
            if (rsT1.tipo != TIPO_INT && rsT1.tipo != TIPO_FLOAT) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } else if (rsT.tipo == TIPO_INT && rsT1.tipo == TIPO_INT) {
                rsT.tipo = TIPO_INT; // tipo inteiro caso operacao seja entre inteiros
            } else {
                rsT.tipo = TIPO_FLOAT; // tipo real caso algum dos operadores seja real
            }// fim if else
        } else if (rsT.tipo == TIPO_BOOL && rsT1.tipo != TIPO_BOOL) { // para OR ambos devem ser logicos
            verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        }//fim if else

    }// fim while

    rsEXPS->tipo = rsT.tipo; // devolve tipo final da expressao S
}// fim EXPS()

// Procedimento T()
// T -> F { (* | && | / | div | mod) F}
// Parte semantica: T -> F { (* 9 | && 10 | / 9 | div 11 | mod 11) F1 12 } 13
void AnalisadorSintatico::T(RegistroSemantico* rsT) {
    RegistroSemantico rsF; // registro semantico auxiliar para F
    F(&rsF);

    // variaveis auxiliares para as analises semanticas
    bool tokenDivisao;
    bool tokenMultiplicacao;

    while (analisadorLexico.getToken() == TOKEN_MULTIPLICACAO || analisadorLexico.getToken() == TOKEN_AND ||
           analisadorLexico.getToken() == TOKEN_DIVISAO       || analisadorLexico.getToken() == TOKEN_DIV ||
           analisadorLexico.getToken() == TOKEN_MOD) {

        tokenDivisao = false;
        tokenMultiplicacao = false;

        switch(analisadorLexico.getToken()) {
            case TOKEN_MULTIPLICACAO:
                CasaToken(TOKEN_MULTIPLICACAO);

                if (rsF.tipo != TIPO_INT && rsF.tipo != TIPO_FLOAT) { // precisa ser tipo numerico
                    verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
                }//fim if
                tokenMultiplicacao = true;
                break;

            case TOKEN_AND:
                CasaToken(TOKEN_AND);

                if (rsF.tipo != TIPO_BOOL) { // precisa ser tipo logico
                    verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
                }//fim if
                break;

            case TOKEN_DIVISAO:
                CasaToken(TOKEN_DIVISAO);

                if (rsF.tipo != TIPO_INT && rsF.tipo != TIPO_FLOAT) { // precisa ser tipo numerico
                    verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
                }//fim if
                tokenDivisao = true;
                break;

            case TOKEN_DIV:
                CasaToken(TOKEN_DIV);

                if (rsF.tipo != TIPO_INT ) { // precisa ser tipo numerico inteiro
                    verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
                }//fim if
                break;

            default:
                CasaToken(TOKEN_MOD);

                if (rsF.tipo != TIPO_INT ) { // precisa ser tipo numerico inteiro
                    verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
                }//fim if
        }//fim switch

        RegistroSemantico rsF1; // registro semantico auxiliar para F1
        F(&rsF1);
        if (tokenDivisao || tokenMultiplicacao) { // verifica se resultado final eh inteiro ou real
            if (rsF1.tipo != TIPO_INT && rsF1.tipo != TIPO_FLOAT)  { // precisa ser tipo numerico
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } else if (tokenMultiplicacao) {
                if (rsF.tipo == TIPO_INT && rsF1.tipo == TIPO_INT) {
                    rsF.tipo = TIPO_INT; // caso seja mult e ambos fatores sejam inteiros, o resultado final eh inteiro
                } else {
                    rsF.tipo = TIPO_FLOAT; // caso seja mult e algum fator seja real, o resultado final eh real
                }// fim if else
            } else {
                rsF.tipo = TIPO_FLOAT; // caso seja divisao, o resultado final eh real
            }// fim if else
        } else if (rsF.tipo == TIPO_INT && rsF1.tipo != TIPO_INT) { // realiza div ou mod e so aceita tipos inteiros. resultado inteiro
            verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        } else if (rsF.tipo == TIPO_BOOL && rsF1.tipo != TIPO_BOOL) { // realiza AND binario e so aceita tipos logicos. resultado logico
            verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        }// fim else if
    }// fim while

    rsT->tipo = rsF.tipo; // atribui o tipo da expressao final de F ao tipo de T
} // fim T()

// Procedimento F()
// F -> ! F | (int | float) "(" EXP ")" | "(" EXP ")" | id [ "[" EXP "]" ] | constante
// Parte semantica: F -> ! F1 1 | (int | float) 2 "(" EXP 3 ")" | "(" EXP 4 ")" | id 5 [ "[" 6 EXP 7 "]" ] | constante 8
void AnalisadorSintatico::F(RegistroSemantico* rsF) {
    if (analisadorLexico.getToken() == TOKEN_NEGADO) {
        CasaToken(TOKEN_NEGADO);

        RegistroSemantico rsF1; // registro semantico auxiliar para F1
        F(&rsF1);
        if (rsF1.tipo != TIPO_BOOL) {
            verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        } else {
            rsF->tipo = rsF1.tipo;
        }// fim if else

    } else if (analisadorLexico.getToken() == TOKEN_INT || analisadorLexico.getToken() == TOKEN_FLOAT) {
        if (analisadorLexico.getToken() == TOKEN_INT) {
            CasaToken(TOKEN_INT);

            rsF->tipo = TIPO_INT; // define tipo do F a ser retornado
        } else {
            CasaToken(TOKEN_FLOAT);

            rsF->tipo = TIPO_FLOAT; // define tipo do F a ser retornado
        }//fim if else

        CasaToken(TOKEN_ABRE_PAREN);

        RegistroSemantico rsEXP; // registro semantico auxiliar para EXP
        EXP(&rsEXP);
        if (rsEXP.tipo != TIPO_INT && rsEXP.tipo != TIPO_FLOAT) {
            verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        }// fim if

        CasaToken(TOKEN_FECHA_PAREN);
    } else if (analisadorLexico.getToken() == TOKEN_ABRE_PAREN) {
        CasaToken(TOKEN_ABRE_PAREN);

        RegistroSemantico rsEXP; // registro semantico auxiliar para EXP
        EXP(&rsEXP);
        rsF->tipo = rsEXP.tipo; // atribui tipo da EXP para o tipo de F

        CasaToken(TOKEN_FECHA_PAREN);
    } else if (analisadorLexico.getToken() == TOKEN_ID) {
        string lexemaID = analisadorLexico.getLexema(); // guarda o lexema do ID para buscar na tabela

        CasaToken(TOKEN_ID);

        Simbolo sID = analisadorLexico.tabelaSimbolos.recuperarSimbolo(lexemaID); // recupera simbolo na tabela de simbolos
        // verifica se id ja foi declarado
        if (sID.classe == CLASSE_VAZIA) {
            verificaErroTipo(analisadorLexico.num_linhas,3,lexemaID); // id nao declarado
        } else {
            rsF->tipo = sID.tipo; // atribui tipo do id ao tipo do F
        }// fim if else

        if (analisadorLexico.getToken() == TOKEN_ABRE_COLCH) {
            CasaToken(TOKEN_ABRE_COLCH);

            // para ter indice, id precisa ser de string
            if (sID.tipo != TIPO_STRING) {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            }// fim if

            RegistroSemantico rsEXP; // registro semantico auxiliar para EXP
            EXP(&rsEXP);
            if (rsEXP.tipo == TIPO_INT) { // tipo do indice
                rsF->tipo = TIPO_CHAR; // F esta devolvendo um valor char
            } else {
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            }// fim if else

            CasaToken(TOKEN_FECHA_COLCH);
        }//fim if
    } else {
        Token constanteTK = analisadorLexico.getRegistroLexico(); // salva uma copia do registro lexico para salvar o tipo da constante

        CasaToken(TOKEN_CONSTANTE);

        rsF->tipo = constanteTK.tipo; // F esta devolvendo um valor do tipo da constante lida
    }// fim if else
}// fim F()

// Verifica o tipo de erro para enviar
void AnalisadorSintatico::verificaTokenNaoEsp(int num_linhas) {
    // Verifica se foi EOF nao esperado e envia erro
    if (analisadorLexico.getToken() == TOKEN_EOF) {
        erro.eofNaoEsperado(num_linhas);
    } 
    // Se nao foi erro de outro token nao esperado
    else {
        erro.tokenNaoEsperado(num_linhas,analisadorLexico.getLexema());
    }// fim if else

    throw ASError();
}// fim verificaTokenNaoEsp()

// Verifica o tipo de erro semantico para enviar
void AnalisadorSintatico::verificaErroTipo(int num_linhas, int indice, string lexema) {
    // Envia mensagem de erro de acordo com o indice recebido
    switch (indice) {
        case 1:
            erro.tipoIncompativel(num_linhas);
            break;
        case 2:
            erro.idJaDeclarado(num_linhas,lexema);
            break;
        case 3:
            erro.idNaoDeclarado(num_linhas,lexema);
            break;
        case 4:
            erro.classeIdIncompativel(num_linhas,lexema);
    }//fim switch

    throw ASError();
}// fim verificaTokenNaoEsp()

//main
int main(int argc, char *argv[]) { 

   AnalisadorSintatico analisadorSintatico;

   // Inicializando analisador sintatico/lexico
   try {
      analisadorSintatico.analisadorLexico.analisar();
      analisadorSintatico.INICIO();
      cout << analisadorSintatico.analisadorLexico.num_linhas << " linhas compiladas." << endl;
   } catch (ASError e) {
      // Programa abortado devido a um erro
   } // fim try catch   

   return 0;
}
