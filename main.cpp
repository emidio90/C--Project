#include <iostream>
#include <algorithm>
#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class Interazione {
public:
    string tipo;
    string data;
    string descrizione;

    Interazione() {}

    Interazione(string _tipo, string _data, string _descrizione) {
        tipo = _tipo;
        data = _data;
        descrizione = _descrizione;
    }

    void stampaDettagli() const {
        cout << "Tipo: " << tipo << endl;
        cout << "Data: " << data << endl;
        cout << "Descrizione: " << descrizione << endl;
    }

    string serializza() const {
        return tipo + "," + data + "," + descrizione;
    }
};

class Cliente {
public:
    string nome, cognome, indirizzo, telefono;
    vector<Interazione> interazioni;

    Cliente() {}

    Cliente(string _nome, string _cognome, string _indirizzo, string _telefono) {
        nome = _nome;
        cognome = _cognome;
        indirizzo = _indirizzo;
        telefono = _telefono;
    }

    void stampaDettagli() const {
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

class GestioneClienti {
private:
    vector<Cliente> clienti;
    string percorso_al_file;

public:
    GestioneClienti(const string& percorso) {
        percorso_al_file = percorso;
        caricaClienti();
    }

    ~GestioneClienti() {
        salvaClienti();
    }

    void caricaClienti() {
        ifstream file(percorso_al_file);
        if (!file.is_open()) {
            cerr << "Errore nell'apertura del file CSV." << endl;
            return;
        }

        string linea;
        while (getline(file, linea)) {
            stringstream ss(linea);
            string nome, cognome, indirizzo, telefono, interazioni_str;

            getline(ss, nome, ',');
            getline(ss, cognome, ',');
            getline(ss, indirizzo, ',');
            getline(ss, telefono, ',');

            Cliente cliente(nome, cognome, indirizzo, telefono);

            // Carica le interazioni
            if (getline(ss, interazioni_str)) {
                stringstream interazioni_ss(interazioni_str);
                string interazione_singola;
                while (getline(interazioni_ss, interazione_singola, ';')) {
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
                file << cliente.nome << "," << cliente.cognome << ","
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

    void visualizzaClienti() const {
        for (const auto& cliente : clienti) {
            cliente.stampaDettagli();
            cout << "---------------------------" << endl;
        }
    }

    void aggiungiCliente() {
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
    }

    void modificaCliente(string& nome, string& cognome) {
        for (auto& cliente : clienti) { // Scorre fra tutti i clienti
            if (cliente.nome == nome && cliente.cognome == cognome) { // Se i dati combaciano, modifica il cliente
                cout << "Modifica nome (attuale: " << cliente.nome << "): ";
                cin.ignore(); // Ignora newline precedente
                getline(cin, cliente.nome);
                cout << "Modifica cognome (attuale: " << cliente.cognome << "): ";
                getline(cin, cliente.cognome);
                cout << "Modifica indirizzo (attuale: " << cliente.indirizzo << "): ";
                getline(cin, cliente.indirizzo);
                cout << "Modifica telefono (attuale: " << cliente.telefono << "): ";
                cin >> cliente.telefono;
                salvaClienti();
                return;
            }
        }
        cout << "Cliente non trovato." << endl;
    }

    void rimuoviCliente(string& nome, string& cognome) {
        for (auto it = clienti.begin(); it != clienti.end(); ++it) { // Scorre fra tutti i clienti
            if (it->nome == nome && it->cognome == cognome) { // Se i dati combaciano
                clienti.erase(it); // Rimuove il cliente dal vettore clienti
                salvaClienti();
                return;
            }
        }
        cout << "Cliente non trovato." << endl;
    }

    Cliente* cercaCliente (const string& nome, const string& cognome) {
        for (auto& cliente : clienti) {
            if (cliente.nome == nome && cliente.cognome == cognome) {
                return &cliente; // Restituisce un puntatore al cliente trovato
            }
        }
        return nullptr; // Cliente non trovato
    }

    const Cliente* cercaCliente(const string& nome, const string& cognome) const {  //overload cercaCliente versione const per sola visualizzazione
            for (auto& cliente : clienti) {
                if (cliente.nome == nome && cliente.cognome == cognome) {
                    return &cliente;  // Restituisce un puntatore const al cliente trovato
                }
            }
            return nullptr;  // Cliente non trovato
        } 

    void cercaEVisualizzaCliente(const string& nome, const string& cognome) const {
        const Cliente* cliente = cercaCliente(nome, cognome);
        if (cliente) {
            printf("\n--- Cliente trovato ---\n");
            cliente->stampaDettagli();
        } else {
            cout << "Cliente non trovato." << endl;
        }
    }

    void aggiungiInterazioneCliente(const string& nome, const string& cognome, const Interazione& interazione) {
        Cliente* cliente = cercaCliente(nome, cognome);
        if (cliente) {
            cliente->aggiungiInterazione(interazione);
            salvaClienti(); // Salva dopo aver aggiunto l'interazione
        } else {
            cout << "Cliente non trovato." << endl;
        }
    }

    void visualizzaInterazioniCliente(const string& nome, const string& cognome) {
        Cliente* cliente = cercaCliente(nome, cognome);
        if (cliente) {
            cliente->visualizzaInterazioni();
        } else {
            cout << "Cliente non trovato." << endl;
        }
    }
};

void print_menu();
void make_choice(GestioneClienti& gestione);
void pause_and_continue();

int main() {
    GestioneClienti gestione("clienti.csv");
    make_choice(gestione);
    return 0;
}

void print_menu() {
    printf(" ---------------------------------\n");
    printf("| Customer Relationship Management |\n");
    printf(" ---------------------------------\n");
    printf("\n1) Visualizza clienti esistenti\n");
    printf("2) Inserisci cliente\n");
    printf("3) Cerca cliente per nome o cognome\n");
    printf("4) Modifica o rimuovi cliente\n");
    printf("5) Visualizza le interazione di un cliente\n");
    printf("6) Aggiungi interazioni ad un cliente\n");
    printf("7) Esci\n");
}

void pause_and_continue() {
    cout << "\nPremi Invio per continuare...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignora tutto fino al prossimo newline
    cin.get(); // Attende che l'utente prema Invio
}

void make_choice(GestioneClienti& gestione) {
    int choice;
    set<int> validChoices {1, 2, 3, 4, 5, 6, 7};

    while (true) {
        print_menu(); // Mostra il menu prima di ogni input
        cout << "\nSeleziona un'opzione: ";
        cin >> choice;

        if (cin.fail()) { // Controlla se l'input non è valido
            cin.clear(); // Ripristina lo stato valido di cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignora il resto dell'input fino al newline
            cout << "Scelta non valida. Inserisci un numero compreso fra 1 e 7." << endl;
            continue; // Torna all'inizio del ciclo
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Pulisce il newline rimanente

        string nome, cognome, tipo, data, descrizione;
        char scelta;

        switch (choice) {
            case 1:
                cout << "\n--- Visualizzazione clienti esistenti ---\n";
                gestione.visualizzaClienti();
                pause_and_continue();
                break;
            case 2:
                cout << "\n--- Inserimento nuovo cliente ---\n";
                gestione.aggiungiCliente();
                pause_and_continue();
                break;
            case 3:
                cout << "\n--- Ricerca cliente ---\n";
                cout << "Inserisci nome del cliente: ";
                getline(cin, nome);
                cout << "Inserisci cognome del cliente: ";
                getline(cin, cognome);
                gestione.cercaEVisualizzaCliente(nome, cognome);
                pause_and_continue();
                break;
            case 4:
                cout << "\n--- Modifica o rimozione cliente ---\n";
                cout << "Inserisci nome del cliente: ";
                getline(cin, nome);
                cout << "Inserisci cognome del cliente: ";
                getline(cin, cognome);
                cout << "Inserisci m per modificare o r per rimuovere un cliente: ";
                cin >> scelta;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Pulisce il newline rimanente
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
                cout << "\n--- Visualizzazione interazioni del cliente ---\n";
                cout << "Inserisci nome del cliente: ";
                getline(cin, nome);
                cout << "Inserisci cognome del cliente: ";
                getline(cin, cognome);
                gestione.visualizzaInterazioniCliente(nome, cognome);
                pause_and_continue();
                break;
            case 6:
                cout << "\n--- Aggiunta interazione per cliente ---\n";
                cout << "Inserisci nome del cliente: ";
                getline(cin, nome);
                cout << "Inserisci cognome del cliente: ";
                getline(cin, cognome);

                // Controlla se il cliente esiste prima di procedere
                if (gestione.cercaCliente(nome, cognome) == nullptr) {
                    cout << "Cliente non trovato. Impossibile aggiungere l'interazione." << endl;
                    pause_and_continue();
                    break;
                }

                cout << "Inserisci il tipo di interazione: ";
                getline(cin, tipo);  // Lettura del tipo di interazione

                cout << "Inserisci la data dell'interazione: ";
                getline(cin, data);  // Lettura della data

                cout << "Inserisci una descrizione: ";
                getline(cin, descrizione);  // Lettura della descrizione

                gestione.aggiungiInterazioneCliente(nome, cognome, Interazione(tipo, data, descrizione));
                pause_and_continue();
                break;
            case 7:
                cout << "\nUscita dal programma...\n";
                return; // Esce dal ciclo, terminando così il programma
            default:
                cout << "Scelta non valida. Inserisci un numero compreso fra 1 e 7." << endl;
                pause_and_continue();
                break;
        }
    }
}
