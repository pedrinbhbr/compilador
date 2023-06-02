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
 * Analisador Sintático + Semântico + Geração de Código.
 * 
 * */

#include "tradutor.h"

// Metodo que inicia a compilacao do programa. Inicia a criacao do arquivo .asm
// que sera usado para converter o codigo em instrucoes de maquina.
void AnalisadorSintatico::Compila(){

    try {

        // Inicio do codigo assembly (A1).
        codigoAssembly.append(";Ciência da Computação - PUC Coração Eucarístico - Compiladores - 2/2021\n");
        codigoAssembly.append(";Igor Machado Seixas, Matheus Mendes Tavares e Pedro Henrique Ferreira de Souza\n\n");
        codigoAssembly.append("; Sessão de dados\n");
        codigoAssembly.append("section .data\n");
        codigoAssembly.append("M: \n");
        codigoAssembly.append("\tresb 10000h\n");
        codigoAssembly.append("section .text\t\t; Sessão de código\n");
        codigoAssembly.append("global _start \t\t; Ponto inicial do programa\n");
        codigoAssembly.append("_start:\t\t; Início do programa\n\n");

        analisadorLexico.analisar();
        INICIO();
        cout << analisadorLexico.num_linhas << " linhas compiladas." << endl;

        // Finalizando processo de criacao do codigo (A2).
        codigoAssembly.append("\n; Halt\n");
        codigoAssembly.append("\tmov rax, 60 \t\t; Chamada de saída\n");
        codigoAssembly.append("\tmov rdi, 0 \t\t; Código de saida sem erros\n");
        codigoAssembly.append("\tsyscall \t\t; Chama o kernel\n");
  
        // Cria arquivo e escreve seu conteúdo
        ofstream arquivoASM("saida.asm");
        arquivoASM << codigoAssembly << endl;
        arquivoASM.close();

    } catch (ASError e) {
      // Programa abortado devido a um erro
    } // fim try catch
    
}// fim Compila()

// Devolve proximo endereco disponivel na area de dados
// E atualiza o proximo endereco disponivel na area de dados
int AnalisadorSintatico::NovoDado(int bytes) {
    int endDisponivel = contadorDados; // salva endereco disponivel na area de dados
    contadorDados += bytes; // acrescenta a quantidade de bytes que sera usado e atualiza proxima area de dados disponivel

    return endDisponivel;
}// fim NovoDado()

// Devolve proximo endereco disponivel na area de temporarios
// E atualiza o proximo endereco disponivel na area de temporarios
int AnalisadorSintatico::NovoTemp(int bytes) {
    int endDisponivel = contadorTemps; // salva endereco disponivel na area de temporarios
    contadorTemps += bytes; // acrescenta a quantidade de bytes que sera usado e atualiza proxima area de temporarios disponivel

    return endDisponivel;
}// fim NovoTemp()

// Devolve proximo novo rotulo a ser utilizado
// E atualiza o proximo
string AnalisadorSintatico::NovoRot() {
    string rotulo = string("Rot" + to_string(indiceRotulos)); // monta string do Rotulo
    indiceRotulos++; // acrescenta indice dos Rotulos

    return rotulo;
}// fim NovoRot()

// Devolve valor do endereco em hexadecimal
string AnalisadorSintatico::EnderecoHex(int end) {
    char endHex[40]; // vetor auxiliar para formatar inteiro em hexadecimal
    sprintf(endHex,"0x%X",end); // formata inteiro em uma string com o valor em hexadecimal e precedido de 0x

    return string(endHex);
}// fim EnderecoHex()

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
// Parte semantica: (A1) {DECLARACOES | COMANDOS} EOF (A2)
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
// Parte semantica: TIPO id (32)(A3) [<- [- (33)] constante (34)(A4)] {, id (32)(A3) [<- [- (33)] constante (34)(A4)]}
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
        //analisadorLexico.tabelaSimbolos.atualizarTabela(lexemaID,sID); // atualiza a tabela de simbolos
    }// fim if else

    // Reserva memoria para a variavel declarada
    codigoAssembly.append("\nsection .data \t\t; Abrindo sessão de dados\n");
    int end; // auxiliar para salvar endereco do id
    if (sID.tipo == TIPO_INT || sID.tipo == TIPO_FLOAT) {        
        codigoAssembly.append("\tresd 1 \t\t; Alocando 4 bytes\n");
        end = NovoDado(4); // salva endereco disponivel e atualiza proximo endereco disponivel
    } else if (sID.tipo == TIPO_CHAR) {
        codigoAssembly.append("\tresb 1 \t\t; Alocando 1 bytes\n"); 
        end = NovoDado(1); // salva endereco disponivel e atualiza proximo endereco disponivel
    } else {
        codigoAssembly.append("\tresb 100h \t\t; Alocando 256 bytes\n"); 
        end = NovoDado(256); // salva endereco disponivel e atualiza proximo endereco disponivel
    }
    codigoAssembly.append("\nsection .text \t\t; Reabrindo sessão do código\n");
    // Atualiza tabela de simbolos
    sID.endereco = end;
    sID.tamanho = 0;
    analisadorLexico.tabelaSimbolos.atualizarTabela(lexemaID,sID); // atualiza a tabela de simbolos

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

        // Atribui valor no endereco do id
        if (sID.tipo == TIPO_INT) {
            codigoAssembly.append("\tmov eax," + constanteTK.lexema + " \t\t; Movendo valor da constante para um reg\n");
            if (tokenNegativo) {
                codigoAssembly.append("\tneg eax \t\t; Nega valor do reg caso tenha um menos\n");
            }//fim if
            codigoAssembly.append("\tmov [qword M+" + EnderecoHex(sID.endereco) + "],eax \t\t; Atribui o valor do reg ao id\n");
        } else if (sID.tipo == TIPO_CHAR) {
            codigoAssembly.append("\tmov al," + constanteTK.lexema + " \t\t; Movendo valor da constante para um reg\n");
            codigoAssembly.append("\tmov [qword M+" + EnderecoHex(sID.endereco) + "],al \t\t; Atribui o valor do reg ao id\n");
        } else if (sID.tipo == TIPO_FLOAT) {
            codigoAssembly.append("\nsection .data \t\t; Abrindo sessão de dados\n");
            // Acrescenta 0 no float caso o lexema comece com ponto
            if (constanteTK.lexema[0] == '.') {
                constanteTK.lexema = '0' + constanteTK.lexema;
            } //fim if
            if (tokenNegativo) {
                codigoAssembly.append("\tdd -" + constanteTK.lexema + " \t\t; Alocando 4 bytes e atribuindo valor\n");
            } else {
                codigoAssembly.append("\tdd " + constanteTK.lexema + " \t\t; Alocando 4 bytes e atribuindo valor\n");
            }//fim if else
            codigoAssembly.append("section .text \t\t; Reabrindo sessão do código\n");
            int floatEndereco = NovoDado(4); // salva endereco disponivel e atualiza proximo endereco disponivel
            codigoAssembly.append("\tmovss xmm0,[qword M+" + EnderecoHex(floatEndereco) + "] \t\t; Atribui valor da constante armazenada ao reg\n");
            codigoAssembly.append("\tmovss [qword M+" + EnderecoHex(sID.endereco) + "],xmm0 \t\t; Atribui valor recuperado ao id\n");
        } else {
            codigoAssembly.append("\nsection .data \t\t; Abrindo sessão de dados\n");
            codigoAssembly.append("\tdb " + constanteTK.lexema + ",0 \t\t; Alocando o tamanho da string+1 em bytes e atribuindo valor\n");
            codigoAssembly.append("\nsection .text \t\t; Reabrindo sessão do código\n");
            // tamanho da string + o 0h do fim de string
            int stringEndereco = NovoDado(constanteTK.tamanho+1); // salva endereco disponivel e atualiza proximo endereco disponivel
            
            sID.tamanho = constanteTK.tamanho; // tamanho da string.
            analisadorLexico.tabelaSimbolos.atualizarTabela(lexemaID,sID); // atualiza a tabela de simbolos
            
            codigoAssembly.append("\tmov rsi,qword M+" + EnderecoHex(stringEndereco) + "\t\t; Salva endereço da constante no rsi\n");
            codigoAssembly.append("\tmov rdi,qword M+" + EnderecoHex(sID.endereco) + "\t\t; Salva endereço do id no rdi\n");
            string loopString = NovoRot();
            codigoAssembly.append(loopString + ": \t\t; Rotulo para loop de leitura da string\n");
            codigoAssembly.append("\tmov al,[rsi] \t\t; Recupera um byte da string\n");
            codigoAssembly.append("\tmov [rdi],al \t\t; Salva byte recuperado da string no id\n");
            codigoAssembly.append("\tadd rsi,1 \t\t; Incrementa posicao em 1 byte\n");
            codigoAssembly.append("\tadd rdi,1 \t\t; Incrementa posicao em 1 byte\n");
            codigoAssembly.append("\tcmp al,0 \t\t; Verifica se é fim de string\n");
            codigoAssembly.append("\tjne " + loopString + "\t\t; Se não for, realiza o loop para o próximo byte\n");
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
            //analisadorLexico.tabelaSimbolos.atualizarTabela(lexemaID,sID); // atualiza a tabela de simbolos
        }// fim if else

        // COPIA DA PARTE DE CIMA. MESMAS ACOES
        // Reserva memoria para a variavel declarada
        codigoAssembly.append("\nsection .data \t\t; Abrindo sessão de dados\n");
        int end; // auxiliar para salvar endereco do id
        if (sID.tipo == TIPO_INT || sID.tipo == TIPO_FLOAT) {        
            codigoAssembly.append("\tresd 1 \t\t; Alocando 4 bytes\n");
            end = NovoDado(4); // salva endereco disponivel e atualiza proximo endereco disponivel
        } else if (sID.tipo == TIPO_CHAR) {
            codigoAssembly.append("\tresb 1 \t\t; Alocando 1 bytes\n"); 
            end = NovoDado(1); // salva endereco disponivel e atualiza proximo endereco disponivel
        } else {
            codigoAssembly.append("\tresb 100h \t\t; Alocando 256 bytes\n"); 
            end = NovoDado(256); // salva endereco disponivel e atualiza proximo endereco disponivel
        }
        codigoAssembly.append("\nsection .text \t\t; Reabrindo sessão do código\n");
        // Atualiza tabela de simbolos
        sID.endereco = end;
        sID.tamanho = 0;
        analisadorLexico.tabelaSimbolos.atualizarTabela(lexemaID,sID); // atualiza a tabela de simbolos

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

            // COPIA DA PARTE DE CIMA. MESMAS ACOES
            // Atribui valor no endereco do id
            if (sID.tipo == TIPO_INT) {
                codigoAssembly.append("\tmov eax," + constanteTK.lexema + " \t\t; Movendo valor da constante para um reg\n");
                if (tokenNegativo) {
                    codigoAssembly.append("\tneg eax \t\t; Nega valor do reg caso tenha um menos\n");
                }//fim if
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(sID.endereco) + "],eax \t\t; Atribui o valor do reg ao id\n");
            } else if (sID.tipo == TIPO_CHAR) {
                codigoAssembly.append("\tmov al," + constanteTK.lexema + " \t\t; Movendo valor da constante para um reg\n");
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(sID.endereco) + "],al \t\t; Atribui o valor do reg ao id\n");
            } else if (sID.tipo == TIPO_FLOAT) {
                codigoAssembly.append("\nsection .data \t\t; Abrindo sessão de dados\n");
                // Acrescenta 0 no float caso o lexema comece com ponto
                if (constanteTK.lexema[0] == '.') {
                    constanteTK.lexema = '0' + constanteTK.lexema;
                } //fim if
                if (tokenNegativo) {
                    codigoAssembly.append("\tdd -" + constanteTK.lexema + " \t\t; Alocando 4 bytes e atribuindo valor\n");
                } else {
                    codigoAssembly.append("\tdd " + constanteTK.lexema + " \t\t; Alocando 4 bytes e atribuindo valor\n");
                }//fim if else
                codigoAssembly.append("section .text \t\t; Reabrindo sessão do código\n");
                int floatEndereco = NovoDado(4); // salva endereco disponivel e atualiza proximo endereco disponivel
                codigoAssembly.append("\tmovss xmm0,[qword M+" + EnderecoHex(floatEndereco) + "] \t\t; Atribui valor da constante armazenada ao reg\n");
                codigoAssembly.append("\tmovss [qword M+" + EnderecoHex(sID.endereco) + "],xmm0 \t\t; Atribui valor recuperado ao id\n");
            } else {
                codigoAssembly.append("\nsection .data \t\t; Abrindo sessão de dados\n");
                codigoAssembly.append("\tdb " + constanteTK.lexema + ",0 \t\t; Alocando o tamanho da string+1 em bytes e atribuindo valor\n");
                codigoAssembly.append("\nsection .text \t\t; Reabrindo sessão do código\n");
                // tamanho da string + o 0h do fim de string
                int stringEndereco = NovoDado(constanteTK.tamanho+1); // salva endereco disponivel e atualiza proximo endereco disponivel
                
                sID.tamanho = constanteTK.tamanho; // tamanho da string.
                analisadorLexico.tabelaSimbolos.atualizarTabela(lexemaID,sID); // atualiza a tabela de simbolos
                
                codigoAssembly.append("\tmov rsi,qword M+" + EnderecoHex(stringEndereco) + "\t\t; Salva endereço da constante no rsi\n");
                codigoAssembly.append("\tmov rdi,qword M+" + EnderecoHex(sID.endereco) + "\t\t; Salva endereço do id no rdi\n");
                string loopString = NovoRot();
                codigoAssembly.append(loopString + ": \t\t; Rotulo para loop de leitura da string\n");
                codigoAssembly.append("\tmov al,[rsi] \t\t; Recupera um byte da string\n");
                codigoAssembly.append("\tmov [rdi],al \t\t; Salva byte recuperado da string no id\n");
                codigoAssembly.append("\tadd rsi,1 \t\t; Incrementa posicao em 1 byte\n");
                codigoAssembly.append("\tadd rdi,1 \t\t; Incrementa posicao em 1 byte\n");
                codigoAssembly.append("\tcmp al,0 \t\t; Verifica se é fim de string\n");
                codigoAssembly.append("\tjne " + loopString + "\t\t; Se não for, realiza o loop para o próximo byte\n");
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
// Parte semantica: DECLA_CONST -> const id (26) = [-] constante (27)(A5)
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
        sID.tamanho = 0; // inicializa tamanho do id para utilizar em string
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

    // Reserva memoria para a constante declarada
    int Endereco;
    codigoAssembly.append("\nsection .data \t\t; Abrindo sessão de dados\n");
    if (sID.tipo == TIPO_INT || sID.tipo == TIPO_FLOAT){
        // Acrescenta 0 no float caso o lexema comece com ponto
        if (constanteTK.lexema[0] == '.') {
            constanteTK.lexema = '0' + constanteTK.lexema;
        } //fim if

        if (tokenNegativo) {
            codigoAssembly.append("\tdd -" + constanteTK.lexema + "\t\t; Alocando 4 bytes e atribuindo valor\n");
        } else {
            codigoAssembly.append("\tdd " + constanteTK.lexema + "\t\t; Alocando 4 bytes e atribuindo valor\n");
        }//fim if else
        
        Endereco = NovoDado(4); // salva endereco disponivel e atualiza proximo endereco disponivel
    } else if (sID.tipo == TIPO_CHAR){
        codigoAssembly.append("\tdb " + constanteTK.lexema + "\t\t; Alocando 1 bytes e atribuindo valor\n");
        Endereco = NovoDado(1);
    } else { // (sID.tipo == TIPO_STRING)
        codigoAssembly.append("\tdb " + constanteTK.lexema + ",0 \t\t; Alocando 1 bytes e atribuindo valor\n");
        Endereco = NovoDado(constanteTK.tamanho+1);
        sID.tamanho = constanteTK.tamanho; // tamanho da string.
    }
    codigoAssembly.append("\nsection .text \t\t; Reabrindo sessão do código\n");

    sID.endereco = Endereco;
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
// Parte semantica: ATRIBUICAO -> id (22) (A10) [ "[" (6) EXP (24)(A16) "]" ] <- EXP (25)(A17)
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
    int endIndiceString = 0; // variavel auxiliar para salvar endereco que contem o indice da string
    contadorTemps = 0; // libera memoria dos temporarios
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

        endIndiceString = NovoTemp(4); // recupera endereco de memoria e aloca a proxima disponivel na area de temps
        codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXP.endereco) + "] \t\t; Recupera o indice da string\n");
        codigoAssembly.append("\tmov [qword M+" + EnderecoHex(endIndiceString) + "],eax \t\t; Atribui o valor do reg ao temp\n");

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
    
    // Atribui valor resultante de EXP ao id indicado
    if(sID.tipo == TIPO_FLOAT){ // tipo real
        if (rsEXP.tipo == TIPO_INT) { // id real recebendo EXP inteira
            // Converte em real.
            codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXP.endereco) + "] \t\t; recupera valor da EXP\n");
            codigoAssembly.append("\tcvtsi2ss xmm0,eax \t\t; converte e armazena em xmm0\n");
            codigoAssembly.append("\tmovss [qword M+" + EnderecoHex(sID.endereco) + "],xmm0 \t\t; Armazenar o resultado em id.end\n");
        } else { // EXP eh real
            codigoAssembly.append("\tmovss xmm0,[qword M+" + EnderecoHex(rsEXP.endereco) + "] \t\t; armazena em valor de EXP\n");
            codigoAssembly.append("\tmovss [qword M+" + EnderecoHex(sID.endereco) + "],xmm0 \t\t; Armazenar o resultado em id.end\n");
        }// fim if else
    }else if (sID.tipo == TIPO_INT) { // id inteiro recebe apenas EXP inteiro
        codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXP.endereco) + "] \t\t; Carregar conteúdo de Exp.end\n");
        codigoAssembly.append("\tmov [qword M+" + EnderecoHex(sID.endereco) + "],eax \t\t; Armazenar o resultado em id.end\n");
    } else if (sID.tipo == TIPO_CHAR) { // id char recebe apenas EXP char
        codigoAssembly.append("\tmov al,[qword M+" + EnderecoHex(rsEXP.endereco) + "] \t\t; Carregar conteúdo de Exp.end\n");
        codigoAssembly.append("\tmov [qword M+" + EnderecoHex(sID.endereco) + "],al \t\t; Armazenar o resultado em id.end\n");        
    } else { // id tipo string        
        if (stringArranjo) { // id string recebendo caractere em alguma posicao da string
            codigoAssembly.append("\tmov rsi,qword M+" + EnderecoHex(sID.endereco) + " \t\t; Movendo endereco do id\n");
            codigoAssembly.append("\tmov rax,0 \t\t; Zera registrador\n");
            codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(endIndiceString) + "] \t\t; Carrega valor do indice calculado anteriormente\n");
            codigoAssembly.append("\tadd rsi,rax \t\t; Atualiza posicao da string de acordo com o indice\n");
            codigoAssembly.append("\tmov al,[qword M+" + EnderecoHex(rsEXP.endereco) + "] \t\t; Carregar conteúdo de Exp.end\n");
            codigoAssembly.append("\tmov [rsi],al \t\t; Armazenar o resultado em id.end\n");  
        } else { // id string recebendo string
            codigoAssembly.append("\tmov rsi,qword M+" + EnderecoHex(rsEXP.endereco) + "\t\t; Salva endereço de EXP no rsi\n");
            codigoAssembly.append("\tmov rdi,qword M+" + EnderecoHex(sID.endereco) + "\t\t; Salva endereço do id no rdi\n");
            string loopString = NovoRot();
            codigoAssembly.append(loopString + ": \t\t; Rotulo para loop de leitura da string\n");
            codigoAssembly.append("\tmov al,[rsi] \t\t; Recupera um byte da string\n");
            codigoAssembly.append("\tmov [rdi],al \t\t; Salva byte recuperado da string no id\n");
            codigoAssembly.append("\tadd rsi,1 \t\t; Incrementa posicao em 1 byte\n");
            codigoAssembly.append("\tadd rdi,1 \t\t; Incrementa posicao em 1 byte\n");
            codigoAssembly.append("\tcmp al,0 \t\t; Verifica se é fim de string\n");
            codigoAssembly.append("\tjne " + loopString + "\t\t; Se não for, realiza o loop para o próximo byte\n");

            sID.tamanho = rsEXP.tamanho; // atualiza tamanho do id pro novo tamanho recebido de EXP
            analisadorLexico.tabelaSimbolos.atualizarTabela(lexemaID,sID); // atualiza a tabela de simbolos
        }//fim if else
    }// fim if else    

}// fim ATRIBUICAO()

// Procedimento REPETICAO()
// REPETICAO -> while EXP ( COMANDOS | "{" {COMANDOS} "}" )
// Parte semantica: REPETICAO -> while (A27) EXP (A28) (23) ( COMANDOS(A29) | "{" {COMANDOS} "}" )(A24)
void AnalisadorSintatico::REPETICAO() {
    CasaToken(TOKEN_WHILE);

    string RotInicio = NovoRot();
    string RotFim = NovoRot();
    codigoAssembly.append(RotInicio + ": \t\t; RotInicio\n");

    RegistroSemantico rsEXP; // registro semantico auxiliar para EXP
    contadorTemps = 0; // limpa area de memoria dos temporarios
    EXP(&rsEXP);
    
    // tipo da EXP deve ser logico
    if (rsEXP.tipo != TIPO_BOOL) {
        verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
    }//fim if 

    codigoAssembly.append("\tmov eax,[qword M+" +EnderecoHex(rsEXP.endereco) + "]\t\t; carregar conteudo de exp.end\n");
    codigoAssembly.append("\tcmp eax,1 \t\t; carregar conteudo de exp.end\n");
    codigoAssembly.append("\tjne " + RotFim+ "\t\t; se exp e falsa, desvia para RotFalso\n");

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
    codigoAssembly.append("\tjmp " + RotInicio + "\t\t; desvia para RotInicio\n");
    codigoAssembly.append(RotFim+ ": \t\t; RotFim\n");
}// fim REPETICAO()

// Procedimento TESTE()
// TESTE -> if EXP ( COMANDOS | "{" {COMANDOS} "}" ) [ else ( COMANDOS | "{" {COMANDOS} "}" ) ]
// Parte semantica: TESTE -> if (A18)EXP (23)(A19) ( COMANDOS | "{" {COMANDOS} "}" (A22)) (A23)[ else ( COMANDOS | "{" {COMANDOS} "}" ) ](A24)
void AnalisadorSintatico::TESTE() {
    CasaToken(TOKEN_IF);
    
    // Criar dos rotulos
    string RotFalso = NovoRot();
    string RotFim = NovoRot();

    RegistroSemantico rsEXP; // registro semantico auxiliar para EXP
    contadorTemps = 0; // limpa area de memoria dos temporarios
    EXP(&rsEXP);

    // tipo da EXP deve ser logico
    if (rsEXP.tipo != TIPO_BOOL) {
        verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
    }//fim if 

    codigoAssembly.append("\tmov eax,[qword M+" +EnderecoHex(rsEXP.endereco) + "]\t\t; carregar conteudo de exp.end\n");
    codigoAssembly.append("\tcmp eax,1 \t\t; carregar conteudo de exp.end\n");
    codigoAssembly.append("\tjne " + RotFalso + "\t\t; se exp e falsa, desvia para RotFalso\n");

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
        codigoAssembly.append("\tjmp " + RotFim + "\t\t; sai do comando de teste if\n");
    } else {
        COMANDOS();
        codigoAssembly.append("\tjmp " + RotFim + "\t\t; sai do comando de teste if\n");
    }// fim if else

    codigoAssembly.append(RotFalso + ": \t\t; RotFalso\n");
    
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

    codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
}// fim TESTE()

// Procedimento LEITURA()
// LEITURA -> readln "(" id ")"
// Parte semantica: LEITURA -> readln "(" id (22) ")" (A30)
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

    // Inicia geracao de codigo para ler do teclado.
    contadorTemps = 0; // limpando area de memoria dos temporarios
    int enderecoTemporario = NovoTemp(256); // reserva 256 bytes. variavel auxiliar para guardar novo temp
    codigoAssembly.append("\tmov rsi, M+" + EnderecoHex(enderecoTemporario) + "\t\t;end. string ou temp\n");
    codigoAssembly.append("\tmov rdx, 100h \t\t; tamanho do buffer\n");
    codigoAssembly.append("\tmov rax, 0 \t\t; chamada para leitura\n");
    codigoAssembly.append("\tmov rdi, 0 \t\t; leitura do teclado\n");
    codigoAssembly.append("\tsyscall \t\t\n");
    // Inicia atribuicao do que foi lido.
    if (sID.tipo == TIPO_INT){
        codigoAssembly.append("\tmov eax, 0 \t\t; Zera acumulador\n");
        codigoAssembly.append("\tmov ebx, 0 \t\t; Zera aux caractere\n");
        codigoAssembly.append("\tmov ecx, 10 \t\t; base 10\n");
        codigoAssembly.append("\tmov dx, 1 \t\t; sinal\n");
        codigoAssembly.append("\tmov rsi, M+" + EnderecoHex(enderecoTemporario) + "\t\t;end. string ou temp\n");
        codigoAssembly.append("\tmov bl, [rsi] \t\t;carrega caractere\n");
        codigoAssembly.append("\tcmp bl, '-' \t\t;sinal - ?\n");
        string Rot0 = NovoRot();
        codigoAssembly.append("\tjne " + Rot0 + " \t\t;se dif -, salta\n");
        codigoAssembly.append("\tmov dx, -1 \t\t;senão, armazena -\n");
        codigoAssembly.append("\tadd rsi, 1 \t\t;inc. ponteiro string\n");
        codigoAssembly.append("\tmov bl, [rsi] \t\t;carrega caractere\n");

        codigoAssembly.append(Rot0 + ": \t\t; Rot0\n");
        codigoAssembly.append("\tpush dx \t\t;empilha sinal\n");
        codigoAssembly.append("\tmov edx, 0 \t\t;reg. multiplicação\n");

        string Rot1 = NovoRot();
        string Rot2 = NovoRot();
        codigoAssembly.append(Rot1 + ": \t\t; Rot1\n");
        codigoAssembly.append("\tcmp bl, 0Ah \t\t;verifica fim string\n");
        codigoAssembly.append("\tje " + Rot2 + " \t\t;salta se fim string\n");
        codigoAssembly.append("\timul ecx \t\t;mult. eax por 10\n");
        codigoAssembly.append("\tsub bl, '0' \t\t;converte caractere\n");
        codigoAssembly.append("\tadd eax, ebx \t\t;soma valor caractere\n");
        codigoAssembly.append("\tadd rsi, 1 \t\t;incrementa base\n");
        codigoAssembly.append("\tmov bl, [rsi] \t\t;carrega caractere\n");
        codigoAssembly.append("\tjmp " + Rot1 + " \t\t;loop\n");

        codigoAssembly.append(Rot2 + ": \t\t; Rot2\n");
        codigoAssembly.append("\tpop cx \t\t;desempilha sinal\n");
        codigoAssembly.append("\tcmp cx, 0\t\t;verifica sinal\n");
        string Rot3 = NovoRot();
        codigoAssembly.append("\tjg " + Rot3 + " \t\t\n");
        codigoAssembly.append("\tneg eax \t\t;mult. sinal\n");
        codigoAssembly.append(Rot3 + ": \t\t; Rot3\n");

        codigoAssembly.append("\tmov [qword M+" +EnderecoHex(sID.endereco) + "], eax\t\t; atualiza valor do id\n");

    } else if (sID.tipo == TIPO_FLOAT) {
        codigoAssembly.append("\tmov rax, 0 \t\t; acumul. parte int.\n");
        codigoAssembly.append("\tsubss xmm0,xmm0 \t\t; acumul. parte frac.\n");
        codigoAssembly.append("\tmov rbx, 0 \t\t; caractere\n");
        codigoAssembly.append("\tmov rcx, 10 \t\t; base 10\n");
        codigoAssembly.append("\tcvtsi2ss xmm3,rcx \t\t; base 10\n");
        codigoAssembly.append("\tmovss xmm2,xmm3 \t\t;potência de 10\n");
        codigoAssembly.append("\tmov rdx, 1 \t\t;sinal\n");     
        codigoAssembly.append("\tmov rsi, M+" + EnderecoHex(enderecoTemporario) + "\t\t;end. buffer\n");
        codigoAssembly.append("\tmov bl, [rsi] \t\t;carrega caractere\n");
        codigoAssembly.append("\tcmp bl, '-' \t\t;sinal - ?\n");
        string Rot0 = NovoRot();
        codigoAssembly.append("\tjne " + Rot0 + " \t\t;se dif -, salta\n");
        codigoAssembly.append("\tmov rdx, -1 \t\t;senao, armazena -\n");
        codigoAssembly.append("\tadd rsi, 1 \t\t;inc. ponteiro stringcarrega caractere\n");
        codigoAssembly.append("\tmov bl, [rsi] \t\t;carrega caractere\n");

        
        codigoAssembly.append(Rot0 + ": \t\t; Rot0\n");
        codigoAssembly.append("\tpush rdx \t\t;empilha sinal\n");
        codigoAssembly.append("\tmov rdx, 0 \t\t;reg. multiplicação\n");
        
        string Rot1 = NovoRot();
        string Rot2 = NovoRot();
        string Rot3 = NovoRot();
        codigoAssembly.append(Rot1 + ": \t\t; Rot1\n");
        codigoAssembly.append("\tcmp bl, 0Ah \t\t;verifica fim string\n");
        codigoAssembly.append("\tje " + Rot2 + " \t\t;salta se fim string\n");
        codigoAssembly.append("\tcmp bl, '.' \t\t;verifica ponto\n");
        codigoAssembly.append("\tje " + Rot3 + " \t\t;salta se salta se ponto\n");
        codigoAssembly.append("\timul ecx \t\t;mult. eax por 10\n");
        codigoAssembly.append("\tsub bl, '0' \t\t;converte caractere\n");
        codigoAssembly.append("\tadd eax, ebx \t\t;soma valor caractere\n");
        codigoAssembly.append("\tadd rsi, 1 \t\t;incrementa base\n");
        codigoAssembly.append("\tmov bl, [rsi] \t\t;carrega caractere\n");
        codigoAssembly.append("\tjmp " + Rot1 + " \t\t;loop\n");

        codigoAssembly.append(Rot3 + ": \t\t; Rot3\n");
        codigoAssembly.append("\t\t; calcula parte fracion'aria em xmm0\n");

        codigoAssembly.append("\tadd rsi, 1 \t\t; inc. ponteiro string\n");
        codigoAssembly.append("\tmov bl, [rsi] \t\t ; carrega caractere\n");
        codigoAssembly.append("\tcmp bl, 0Ah \t\t; verifica fim string\n");
        codigoAssembly.append("\tje " + Rot2 + " \t\t;salta se fim string\n");
        codigoAssembly.append("\tsub bl, '0' \t\t; converte caractere\n");
        codigoAssembly.append("\tcvtsi2ss xmm1, rbx \t\t;conv real\n");
        codigoAssembly.append("\tdivss xmm1, xmm2 \t\t; transf casa decimal\n");
        codigoAssembly.append("\taddss xmm0, xmm1 \t\t; soma acumul\n");
        codigoAssembly.append("\tmulss xmm2,xmm3 \t\t; atualiza potencia\n");
        codigoAssembly.append("\tjmp " + Rot3 + " \t\t;loop\n");
        
        codigoAssembly.append(Rot2 + ": \t\t; Rot2\n");
        codigoAssembly.append("\tcvtsi2ss xmm1, rax \t\t; conv parte inteira\n");
        codigoAssembly.append("\taddss xmm0, xmm1\t\t; soma parte frac.\n");     
        codigoAssembly.append("\tpop rcx \t\t; desempilha sinal\n");
        codigoAssembly.append("\tcvtsi2ss xmm1, rcx\t\t; conv sinal\n");
        codigoAssembly.append("\tmulss xmm0, xmm1 \t\t; mult. sinal\n");        

        codigoAssembly.append("\tmovss [qword M+" +EnderecoHex(sID.endereco) + "], xmm0\t\t; atualiza valor do id\n");
    } else if (sID.tipo == TIPO_CHAR) {
        codigoAssembly.append("\tmov al, [M+" + EnderecoHex(enderecoTemporario) + "] \t\t;carrega caractere no endereco do temporario\n");
        codigoAssembly.append("\tmov [qword M+" +EnderecoHex(sID.endereco) + "], al\t\t; atualiza valor do id\n");
    } else { // tipo string
        codigoAssembly.append("\tmov rsi,qword M+" + EnderecoHex(enderecoTemporario) + "\t\t; Salva endereço do temporario no rsi\n");
        codigoAssembly.append("\tmov rdi,qword M+" + EnderecoHex(sID.endereco) + "\t\t; Salva endereço do id no rdi\n");
        string loopString = NovoRot();
        codigoAssembly.append(loopString + ": \t\t; Rotulo para loop de leitura da string\n");
        codigoAssembly.append("\tmov al,[rsi] \t\t; Recupera um byte da string\n");
        codigoAssembly.append("\tmov [rdi],al \t\t; Salva byte recuperado da string no id\n");
        codigoAssembly.append("\tadd rsi,1 \t\t; Incrementa posicao em 1 byte\n");
        codigoAssembly.append("\tadd rdi,1 \t\t; Incrementa posicao em 1 byte\n");
        codigoAssembly.append("\tcmp al,0Ah \t\t; Verifica se é quebra de linha\n");
        codigoAssembly.append("\tjne " + loopString + "\t\t; Se não for, realiza o loop para o próximo byte\n");

        codigoAssembly.append("\tsub rdi,1 \t\t; Decrementa posicao em 1 byte da string\n");
        codigoAssembly.append("\tmov bl,0 \t\t; carrega fim de string em reg\n");
        codigoAssembly.append("\tmov [rdi],bl \t\t; Altera valor de quebra de linha (0Ah) para fim de string (0)\n");

        sID.tamanho = 255; // tamanho maximo de bytes uteis em string
        analisadorLexico.tabelaSimbolos.atualizarTabela(lexemaID,sID); // atualiza o tamanho do id na tabela de simbolos
    }// fim if else

}// fim LEITURA()

// Procedimento ESCRITA()
// ESCRITA -> (write | writeln) "(" EXP {, EXP} ")"
// Parte semantica: ESCRITA -> (write | writeln) "(" (A10) EXP (21) {, (A6)(A10) EXP (21)} ")" (A6)
void AnalisadorSintatico::ESCRITA() {
    bool quebralinha = false; // variavel para verificar se deve ou nao quebrar linha

    if (analisadorLexico.getToken() == TOKEN_WRITE) {
        CasaToken(TOKEN_WRITE);
    } else {
        CasaToken(TOKEN_WRITELN);
        quebralinha = true;
    }//fim if else

    CasaToken(TOKEN_ABRE_PAREN);

    RegistroSemantico rsEXP; // registro semantico auxiliar para EXP
    contadorTemps = 0; // libera memoria dos temporarios
    codigoAssembly.append("\t\t\t; zerando contador\n");
    EXP(&rsEXP);

    // tipo da EXP nao pode ser logico
    if (rsEXP.tipo == TIPO_BOOL) {
        verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
    }//fim if     

    while (analisadorLexico.getToken() == TOKEN_VIRGULA) {
        CasaToken(TOKEN_VIRGULA);

        int enderecoTemporario; // variavel auxiliar para guardar novo temp
        // Inicia geracao de codigo para imprimir na tela.
        if (rsEXP.tipo == TIPO_INT){
            codigoAssembly.append("\tmov eax, [qword M+" + EnderecoHex(rsEXP.endereco) + "] \t\t; inteiro a ser convertido\n");
            enderecoTemporario = NovoTemp(256); // reserva 256 bytes.
            codigoAssembly.append("\tmov rsi, M+" + EnderecoHex(enderecoTemporario) + "\t\t;end. string ou temp\n");
            codigoAssembly.append("\tmov rcx, 0 \t\t;contador pilha\n");
            codigoAssembly.append("\tmov rdi, 0 \t\t;tam. string convertido\n");
            codigoAssembly.append("\tcmp eax, 0 \t\t;verifica sinal\n");

            string novoRot = NovoRot();
            codigoAssembly.append("\tjge " + novoRot + "\t\t;salta se número positivo\n");
            codigoAssembly.append("\tmov bl, '-' \t\t;senão, escreve sinal –\n");
            codigoAssembly.append("\tmov [rsi], bl \t\t\n");
            codigoAssembly.append("\tadd rsi, 1 \t\t;incrementa índice\n");
            codigoAssembly.append("\tadd rdi, 1 \t\t;incrementa tamanho\n");
            codigoAssembly.append("\tneg eax \t\t;toma módulo do número\n");

            codigoAssembly.append(novoRot + ": \t\t\n");
            codigoAssembly.append("\tmov ebx, 10 \t\t;divisor\n");

            novoRot = NovoRot();
            codigoAssembly.append(novoRot + ": \t\t\n");
            codigoAssembly.append("\tadd rcx, 1 \t\t;incrementa contador\n");
            codigoAssembly.append("\tcdq \t\t;estende edx:eax p/ div.\n");
            codigoAssembly.append("\tidiv ebx \t\t;divide edx:eax por ebx\n");
            codigoAssembly.append("\tpush dx \t\t;empilha valor do resto\n");
            codigoAssembly.append("\tcmp eax, 0 \t\t;verifica se quoc. é 0\n");
            codigoAssembly.append("\tjne " + novoRot + "\t\t;se não é 0, continua\n");
            codigoAssembly.append("\tadd rdi,rcx \t\t;atualiza tam. string\n");
            codigoAssembly.append(";Desempilha os valores e escreve o string\n");

            novoRot = NovoRot();
            codigoAssembly.append(novoRot + ": \t\t\n");
            codigoAssembly.append("\tpop dx \t\t;desempilha valor\n");
            codigoAssembly.append("\tadd dl, '0' \t\t;transforma em caractere\n");
            codigoAssembly.append("\tmov [rsi], dl \t\t;escreve caractere\n");
            codigoAssembly.append("\tadd rsi, 1 \t\t;incrementa base\n");
            codigoAssembly.append("\tsub rcx, 1 \t\t;decrementa contador\n");
            codigoAssembly.append("\tcmp rcx, 0 \t\t;verifica pilha vazia\n");
            codigoAssembly.append("\tjne " + novoRot + "\t\t;se não pilha vazia, loop\n");

            // if(quebralinha){
            //     codigoAssembly.append("\tmov rsi, M+" + EnderecoHex(enderecoTemporario) + "\t\t; endereco inicial da string\n");
            //     codigoAssembly.append("\tadd rsi, rdi\t\t; adiciona tamanho da string no endereco inicial\n");
            //     codigoAssembly.append("\tadd rdi, 1\t\t; adicionando 1 para rdi\n");
            //     codigoAssembly.append("\tmov al, 0xA\t\t; movendo quebra de linha para o registrador\n");
            //     codigoAssembly.append("\tmov [rsi], al\t\t; movendo quebra de linha para o rsi\n");
            // }// fim if

            codigoAssembly.append("\tmov rsi, M+" + EnderecoHex(enderecoTemporario) + "\t\t; endereco inicial da string\n");
            codigoAssembly.append("\tmov rdx, rdi\t\t; adicionando o tamanho da string\n");
            codigoAssembly.append("\tmov rax, 1\t\t; chamada para saida\n");
            codigoAssembly.append("\tmov rdi, 1\t\t; saida para tela\n");
            codigoAssembly.append("\tsyscall \t\t; Chama o kernel\n");

        }else if (rsEXP.tipo == TIPO_FLOAT){
            codigoAssembly.append("\tmovss xmm0, [qword M+" + EnderecoHex(rsEXP.endereco) + "] \t\t; real a ser convertido\n");
            enderecoTemporario = NovoTemp(256); // reserva 256 bytes.
            codigoAssembly.append("\tmov rsi, M+" + EnderecoHex(enderecoTemporario) + "\t\t;end. string ou temp\n");
            codigoAssembly.append("\tmov rcx, 0 \t\t;contador pilha\n");
            codigoAssembly.append("\tmov rdi, 6 \t\t;precisao 6 casas compartilhadas\n");
            codigoAssembly.append("\tmov rbx, 10 \t\t;divisor\n");
            codigoAssembly.append("\tcvtsi2ss xmm2, rbx \t\t;divisor real\n");
            codigoAssembly.append("\tsubss xmm1, xmm1 \t\t;zera registrador\n");
            codigoAssembly.append("\tcomiss xmm0, xmm1 \t\t;verifica sinal\n");

            string novoRot = NovoRot();
            codigoAssembly.append("\tjae " + novoRot + "\t\t;salta se número positivo\n");
            codigoAssembly.append("\tmov dl, '-' \t\t;senão, escreve sinal –\n");
            codigoAssembly.append("\tmov [rsi], dl \t\t\n");
            codigoAssembly.append("\tmov rdx, -1 \t\t;Carrega -1 em RDX\n");
            codigoAssembly.append("\tcvtsi2ss xmm1, rdx \t\t;Converte para real\n");
            codigoAssembly.append("\tmulss xmm0, xmm1 \t\t;Toma módulo\n");
            codigoAssembly.append("\tadd rsi, 1 \t\t;incrementa índice\n");

            codigoAssembly.append(novoRot + ": \t\t\n");
            codigoAssembly.append("\troundss xmm1, xmm0, 0b0011 \t\t;parte inteira xmm1\n");
            codigoAssembly.append("\tsubss xmm0, xmm1 \t\t;parte frac xmm0\n");
            codigoAssembly.append("\tcvtss2si rax, xmm1 \t\t;convertido para int\n");

            codigoAssembly.append(";converte parte inteira que está em rax\n");
            novoRot = NovoRot();
            codigoAssembly.append(novoRot + ": \t\t\n");
            codigoAssembly.append("\tadd rcx, 1 \t\t;incrementa contador\n");
            codigoAssembly.append("\tcdq \t\t;estende edx:eax p/ div.\n");
            codigoAssembly.append("\tidiv ebx \t\t;divide edx:eax por ebx\n");
            codigoAssembly.append("\tpush dx \t\t;empilha valor do resto\n");
            codigoAssembly.append("\tcmp eax, 0 \t\t;verifica se quoc. é 0\n");
            codigoAssembly.append("\tjne " + novoRot + "\t\t;se não é 0, continua\n");
            codigoAssembly.append("\tsub rdi,rcx \t\t;decrementa precisao\n");

            codigoAssembly.append(";Desempilha os valores e escreve o string\n");
            novoRot = NovoRot();
            codigoAssembly.append(novoRot + ": \t\t\n");
            codigoAssembly.append("\tpop dx \t\t;desempilha valor\n");
            codigoAssembly.append("\tadd dl, '0' \t\t;transforma em caractere\n");
            codigoAssembly.append("\tmov [rsi], dl \t\t;escreve caractere\n");
            codigoAssembly.append("\tadd rsi, 1 \t\t;incrementa base\n");
            codigoAssembly.append("\tsub rcx, 1 \t\t;decrementa contador\n");
            codigoAssembly.append("\tcmp rcx, 0 \t\t;verifica pilha vazia\n");
            codigoAssembly.append("\tjne " + novoRot + "\t\t;se não pilha vazia, loop\n");

            codigoAssembly.append("\tmov dl, '.' \t\t;escreve ponto decimal\n");
            codigoAssembly.append("\tmov [rsi], dl \t\t;escreve ponto decimal\n");
            codigoAssembly.append("\tadd rsi, 1 \t\t;incrementa base\n");

            codigoAssembly.append(";converte parte fracionaria que está em xmm0\n");
            novoRot = NovoRot();
            string rotImpressao = NovoRot();
            codigoAssembly.append(novoRot + ": \t\t\n");
            codigoAssembly.append("\tcmp rdi, 0 \t\t;verifica precisao\n");
            codigoAssembly.append("\tjle " + rotImpressao + " \t\t; verifica se terminou a precisao disponivel\n");
            codigoAssembly.append("\tmulss xmm0,xmm2 \t\t;desloca para esquerda\n");
            codigoAssembly.append("\troundss xmm1,xmm0,0b0011 \t\t;parte inteira xmm1\n");
            codigoAssembly.append("\tsubss xmm0,xmm1 \t\t;atualiza xmm0\n");
            codigoAssembly.append("\tcvtss2si rdx, xmm1 \t\t;convertido para int\n");
            codigoAssembly.append("\tadd dl, '0' \t\t;transforma em caractere\n");
            codigoAssembly.append("\tmov [rsi], dl \t\t;escreve caractere\n");
            codigoAssembly.append("\tadd rsi, 1 \t\t;incrementa base\n");
            codigoAssembly.append("\tsub rdi, 1 \t\t;decrementa precisao\n");
            codigoAssembly.append("\tjmp " + novoRot + "\t\t;retorna no loop\n");

            codigoAssembly.append("; impressão\n");
            codigoAssembly.append(rotImpressao + ": \t\t\n");

            // if(quebralinha){
            //     codigoAssembly.append("\tmov dl, 0xA \t\t;movendo quebra de linha para o registrador\n");
            //     codigoAssembly.append("\tmov [rsi], dl \t\t;escreve caractere\n");
            //     codigoAssembly.append("\tadd rsi, 1 \t\t;incrementa base\n");
            // } // fim if

            codigoAssembly.append("\tmov rdx, rsi \t\t;calcula tamanho string convertido\n");
            codigoAssembly.append("\tmov rbx, M+" + EnderecoHex(enderecoTemporario) + "\t\t;end. string ou temp\n");
            codigoAssembly.append("\tsub rdx, rbx \t\t;tamanho=rsi-enderecoTemporario\n");

            codigoAssembly.append("\tmov rsi, M+" + EnderecoHex(enderecoTemporario) + "\t\t; endereco inicial da string\n");
            codigoAssembly.append("; rdx ja possui tamanho da string\n");
            codigoAssembly.append("\tmov rax, 1\t\t; chamada para saida\n");
            codigoAssembly.append("\tmov rdi, 1\t\t; saida para tela\n");
            codigoAssembly.append("\tsyscall \t\t; Chama o kernel\n");

        }else{ // char ou string
            codigoAssembly.append("\tmov rcx,0 \t\t; zera contador\n");
            if (rsEXP.tipo == TIPO_CHAR){
                //rsEXP.tamanho = 1;
                // recupera um byte a mais para caso precise imprimir quebra de linha
                enderecoTemporario = NovoTemp(2); // recupera endereco de memoria e aloca a proxima disponivel na area de temps

                codigoAssembly.append("\tmov al,[qword M+" + EnderecoHex(rsEXP.endereco) + "] \t\t; Recupera o byte do caractere\n");
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(enderecoTemporario) + "],al \t\t; Atribui o valor do reg ao temp\n");
                codigoAssembly.append("\tmov ecx,1 \t\t; adiciona 1 byte\n");
            } else { // string
                enderecoTemporario = NovoTemp(rsEXP.tamanho+1); // recupera endereco de memoria e aloca a proxima disponivel na area de temps

                codigoAssembly.append("\tmov rsi,qword M+" + EnderecoHex(rsEXP.endereco) + "\t\t; Salva endereço do EXP no rsi\n");
                codigoAssembly.append("\tmov rdi,qword M+" + EnderecoHex(enderecoTemporario) + "\t\t; Salva endereço do temp no rdi\n");
                string loopString = NovoRot();
                codigoAssembly.append(loopString + ": \t\t; Rotulo para loop de leitura da string\n");
                codigoAssembly.append("\tmov al,[rsi] \t\t; Recupera um byte da string\n");
                codigoAssembly.append("\tmov [rdi],al \t\t; Salva byte recuperado da string no id\n");
                codigoAssembly.append("\tadd ecx,1 \t\t; Incrementa em 1 o contador\n");
                codigoAssembly.append("\tadd rsi,1 \t\t; Incrementa posicao em 1 byte\n");
                codigoAssembly.append("\tadd rdi,1 \t\t; Incrementa posicao em 1 byte\n");
                codigoAssembly.append("\tcmp al,0 \t\t; Verifica se é fim de string\n");
                codigoAssembly.append("\tjne " + loopString + "\t\t; Se não for, realiza o loop para o próximo byte\n");
                codigoAssembly.append("\tsub ecx,1 \t\t; Decrementa em 1 o contador, removendo o fim de string\n");
            }// fim if

            // if(quebralinha){
            //     //codigoAssembly.append("\tmov rsi, M+" + EnderecoHex((enderecoTemporario+rsEXP.tamanho)) + "\t\t; endereco inicial da string\n");
            //     codigoAssembly.append("\tmov rsi, M+" + EnderecoHex(enderecoTemporario) + "\t\t; endereco inicial da string\n");
            //     codigoAssembly.append("\tadd rsi, rcx \t\t; endereco inicial da string\n");
            //     codigoAssembly.append("\tmov al, 0xA \t\t; movendo quebra de linha para o registrador\n");
            //     codigoAssembly.append("\tmov [rsi], al \t\t; movendo quebra de linha para o rsi\n");
            //     codigoAssembly.append("\tadd ecx,1 \t\t; Incrementa em 1 o contador para imprimir a quebra de linha\n");
            //     //rsEXP.tamanho += 1;
            // }// fim if

            codigoAssembly.append("\tmov rsi, M+" + EnderecoHex(enderecoTemporario) + "\t\t; endereco inicial da string ou caractere\n");
            //codigoAssembly.append("\tmov rdx, " + to_string(rsEXP.tamanho) + "\t\t; tamanho da string ou caractere\n");
            codigoAssembly.append("\tmov rdx, rcx\t\t; tamanho da string ou caractere\n");
            codigoAssembly.append("\tmov rax, 1 \t\t; chamada para saida\n");
            codigoAssembly.append("\tmov rdi, 1 \t\t; saida para tela\n");
            codigoAssembly.append("\tsyscall \t\t; Chama o kernel\n");
        }// fim if else

        contadorTemps = 0; // libera memoria dos temporarios
        EXP(&rsEXP);

        // tipo da EXP nao pode ser logico
        if (rsEXP.tipo == TIPO_BOOL) {
            verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        }//fim if 

    }// fim while
    
    CasaToken(TOKEN_FECHA_PAREN);

    // COPIA DA PARTE DE CIMA. POREM, VALIDA QUEBRA DE LINHA NESSA PARTE (WRITELN)
    int enderecoTemporario; // variavel auxiliar para guardar novo temp
    // Inicia geracao de codigo para imprimir na tela.
    if (rsEXP.tipo == TIPO_INT){
        codigoAssembly.append("\tmov eax, [qword M+" + EnderecoHex(rsEXP.endereco) + "] \t\t; inteiro a ser convertido\n");
        enderecoTemporario = NovoTemp(256); // reserva 256 bytes.
        codigoAssembly.append("\tmov rsi, M+" + EnderecoHex(enderecoTemporario) + "\t\t;end. string ou temp\n");
        codigoAssembly.append("\tmov rcx, 0 \t\t;contador pilha\n");
        codigoAssembly.append("\tmov rdi, 0 \t\t;tam. string convertido\n");
        codigoAssembly.append("\tcmp eax, 0 \t\t;verifica sinal\n");

        string novoRot = NovoRot();
        codigoAssembly.append("\tjge " + novoRot + "\t\t;salta se número positivo\n");
        codigoAssembly.append("\tmov bl, '-' \t\t;senão, escreve sinal –\n");
        codigoAssembly.append("\tmov [rsi], bl \t\t\n");
        codigoAssembly.append("\tadd rsi, 1 \t\t;incrementa índice\n");
        codigoAssembly.append("\tadd rdi, 1 \t\t;incrementa tamanho\n");
        codigoAssembly.append("\tneg eax \t\t;toma módulo do número\n");

        codigoAssembly.append(novoRot + ": \t\t\n");
        codigoAssembly.append("\tmov ebx, 10 \t\t;divisor\n");

        novoRot = NovoRot();
        codigoAssembly.append(novoRot + ": \t\t\n");
        codigoAssembly.append("\tadd rcx, 1 \t\t;incrementa contador\n");
        codigoAssembly.append("\tcdq \t\t;estende edx:eax p/ div.\n");
        codigoAssembly.append("\tidiv ebx \t\t;divide edx:eax por ebx\n");
        codigoAssembly.append("\tpush dx \t\t;empilha valor do resto\n");
        codigoAssembly.append("\tcmp eax, 0 \t\t;verifica se quoc. é 0\n");
        codigoAssembly.append("\tjne " + novoRot + "\t\t;se não é 0, continua\n");
        codigoAssembly.append("\tadd rdi,rcx \t\t;atualiza tam. string\n");
        codigoAssembly.append(";Desempilha os valores e escreve o string\n");

        novoRot = NovoRot();
        codigoAssembly.append(novoRot + ": \t\t\n");
        codigoAssembly.append("\tpop dx \t\t;desempilha valor\n");
        codigoAssembly.append("\tadd dl, '0' \t\t;transforma em caractere\n");
        codigoAssembly.append("\tmov [rsi], dl \t\t;escreve caractere\n");
        codigoAssembly.append("\tadd rsi, 1 \t\t;incrementa base\n");
        codigoAssembly.append("\tsub rcx, 1 \t\t;decrementa contador\n");
        codigoAssembly.append("\tcmp rcx, 0 \t\t;verifica pilha vazia\n");
        codigoAssembly.append("\tjne " + novoRot + "\t\t;se não pilha vazia, loop\n");
        
        if(quebralinha){
            codigoAssembly.append("\tmov rsi, M+" + EnderecoHex(enderecoTemporario) + "\t\t; endereco inicial da string\n");
            codigoAssembly.append("\tadd rsi, rdi\t\t; adiciona tamanho da string no endereco inicial\n");
            codigoAssembly.append("\tadd rdi, 1\t\t; adicionando 1 para rdi\n");
            codigoAssembly.append("\tmov al, 0xA\t\t; movendo quebra de linha para o registrador\n");
            codigoAssembly.append("\tmov [rsi], al\t\t; movendo quebra de linha para o rsi\n");
        }// fim if

        codigoAssembly.append("\tmov rsi, M+" + EnderecoHex(enderecoTemporario) + "\t\t; endereco inicial da string\n");
        codigoAssembly.append("\tmov rdx, rdi\t\t; adicionando o tamanho da string\n");
        codigoAssembly.append("\tmov rax, 1\t\t; chamada para saida\n");
        codigoAssembly.append("\tmov rdi, 1\t\t; saida para tela\n");
        codigoAssembly.append("\tsyscall \t\t; Chama o kernel\n");

    }else if (rsEXP.tipo == TIPO_FLOAT){
        codigoAssembly.append("\tmovss xmm0, [qword M+" + EnderecoHex(rsEXP.endereco) + "] \t\t; real a ser convertido\n");
        enderecoTemporario = NovoTemp(256); // reserva 256 bytes.
        codigoAssembly.append("\tmov rsi, M+" + EnderecoHex(enderecoTemporario) + "\t\t;end. string ou temp\n");
        codigoAssembly.append("\tmov rcx, 0 \t\t;contador pilha\n");
        codigoAssembly.append("\tmov rdi, 6 \t\t;precisao 6 casas compartilhadas\n");
        codigoAssembly.append("\tmov rbx, 10 \t\t;divisor\n");
        codigoAssembly.append("\tcvtsi2ss xmm2, rbx \t\t;divisor real\n");
        codigoAssembly.append("\tsubss xmm1, xmm1 \t\t;zera registrador\n");
        codigoAssembly.append("\tcomiss xmm0, xmm1 \t\t;verifica sinal\n");

        string novoRot = NovoRot();
        codigoAssembly.append("\tjae " + novoRot + "\t\t;salta se número positivo\n");
        codigoAssembly.append("\tmov dl, '-' \t\t;senão, escreve sinal –\n");
        codigoAssembly.append("\tmov [rsi], dl \t\t\n");
        codigoAssembly.append("\tmov rdx, -1 \t\t;Carrega -1 em RDX\n");
        codigoAssembly.append("\tcvtsi2ss xmm1, rdx \t\t;Converte para real\n");
        codigoAssembly.append("\tmulss xmm0, xmm1 \t\t;Toma módulo\n");
        codigoAssembly.append("\tadd rsi, 1 \t\t;incrementa índice\n");

        codigoAssembly.append(novoRot + ": \t\t\n");
        codigoAssembly.append("\troundss xmm1, xmm0, 0b0011 \t\t;parte inteira xmm1\n");
        codigoAssembly.append("\tsubss xmm0, xmm1 \t\t;parte frac xmm0\n");
        codigoAssembly.append("\tcvtss2si rax, xmm1 \t\t;convertido para int\n");
        
        codigoAssembly.append(";converte parte inteira que está em rax\n");
        novoRot = NovoRot();
        codigoAssembly.append(novoRot + ": \t\t\n");
        codigoAssembly.append("\tadd rcx, 1 \t\t;incrementa contador\n");
        codigoAssembly.append("\tcdq \t\t;estende edx:eax p/ div.\n");
        codigoAssembly.append("\tidiv ebx \t\t;divide edx:eax por ebx\n");
        codigoAssembly.append("\tpush dx \t\t;empilha valor do resto\n");
        codigoAssembly.append("\tcmp eax, 0 \t\t;verifica se quoc. é 0\n");
        codigoAssembly.append("\tjne " + novoRot + "\t\t;se não é 0, continua\n");
        codigoAssembly.append("\tsub rdi,rcx \t\t;decrementa precisao\n");

        codigoAssembly.append(";Desempilha os valores e escreve o string\n");
        novoRot = NovoRot();
        codigoAssembly.append(novoRot + ": \t\t\n");
        codigoAssembly.append("\tpop dx \t\t;desempilha valor\n");
        codigoAssembly.append("\tadd dl, '0' \t\t;transforma em caractere\n");
        codigoAssembly.append("\tmov [rsi], dl \t\t;escreve caractere\n");
        codigoAssembly.append("\tadd rsi, 1 \t\t;incrementa base\n");
        codigoAssembly.append("\tsub rcx, 1 \t\t;decrementa contador\n");
        codigoAssembly.append("\tcmp rcx, 0 \t\t;verifica pilha vazia\n");
        codigoAssembly.append("\tjne " + novoRot + "\t\t;se não pilha vazia, loop\n");

        codigoAssembly.append("\tmov dl, '.' \t\t;escreve ponto decimal\n");
        codigoAssembly.append("\tmov [rsi], dl \t\t;escreve ponto decimal\n");
        codigoAssembly.append("\tadd rsi, 1 \t\t;incrementa base\n");

        codigoAssembly.append(";converte parte fracionaria que está em xmm0\n");
        novoRot = NovoRot();
        string rotImpressao = NovoRot();
        codigoAssembly.append(novoRot + ": \t\t\n");
        codigoAssembly.append("\tcmp rdi, 0 \t\t;verifica precisao\n");
        codigoAssembly.append("\tjle " + rotImpressao + " \t\t; verifica se terminou a precisao disponivel\n");
        codigoAssembly.append("\tmulss xmm0,xmm2 \t\t;desloca para esquerda\n");
        codigoAssembly.append("\troundss xmm1,xmm0,0b0011 \t\t;parte inteira xmm1\n");
        codigoAssembly.append("\tsubss xmm0,xmm1 \t\t;atualiza xmm0\n");
        codigoAssembly.append("\tcvtss2si rdx, xmm1 \t\t;convertido para int\n");
        codigoAssembly.append("\tadd dl, '0' \t\t;transforma em caractere\n");
        codigoAssembly.append("\tmov [rsi], dl \t\t;escreve caractere\n");
        codigoAssembly.append("\tadd rsi, 1 \t\t;incrementa base\n");
        codigoAssembly.append("\tsub rdi, 1 \t\t;decrementa precisao\n");
        codigoAssembly.append("\tjmp " + novoRot + "\t\t;retorna no loop\n");

        codigoAssembly.append("; impressão\n");
        codigoAssembly.append(rotImpressao + ": \t\t\n");
        
        if(quebralinha){
            codigoAssembly.append("\tmov dl, 0xA \t\t;movendo quebra de linha para o registrador\n");
            codigoAssembly.append("\tmov [rsi], dl \t\t;escreve caractere\n");
            codigoAssembly.append("\tadd rsi, 1 \t\t;incrementa base\n");
        } // fim if

        codigoAssembly.append("\tmov rdx, rsi \t\t;calcula tamanho string convertido\n");
        codigoAssembly.append("\tmov rbx, M+" + EnderecoHex(enderecoTemporario) + "\t\t;end. string ou temp\n");
        codigoAssembly.append("\tsub rdx, rbx \t\t;tamanho=rsi-enderecoTemporario\n");

        codigoAssembly.append("\tmov rsi, M+" + EnderecoHex(enderecoTemporario) + "\t\t; endereco inicial da string\n");
        codigoAssembly.append("; rdx ja possui tamanho da string\n");
        codigoAssembly.append("\tmov rax, 1\t\t; chamada para saida\n");
        codigoAssembly.append("\tmov rdi, 1\t\t; saida para tela\n");
        codigoAssembly.append("\tsyscall \t\t; Chama o kernel\n");

    }else{ // char ou string
        codigoAssembly.append("\tmov rcx,0 \t\t; zera contador\n");
        if (rsEXP.tipo == TIPO_CHAR){
            //rsEXP.tamanho = 1;
            // recupera um byte a mais para caso precise imprimir quebra de linha
            enderecoTemporario = NovoTemp(2); // recupera endereco de memoria e aloca a proxima disponivel na area de temps

            codigoAssembly.append("\tmov al,[qword M+" + EnderecoHex(rsEXP.endereco) + "] \t\t; Recupera o byte do caractere\n");
            codigoAssembly.append("\tmov [qword M+" + EnderecoHex(enderecoTemporario) + "],al \t\t; Atribui o valor do reg ao temp\n");
            codigoAssembly.append("\tmov ecx,1 \t\t; adiciona 1 byte\n");
        } else { // string
            enderecoTemporario = NovoTemp(rsEXP.tamanho+1); // recupera endereco de memoria e aloca a proxima disponivel na area de temps

            codigoAssembly.append("\tmov rsi,qword M+" + EnderecoHex(rsEXP.endereco) + "\t\t; Salva endereço do EXP no rsi\n");
            codigoAssembly.append("\tmov rdi,qword M+" + EnderecoHex(enderecoTemporario) + "\t\t; Salva endereço do temp no rdi\n");
            string loopString = NovoRot();
            codigoAssembly.append(loopString + ": \t\t; Rotulo para loop de leitura da string\n");
            codigoAssembly.append("\tmov al,[rsi] \t\t; Recupera um byte da string\n");
            codigoAssembly.append("\tmov [rdi],al \t\t; Salva byte recuperado da string no id\n");
            codigoAssembly.append("\tadd ecx,1 \t\t; Incrementa em 1 o contador\n");
            codigoAssembly.append("\tadd rsi,1 \t\t; Incrementa posicao em 1 byte\n");
            codigoAssembly.append("\tadd rdi,1 \t\t; Incrementa posicao em 1 byte\n");
            codigoAssembly.append("\tcmp al,0 \t\t; Verifica se é fim de string\n");
            codigoAssembly.append("\tjne " + loopString + "\t\t; Se não for, realiza o loop para o próximo byte\n");
            codigoAssembly.append("\tsub ecx,1 \t\t; Decrementa em 1 o contador, removendo o fim de string\n");
        }// fim if

        if(quebralinha){
            //codigoAssembly.append("\tmov rsi, M+" + EnderecoHex((enderecoTemporario+rsEXP.tamanho)) + "\t\t; endereco inicial da string\n");
            codigoAssembly.append("\tmov rsi, M+" + EnderecoHex(enderecoTemporario) + "\t\t; endereco inicial da string\n");
            codigoAssembly.append("\tadd rsi, rcx \t\t; endereco inicial da string\n");
            codigoAssembly.append("\tmov al, 0xA \t\t; movendo quebra de linha para o registrador\n");
            codigoAssembly.append("\tmov [rsi], al \t\t; movendo quebra de linha para o rsi\n");
            codigoAssembly.append("\tadd ecx,1 \t\t; Incrementa em 1 o contador para imprimir a quebra de linha\n");
            //rsEXP.tamanho += 1;
        }// fim if

        codigoAssembly.append("\tmov rsi, M+" + EnderecoHex(enderecoTemporario) + "\t\t; endereco inicial da string ou caractere\n");
        //codigoAssembly.append("\tmov rdx, " + to_string(rsEXP.tamanho) + "\t\t; tamanho da string ou caractere\n");
        codigoAssembly.append("\tmov rdx, rcx\t\t; tamanho da string ou caractere\n");
        codigoAssembly.append("\tmov rax, 1 \t\t; chamada para saida\n");
        codigoAssembly.append("\tmov rdi, 1 \t\t; saida para tela\n");
        codigoAssembly.append("\tsyscall \t\t; Chama o kernel\n");
    }// fim if else

}// fim ESCRITA()

// Procedimento EXP()
// EXP -> EXPS [ (= | != | < | > | <= | >=) EXPS]
// Parte semantica: EXP -> EXPS [ (= | != | < | > | <= | >=) EXPS1 (19)(A25)] (20)
void AnalisadorSintatico::EXP(RegistroSemantico* rsEXP) {
    RegistroSemantico rsEXPS; // registro semantico auxiliar para EXPS
    EXPS(&rsEXPS);
    RegistroSemantico rsEXPS_aux = rsEXPS;
    
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

            // Codigo de maquina
            if(rsEXPS_aux.tipo == TIPO_INT && rsEXPS1.tipo == TIPO_INT){
                codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar conteudo de EXPS.end\n");
                codigoAssembly.append("\tmov ebx,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar conteudo de EXPS1.end\n");
                codigoAssembly.append("\tcmp eax,ebx\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tje " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar conteudo em EXPS.end\n");
            }

            if(rsEXPS_aux.tipo == TIPO_FLOAT || rsEXPS1.tipo == TIPO_FLOAT){
                if(rsEXPS_aux.tipo == TIPO_INT){ // verifica tipo de EXPS
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; Carrega conteudo de EXPS.end no eax\n");
                    codigoAssembly.append("\tcvtsi2ss xmm0,eax\t\t; converte reg para real\n");
                } else {
                    codigoAssembly.append("\tmovss xmm0,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar EXPS.end\n");
                }// fim if else

                if(rsEXPS1.tipo == TIPO_INT){ // verifica tipo de EXPS1
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; Carrega conteudo de EXPS1.end no eax\n");
                    codigoAssembly.append("\tcvtsi2ss xmm1,eax \t\t; converte reg para real\n");
                } else {
                    codigoAssembly.append("\tmovss xmm1,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar EXPS1.end\n");
                }// fim if else

                codigoAssembly.append("\tcomiss xmm0,xmm1\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tje " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar EXPS.end\n");
            }

            if(rsEXPS_aux.tipo == TIPO_CHAR && rsEXPS1.tipo == TIPO_CHAR){
                codigoAssembly.append("\tmov eax,0 \t\t; Zera registrador eax\n");
                codigoAssembly.append("\tmov ebx,0 \t\t; Zera registrador ebx\n");
                codigoAssembly.append("\tmov al,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar EXPS.end\n");
                codigoAssembly.append("\tmov bl,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar EXPS1.end\n");
                codigoAssembly.append("\tcmp eax,ebx\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tje " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar exp1.end\n");
            }

            if(rsEXPS_aux.tipo == TIPO_STRING && rsEXPS1.tipo == TIPO_STRING){

                codigoAssembly.append("\tmov eax,0 \t\t; Zera registrador eax\n");
                codigoAssembly.append("\tmov ebx,0 \t\t; Zera registrador ebx\n");
                codigoAssembly.append("\tmov rsi,qword M+" + EnderecoHex(rsEXPS.endereco) + "\t\t; Salva endereço do EXPS no rsi\n");
                codigoAssembly.append("\tmov rdi,qword M+" + EnderecoHex(rsEXPS1.endereco) + "\t\t; Salva endereço do EXPS1 no rdi\n");
                string loopString = NovoRot();
                codigoAssembly.append(loopString + ": \t\t; Rotulo para loop de leitura da string\n");
                codigoAssembly.append("\tmov al,[rsi] \t\t; Recupera um byte da string em EXPS\n");
                codigoAssembly.append("\tmov bl,[rdi] \t\t; Recupera um byte da string em EXPS1\n");
                codigoAssembly.append("\tadd rsi,1 \t\t; Incrementa posicao em 1 byte\n");
                codigoAssembly.append("\tadd rdi,1 \t\t; Incrementa posicao em 1 byte\n");
                codigoAssembly.append("\tcmp al,bl \t\t; compara byte das strings\n");
                string RotFalso = NovoRot();
                codigoAssembly.append("\tjne " + RotFalso + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tcmp eax,0\t\t; compara se fim de string\n");
                codigoAssembly.append("\tjne " + loopString + "\t\t; Se não for, realiza o loop para o próximo byte\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotFalso + ": \t\t; RotFalso\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso ao registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar conteudo em EXPS.end\n");
            }

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

            // Codigo de maquina
            if(rsEXPS_aux.tipo == TIPO_INT && rsEXPS1.tipo == TIPO_INT){
                codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar exp.end\n");
                codigoAssembly.append("\tmov ebx,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar exp1.end\n");
                codigoAssembly.append("\tcmp eax,ebx\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tjne " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar exp1.end\n");
            }

            if(rsEXPS_aux.tipo == TIPO_FLOAT || rsEXPS1.tipo == TIPO_FLOAT){
                if(rsEXPS_aux.tipo == TIPO_INT){ // verifica tipo de EXPS
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; Carrega conteudo de EXPS.end no eax\n");
                    codigoAssembly.append("\tcvtsi2ss xmm0,eax\t\t; converte reg para real\n");
                } else {
                    codigoAssembly.append("\tmovss xmm0,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar exp.end\n");
                }// fim if else

                if(rsEXPS1.tipo == TIPO_INT){ // verifica tipo de EXPS1
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; Carrega conteudo de EXPS1.end no eax\n");
                    codigoAssembly.append("\tcvtsi2ss xmm1,eax \t\t; converte reg para real\n");
                } else {
                    codigoAssembly.append("\tmovss xmm1,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar exp1.end\n");
                }// fim if else

                codigoAssembly.append("\tcomiss xmm0,xmm1\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tjne " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar exp1.end\n");
            }

            if(rsEXPS_aux.tipo == TIPO_CHAR && rsEXPS1.tipo == TIPO_CHAR){
                codigoAssembly.append("\tmov eax,0 \t\t; Zera registrador eax\n");
                codigoAssembly.append("\tmov ebx,0 \t\t; Zera registrador ebx\n");
                codigoAssembly.append("\tmov al,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar exp.end\n");
                codigoAssembly.append("\tmov bl,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar exp1.end\n");
                codigoAssembly.append("\tcmp eax,ebx\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tjne " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar exp1.end\n");
            }

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
            
            // Codigo de maquina
            if(rsEXPS_aux.tipo == TIPO_INT && rsEXPS1.tipo == TIPO_INT){
                codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar exp.end\n");
                codigoAssembly.append("\tmov ebx,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar exp1.end\n");
                codigoAssembly.append("\tcmp eax,ebx\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tjl " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar exp1.end\n");
            }

            if(rsEXPS_aux.tipo == TIPO_FLOAT || rsEXPS1.tipo == TIPO_FLOAT){
                if(rsEXPS_aux.tipo == TIPO_INT){ // verifica tipo de EXPS
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; Carrega conteudo de EXPS.end no eax\n");
                    codigoAssembly.append("\tcvtsi2ss xmm0,eax\t\t; converte reg para real\n");
                } else {
                    codigoAssembly.append("\tmovss xmm0,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar exp.end\n");
                }// fim if else

                if(rsEXPS1.tipo == TIPO_INT){ // verifica tipo de EXPS1
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; Carrega conteudo de EXPS1.end no eax\n");
                    codigoAssembly.append("\tcvtsi2ss xmm1,eax \t\t; converte reg para real\n");
                } else {
                    codigoAssembly.append("\tmovss xmm1,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar exp1.end\n");
                }// fim if else

                codigoAssembly.append("\tcomiss xmm0,xmm1\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tjb " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar exp1.end\n");
            }

            if(rsEXPS_aux.tipo == TIPO_CHAR && rsEXPS1.tipo == TIPO_CHAR){
                codigoAssembly.append("\tmov eax,0 \t\t; Zera registrador eax\n");
                codigoAssembly.append("\tmov ebx,0 \t\t; Zera registrador ebx\n");
                codigoAssembly.append("\tmov al,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar exp.end\n");
                codigoAssembly.append("\tmov bl,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar exp1.end\n");
                codigoAssembly.append("\tcmp eax,ebx\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tjl " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar exp1.end\n");
            }
            
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
            
            // Codigo de maquina
            if(rsEXPS_aux.tipo == TIPO_INT && rsEXPS1.tipo == TIPO_INT){
                codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar exp.end\n");
                codigoAssembly.append("\tmov ebx,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar exp1.end\n");
                codigoAssembly.append("\tcmp eax,ebx\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tjg " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar exp1.end\n");
            }

            if(rsEXPS_aux.tipo == TIPO_FLOAT || rsEXPS1.tipo == TIPO_FLOAT){
                if(rsEXPS_aux.tipo == TIPO_INT){ // verifica tipo de EXPS
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; Carrega conteudo de EXPS.end no eax\n");
                    codigoAssembly.append("\tcvtsi2ss xmm0,eax\t\t; converte reg para real\n");
                } else {
                    codigoAssembly.append("\tmovss xmm0,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar exp.end\n");
                }// fim if else

                if(rsEXPS1.tipo == TIPO_INT){ // verifica tipo de EXPS1
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; Carrega conteudo de EXPS1.end no eax\n");
                    codigoAssembly.append("\tcvtsi2ss xmm1,eax \t\t; converte reg para real\n");
                } else {
                    codigoAssembly.append("\tmovss xmm1,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar exp1.end\n");
                }// fim if else

                codigoAssembly.append("\tcomiss xmm0,xmm1\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tja " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar exp1.end\n");
            }

            if(rsEXPS_aux.tipo == TIPO_CHAR && rsEXPS1.tipo == TIPO_CHAR){
                codigoAssembly.append("\tmov eax,0 \t\t; Zera registrador eax\n");
                codigoAssembly.append("\tmov ebx,0 \t\t; Zera registrador ebx\n");
                codigoAssembly.append("\tmov al,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar exp.end\n");
                codigoAssembly.append("\tmov bl,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar exp1.end\n");
                codigoAssembly.append("\tcmp eax,ebx\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tjg " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar exp1.end\n");
            }
            
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
            }//fim if else7.11000
            
            // Codigo de maquina
            if(rsEXPS_aux.tipo == TIPO_INT && rsEXPS1.tipo == TIPO_INT){
                codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar exp.end\n");
                codigoAssembly.append("\tmov ebx,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar exp1.end\n");
                codigoAssembly.append("\tcmp eax,ebx\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tjle " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar exp1.end\n");
            }

            if(rsEXPS_aux.tipo == TIPO_FLOAT || rsEXPS1.tipo == TIPO_FLOAT){
                if(rsEXPS_aux.tipo == TIPO_INT){ // verifica tipo de EXPS
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; Carrega conteudo de EXPS.end no eax\n");
                    codigoAssembly.append("\tcvtsi2ss xmm0,eax\t\t; converte reg para real\n");
                } else {
                    codigoAssembly.append("\tmovss xmm0,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar exp.end\n");
                }// fim if else

                if(rsEXPS1.tipo == TIPO_INT){ // verifica tipo de EXPS1
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; Carrega conteudo de EXPS1.end no eax\n");
                    codigoAssembly.append("\tcvtsi2ss xmm1,eax \t\t; converte reg para real\n");
                } else {
                    codigoAssembly.append("\tmovss xmm1,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar exp1.end\n");
                }// fim if else

                codigoAssembly.append("\tcomiss xmm0,xmm1\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tjbe " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar exp1.end\n");
            }

            if(rsEXPS_aux.tipo == TIPO_CHAR && rsEXPS1.tipo == TIPO_CHAR){
                codigoAssembly.append("\tmov eax,0 \t\t; Zera registrador eax\n");
                codigoAssembly.append("\tmov ebx,0 \t\t; Zera registrador ebx\n");
                codigoAssembly.append("\tmov al,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar exp.end\n");
                codigoAssembly.append("\tmov bl,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar exp1.end\n");
                codigoAssembly.append("\tcmp eax,ebx\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tjle " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar exp1.end\n");
            }
            
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
            
            // Codigo de maquina
            if(rsEXPS_aux.tipo == TIPO_INT && rsEXPS1.tipo == TIPO_INT){
                codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar exp.end\n");
                codigoAssembly.append("\tmov ebx,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar exp1.end\n");
                codigoAssembly.append("\tcmp eax,ebx\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tjge " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar exp1.end\n");
            }

            if(rsEXPS_aux.tipo == TIPO_FLOAT || rsEXPS1.tipo == TIPO_FLOAT){
                if(rsEXPS_aux.tipo == TIPO_INT){ // verifica tipo de EXPS
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; Carrega conteudo de EXPS.end no eax\n");
                    codigoAssembly.append("\tcvtsi2ss xmm0,eax\t\t; converte reg para real\n");
                } else {
                    codigoAssembly.append("\tmovss xmm0,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar exp.end\n");
                }// fim if else

                if(rsEXPS1.tipo == TIPO_INT){ // verifica tipo de EXPS1
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; Carrega conteudo de EXPS1.end no eax\n");
                    codigoAssembly.append("\tcvtsi2ss xmm1,eax \t\t; converte reg para real\n");
                } else {
                    codigoAssembly.append("\tmovss xmm1,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar exp1.end\n");
                }// fim if else

                codigoAssembly.append("\tcomiss xmm0,xmm1\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tjae " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar exp1.end\n");
            }

            if(rsEXPS_aux.tipo == TIPO_CHAR && rsEXPS1.tipo == TIPO_CHAR){
                codigoAssembly.append("\tmov eax,0 \t\t; Zera registrador eax\n");
                codigoAssembly.append("\tmov ebx,0 \t\t; Zera registrador ebx\n");
                codigoAssembly.append("\tmov al,[qword M+" + EnderecoHex(rsEXPS.endereco) + "]\t\t; armazenar exp.end\n");
                codigoAssembly.append("\tmov bl,[qword M+" + EnderecoHex(rsEXPS1.endereco) + "]\t\t; armazenar exp1.end\n");
                codigoAssembly.append("\tcmp eax,ebx\t\t; comparar resultado\n");
                string RotVerdadeiro = NovoRot();
                codigoAssembly.append("\tjge " + RotVerdadeiro + "\t\t; pula para rotverdadeiro\n");
                codigoAssembly.append("\tmov eax,0 \t\t; atribui falso a registrador\n");
                string RotFim = NovoRot();
                codigoAssembly.append("\tjmp " + RotFim + " \t\t; pula para rot fim\n");
                codigoAssembly.append(RotVerdadeiro + ": \t\t; RotVerdadeiro\n");
                codigoAssembly.append("\tmov eax,1 \t\t; atribui verdadeiro a registrador\n");
                codigoAssembly.append(RotFim + ": \t\t; RotFim\n");
                rsEXPS.endereco = NovoTemp(4);
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsEXPS.endereco) + "], eax\t\t; armazenar exp1.end\n");
            }

            break;

        default:
            break;
            // Faz nada (lambda)
    }//fim switch

    rsEXP->tipo = rsEXPS.tipo; // devolve tipo final da expressao
    rsEXP->tamanho = rsEXPS.tamanho; // atribui tamanho a expressao superior
    rsEXP->endereco = rsEXPS.endereco; // atribui endereco a expressao superior
}// fim EXP()

// Procedimento EXPS()
// EXPS -> [-] T { (+ | - | "||") T}
// Parte semantica: EXPS -> [-] T (14)(A7) { (+ (15) | - (15) | "||" (16)) T1 (17)(A26)} (18)
void AnalisadorSintatico::EXPS(RegistroSemantico* rsEXPS) {
    bool tokenNegativo = false; // variavel auxiliar para as analises semanticas

    if (analisadorLexico.getToken() == TOKEN_MENOS) {
        CasaToken(TOKEN_MENOS);
        tokenNegativo = true;
    }// fim if

    RegistroSemantico rsT; // registro semantico auxiliar para T
    //rsT.negativo = tokenNegativo; // passa informacao de token negativo.
    T(&rsT);

    // se tem um - antes, o tipo de T deve ser numerico
    if (tokenNegativo && (rsT.tipo != TIPO_INT && rsT.tipo != TIPO_FLOAT)) {
        verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
    }//fim if

    // Caso seja tipo numerico e precedido de um menos, nega valor e atribui ao um novo temporario
    if (tokenNegativo) {
        int enderecoTemporario = NovoTemp(4); // recupera endereco de memoria e aloca a proxima disponivel na area de temps
        if (rsT.tipo == TIPO_INT) {
            codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsT.endereco) + "] \t\t; Recupera o valor do inteiro\n");
            codigoAssembly.append("\tneg eax \t\t; Nega valor do reg \n");
            codigoAssembly.append("\tmov [qword M+" + EnderecoHex(enderecoTemporario) + "],eax \t\t; Atribui o valor do reg ao temp\n");
        } else { // tipo float
            codigoAssembly.append("\tmovss xmm0,[qword M+" + EnderecoHex(rsT.endereco) + "] \t\t; Recupera o valor do real\n");
            codigoAssembly.append("\tmov eax,-1 \t\t; Atribui -1 para um reg inteiro\n");
            codigoAssembly.append("\tcvtsi2ss xmm1,eax \t\t; Converte -1 para um reg real\n");
            codigoAssembly.append("\tmulss xmm0,xmm1 \t\t; Multiplica valor real por -1 para pegar negativo\n");
            codigoAssembly.append("\tmovss [qword M+" + EnderecoHex(enderecoTemporario) + "],xmm0 \t\t; Atribui o valor do reg ao temp\n");
        }// fim if else
        rsT.endereco = enderecoTemporario;
    }//fim if

    bool tokenPositivo; // variavel auxiliar para as analises semanticas
    unsigned char rsT_auxTipo; // variavel auxiliar para geracao de codigo
    int enderecoTemporario; // variavel auxiliar para salvar endereco de temporario

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
        // Salva tipo inicial de T para converter corretamente nas operacoes aritmeticas
        rsT_auxTipo = rsT.tipo;

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

        enderecoTemporario = NovoTemp(4); // recupera endereco de memoria e aloca a proxima disponivel na area de temps
        // Realiza operacao desejada e salva em um novo temporario
        if (tokenNegativo || tokenPositivo) { // se for operacao aritmetica: subtracao ou soma
            if (rsT.tipo == TIPO_INT) { // se resultado eh inteiro, ambos T e T1 eram inteiros
                codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsT.endereco) + "]\t\t; Carrega conteudo de T.end no eax\n");
                codigoAssembly.append("\tmov ebx,[qword M+" + EnderecoHex(rsT1.endereco) + "]\t\t; Carrega conteudo de T1.end no ebx\n");
                
                // Realiza operacao entre eles
                if (tokenNegativo) { // subtracao
                    codigoAssembly.append("\tsub eax,ebx\t\t; executa a operacao de subtracao\n");
                } else { // adicao
                    codigoAssembly.append("\tadd eax,ebx\t\t; executa a operacao de adicao\n");
                }// fim if else
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(enderecoTemporario) + "],eax \t\t; move resultado para o endereco alocado\n");
            } else { // se resultado eh real, pelo menos um deles eh real
                // Guarda valor de T em um registrador
                if (rsT_auxTipo == TIPO_INT) { // valor de T eh inteiro
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsT.endereco) + "]\t\t; Carrega conteudo de T.end no eax\n");
                    // Converte em real.
                    codigoAssembly.append("\tcvtsi2ss xmm0,eax\t\t; converte reg para real\n");
                } else { // valor de T eh real
                    codigoAssembly.append("\tmovss xmm0,[qword M+" + EnderecoHex(rsT.endereco) + "]\t\t; Carrega conteudo de T.end no xmm0\n");
                }// fim if else
                
                // Guarda valor de T1 em outro registrador
                if (rsT1.tipo == TIPO_INT) { // valor de T1 eh inteiro
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsT1.endereco) + "]\t\t; Carrega conteudo de T1.end no eax\n");
                    // Converte em real.
                    codigoAssembly.append("\tcvtsi2ss xmm1,eax\t\t; converte reg para real\n");
                } else { // valor de T1 eh real
                    codigoAssembly.append("\tmovss xmm1,[qword M+" + EnderecoHex(rsT1.endereco) + "]\t\t; Carrega conteudo de T1.end no xmm1\n");
                }// fim if else

                // Realiza operacao entre eles
                if (tokenNegativo) { // subtracao
                    codigoAssembly.append("\tsubss xmm0,xmm1 \t\t; executa a operacao de subtracao\n");
                } else { // adicao
                    codigoAssembly.append("\taddss xmm0,xmm1 \t\t; executa a operacao de adicao\n");
                }// fim if else
                codigoAssembly.append("\tmovss [qword M+" + EnderecoHex(enderecoTemporario) + "],xmm0 \t\t; move resultado para o endereco alocado\n");
            }// fim if else
            
        } else { // operacao de OR
            codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsT.endereco) + "]\t\t; Carrega conteudo de T.end no eax\n");
            codigoAssembly.append("\tneg eax \t\t; Nega conteudo de T\n");
            codigoAssembly.append("\tadd eax,1 \t\t; Realiza complemento de 2 em T\n");
            codigoAssembly.append("\tmov ebx,[qword M+" + EnderecoHex(rsT1.endereco) + "]\t\t; Carrega conteudo de T1.end no ebx\n");
            codigoAssembly.append("\tneg ebx \t\t; Nega conteudo de T1\n");
            codigoAssembly.append("\tadd ebx,1 \t\t; Realiza complemento de 2 em T1\n");

            codigoAssembly.append("\tmov edx,0 \t\t; Zera registrador para multipliacao\n");
            codigoAssembly.append("\timul ebx \t\t; Executa operacao de OR\n");
            codigoAssembly.append("\tneg eax \t\t; Nega resultado\n");
            codigoAssembly.append("\tadd eax,1 \t\t; Recupera resultado logico do OR\n");

            codigoAssembly.append("\tmov [qword M+" + EnderecoHex(enderecoTemporario) + "],eax \t\t; move resultado para o endereco alocado\n");
        }//fim if else

        rsT.endereco = enderecoTemporario; // atualiza endereco de T (acumulador)

    }// fim while

    rsEXPS->tipo = rsT.tipo; // devolve tipo final da expressao S
    rsEXPS->tamanho = rsT.tamanho; // atribui tamanho a expressao superior
    rsEXPS->endereco = rsT.endereco; // atribui endereco a expressao superior
}// fim EXPS()

// Procedimento T()
// T -> F { (* | && | / | div | mod) F}
// Parte semantica: T -> F (A13){ (* (9) | && (10) | / (9) | div (11) | mod (11)) (A14) F1 (12)(A15) } (13)
void AnalisadorSintatico::T(RegistroSemantico* rsT) {
    RegistroSemantico rsF; // registro semantico auxiliar para F
    F(&rsF);

    // variaveis auxiliares para as analises semanticas
    bool tokenMultiplicacao;
    bool tokenAnd;
    bool tokenDivisao;
    bool tokenDiv;
    bool tokenMod;

    while (analisadorLexico.getToken() == TOKEN_MULTIPLICACAO || analisadorLexico.getToken() == TOKEN_AND ||
           analisadorLexico.getToken() == TOKEN_DIVISAO       || analisadorLexico.getToken() == TOKEN_DIV ||
           analisadorLexico.getToken() == TOKEN_MOD) {

        tokenDivisao = false;
        tokenMultiplicacao = false;
        tokenAnd = false;
        tokenDivisao = false;
        tokenDiv = false;
        tokenMod = false;

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
                tokenAnd = true;
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
                tokenDiv = true;
                break;

            default:
                CasaToken(TOKEN_MOD);

                if (rsF.tipo != TIPO_INT ) { // precisa ser tipo numerico inteiro
                    verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
                }//fim if
                tokenMod = true;
        }//fim switch

        RegistroSemantico rsF1; // registro semantico auxiliar para F1
        F(&rsF1);

        // if (tokenDivisao || tokenMultiplicacao) { // verifica se resultado final eh inteiro ou real
        //     if (rsF1.tipo != TIPO_INT && rsF1.tipo != TIPO_FLOAT)  { // precisa ser tipo numerico
        //         verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        //     } else if (tokenMultiplicacao) {
        //         if (rsF.tipo == TIPO_INT && rsF1.tipo == TIPO_INT) {
        //             rsF.tipo = TIPO_INT; // caso seja mult e ambos fatores sejam inteiros, o resultado final eh inteiro
        //         } else {
        //             if(rsF.tipo == TIPO_INT){
        //                 tmp_rsF_tipo = TIPO_INT;
        //             }
        //             if(rsF1.tipo == TIPO_INT){
        //                 tmp_rsF1_tipo = TIPO_INT;
        //             }
        //             rsF.tipo = TIPO_FLOAT; // caso seja mult e algum fator seja real, o resultado final eh real
        //         }// fim if else
        //     } else {
        //         rsF.tipo = TIPO_FLOAT; // caso seja divisao, o resultado final eh real
        //     }// fim if else
        // } else if (rsF.tipo == TIPO_INT && rsF1.tipo != TIPO_INT) { // realiza div ou mod e so aceita tipos inteiros. resultado inteiro
        //     verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        // } else if (rsF.tipo == TIPO_BOOL && rsF1.tipo != TIPO_BOOL) { // realiza AND binario e so aceita tipos logicos. resultado logico
        //     verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        // }// fim else if

        if (tokenDivisao || tokenMultiplicacao) { // verifica se resultado final eh inteiro ou real
            if (rsF1.tipo != TIPO_INT && rsF1.tipo != TIPO_FLOAT)  { // precisa ser tipo numerico
                verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
            } else if (tokenMultiplicacao) {
                if (rsF.tipo == TIPO_INT && rsF1.tipo == TIPO_INT) {
                    rsF.tipo = TIPO_INT; // caso seja mult e ambos fatores sejam inteiros, o resultado final eh inteiro

                    // Codigo de maquina.
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsF.endereco) + "]\t\t; Carrega conteudo de F.end no eax\n");
                    codigoAssembly.append("\tmov ebx,[qword M+" + EnderecoHex(rsF1.endereco) + "]\t\t; Carrega conteudo de F1.end no ebx\n");
                    codigoAssembly.append("\tmov edx,0 \t\t; zera reg para multiplicacao\n");
                    codigoAssembly.append("\timul ebx\t\t; executa a operacao de multiplicacao\n");
                    rsF.endereco = NovoTemp(4);
                    codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsF.endereco) + "],eax \t\t; move resultado para o endereco alocado\n");
                } else if(rsF.tipo  == TIPO_INT){
                    rsF.tipo = TIPO_FLOAT; // caso seja mult e algum fator seja real, o resultado final eh real

                    // Codigo maquina.
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsF.endereco) + "]\t\t; Carrega conteudo de F.end no eax\n");
                    // Converte em real.
                    codigoAssembly.append("\tcvtsi2ss xmm0,eax\t\t; converte e armazena em xmm0\n");
                    codigoAssembly.append("\tmovss xmm1,[qword M+" + EnderecoHex(rsF1.endereco) + "]\t\t; Carrega conteudo de F1.end no xmm1\n");
                    // Executa multiplicacao.
                    codigoAssembly.append("\tmulss xmm0,xmm1\t\t; executa a operacao de multiplicacao\n");
                    rsF.endereco = NovoTemp(4);
                    codigoAssembly.append("\tmovss [qword M+" + EnderecoHex(rsF.endereco) + "],xmm0 \t\t; move resultado para o endereco alocado\n");
                }else if(rsF1.tipo == TIPO_INT){
                    // Codigo maquina.
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsF1.endereco) + "]\t\t; Carrega conteudo de F1.end no eax\n");
                    // Converte em real.
                    codigoAssembly.append("\tcvtsi2ss xmm1,eax\t\t; converte e armazena em xmm1\n");
                    codigoAssembly.append("\tmovss xmm0,[qword M+" + EnderecoHex(rsF.endereco) + "]\t\t; Carrega conteudo de F.end no xmm0\n");
                    // Executa multiplicacao.
                    codigoAssembly.append("\tmulss xmm0,xmm1\t\t; executa a operacao de multiplicacao\n");
                    rsF.endereco = NovoTemp(4);
                    codigoAssembly.append("\tmovss [qword M+" + EnderecoHex(rsF.endereco) + "],xmm0 \t\t; move resultado para o endereco alocado\n");
                }else{
                    codigoAssembly.append("\tmovss xmm0,[qword M+" + EnderecoHex(rsF.endereco) + "]\t\t; Carrega conteudo de F1.end no xmm0\n");
                    codigoAssembly.append("\tmovss xmm1,[qword M+" + EnderecoHex(rsF1.endereco) + "]\t\t; Carrega conteudo de F1.end no xmm1\n");
                    codigoAssembly.append("\tmulss xmm0,xmm1\t\t; executa a operacao de multiplicacao\n");
                    rsF.endereco = NovoTemp(4);
                    codigoAssembly.append("\tmovss [qword M+" + EnderecoHex(rsF.endereco) + "],xmm0 \t\t; move resultado para o endereco alocado\n");
                }
            } else {
                if (rsF.tipo == TIPO_INT && rsF1.tipo == TIPO_INT) {
                    // Codigo de maquina.
                    // Carrega as duas informacoes e converte em real.
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsF.endereco) + "]\t\t; Carrega conteudo de F.end no eax\n");
                    codigoAssembly.append("\tcvtsi2ss xmm0,eax\t\t; converte e armazena em xmm0\n");
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsF1.endereco) + "]\t\t; Carrega conteudo de F1.end no eax\n");
                    codigoAssembly.append("\tcvtsi2ss xmm1,eax\t\t; converte e armazena em xmm1\n");
                    //Executa a divisao.
                    codigoAssembly.append("\tdivss xmm0,xmm1\t\t; executa a operacao de divisao\n");
                    rsF.endereco = NovoTemp(4);
                    codigoAssembly.append("\tmovss [qword M+" + EnderecoHex(rsF.endereco) + "],xmm0 \t\t; move resultado para o endereco alocado\n");
                }else if(rsF.tipo == TIPO_INT){
                    // Codigo maquina.
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsF.endereco) + "]\t\t; Carrega conteudo de F.end no eax\n");
                    // Converte em real.
                    codigoAssembly.append("\tcvtsi2ss xmm0,eax\t\t; converte e armazena em xmm0\n");
                    codigoAssembly.append("\tmovss xmm1,[qword M+" + EnderecoHex(rsF1.endereco) + "]\t\t; Carrega conteudo de F1.end no xmm1\n");
                    // Executa divisao.
                    codigoAssembly.append("\tdivss xmm0,xmm1\t\t; executa a operacao de divisao\n");
                    rsF.endereco = NovoTemp(4);
                    codigoAssembly.append("\tmovss [qword M+" + EnderecoHex(rsF.endereco) + "],xmm0 \t\t; move resultado para o endereco alocado\n");
                }else if(rsF1.tipo == TIPO_INT){
                    // Codigo maquina.
                    codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsF1.endereco) + "]\t\t; Carrega conteudo de F1.end no eax\n");
                    // Converte em real.
                    codigoAssembly.append("\tcvtsi2ss xmm1,eax\t\t; converte e armazena em xmm1\n");
                    codigoAssembly.append("\tmovss xmm0,[qword M+" + EnderecoHex(rsF.endereco) + "]\t\t; Carrega conteudo de F.end no xmm0\n");
                    // Executa divisao.
                    codigoAssembly.append("\tdivss xmm0,xmm1\t\t; executa a operacao de divisao\n");
                    rsF.endereco = NovoTemp(4);
                    codigoAssembly.append("\tmovss [qword M+" + EnderecoHex(rsF.endereco) + "],xmm0 \t\t; move resultado para o endereco alocado\n");
                }else{
                    codigoAssembly.append("\tmovss xmm0,[qword M+" + EnderecoHex(rsF.endereco) + "]\t\t; Carrega conteudo de F.end no xmm0\n");
                    codigoAssembly.append("\tmovss xmm1,[qword M+" + EnderecoHex(rsF1.endereco) + "]\t\t; Carrega conteudo de F1.end no xmm1\n");
                    codigoAssembly.append("\tdivss xmm0,xmm1\t\t; executa a operacao de divisao\n");
                    rsF.endereco = NovoTemp(4);
                    codigoAssembly.append("\tmovss [qword M+" + EnderecoHex(rsF.endereco) + "],xmm0 \t\t; move resultado para o endereco alocado\n");
                }
                rsF.tipo = TIPO_FLOAT; // caso seja divisao, o resultado final eh real
            }// fim if else
        } else if (rsF.tipo == TIPO_INT && rsF1.tipo != TIPO_INT) { // realiza div ou mod e so aceita tipos inteiros. resultado inteiro
            verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        } else if (rsF.tipo == TIPO_BOOL && rsF1.tipo != TIPO_BOOL) { // realiza AND binario e so aceita tipos logicos. resultado logico
            verificaErroTipo(analisadorLexico.num_linhas,1,""); // tipo incompativel
        } // fim if else

        // Codigo de maquina para o Div Mod e AND.
        if(tokenDiv){
            codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsF.endereco) + "]\t\t; Carrega conteudo de F.end no eax\n");
            codigoAssembly.append("\tmov ebx,[qword M+" + EnderecoHex(rsF1.endereco) + "]\t\t; Carrega conteudo de F1.end no ebx\n");
            codigoAssembly.append("\tmov edx,0 \t\t; zera reg para multiplicacao\n");
            codigoAssembly.append("\tidiv ebx\t\t; executa a operacao de div\n");
            rsF.endereco = NovoTemp(4);
            codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsF.endereco) + "],eax \t\t; move resultado para o endereco alocado\n");
        }else if(tokenMod){
            codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsF.endereco) + "]\t\t; Carrega conteudo de F.end no eax\n");
            codigoAssembly.append("\tmov ebx,[qword M+" + EnderecoHex(rsF1.endereco) + "]\t\t; Carrega conteudo de F1.end no ebx\n");
            codigoAssembly.append("\tmov edx,0 \t\t; zera reg para multiplicacao\n");
            codigoAssembly.append("\tidiv ebx\t\t; executa a operacao de div\n");
            rsF.endereco = NovoTemp(4);
            codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsF.endereco) + "],edx \t\t; move resultado para o endereco alocado\n");
        }else if(tokenAnd){
            codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsF.endereco) + "]\t\t; Carrega conteudo de F.end no eax\n");
            codigoAssembly.append("\tmov ebx,[qword M+" + EnderecoHex(rsF1.endereco) + "]\t\t; Carrega conteudo de F1.end no ebx\n");
            codigoAssembly.append("\tmov edx,0 \t\t; zera reg para multiplicacao\n");
            codigoAssembly.append("\timul ebx\t\t; executa a operacao de multiplicacao\n");
            rsF.endereco = NovoTemp(4);
            codigoAssembly.append("\tmov [qword M+" + EnderecoHex(rsF.endereco) + "],eax \t\t; move resultado para o endereco alocado\n");
        }// fim if else
    }// fim while

    rsT->tipo = rsF.tipo; // atribui o tipo da expressao final de F ao tipo de T
    rsT->tamanho = rsF.tamanho; // atribui tamanho a expressao superior
    rsT->endereco = rsF.endereco; // atribui endereco a expressao superior
} // fim T()

// Procedimento F()
// F -> ! F | (int | float) "(" EXP ")" | "(" EXP ")" | id [ "[" EXP "]" ] | constante
// Parte semantica: F -> ! F1 (1)(A12) | (int | float) (2) "(" EXP (3) ")" (A20) | "(" EXP (4) ")" (A21) | id (5)(A8) [ "[" (6) EXP (7)(A11) "]" ] | constante (8)(A9)
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

        // Aloca novo temporario com o valor do booleano (inteiro) negado
        int enderecoTemporario = NovoTemp(4); // recupera endereco de memoria e aloca a proxima disponivel na area de temps
        codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsF1.endereco) + "] \t\t; Recupera valor do inteiro (booleano)\n");
        codigoAssembly.append("\tneg eax \t\t; Nega valor inteiro do registrador\n");
        codigoAssembly.append("\tadd eax,1 \t\t; Realiza o complemento de 2\n");
        codigoAssembly.append("\tmov [qword M+" + EnderecoHex(enderecoTemporario) + "],eax \t\t; Guarda novo booleano negado no novo endereco\n");
        rsF->tamanho = 0; // zera variavel de tamanho por nao ser necessaria
        rsF->endereco = enderecoTemporario; // atribui endereco do novo temporario ao endereco do F

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

        // Realiza conversao do valor de EXP para inteiro ou float em um endereco temporario e atualiza endereco de F
        int enderecoTemporario = NovoTemp(4); // recupera endereco de memoria e aloca a proxima disponivel na area de temps
        if (rsF->tipo == TIPO_INT) { // conversao para inteiro
            if (rsEXP.tipo == TIPO_INT) { // EXP inteira
                codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXP.endereco) + "] \t\t; Recupera o valor de EXP\n");
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(enderecoTemporario) + "],eax \t\t; Atribui o valor do reg ao temp\n");
            } else { // EXP real. Precisa de conversao
                codigoAssembly.append("\tmovss xmm0,[qword M+" + EnderecoHex(rsEXP.endereco) + "] \t\t; Recupera o valor de EXP\n");
                codigoAssembly.append("\troundss xmm1, xmm0, 0b0011 \t\t;parte inteira em xmm1\n");
                codigoAssembly.append("\tcvtss2si eax, xmm1 \t\t;convertido para int\n");
                codigoAssembly.append("\tmov [qword M+" + EnderecoHex(enderecoTemporario) + "],eax \t\t; Atribui o valor do reg ao temp\n");
            }// fim if else
        } else { // conversao para real
            if (rsEXP.tipo == TIPO_INT) { // EXP inteira. Precisa de conversao
                codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXP.endereco) + "]\t\t; Carrega conteudo do endereco de EXP\n");
                codigoAssembly.append("\tcvtsi2ss xmm0,eax \t\t; converte para real e armazena valor\n");
                codigoAssembly.append("\tmovss [qword M+" + EnderecoHex(enderecoTemporario) + "],xmm0 \t\t; Atribui o valor do reg ao temp\n");
            } else { // EXP real
                codigoAssembly.append("\tmovss xmm0,[qword M+" + EnderecoHex(rsEXP.endereco) + "] \t\t; Recupera o valor de EXP\n");
                codigoAssembly.append("\tmovss [qword M+" + EnderecoHex(enderecoTemporario) + "],xmm0 \t\t; Atribui o valor do reg ao temp\n");
            }// fim if else
        }// fim if else

        rsF->endereco = enderecoTemporario; // atribui endereco temporario ao endereco de F
    } else if (analisadorLexico.getToken() == TOKEN_ABRE_PAREN) {
        CasaToken(TOKEN_ABRE_PAREN);

        RegistroSemantico rsEXP; // registro semantico auxiliar para EXP
        EXP(&rsEXP);
        rsF->tipo = rsEXP.tipo; // atribui tipo da EXP para o tipo de F
        rsF->endereco = rsEXP.endereco; // atribui endereco de EXP para o endereco de F
        rsF->tamanho = rsEXP.tamanho; // atribui tamanho de EXP para o tamanho de F

        CasaToken(TOKEN_FECHA_PAREN);
    } else if (analisadorLexico.getToken() == TOKEN_ID) {
        string lexemaID = analisadorLexico.getLexema(); // guarda o lexema do ID para buscar na tabela

        CasaToken(TOKEN_ID);

        Simbolo sID = analisadorLexico.tabelaSimbolos.recuperarSimbolo(lexemaID); // recupera simbolo na tabela de simbolos

        // verifica se id ja foi declarado
        if (sID.classe == CLASSE_VAZIA){
            verificaErroTipo(analisadorLexico.num_linhas,3,lexemaID); // id nao declarado
        } else {
            rsF->tipo = sID.tipo; // atribui tipo do id ao tipo do F
            rsF->tamanho = sID.tamanho; // atribui tamanho do id ao tamanho do F
            rsF->endereco = sID.endereco; // atribui endereco do id ao endereco do F
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

            // Aloca novo temporario com o valor do caractere na string desejada
            int enderecoTemporario = NovoTemp(1); // recupera endereco de memoria e aloca a proxima disponivel na area de temps
            codigoAssembly.append("\tmov rsi,qword M+" + EnderecoHex(rsF->endereco) + "\t\t; Pega posicao inicial da string\n");
            codigoAssembly.append("\tmov rax,0 \t\t; Limpa registrador\n");
            codigoAssembly.append("\tmov eax,[qword M+" + EnderecoHex(rsEXP.endereco) + "] \t\t; Recupera valor do indice\n");
            codigoAssembly.append("\tadd rsi,rax \t\t; Atualiza endereco para a posição desejada\n");
            codigoAssembly.append("\tmov bl,[rsi] \t\t; Recupera valor do caractere apontado\n");
            codigoAssembly.append("\tmov [qword M+" + EnderecoHex(enderecoTemporario) + "],bl \t\t; Guarda caractere no novo endereco\n");

            rsF->tamanho = 1; // atribui tamanho do caractere ao tamanho do F
            rsF->endereco = enderecoTemporario; // atribui endereco do novo temporario ao endereco do F

            CasaToken(TOKEN_FECHA_COLCH);
        }//fim if
    } else {
        Token constanteTK = analisadorLexico.getRegistroLexico(); // salva uma copia do registro lexico para salvar o tipo da constante

        CasaToken(TOKEN_CONSTANTE);

        rsF->tipo = constanteTK.tipo; // F esta devolvendo um valor do tipo da constante lida
        
        // Reserva memoria para a constante declarada
        int Endereco;
        if (rsF->tipo == TIPO_INT) {
            Endereco = NovoTemp(4); // recupera endereco e aloca proxima area disponivel
            codigoAssembly.append("\tmov eax," + constanteTK.lexema + "\t\t; movendo valor inteiro\n");
            codigoAssembly.append("\tmov [qword M+" + EnderecoHex(Endereco) + "], eax \t\t; atribuindo valor no endereco temp reservado\n");
        } else if (rsF->tipo == TIPO_CHAR) {
            Endereco = NovoTemp(1); // recupera endereco e aloca proxima area disponivel
            codigoAssembly.append("\tmov al," + constanteTK.lexema + "\t\t; movendo valor caractere\n");
            codigoAssembly.append("\tmov [qword M+" + EnderecoHex(Endereco) + "], al \t\t; atribuindo valor no endereco temp reservado\n");
        } else if (rsF->tipo == TIPO_FLOAT) {
            codigoAssembly.append("\nsection .data \t\t; Abrindo sessão de dados\n");
            // Acrescenta 0 no float caso o lexema comece com ponto
            if (constanteTK.lexema[0] == '.') {
                constanteTK.lexema = '0' + constanteTK.lexema;
            } //fim if
            codigoAssembly.append("\tdd " + constanteTK.lexema + " \t\t; Alocando 4 bytes e atribuindo valor\n");
            codigoAssembly.append("section .text \t\t; Reabrindo sessão do código\n");
            Endereco = NovoDado(4); // salva endereco disponivel e atualiza proximo endereco disponivel
        } else {
            codigoAssembly.append("\nsection .data \t\t; Abrindo sessão de dados\n");
            codigoAssembly.append("\tdb " + constanteTK.lexema + ",0 \t\t; Alocando o tamanho da string+1 em bytes e atribuindo valor\n");
            codigoAssembly.append("\nsection .text \t\t; Reabrindo sessão do código\n");
            // tamanho da string + o 0h do fim de string
            Endereco = NovoDado(constanteTK.tamanho+1); // salva endereco disponivel e atualiza proximo endereco disponivel
            rsF->tamanho = constanteTK.tamanho; // tamanho da string.
        }// fim if else

        rsF->endereco = Endereco; // atribui endereco recebido.

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