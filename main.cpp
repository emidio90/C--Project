#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
using namespace std;

class Interazione {
    public:
        string tipo;         // Tipo di interazione ("Appuntamento", "Contratto",...)
        string data;         
        string descrizione;  

        Interazione() {}

        Interazione(string _tipo, string _data, string _descrizione){
            tipo = _tipo;
            data = _data;
            descrizione = _descrizione;
        }

        void stampaDettagli() const {
            cout << "Tipo: " << tipo << endl;
            cout << "Data: " << data << endl;
            cout << "Descrizione: " << descrizione << endl;
        }

        string serializza() const {   // Serializza l'interazione per l'inserimento nel CSV
            return tipo + "," + data + "," + descrizione;
        }
};
class Cliente{
    public:
        string nome, cognome, indirizzo, telefono;
        vector<Interazione> interazioni;
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
            cout << "Interazioni:" << endl;
            for (const auto& interazione : interazioni) {
                interazione.stampaDettagli();
                cout << "---------------------------" << endl;
            }
        }

        void aggiungiInterazione(const Interazione& interazione) {
        interazioni.push_back(interazione);
        }

        string serializzaInterazioni() const {
            string risultato;
            for (size_t i = 0; i < interazioni.size(); ++i) {
                if (i > 0) risultato += ";"; // separatore per diverse interazioni
                risultato += interazioni[i].serializza();
            }
            return risultato;
        }

        void visualizzaInterazioni() const {
            if (interazioni.empty()) {
                cout << "Nessuna interazione presente." << endl;
            } else {
                for (const auto& interazione : interazioni) {
                    interazione.stampaDettagli();
                    cout << "---------------------------" << endl;
                }
            }
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

        void caricaClienti() {
            ifstream file(percorso_al_file);
            if (!file.is_open()) {
                cerr << "Errore nell'apertura del file CSV." << endl;
                return;
            }

            string riga;
            while (getline(file, riga)) {      // Legge ogni riga del file
                stringstream ss(riga);
                string nome, cognome, indirizzo, telefono, interazioni_str;
                // Suddivide la riga in base alle virgole e memorizza i char nelle variabili
                getline(ss, nome, ',');
                getline(ss, cognome, ',');
                getline(ss, indirizzo, ',');
                getline(ss, telefono, ',');

                Cliente cliente(nome, cognome, indirizzo, telefono);

                // Carica le interazioni
                if (getline(ss, interazioni_str)) {
                    stringstream interazioni_ss(interazioni_str);
                    string interazione_singola;
                    while (getline(interazioni_ss, interazione_singola, ';')) {   // Suddivide le interazione in base al punto e virgola
                        string tipo, data, descrizione;
                        stringstream iss(interazione_singola);
                        getline(iss, tipo, ',');
                        getline(iss, data, ',');
                        getline(iss, descrizione, ',');
                        cliente.aggiungiInterazione(Interazione(tipo, data, descrizione));
                    }
                }
                clienti.push_back(cliente);
            }
            file.close();
        }

         void salvaClienti() {
            ofstream file(percorso_al_file.c_str());
            if (file.is_open()) {
                for (const auto& cliente : clienti) {
                    file << cliente.nome << "," << cliente.cognome << ","         // Aggiunge i dati del cliente in fondo al file
                        << cliente.indirizzo << "," << cliente.telefono;

                    // Salva le interazioni
                    string interazioni_serializzate = cliente.serializzaInterazioni();
                    if (!interazioni_serializzate.empty()) {
                        file << "," << interazioni_serializzate;
                    }
                    file << endl;
                }
                file.close();
            }
        }

        void visualizzaClienti(){
            cout << "Elenco clienti esistenti:\n" << endl;
            for (size_t i = 0; i < clienti.size(); i++) {
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
        void modificaCliente(string& nome, string& cognome){
            for (size_t i = 0; i < clienti.size(); i++) {                           //scorre fra tutti i clienti
                if (clienti[i].nome == nome && clienti[i].cognome == cognome) {  //se i dati combaciano modifica il cliente
                    cout << "Modifica nome (attuale: " << clienti[i].nome << "): ";
                    cin.ignore();                                                //ignora newline precedente
                    getline(cin, clienti[i].nome);
                    cout << "Modifica cognome (attuale: " << clienti[i].cognome << "): ";
                    getline(cin, clienti[i].cognome);
                    cout << "Modifica indirizzo (attuale: " << clienti[i].indirizzo << "): ";
                    getline(cin, clienti[i].indirizzo);
                    cout << "Modifica telefono (attuale: " << clienti[i].telefono << "): ";
                    cin >> clienti[i].telefono;
                    salvaClienti();
                    cout << "Cliente modificato con successo." << endl;
                    return;
                }
            }
            cout << "Cliente non trovato." << endl;
        }
        void rimuoviCliente(string& nome, string& cognome){
            for (size_t i = 0; i < clienti.size(); i++) {                           //scorre fra tutti i clienti
                if (clienti[i].nome == nome && clienti[i].cognome == cognome) {  //se i dati combaciano
                    clienti.erase(clienti.begin() + i);                          //rimuove il cliente dal vettore clienti
                    salvaClienti();
                    cout << "Cliente rimosso con successo." << endl;
                    return;
                }
            }
            cout << "Cliente non trovato." << endl;
        }
        void cercaCliente(string& nome, string& cognome) {
            bool trovato = false;
            for (size_t i = 0; i < clienti.size(); i++) {
                if (clienti[i].nome == nome && clienti[i].cognome == cognome) {
                    clienti[i].stampaDettagli();
                    trovato = true;
                    break;  
                }
            }
            if (!trovato) {
                cout << "Cliente non trovato." << endl;
            }
        }
        Cliente* cercaCliente(const string& nome, const string& cognome) {
            for (auto& cliente : clienti) {
                if (cliente.nome == nome && cliente.cognome == cognome) {
                    return &cliente;  // Restituisce un puntatore al cliente trovato
                }
            }
            return nullptr;  // Cliente non trovato
        }

    void aggiungiInterazioneCliente(const string& nome, const string& cognome, const Interazione& interazione) {
        Cliente* cliente = cercaCliente(nome, cognome);
        if (cliente) {
            cliente->aggiungiInterazione(interazione);            //Se trova il cliente, aggiunge l'interazione
            salvaClienti();
            cout << "Interazione aggiunta con successo." << endl;
        } else {
            cout << "Cliente non trovato." << endl;
        }
    }

    void visualizzaInterazioniCliente(const string& nome, const string& cognome) {
        Cliente* cliente = cercaCliente(nome, cognome);
        if (cliente) {
            cliente->visualizzaInterazioni();                      //Se trova il cliente, visualizza l'interazione
        } else {
            cout << "Cliente non trovato." << endl;
        }
    }
};

void print_menu();
void make_choice(GestioneClienti& gestione);

int main(){
    GestioneClienti gestione("clienti.csv");
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
    printf("5) Visualizza le interazione di un cliente\n");
    printf("6) Aggiungi interazioni ad un cliente\n");
    printf("7) Esci dal programma\n");
}

//Funzione per attendere che l'utente prema Invio prima di proseguire
void pause_and_continue(){
    cout << "\nPremi Invio per continuare...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignora tutto fino al prossimo newline
    cin.get(); // Attende che l'utente prema Invio
}

void make_choice(GestioneClienti& gestione) {
    int choice;
    set<int> validChoices {1, 2, 3, 4, 5, 6, 7};
    print_menu();
    printf("\nSeleziona un'opzione: ");
    while (true) {
        if (scanf("%d", &choice) == 1 && validChoices.find(choice) != validChoices.end()) {
            break;  // L'input è valido
        } else {
            printf("Scelta non valida. Inserisci un numero compreso fra 1 e 7: ");
            scanf("%*[^\n]");  // pulisce il buffer
            scanf("%*c");      // cancella il carattere newline
        }
    }

    string nome, cognome, tipo, data, descrizione;
    char scelta;

    switch (choice) {
        case 1:
            gestione.visualizzaClienti();
            pause_and_continue();
            break;
        case 2:
            gestione.aggiungiCliente();
            pause_and_continue();
            break;
        case 3:
            cout << "Inserisci nome del cliente: ";
            cin >> nome;
            cout << "Inserisci cognome del cliente: ";
            cin >> cognome;
            gestione.cercaCliente(nome, cognome);
            pause_and_continue();
            break;
        case 4:
            cout << "Inserisci nome del cliente: ";
            cin >> nome;
            cout << "Inserisci cognome del cliente: ";
            cin >> cognome;
            cout << "Inserisci m per modificare o r per rimuovere un cliente: ";
            cin >> scelta;
            if (scelta == 'm') {
                gestione.modificaCliente(nome, cognome);
            } else if (scelta == 'r') {
                gestione.rimuoviCliente(nome, cognome);
            } else {
                cout << "Scelta non valida. Riprova." << endl;
            }
            pause_and_continue();
            break;
        case 5:
            cout << "Inserisci nome del cliente: ";
            cin >> nome;
            cout << "Inserisci cognome del cliente: ";
            cin >> cognome;
            gestione.visualizzaInterazioniCliente(nome, cognome);
            pause_and_continue();
            break;
        case 6:
            cout << "Inserisci nome del cliente: ";
            cin >> nome;
            cout << "Inserisci cognome del cliente: ";
            cin >> cognome;
            cout << "Inserisci il tipo di interazione: ";
            cin.ignore();  
            getline(cin, tipo);
            cout << "Inserisci la data dell'interazione: ";
            getline(cin, data);
            cout << "Inserisci una descrizione: ";
            getline(cin, descrizione);
            gestione.aggiungiInterazioneCliente(nome, cognome, Interazione(tipo, data, descrizione));
            //pause_and_continue();
            break;
        case 7:
            cout << "\nUscita dal programma...\n";
            return; // Esce dal ciclo, terminando così il programma
        default:
            break;
    }
    make_choice(gestione);
}