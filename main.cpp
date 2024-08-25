#include <bits/stdc++.h>
#include <algorithm>
#include <string>
using namespace std;

class Cliente{
    public:
        string nome, cognome, indirizzo, telefono;
        Cliente(){}
        Cliente(string _nome, string _cognome, string _indirizzo, string _telefono){
            nome = _nome;
            cognome = _cognome;
            indirizzo = _indirizzo;
            telefono = _telefono;
        }
       void stampaDettagli() {
            cout << "Nome: " << nome << endl;
            cout << "Cognome: " << cognome << endl;
            cout << "Indirizzo: " << indirizzo << endl;
            cout << "Telefono: " << telefono << endl;
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
    private: 
        vector<Cliente> clienti;
        string percorso_al_file;
    public:
        GestioneClienti(const string& percorso){
            percorso_al_file = percorso;
            caricaClienti();
        }
        ~GestioneClienti(){
            salvaClienti();
        }
        void caricaClienti(){
            ifstream file(percorso_al_file);
            if (!file.is_open()) {
                cerr << "Errore nell'apertura del file CSV." << endl;
                return;
            }

            string linea;
            while (getline(file, linea)) {
                stringstream ss(linea);
                string nome, cognome, indirizzo, telefono;

                getline(ss, nome, ',');
                getline(ss, cognome, ',');
                getline(ss, indirizzo, ',');
                getline(ss, telefono, ',');

                clienti.emplace_back(nome, cognome, indirizzo, telefono);
            }
            file.close();
        }
        void salvaClienti() {
            ofstream file(percorso_al_file.c_str());
            if (file.is_open()) {
                for (std::size_t i = 0; i < clienti.size(); i++) {
                    file << clienti[i].nome << "," << clienti[i].cognome << ","
                        << clienti[i].indirizzo << "," << clienti[i].telefono << endl;
                }
                file.close();
            }
        }
        void visualizzaClienti(){
            cout << "Elenco clienti esistenti:\n" << endl;
            for (std::size_t i = 0; i < clienti.size(); i++) {
            clienti[i].stampaDettagli();
            cout << "---------------------------" << endl;
            }
        }
        void aggiungiCliente(){
            string nome, cognome, indirizzo, telefono;
            printf("Inserisci nome: ");
            cin >> nome;
            printf("Inserisci cognome: ");
            cin >> cognome;
            cin.ignore();
            printf("Inserisci indirizzo: ");
            getline(cin, indirizzo);
            printf("Inserisci numero di telefono: ");
            cin >> telefono;
            Cliente cliente(nome, cognome, indirizzo, telefono);
            clienti.push_back(cliente);
            salvaClienti();
            printf("Cliente inserito con successo");
        }
};

void print_menu();
void make_choice(GestioneClienti& gestione);

int main(){
    GestioneClienti gestione("clienti.csv");
    print_menu();
    make_choice(gestione);
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
void make_choice(GestioneClienti& gestione){
    int choice;
    std::set<int> mySet {1,2,3,4};
    printf("\nSelezione un'opzione: ");
    scanf("%d", &choice);
    if (mySet.find(choice) != mySet.end()){
        switch (choice)
        {
        case 1:
            gestione.visualizzaClienti();
            break;
        case 2:
            gestione.aggiungiCliente();
            break;
        default:
            break;
        }
        make_choice(gestione);
    }
    else{
        printf("Scelta non valida. Inserisci un numero compreso fra 1 e 4.");
        make_choice(gestione);
    }
}