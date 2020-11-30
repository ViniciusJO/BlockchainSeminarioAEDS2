#include <iostream>
#include <string>
#include <chrono>
#include "sha256.h"
using namespace std;
using namespace chrono;

#define DIFICULDADE 3
#define OBJETIVO "000"

struct Bloco{
    string registro;
    int indice;
    int nounce;
    string hash;
    Bloco* anterior;
    time_t criacao;

    string texto(){
        return (string)(to_string(indice) + anterior->hash + to_string(nounce) + registro + to_string(criacao));
    }
    
    string gerarHash(){
        return sha256(texto());
    }

    bool validar(){
        return hash.compare(gerarHash()) && gerarHash().substr(0,DIFICULDADE).compare(OBJETIVO);
    }
    
    Bloco(string opcao){
        if(opcao=="GENESIS"){
            registro = "Este e o bloco genesis";
            indice = 0;
            anterior = nullptr;
            criacao = system_clock::to_time_t(system_clock::now());
            hash = "GENESIS";
            nounce = 0;
        }
    }

    Bloco(Bloco* block, string reg){
        criacao = system_clock::to_time_t(system_clock::now());
        indice = block->indice+1;
        anterior = block;
        block = this;
        registro = reg;
        nounce = 0;
        while(gerarHash().substr(0,DIFICULDADE)!=OBJETIVO){ //158
            nounce++;
        }
        hash = gerarHash();
        // cout << registro << endl << nounce << endl << hash << endl << gerarHash() << endl;
        // cout << "Bloco criado em: " << system_clock::to_time_t(system_clock::now())-criacao << " segundos" << endl;
    }
};

struct Blockchain{
    Bloco* genesis;
    Bloco* ultimoBloco;

    bool validar(){
        Bloco* it = ultimoBloco;
        while(it!=genesis){
            if(it->validar()){
                return false;
            }
            it = it->anterior;
        }
        return true;
    }

    void adicionarBloco(string reg){
        ultimoBloco = new Bloco(this->ultimoBloco, reg);
    }

    Blockchain(){
        genesis = new Bloco("GENESIS");
        ultimoBloco = genesis;
    }
};

#include "menu.h"

int main(){
    Blockchain* blockchain = new Blockchain();
    blockchain->adicionarBloco("Teste");
    blockchain->adicionarBloco("Teste1");
    blockchain->adicionarBloco("Teste2");
    blockchain->adicionarBloco("Teste3");
    blockchain->adicionarBloco("Teste4");

    Bloco* penultimoBloco = blockchain->ultimoBloco->anterior;
    //Descomentando a linha abaixo altera-se o registro do penultimo bloco, permitindo testar a validação da blockchain
    //penultimoBloco->registro = "Teste alterando dados";
    
    menu(blockchain);
    
    return 0;
}