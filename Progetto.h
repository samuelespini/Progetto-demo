/*!
\file Progetto.h
\brief definizione della classe Progetto
\author G. Meini
\version 1.0
\date 20/08/2016
*/

#ifndef _PROGETTO
#define _PROGETTO

#include <cstring>
#include <cstdio>
#include "Attivita.h"

const unsigned int NUMERO_MASSIMO_ATTIVITA_PROGETTO = 64;

enum FILETYPE
{
	CSV,
	HTML
};

/*!
\brief struttura del record del file binario che rappresenta un'attivita' del progetto
*/
struct FILERECORD
{
	char descrizione[1024];
	unsigned short svolgimento;
	unsigned short giorno_scadenza;
	unsigned short mese_scadenza;
	unsigned short anno_scadenza;
	unsigned short giorno_completamento;
	unsigned short mese_completamento;
	unsigned short anno_completamento;
};

/*!
\brief classe per la rappresentazione di un progetto
*/
class Progetto
{
private:
	//! \brief denominazione del progetto (nome del file relativo al progetto)
	char denominazione[256];
	//! \brief elenco di attivita' del progetto
	Attivita attivita[NUMERO_MASSIMO_ATTIVITA_PROGETTO];
	unsigned int numero_attivita;

	/*! \brief ricerca di un'attivita' del progetto
	\param[in] descrizione descrizione dell'attivita'
	\return >=0: indice dell'attivita' nel vettore delle attivita', <0 attivita' non trovata
	*/
	int ricercaAttivita(const char descrizione[]);

public:
	/*! \brief costruttore (se il file relativo al progetto non esiste crea un nuovo progetto)
		\param[in] denominazione denominazione del progetto (nome del file relativo al progetto)
		\note nel caso di denominazione errata viene creato il progetto predefinito denominato "progetto"
	*/
	Progetto(const char denominazione[]);
	/*! \brief acquisizione denominazione del progetto
		\param[out] denominazione denominazione del progetto
	*/
	void getDenominazione(char* denominazione);
	/*! \brief aggiunta nuova attivita' al progetto
		\param[in] descrizione descrizione della nuova attivita' (la identifica univocamente)
		\param[in] scadenza data di scadenza dell'attivita'
		\retval VERO l'attivita' e' stata aggiunta al progetto
		\retval FALSO l'attivita' non e' stata aggiunta al progetto
	*/
	bool creaAttivita(const char descrizione[], const Date scadenza);
	/*! \brief eliminazione di un'attivita' dal progetto
		\param[in] descrizione descrizione dell'attivita' (la identifica univocamente)
		\retval VERO l'attivita' e' stata eliminata
		\retval FALSO l'attivita' non e' stata eliminata 
	*/
	bool eliminaAttivita(const char descrizione[]);
	/*! \brief aggiornamento di un'attivita' del progetto
		\param[in] descrizione descrizione dell'attivita' (la identifica univocamente)
		\param[in] svolgimento percentuale aggiornata di svolgimento dell'attivita' (100% = completamento)
		\param[in] data data di aggiornamento della percentuale di svolgimento
		\retval VERO l'attivita' e' stata aggiornata
		\retval FALSO l'attivita' non e' stata aggiornata
	*/
	bool aggiornaAttivita(const char descrizione[], const unsigned short svolgimento, const Date data);
	/*! \brief aggiornamento di un'attivita' del progetto
		\param[in] descrizione descrizione dell'attivita' (la identifica univocamente)
		\param[in] scadenza data aggiornata di scadenza dell'attivita'
		\retval VERO l'attivita' e' stata aggiornata
		\retval FALSO l'attivita' non e' stata aggiornata
	*/
	bool aggiornaAttivita(const char descrizione[], const Date scadenza);
	/*! \brief elenco delle attivita' del progetto
		\param[out] elenco elenco delle attivita' del progetto
		\return numero di attivita' presenti nell'elenco
	*/
	unsigned int elencaAttivita(Attivita elenco[]);
	/*! \brief elenco delle attivita' completate del progetto
		\param[out] elenco elenco delle attivita' completate
		\return numero di attivita' presenti nell'elenco
	*/
	unsigned int elencaAttivitaCompletate(Attivita elenco[]);
	/*! \brief elenco delle attivita' in scadenza prima di una specifica data del progetto
		\param[in] data data di riferimento per la valutazione della scadenza
		\param[out] elenco elenco delle attivita' in scadenza
		\return numero di attivita' presenti nell'elenco
	*/
	unsigned int elencaAttivitaInScadenza(Date data, Attivita elenco[]);
	/*! \brief esportazione delle attivita' del progetto su file
		\param[in] tipo_file tipo del file (CSV o HTML)
		\param[in] nome_file denominazione del file
		\retval VERO il file e' stato generato
		\retval FALSO il file non e' stato generato
	*/
	bool esportaAttivitaSuFile(FILETYPE tipo_file, char nome_file[]);
	/*! \brief esportazione delle attivita' COMPLETATE del progetto su file
		\param[in] tipo_file tipo del file (CSV o HTML)
		\param[in] nome_file denominazione del file
		\retval VERO il file e' stato generato
		\retval FALSO il file non e' stato generato
	*/
	bool esportaAttivitaCompletateSuFile(FILETYPE tipo_file, char nome_file[]);
	/*! \brief esportazione delle attivita' in scadenza prima di una specifica data del progetto su file
		\param[in] data data di riferimento per la valutazione della scadenza
		\param[in] tipo_file tipo del file (CSV o HTML)
		\param[in] nome_file denominazione del file
		\retval VERO il file e' stato generato
		\retval FALSO il file non e' stato generato
	*/
	bool esportaAttivitaInScadenzaSuFile(Date data, FILETYPE tipo_file, char nome_file[]);
};

#endif
