/**

    Uma árvore de pesquisa binária é uma árvore rubro-negra se satifaz às
    seguintes propriedades: 
    1. Todo nó é rubro ou preto
    2. A raiz é preta.
    3. Toda folha (NIL) é preta.
    4. Se um nó é rubro, então ambos os seus filhos são pretos.
    5. Para cada nó, todos os caminhos de um nó até as folhas descendentes 
    contêm os mesmo número d e nós pretos.

    @author Moroni Neres Vieira
    @version 1.0 11/2018
*/

//Bibliotecas de funções internas 
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <exception>
#include <stdexcept>

//Bibliotecas externas
#include "struct.h" //Definição da estrutura de uma árvore binária
#include "functions.h" //Funções para árvore rubro-negra

using namespace std;

int lerpalavras(string);
void menu();
void opcoes(int);

int main(int argc, char *argv[]) {

    ifstream file;
    string arquivo;
    arquivo=argv[1];
    file.open(arquivo);

    //Cria a estrutura da árvore
    create_Nill_and_Root();

    if(!file.is_open()){
        cout << "Arquivo informado não existe ou não pode ser aberto!" << endl;  
        return -1;

    }else{
        lerpalavras(argv[1]); 
    }
    
    return 0;
}

int lerpalavras(string nomearq) {
    ifstream dic;
    string linha; 

    dic.open(nomearq);

    while (!dic.eof()) {
        getline(dic, linha);
        if(!linha.empty()){

            string::size_type pos1 = linha.find_first_of(' '); //Alias para string:size_type e a posição da palavra
            string palavra = linha.substr(0, pos1);
            string::size_type pos2 = linha.find_first_of(' ');
            string num = linha.substr(pos1+1, pos2);
                
            int opcao = stoi(num);
            
                if(palavra.length() > 20){
                    cout << "Chave não pode ser inserida por ter tamanho > 20" << endl;

                }else{
                    if (opcao == 1){
                        cout << "Inserindo " << palavra << endl;
                        insert_rb(palavra);
                    }else{
                    cout << "Removendo " << palavra << endl;
                    delete_rb(palavra);
                }

       }
    }
}


menu();

dic.close();

return 0;
}

//Menu de opções
void menu(){
    int opcao;
    string key;

    do{

        cout<<"\n\n\n";
        cout << "Após a inclusão ou exclusão deseja fazer alguma operação: \n";

        cout<<"(1) - Busca\n";
        cout<<"(2) - Imprimir\n";
        cout<<"(3) - Checar\n";
        cout<<"(0) - Exit\n";
        cout<<"Entre com uma opção:" << endl;
        
            cin >> opcao;

            switch(opcao){
                case 1:
                {
                    cout << "Entre com a chave para busca: " << endl;
                    cin >> key;
                    
                    node * aux = search_node(root, key);
                    if(aux != nill){
                        cout<<"Nó encontrado\n";
                        cout<<"(";
                        if(aux->parent!=nill)
                            cout<<aux->parent->key<<", ";
                        else
                            cout<<"NILL, ";
                        cout<<aux->key<<", ";
                        if(aux->color==RED)
                            cout<<" Vermelho, ";
                        else
                            cout<<" Preto, ";
                        cout<<blackHeightNode(key)<<", ";
                        if(aux->left != nill)
                            cout<<aux->left->key<<", ";
                        else
                            cout<<"NILL, ";
                        if(aux->right != nill)
                            cout<<aux->right->key<<" )\n";
                        else
                            cout<<"NILL )\n";
                    }
                    else
                        cout<<"Nó não encontrado!\n";
                }       break;

                case 2:
                {
                    cout << "Imprimindo a árvore" << endl;
                    RBPrint(root);
                }
        
                case 3:
                    cout << "Checando a árvore" << endl;
                    RBCheck(root);
                case 0:
                    break;

                default:
                    cout << "Opção inválida, entre com uma opção do menu válida!";
            }
        }while(opcao!=0);
}
