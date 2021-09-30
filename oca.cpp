#include <iostream>
#include <random>
#include <string.h>
using namespace std;

struct Player{
  string nome;
  int somma_dadi;
  int casella;
  int carcerato;
};
using funcptr = int(*)(Player&);
funcptr percorso[63];

int partita_finita = 0;
int trigger_check = 0;
///////////////////////////////////////////////////////////////////////////////////////// LANCIO DADI
int _dado(){
  return rand() % 6 + 1;
}

int tira_dadi(Player& p){
  int dado_uno = _dado();
  int dado_due = _dado();
  p.somma_dadi = dado_uno+dado_due;
  cout << p.nome << " ha tirato i dadi, ed e' uscito " << dado_uno << " e " << dado_due << "!" << endl;
  cout <<   dado_due + dado_uno << endl;
  percorso[p.casella](p);
  return dado_uno + dado_due;
}
/////////////////////////////////////////////////////////////////////////////////////////   CASELLE
int _oca(Player& p){
  cout << p.nome << " salta in groppa ad un'oca e va avanti di " << p.somma_dadi << "caselle" << endl;
  p.casella = p.casella+p.somma_dadi;
  percorso[p.casella](p);
  return 0;
}
int _ponte(Player& p){
  cout << p.nome << " sta passando per un ponte!" << endl << "Rilancia i dadi!" << endl;
  tira_dadi(p);
  return 0;
}
int _locanda(Player& p){
  cout << p.nome << " e' andato alla locanda!" << endl << "Si ferma a bere per 3 turni" << endl;
  p.carcerato = 3;
  return 0;
}
int _pozzoPrigione(Player& p){
  cout << p.nome << " e' finito nel pozzo o nella prigione!"<< endl << "Stara' li' per un po'..." << endl;
  trigger_check = 1;
  return 0;
}
int _labirinto(Player& p){
  cout << p.nome << " e' finito nel labirinto!" << endl << "Si perde e si ritrova nella casella 39!" << endl;
  p.casella == 39;
  return 0;
}
int _scheletro(Player& p){
  cout << p.nome << " e' morto!- F\nRicomincera' da capo, gl" << endl;
  p.casella = 0;
  return 0;
}
int _finale(Player& p){
  partita_finita == 1;
  cout << "La partita e' finita! Vince " << p.nome << "!" << endl;
  return 0;
}
int _generica(Player& p){
  cout << p.nome << " non fa niente di speciale..." << endl;
  return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////   rules
int checkPrigione(Player p[], int n, int player_scambio){
  for(int x = 0; x < n; x++){
    if(p[x].carcerato == 1 && x != player_scambio){

    }
  }
  trigger_check == 0;
  return 0;
}
void makePercorso(funcptr percorso[]){
  for(int x = 0; x < 63; x++){
    int oca_tmp[7] = {5,9,18,27,36,45,54};
    for(int y=0; y>7; y++){
      if(x == oca_tmp[y]){
        percorso[x] = &_oca;
      }
    }
    if(x == 6){
      percorso[x] = &_ponte;
    }
    else if(x == 19){
      percorso[x] = &_locanda;
    }
    else if(x == 31 || x == 52){
      percorso[x] = &_pozzoPrigione;
    }
    else if(x == 42){
      percorso[x] = &_labirinto;
       }
    else if(x == 58){
      percorso[x] = &_scheletro;
    }
    else if(x == 62){
      percorso[x] = &_finale;
    }
    else{
      percorso[x] = &_generica;
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////
int main(){
  srand(time(NULL));
  /////////////// INIT PERCORSO
  makePercorso(percorso);
  /////////////// INPUT NUMERO GIOCATORI
  cout << "Quanti giocatori ci sono?" << endl;
  int n_players;
  cin >> n_players;
  Player players[n_players];
  /////////////// INIT PLAYERS STRUCT
  for(int x = 0; x < n_players; x++){
    cout << "Nome del giocatore: " << endl;
    cin >> players[x].nome;
    cout << "Ciao " << players[x].nome << endl;
    players[x].somma_dadi = 0;
    players[x].casella = 0;
    players[x].carcerato = 0;
  }
  /////////////// MAINLOOP
  while(partita_finita == 0){
    for(int turno = 0; turno < n_players; turno++){
      cout << "e' il turno di " << players[turno].nome << "!" << endl;
      if(players[turno].carcerato == 0){
        players[turno].somma_dadi = tira_dadi(players[turno]);
      }
      //se il player arriva a una casella >62, torna indietro di tante caselle quante sono quelle dopo il 63
      if(players[turno].casella > 62){
        players[turno].casella == 62 - (players[turno].casella - 62);
      }
      cout << players[turno].nome << " e' nella casella " << players[turno].casella << endl;
      percorso[players[turno].casella](players[turno]);
      //check per scambio nella prigione (se un player arriva nella casella prigione, aspetta li' finche' non lo salvano)
      if(trigger_check == 1){
        checkPrigione(players, n_players, turno);
      }
    }
  }

  return 0;
}
