/*!
\file TODO.h
\brief definizione e implementazione della classe TODO
\author G. Meini
\version 1.0
\date 20/08/2016
*/

#include <cstring>
#include <cstdlib>
#include <cstdio>
#ifdef WIN32
#include "dirent.h"
#else
#include <dirent.h>
#endif
#include "Progetto.h"


/*!
\brief classe di metodi statici per la gestione dei progetti memorizzati su file
*/
class TODO
{
public:
	
	/*! \brief elenco dei nomi dei progetti
		\param[out] elenco vettore di puntatori alle stringhe dei nomi dei progetti
		\param[in] numero_massimo_progetti dimensione del vettore
		\return numero di progetti
		\note la memoria per la memorizzazione dei nomi dei progetti e' allocata dal codice che invoca il metodo
	*/
	static unsigned int elencaProgetti(char* elenco[], unsigned int numero_massimo_progetti)
	{
		DIR *directory;
		struct dirent *file;
		unsigned int numero_progetti = 0;
#ifdef WIN32	
		if ((directory = opendir(".\\progetti")) != NULL)	
#else
		if ((directory = opendir("./progetti")) != NULL)
#endif
		  {
			while ((file = readdir(directory))!=NULL && numero_progetti<numero_massimo_progetti)
			     {
					if (file->d_type == DT_REG)
					   {
						 strncpy(elenco[numero_progetti], file->d_name, 255);
						 numero_progetti++;
					   }
				 }
			closedir(directory);
		   }
		return numero_progetti;
	}

	/*! \brief selezione di un progetto esistente
		\param[in] denominazione nome del progetto da selezionare
		\param[out] progetto progetto selezionato
		\return puntatore ad un'istanza del progetto selezionato (nullptr in caso di errore)
		\note la memoria allocata per il progetto dovrebbe essere liberata prima di riutilizzare il puntatore
	*/
	static Progetto* selezionaProgetto(char denominazione[])
	{
#ifdef WIN32
		char nome_file[1024] = ".\\progetti\\";
#else
		char nome_file[1024] = "progetti/";
#endif
		FILE* file;

	strcat(nome_file, denominazione);
		file = fopen(nome_file, "rb");
		if (file == NULL)
		  {
			  return nullptr;
		  }
		else
		    {
				return new Progetto(denominazione);
		    }
	}

	/*! \brief creazione di un nuovo progetto
		\param[in] denominazione nome del progetto da creare
		\param[out] progetto progetto creato
		\return puntatore ad un'istanza del progetto creato (nullptr in caso di errore)
		\note la memoria allocata per il progetto dovrebbe essere liberata prima di riutilizzare il puntatore
	*/
	static Progetto* creaProgetto(char denominazione[])
	{
#ifdef WIN32
		char nome_file[1024] = ".\\progetti\\";
#else
		char nome_file[1024] = "./progetti/";
#endif
		FILE* file;

		strcat(nome_file, denominazione);
		file = fopen(nome_file, "rb");
		if (file != NULL)
		{
			fclose(file);
			return nullptr;
		}
		else
		{
			return new Progetto(denominazione);
		}
	}

	/*! \brief eliminazione di un nuovo progetto esistente
		\param[in] denominazione nome del progetto da creare
		\retval VERO progetto eliminato
		\retval FALSO progetto non eliminato
	*/
	static bool eliminaProgetto(char denominazione[])
	{
#ifdef WIN32
		char nome_file[1024] = ".\\progetti\\";
#else
		char nome_file[1024] = "./progetti/";
#endif

		strcat(nome_file, denominazione);
		if (remove(nome_file) == 0)
			return true;
		else
			return false;
	}
};
