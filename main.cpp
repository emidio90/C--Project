#include <bits/stdc++.h>
#include <algorithm>

void print_menu();
void make_choice();

int main(){
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
        printf("ok");
    }
    else{
        printf("Scelta non valida. Inserisci un numero compreso fra 1 e 4.");
        make_choice();
    }
}