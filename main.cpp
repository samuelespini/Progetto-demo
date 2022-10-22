/*!
\file main.cpp
\brief programma principale dell'applicazione TODO-LIST
\author G. Meini
\version 1.0
\date 20/08/2016
\note realizza un'interfaccia non grafica per l'utente dell'applicazione
*/

#include <iostream>
#include <string>
#include "Attivita.h"
#include "Progetto.h"
#include "TODO.h"

using namespace std;

//! \brief costante che rappresenta il numero massimo di progetti gestiti dall'applicazione
const unsigned int NUMERO_MASSIMO_PROGETTI = 16;




int main(void)
{
	//! \brief vettore dei nomi dei progetti
	char *progetti[NUMERO_MASSIMO_PROGETTI];
	//! \brief vettore delle attività selezionate dal progetto attivo
	Attivita attivita[NUMERO_MASSIMO_ATTIVITA_PROGETTO];
	//! \brief puntatore ad un'istanza del progetto attivo
	Progetto *progetto = nullptr;
	//! numero di nomi di progetti presenti nel vettore progetti
	unsigned int numero_progetti = 0;
	//! numero di attivita' presenti nel vettore attivita
	unsigned int numero_attivita = 0;
	//! indicatore di validita' del contenuto del vettore progetti
	bool vettore_progetti_valido = false;
	//! indicatore di validità del contenuto del vettore attivita
	bool vettore_attivita_valido = false;

	char nome_progetto[256], descrizione_attivita[256], nome_file[1024];
	unsigned int indice_progetto, indice_attivita;
	int comando, giorno, mese, anno, percentuale;
	Date data;
	string tmp;
	const char* descrizione;
	char tipo_file;

	// allocazione memoria per vettore dei nomi dei progetti
	for (unsigned int _i=0; _i<NUMERO_MASSIMO_PROGETTI; _i++)
	   progetti[_i] = new char[1024];
	
	// ciclo di funzionamento dell'applicazione
	while (true)
	     {
#ifdef WIN32	 
			 system("CLS");
#else
			 system("clear");
#endif
			 if (progetto == nullptr)
			   {
				 // nessun progetto attivo: visualizzazione del menu' dei progetti
				 cout << "0:\tEsci" << endl;
				 cout << "1:\tElenco progetti" << endl;
				 cout << "2:\tNuovo progetto" << endl;
				 cout << "3:\tApri progetto" << endl;
				 cout << "4:\tElimina progetto" << endl;
				 cout << endl << "Comando? ";
				 cin >> comando;
				 // esecuzione comando digitato
				 switch (comando)
				       {
						case 0:	// uscita applicazione
								goto END;
						case 1:	// visualizzazione elenco progetti
								numero_progetti = TODO::elencaProgetti(progetti, NUMERO_MASSIMO_PROGETTI);
								if (numero_progetti == 0)
								  cout << "Nessun progetto" << endl;
								else
									{
										for (unsigned int _p = 0; _p < numero_progetti; _p++)
										   cout << "#" << _p + 1 << ":\t" << progetti[_p] << endl;
										vettore_progetti_valido = true;
									}
								break;
						case 2:	// creazione nuovo progetto
								cout << endl << "Nome progetto? ";
								cin >> nome_progetto;
								if ((progetto = TODO::creaProgetto(nome_progetto)) != nullptr)
								  {
									cout << "Progetto " << nome_progetto << " creato" << endl;
									vettore_progetti_valido = false;
								  }
								else
									cout << "Errore creazione progetto " << nome_progetto << endl;
								break;
						case 3:	// apertura progetto
								if (!vettore_progetti_valido)
								  {
									cout << "Visualizzare elenco progetti" << endl;
									break;
								  }
								cout << endl << "Numero progetto? ";
								cin >> indice_progetto;
								if (indice_progetto < 1 || indice_progetto > numero_progetti)
								  cout << "Progetto inesistente" << endl;
								else
									{
										strcpy(nome_progetto, progetti[indice_progetto - 1]);
										if ((progetto = TODO::selezionaProgetto(nome_progetto)) != nullptr)
										  cout << "Progetto " << nome_progetto << " aperto" << endl;
										else
											cout << "Errore apertura progetto " << nome_progetto << endl;
									}
								break;
						case 4:	// eliminazione progetto
								if (!vettore_progetti_valido)
								  {
									cout << "Visualizzare elenco progetti" << endl;
									break;
								  }
								cout << endl << "Numero progetto? ";
								cin >> indice_progetto;
								if (indice_progetto < 1 || indice_progetto > numero_progetti)
								  cout << "Progetto inesistente" << endl;
								else
									{
									 strcpy(nome_progetto, progetti[indice_progetto - 1]);
									 if (TODO::eliminaProgetto(nome_progetto))
									   {
										cout << "Progetto " << nome_progetto << " eliminato" << endl;
										vettore_progetti_valido = false;
									   }
									else
										cout << "Errore eliminazione progetto " << nome_progetto << endl;
								   }
								break;
						default:
								cout << "Comando errato" << endl;
								break;
					 }
			   }
			 else
				 {
					 // progetto attivo: visualizzazione del menu' delle attivita'
					 cout << "0:\tChiudi progetto" << endl;
					 cout << "1:\tElenco attivita'" << endl;
					 cout << "2:\tElenco attivita' completate" << endl;
					 cout << "3:\tElenco attivita' in scadenza" << endl;
					 cout << "4:\tEsporta elenco attivita'" << endl;
					 cout << "5:\tEsporta elenco attivita' completate" << endl;
					 cout << "6:\tEsporta elenco attivita' in scadenza" << endl;
					 cout << "7:\tAggiungi attivita'" << endl;
					 cout << "8:\tAggiorna svolgimento attivita'" << endl;
					 cout << "9:\tAggiorna scadenza attivita'" << endl;
					 cout << "10:\tElimina attivita'" << endl;
					 cout << endl << "Comando? ";
					 cin >> comando;
					 // esecuzione comando digitato
					 switch (comando)
						   {
							case 0:		// chiusura progetto
										delete progetto;
										progetto = nullptr;
										vettore_attivita_valido = false;
										break;
							case 1:		// visualizzazione elenco completo attivita'
										numero_attivita = progetto->elencaAttivita(attivita);
										if (numero_attivita == 0)
											cout << "Nessuna attivita'" << endl;
										else
											{
												for (unsigned int _a = 0; _a < numero_attivita; _a++)
												   {
													attivita[_a].getDescrizione(descrizione_attivita);
													if (attivita[_a].getSvolgimento() == 100)
													  cout << "#" << _a + 1 << ":\t" << descrizione_attivita << "\t" << attivita[_a].getSvolgimento() << "%\t" << attivita[_a].getCompletamento().getDay() << "-" << attivita[_a].getCompletamento().getMonth() << "-" << attivita[_a].getCompletamento().getYear() << endl;
													else
													    cout << "#" << _a + 1 << ":\t" << descrizione_attivita << "\t" << attivita[_a].getSvolgimento() << "%\t" << attivita[_a].getScadenza().getDay() << "-" << attivita[_a].getScadenza().getMonth() << "-" << attivita[_a].getScadenza().getYear() << endl;
												   }
												vettore_attivita_valido = true;
											}
										break;
							case 2:		// visualizzazione elenco attivita' completate
										numero_attivita = progetto->elencaAttivitaCompletate(attivita);
										if (numero_attivita == 0)
										  cout << "Nessuna attivita' completata" << endl;
										else
											{
												for (unsigned int _a = 0; _a < numero_attivita; _a++)
												   {
													attivita[_a].getDescrizione(descrizione_attivita);
													cout << "#" << _a + 1 << ":\t" << descrizione_attivita << "\t" << attivita[_a].getSvolgimento() << "%\t" << attivita[_a].getCompletamento().getDay() << "-" << attivita[_a].getCompletamento().getMonth() << "-" << attivita[_a].getCompletamento().getYear() << endl;
												   }
												vettore_attivita_valido = true;
											}
										break;
							case 3:		// visualizza elenco attività in scadenza prima di una specifica data
										cout << endl << "Data di riferimento:" << endl;
										cout << endl << "Giorno? "; cin >> giorno;
										cout << "Mese? "; cin >> mese;
										cout << "Anno? "; cin >> anno;
										data.setDate(giorno, mese, anno);
										numero_attivita = progetto->elencaAttivitaInScadenza(data, attivita);
										if (numero_attivita == 0)
										  cout << "Nessuna attivita' in scadenza prima del " << giorno << "-" << mese << "-" << anno << endl;
										else
											{
												for (unsigned int _a = 0; _a < numero_attivita; _a++)
												   {
													attivita[_a].getDescrizione(descrizione_attivita);
													cout << "#" << _a + 1 << ":\t" << descrizione_attivita << "\t" << attivita[_a].getSvolgimento() << "%\t" << attivita[_a].getScadenza().getDay() << "-" << attivita[_a].getScadenza().getMonth() << "-" << attivita[_a].getScadenza().getYear() << endl;
												   }
												vettore_attivita_valido = true;
											}
										break;
							case 4:		// esportazione su file elenco completo attivita'
										cout << endl << "Tipo del file (C per CSV, H per HTML)? ";
										cin >> tipo_file;
										if (tipo_file != 'C' && tipo_file != 'H' && tipo_file != 'c' && tipo_file != 'h')
											cout << "Tipo file errato" << endl;
										else
											{
												cout << endl << "Nome del file? ";
												cin >> nome_file;
												if (progetto->esportaAttivitaSuFile(((tipo_file == 'H' || tipo_file == 'h') ? HTML : CSV), nome_file))
												  cout << "File " << nome_file << " generato" << endl;
												else
												    cout << "Errore generazione file " << nome_file << endl;
											}
										break;
							case 5:		// esportazione su file elenco attivita' completate
										cout << endl << "Tipo del file (C per CSV, H per HTML)? ";
										cin >> tipo_file;
										if (tipo_file != 'C' && tipo_file != 'H' && tipo_file != 'c' && tipo_file != 'h')
										  cout << "Tipo file errato" << endl;
										else
											{
											 cout << endl << "Nome del file? ";
											 cin >> nome_file;
											 if (progetto->esportaAttivitaCompletateSuFile(((tipo_file == 'H' || tipo_file == 'h') ? HTML : CSV), nome_file))
											   cout << "File " << nome_file << " generato" << endl;
											 else
												 cout << "Errore generazione file " << nome_file << endl;
											}
										break;
							case 6:		// esportazione su file elenco attività in scadenza prima di una specifica data
										cout << endl << "Tipo del file (C per CSV, H per HTML)? ";
										cin >> tipo_file;
										if (tipo_file != 'C' && tipo_file != 'H' && tipo_file != 'c' && tipo_file != 'h')
										  cout << "Tipo file errato" << endl;
										else
											{
												cout << endl << "Data di riferimento:" << endl;
												cout << "Giorno ? "; cin >> giorno;
												cout << "Mese? "; cin >> mese;
												cout << "Anno? "; cin >> anno;
												data.setDate(giorno, mese, anno);
												cout << endl << "Nome del file? ";
												cin >> nome_file;
												if (progetto->esportaAttivitaInScadenzaSuFile(data, ((tipo_file == 'H' || tipo_file == 'h') ? HTML : CSV), nome_file))
												  cout << "File " << nome_file << " generato" << endl;
												else
													cout << "Errore generazione file " << nome_file << endl;
											}
										break;
							case 7:		// aggiunta di nuova attivita' al progetto
										cout << endl << "Data di scadenza:" << endl;
										cout << endl << "Giorno? "; cin >> giorno;
										cout << "Mese? "; cin >> mese;
										cout << "Anno? "; cin >> anno;
										data.setDate(giorno, mese, anno);
										cout << endl << "Descrizione dell'attivita? ";
										getline(cin, tmp); // ?
										getline(cin, tmp);
										descrizione = tmp.c_str();
										if (progetto->creaAttivita(descrizione, data))
										  {
											cout << "Attivita' aggiunta al progetto" << endl;
											vettore_attivita_valido = false;
										  }
										else
											cout << "Errore aggiunta attivita' al progetto" << endl;
										break;
							case 8:		// aggiornamento data di scadenza attivita'
										if (!vettore_attivita_valido)
										  {
											cout << "Visualizzare elenco attivita" << endl;
											break;
										  }
										cout << endl << "Numero dell'attivita? ";
										cin >> indice_attivita;
										if (indice_attivita < 1 || indice_attivita > numero_attivita)
											cout << "Attivita' inesistente" << endl;
										else
											{
												attivita[indice_attivita - 1].getDescrizione(descrizione_attivita);
												cout << endl << "Percentuale di svolgimento dell'attivita? ";
												cin >> percentuale;
												if (percentuale == 100)
												{
													cout << endl << "Data di completamento:" << endl;
													cout << endl << "Giorno? "; cin >> giorno;
													cout << "Mese? "; cin >> mese;
													cout << "Anno? "; cin >> anno;
													data.setDate(giorno, mese, anno);
												}
												else
													data.setDate(1, 1, 2000);
												if (progetto->aggiornaAttivita(descrizione_attivita, percentuale, data))
												  cout << "Percentuale svolgimento attivita' del progetto aggiornata" << endl;
												else
													cout << "Errore aggiornamento percentuale svolgimento attivita' del progetto" << endl;
											}
										break;
							case 9:		// aggiornamento percentuale di svolgimento attivita'
										if (!vettore_attivita_valido)
										  {
											cout << "Visualizzare elenco attivita" << endl;
											break;
										  }
										cout << endl << "Numero dell'attivita? ";
										cin >> indice_attivita;
										if (indice_attivita < 1 || indice_attivita > numero_attivita)
										  cout << "Attivita' inesistente" << endl;
										else
											{
												attivita[indice_attivita - 1].getDescrizione(descrizione_attivita);
												cout << endl << "Data di scadenza dell'attivita?" << endl;
												cout << endl << "Giorno? "; cin >> giorno;
												cout << "Mese? "; cin >> mese;
												cout << "Anno? "; cin >> anno;
												data.setDate(giorno, mese, anno);
												if (progetto->aggiornaAttivita(descrizione_attivita, data))
												  cout << "Percentuale svolgimento attivita' del progetto aggiornata" << endl;
												else
													cout << "Errore aggiornamento percentuale svolgimento attivita' del progetto" << endl;
											}
										break;
							case 10:	// eliminazione attivita'
										if (!vettore_attivita_valido)
										  {
											cout << "Visualizzare elenco attivita" << endl;
											break;
										  }
										cout << endl << "Numero dell'attivita? ";
										cin >> indice_attivita;
										if (indice_attivita < 1 || indice_attivita > numero_attivita)
										  cout << "Attivita' inesistente" << endl;
										else
											{
											 attivita[indice_attivita - 1].getDescrizione(descrizione_attivita);
											 if (progetto->eliminaAttivita(descrizione_attivita))
											   {
												 cout << "Attivita' eliminata dal progetto" << endl;
												 vettore_attivita_valido = false;
											   }
											else
												cout << "Errore eliminazione attivita' dal progetto" << endl;
										break;
										}
							default:
										cout << "Comando errato" << endl;
										break;
						}
					 }
			cout << endl << "Premere <INVIO> per continuare...";
			cin.get(); cin.get();
		}

END:
	if (progetto != nullptr)
	  delete progetto;
	for (unsigned int _i=0; _i<NUMERO_MASSIMO_PROGETTI; _i++)
		delete[] progetti[_i];
	return 0;
}
