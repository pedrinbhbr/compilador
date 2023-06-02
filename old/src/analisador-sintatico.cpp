/**
 *  
 * Analisador Sintático.
 * 
 * */

#include "analisador-sintatico.h"

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
void AnalisadorSintatico::DECLA_VAR() {
    TIPO();

    CasaToken(TOKEN_ID);

    if (analisadorLexico.getToken() == TOKEN_ATRIBUICAO) {
        CasaToken(TOKEN_ATRIBUICAO);

        if (analisadorLexico.getToken() == TOKEN_MENOS) {
            CasaToken(TOKEN_MENOS);
        }//fim if

        CasaToken(TOKEN_CONSTANTE);
    }// fim if

    while (analisadorLexico.getToken() == TOKEN_VIRGULA) {
        CasaToken(TOKEN_VIRGULA);
        CasaToken(TOKEN_ID);

        if (analisadorLexico.getToken() == TOKEN_ATRIBUICAO) {
            CasaToken(TOKEN_ATRIBUICAO);

            if (analisadorLexico.getToken() == TOKEN_MENOS) {
                CasaToken(TOKEN_MENOS);
            }//fim if

            CasaToken(TOKEN_CONSTANTE);
        }// fim if
    }//fim while

}// fim DECLA_VAR()

// Procedimento TIPO()
// TIPO -> int | float | string | char
void AnalisadorSintatico::TIPO() {
    if (analisadorLexico.getToken() == TOKEN_INT) {
        CasaToken(TOKEN_INT);
    } else if(analisadorLexico.getToken() == TOKEN_FLOAT) {
        CasaToken(TOKEN_FLOAT);
    } else if (analisadorLexico.getToken() == TOKEN_STRING) {
        CasaToken(TOKEN_STRING);
    } else { // tem que ser token char
        CasaToken(TOKEN_CHAR);
    }//fim if else
}// fim TIPO()

// Procedimento DECLA_CONST()
// DECLA_CONST -> const id = [-] constante
void AnalisadorSintatico::DECLA_CONST() {
    CasaToken(TOKEN_CONST);
    CasaToken(TOKEN_ID);
    CasaToken(TOKEN_IGUAL);

    if (analisadorLexico.getToken() == TOKEN_MENOS) {
        CasaToken(TOKEN_MENOS);
    }//fim if

    CasaToken(TOKEN_CONSTANTE);
}// fim DECLA_CONST()

// Procedimento COMANDOS()
// COMANDOS -> [ATRIBUICAO | LEITURA | ESCRITA] ; | (REPETICAO | TESTE)
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
void AnalisadorSintatico::ATRIBUICAO() {
    CasaToken(TOKEN_ID);

    if (analisadorLexico.getToken() == TOKEN_ABRE_COLCH) {
        CasaToken(TOKEN_ABRE_COLCH);
        
        EXP();

        CasaToken(TOKEN_FECHA_COLCH);
    }//fim if

    CasaToken(TOKEN_ATRIBUICAO);

    EXP();
}// fim ATRIBUICAO()

// Procedimento REPETICAO()
// REPETICAO -> while EXP ( COMANDOS | "{" {COMANDOS} "}" )
void AnalisadorSintatico::REPETICAO() {
    CasaToken(TOKEN_WHILE);

    EXP();

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
void AnalisadorSintatico::TESTE() {
    CasaToken(TOKEN_IF);

    EXP();

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
void AnalisadorSintatico::LEITURA() {
    CasaToken(TOKEN_READLN);
    CasaToken(TOKEN_ABRE_PAREN);
    CasaToken(TOKEN_ID);
    CasaToken(TOKEN_FECHA_PAREN);
}// fim LEITURA()

// Procedimento ESCRITA()
// ESCRITA -> (write | writeln) "(" EXP {, EXP} ")"
void AnalisadorSintatico::ESCRITA() {
    if (analisadorLexico.getToken() == TOKEN_WRITE) {
        CasaToken(TOKEN_WRITE);
    } else {
        CasaToken(TOKEN_WRITELN);
    }//fim if else

    CasaToken(TOKEN_ABRE_PAREN);
    EXP();

    while (analisadorLexico.getToken() == TOKEN_VIRGULA) {
        CasaToken(TOKEN_VIRGULA);
        EXP();
    }// fim while

    CasaToken(TOKEN_FECHA_PAREN);
}// fim ESCRITA()

// Procedimento EXP()
// EXP -> EXPS [ (= | != | < | > | <= | >=) EXPS]
void AnalisadorSintatico::EXP() {
    EXPS();

    switch(analisadorLexico.getToken()) {
        case TOKEN_IGUAL:
            CasaToken(TOKEN_IGUAL);
            EXPS();
            break;

        case TOKEN_DIFERENTE:
            CasaToken(TOKEN_DIFERENTE);
            EXPS();
            break;

        case TOKEN_MENOR:
            CasaToken(TOKEN_MENOR);
            EXPS();
            break;

        case TOKEN_MAIOR:
            CasaToken(TOKEN_MAIOR);
            EXPS();
            break;

        case TOKEN_MENOR_IGUAL:
            CasaToken(TOKEN_MENOR_IGUAL);
            EXPS();
            break;

        case TOKEN_MAIOR_IGUAL:
            CasaToken(TOKEN_MAIOR_IGUAL);
            EXPS();
            break;

        default:
            break;
            // Faz nada (lambda)
    }//fim switch

}// fim EXP()

// Procedimento EXPS()
// EXPS -> [-] T { (+ | - | "||") T}
void AnalisadorSintatico::EXPS() {
    if (analisadorLexico.getToken() == TOKEN_MENOS) {
        CasaToken(TOKEN_MENOS);
    }// fim if

    T();

    while (analisadorLexico.getToken() == TOKEN_MAIS || analisadorLexico.getToken() == TOKEN_MENOS ||
           analisadorLexico.getToken() == TOKEN_OR) {

        switch(analisadorLexico.getToken()) {
            case TOKEN_MAIS:
                CasaToken(TOKEN_MAIS);
                break;

            case TOKEN_MENOS:
                CasaToken(TOKEN_MENOS);
                break;

            default:
                CasaToken(TOKEN_OR);
        }//fim switch

        T();
    }// fim while
}// fim EXPS()

// Procedimento T()
// T -> F { (* | && | / | div | mod) F}
void AnalisadorSintatico::T() {
    F();

    while (analisadorLexico.getToken() == TOKEN_MULTIPLICACAO || analisadorLexico.getToken() == TOKEN_AND ||
           analisadorLexico.getToken() == TOKEN_DIVISAO       || analisadorLexico.getToken() == TOKEN_DIV ||
           analisadorLexico.getToken() == TOKEN_MOD) {

        switch(analisadorLexico.getToken()) {
            case TOKEN_MULTIPLICACAO:
                CasaToken(TOKEN_MULTIPLICACAO);
                break;

            case TOKEN_AND:
                CasaToken(TOKEN_AND);
                break;

            case TOKEN_DIVISAO:
                CasaToken(TOKEN_DIVISAO);
                break;

            case TOKEN_DIV:
                CasaToken(TOKEN_DIV);
                break;

            default:
                CasaToken(TOKEN_MOD);
        }//fim switch

        F();
    }// fim while
} // fim T()

// Procedimento F()
// F -> ! F | (int | float) "(" EXP ")" | "(" EXP ")" | id [ "[" EXP "]" ] | constante
void AnalisadorSintatico::F() {
    if (analisadorLexico.getToken() == TOKEN_NEGADO) {
        CasaToken(TOKEN_NEGADO);
        F();
    } else if (analisadorLexico.getToken() == TOKEN_INT || analisadorLexico.getToken() == TOKEN_FLOAT) {
        if (analisadorLexico.getToken() == TOKEN_INT) {
            CasaToken(TOKEN_INT);
        } else {
            CasaToken(TOKEN_FLOAT);
        }//fim if else

        CasaToken(TOKEN_ABRE_PAREN);
        EXP();
        CasaToken(TOKEN_FECHA_PAREN);
    } else if (analisadorLexico.getToken() == TOKEN_ABRE_PAREN) {
        CasaToken(TOKEN_ABRE_PAREN);
        EXP();
        CasaToken(TOKEN_FECHA_PAREN);
    } else if (analisadorLexico.getToken() == TOKEN_ID) {
        CasaToken(TOKEN_ID);
        if (analisadorLexico.getToken() == TOKEN_ABRE_COLCH) {
            CasaToken(TOKEN_ABRE_COLCH);
            EXP();
            CasaToken(TOKEN_FECHA_COLCH);
        }//fim if
    } else {
        CasaToken(TOKEN_CONSTANTE);
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