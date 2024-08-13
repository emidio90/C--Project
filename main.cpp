#include <bits/stdc++.h>
#include <algorithm>
#include <string>
using namespace std;

class Cliente{
    string nome, cognome, indirizzo, telefono;
    public:
        Cliente(string _nome, string _cognome, string _indirizzo, string _telefono){
            nome = _nome;
            cognome = _cognome;
            indirizzo = _indirizzo;
            telefono = _telefono;
        }
        void stampaDettagli(){
            cout << "Nome: " << nome << "\nCognome: " << cognome << "\nIndirizzo: " << indirizzo << "\nTelefono: " << telefono << endl;
        }
        void setNome(string _nome){
            nome = _nome;
        }
        void setCognome(string _cognome){
            cognome = _cognome;
        }
        void setIndirizzo(string _indirizzo){
            indirizzo = _indirizzo;
        }
        void setTelefono(string _telefono){
            telefono = _telefono;
        }
};
class GestioneClienti{
    vector<Cliente> clienti;
    FILE *file;
    GestioneClienti(FILE* _file){
        file = _file;
        char percorso_al_file[20];
        strcpy(percorso_al_file, "clienti.txt");
        file = fopen(percorso_al_file, "a+");
    }
};

void print_menu();
void make_choice();

int main(){
    FILE *file;
    char percorso_al_file[20];
    strcpy(percorso_al_file, "clienti.txt");
    file = fopen(percorso_al_file, "a+");
    print_menu();
    make_choice();
    return 0;
}

void print_menu(){
    printf(" ---------------------------------\n");
    printf("| Customer Relationship Management |\n");
    printf(" ---------------------------------\n");
    printf("\n1) Visualizza clienti esistenti\n");
    printf("2) Inserisci cliente\n");
    printf("3) Cerca cliente per nome o cognome\n");
    printf("4) Modifica o rimuovi cliente\n");
}
void make_choice(){
    int choice;
    std::set<int> mySet {1,2,3,4};
    printf("\nSelezione un'opzione: ");
    scanf("%d", &choice);
    if (mySet.find(choice) != mySet.end()){
        switch (choice)
        {
        case 1:
            /* code */
            break;
        
        default:
            break;
        }
    }
    else{
        printf("Scelta non valida. Inserisci un numero compreso fra 1 e 4.");
        make_choice();
    }
}