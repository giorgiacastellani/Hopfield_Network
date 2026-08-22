#ifndef RECALL_HPP
#  define RECALL_HPP

#  include "acquisition.hpp"
#  include "training.hpp"
#  include <vector>

namespace Hopfield {

double calculateEnergy(const HopfieldPattern& pattern,
                       const std::vector<std::vector<double>>& weights);

void patternUpdate(Hopfield::HopfieldPattern& pattern,
                   std::vector<std::vector<double>>& weights);

} // namespace Hopfield
#endif

//---------------------
//------ PROGETTO -----
//---------------------

// metodo (della classe HopfieldPattern) per corrompere il pattern
// prende in argomento un Hopfield::HopfieldPattern pattern&
// restituisce un oggetto della stessa classe

// funzione Hopfield::HopfieldPattern pattern update (Hopfield::HopfieldPattern&
// pattern)
//  ciclo che ad ogni iterazione - per tutti i neuroni del pattern - (size_t
//  i=0; i<N; i++) calcola la funzione si(t+1) che caratterizza lo stato del
//  neurone e restituisce int +/- 1. funzione ricorsiva...forse serve un ciclo
//  (unsigned int t=0; t|si(t+1)=si(t);++t) nel ciclo. all'interno inseriamo un
//  CHECK che la funzione E(t) | dE/dt=0 d2E/dt2>0.

// problemi
// 1 uso la classe HopfieldPattern come argomento e risultato delle
// funzioni...sfrutto i metodi della classe per accedere alle variabili private
// e inserisco nella classe i metodi per la corruzione
// 2 uso un ciclo while per rappresentare la funzione ricorsiva
// 3 avrò due cicli annidati:
// ciclo esterno: while(!converged) {}
// dove definisco converged come una variabile booleana: se l'espressione è true
// il ciclo esegue le istruzioni, se è false si interrompe. qui si interrompe se
// converge bool converged=false -> !converged=true -> "esegui questo ciclo
// finché NON siamo arrivati a convergenza" ciclo interno: for (i=0; i<N ;++i)
// e definiremo nel secondo ciclo una condizione del tipo if(new_state !=
// current_state[i]) {current_state[i]=new_state[i]; converged = false;}
// {}