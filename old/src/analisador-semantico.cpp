/**
 *  
 * Analisador Sintático + Semântico.
 * 
 * */

#include "analisador-semantico.h"

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