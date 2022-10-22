/*!
\file Progetto.cpp
\brief implementazione della classe Progetto
\author G. Meini
\version 1.0
\date 20/08/2016
*/


#include <cctype>
#include "Progetto.h"


static bool confrontaStringhe(const char s1[], const char s2[])
{
	while (*s1 != '\0' && *s2 != '\0')
	     {
			if (toupper(*s1) != toupper(*s2))
			  return false;
			s1++; s2++;
	    }
	if (*s1 == '\0' && *s2 == '\0')
		return true;
	else
		return false;
}

int Progetto::ricercaAttivita(const char descrizione[])
{
	char _descrizione[256];

	for (unsigned int _indice = 0; _indice < numero_attivita; _indice++)
	{
		attivita[_indice].getDescrizione(_descrizione);
		if (confrontaStringhe(_descrizione, descrizione))
			return _indice;
	}
	return -1;
}

Progetto::Progetto(const char denominazione[])
{
	FILE* file;
	FILERECORD record;
	Date data;
#ifdef WIN32
		char nome_file[1024] = ".\\progetti\\";
#else
		char nome_file[1024] = "./progetti/";
#endif

	numero_attivita = 0;
	strcat(nome_file, denominazione);
	file = fopen(nome_file, "rb");
	if (file == NULL)
	  {
		  file = fopen(nome_file, "wb");
		  if (file == NULL)
		    {
			  file = fopen(".\\progetti\\progetto", "wb"); // progetto predefinito in caso di errore
			  fclose(file);
			  strcpy(this->denominazione, "progetto");
			  return;
		    }
	  }
	else
	    {
			while (!feof(file) && numero_attivita<NUMERO_MASSIMO_ATTIVITA_PROGETTO)
			     {
					 if (fread(&record, sizeof(FILERECORD), 1, file) == 1)
					   {
						   data.setDate(record.giorno_scadenza, record.mese_scadenza, record.anno_scadenza);
						   attivita[numero_attivita] = Attivita(record.descrizione, data);
						   data.setDate(record.giorno_completamento, record.mese_completamento, record.anno_completamento);
						   attivita[numero_attivita].setSvoglimento(record.svolgimento, data);
						   numero_attivita++;
					   }
			     }
	    }
	fclose(file);
	strncpy(this->denominazione, denominazione, sizeof(this->denominazione)-1);
}

void Progetto::getDenominazione(char* denominazione)
{
	strcpy(denominazione, this->denominazione);
}

bool Progetto::creaAttivita(const char descrizione[], Date scadenza)
{
	FILE* file;
	FILERECORD record;
#ifdef WIN32
		char nome_file[1024] = ".\\progetti\\";
#else
		char nome_file[1024] = "./progetti/";
#endif

	if (numero_attivita >= NUMERO_MASSIMO_ATTIVITA_PROGETTO)
		return false;
	else
	    {
			// verifica univocita' attivita'
			if (ricercaAttivita(descrizione) >= 0)
              return false; // attivita' con la stessa descrizione gia' presente nel progetto
			// aggiunta attivita'
			attivita[numero_attivita] = Attivita(descrizione, scadenza);
			numero_attivita++;
			// aggiornamento file
			strcpy(record.descrizione, descrizione);
			record.giorno_scadenza = scadenza.getDay();
			record.mese_scadenza = scadenza.getMonth();
			record.anno_scadenza = scadenza.getYear();
			record.svolgimento = 0;
			record.giorno_completamento = 1;
			record.mese_completamento = 1;
			record.anno_completamento = 2000;
			strcat(nome_file, denominazione);
			file = fopen(nome_file, "ab");
			fwrite(&record, sizeof(FILERECORD), 1, file);
			fclose(file);
			return true;
	    }
}

bool Progetto::eliminaAttivita(const char descrizione[])
{
	FILE* file;
	FILERECORD record;
	int indice;
	char _descrizione[256];
#ifdef WIN32
		char nome_file[1024] = ".\\progetti\\";
#else
		char nome_file[1024] = "./progetti/";
#endif

	if (numero_attivita == 0)
	  return false;
	else
	    {
			// verifica esistenza attivita'
			if ((indice = ricercaAttivita(descrizione)) < 0)
			  return false; // attivita' inesistente nel progetto
			for (unsigned int _indice = indice + 1; _indice < numero_attivita; _indice++)
			   attivita[_indice - 1] = attivita[_indice];
			numero_attivita--;
			// riscrittura file
			strcat(nome_file, denominazione);
			file = fopen(nome_file, "wb");
			for (unsigned int _indice = 0; _indice < numero_attivita; _indice++)
			   {
					attivita[_indice].getDescrizione(_descrizione);
					strcpy(record.descrizione, _descrizione);
					record.giorno_scadenza = attivita[_indice].getScadenza().getDay();
					record.mese_scadenza = attivita[_indice].getScadenza().getMonth();
					record.anno_scadenza = attivita[_indice].getScadenza().getYear();
					record.svolgimento = attivita[_indice].getSvolgimento();
					record.giorno_completamento = attivita[_indice].getCompletamento().getDay();
					record.mese_completamento = attivita[_indice].getCompletamento().getMonth();
					record.anno_completamento = attivita[_indice].getCompletamento().getYear();
					fwrite(&record, sizeof(FILERECORD), 1, file);
		       }
		 fclose(file);
		 return true;
	    }
}

bool Progetto::aggiornaAttivita(const char descrizione[], const unsigned short svolgimento, const Date data)
{
	FILE* file;
	FILERECORD record;
#ifdef WIN32
		char nome_file[1024] = ".\\progetti\\";
#else
		char nome_file[1024] = "./progetti/";
#endif
	int indice = ricercaAttivita(descrizione);

	// verifica esistenza attivita'
	if (indice < 0)
	  return false; // attivita' inesistente nel progetto
	else
	    {
			attivita[indice].setSvoglimento(svolgimento, data);
			// aggiornamento file
			strcpy(record.descrizione, descrizione);
			record.giorno_scadenza = attivita[indice].getScadenza().getDay();
			record.mese_scadenza = attivita[indice].getScadenza().getMonth();
			record.anno_scadenza = attivita[indice].getScadenza().getYear();
			record.svolgimento = attivita[indice].getSvolgimento();
			record.giorno_completamento = attivita[indice].getCompletamento().getDay();
			record.mese_completamento = attivita[indice].getCompletamento().getMonth();
			record.anno_completamento = attivita[indice].getCompletamento().getYear();
			strcat(nome_file, denominazione);
			file = fopen(nome_file, "r+");
			fseek(file, indice*sizeof(FILERECORD), SEEK_SET);
			fwrite(&record, sizeof(FILERECORD), 1, file);
			fclose(file);
			return true;
	    }
}

bool Progetto::aggiornaAttivita(const char descrizione[], const Date scadenza)
{
	FILE* file;
	FILERECORD record;
#ifdef WIN32
		char nome_file[1024] = ".\\progetti\\";
#else
		char nome_file[1024] = "./progetti/";
#endif
	int indice = ricercaAttivita(descrizione);

	// verifica esistenza attivita'
	if (indice < 0)
		return false; // attivita' inesistente nel progetto
	else
	{
		attivita[indice].setScadenza(scadenza);
		// aggiornamento file
		strcpy(record.descrizione, descrizione);
		record.giorno_scadenza = attivita[indice].getScadenza().getDay();
		record.mese_scadenza = attivita[indice].getScadenza().getMonth();
		record.anno_scadenza = attivita[indice].getScadenza().getYear();
		record.svolgimento = attivita[indice].getSvolgimento();
		record.giorno_completamento = attivita[indice].getCompletamento().getDay();
		record.mese_completamento = attivita[indice].getCompletamento().getMonth();
		record.anno_completamento = attivita[indice].getCompletamento().getYear();
		strcat(nome_file, denominazione);
		file = fopen(nome_file, "r+");
		fseek(file, indice*sizeof(FILERECORD), SEEK_SET);
		fwrite(&record, sizeof(FILERECORD), 1, file);
		fclose(file);
		return true;
	}
}

unsigned int Progetto::elencaAttivita(Attivita elenco[])
{
	for (unsigned int _indice = 0; _indice < numero_attivita; _indice++)
		elenco[_indice] = attivita[_indice];
	return numero_attivita;
}

unsigned int Progetto::elencaAttivitaCompletate(Attivita elenco[])
{
	unsigned int indice = 0;

	for (unsigned int _indice = 0; _indice < numero_attivita; _indice++)
		if (attivita[_indice].getSvolgimento() == 100)
		  {
			elenco[indice] = attivita[_indice];
			indice++;
		  }
			
	return indice;
}

unsigned int Progetto::elencaAttivitaInScadenza(Date data, Attivita elenco[])
{
	unsigned int indice = 0;

	for (unsigned int _indice = 0; _indice < numero_attivita; _indice++)
		if (attivita[_indice].getSvolgimento() < 100 && data.diff(attivita[_indice].getScadenza()) < 0)
		  {
			elenco[indice] = attivita[_indice];
			indice++;
		  }

	return indice;
}

static void writeHTMLheader(FILE* file, char progetto[])
{
	fprintf(file, "<!DOCTYPE HTML>\r\n");
	fprintf(file, "<html>\r\n");
	fprintf(file, "<head>\r\n");
	fprintf(file, "<title>Elenco attivit&agrave progetto %s</title>\r\n", progetto);
	fprintf(file, "</head>\r\n");
	fprintf(file, "<body>\r\n");
	fprintf(file, "<table border>\r\n");
	fprintf(file, "<caption><h1>Elenco attivit&agrave progetto %s</h1></caption>\r\n", progetto);
	fprintf(file, "<thead>\r\n<tr>\r\n<th>Attivit&agrave</th><th>Svolgimento</th><th>Data scadenza/completamento</th>\r\n</tr>\r\n</thead>\r\n");
	fprintf(file, "<tbody>\r\n");
}

static void writeHTMLfooter(FILE* file)
{
	fprintf(file, "</tbody>\r\n");
	fprintf(file, "</table>\r\n");
	fprintf(file, "</body>\r\n");
	fprintf(file, "</html>\r\n");
}

bool Progetto::esportaAttivitaSuFile(FILETYPE tipo_file, char nome_file[])
{
	FILE* file;
	char descrizione[256];
	char nome_file_completo[1024];

	switch (tipo_file)
	      {
			case CSV:
	  					strncpy(nome_file_completo, nome_file, sizeof(nome_file_completo)-1);
						strcat(nome_file_completo, ".csv");
						file = fopen(nome_file_completo, "wt");
						if (file == NULL)
						  return false;
						for (unsigned int _indice = 0; _indice < numero_attivita; _indice++)
						   {
							attivita[_indice].getDescrizione(descrizione);
							// descrizione, svolgimento, giorno, mese, anno
							if (attivita[_indice].getSvolgimento() == 100)
							  fprintf(file, "%s,%u,%u,%u,%u\r\n", descrizione, 100, attivita[_indice].getCompletamento().getDay(), attivita[_indice].getCompletamento().getMonth(), attivita[_indice].getCompletamento().getYear());
							else
								fprintf(file, "%s,%u,%u,%u,%u\r\n", descrizione, attivita[_indice].getSvolgimento(), attivita[_indice].getScadenza().getDay(), attivita[_indice].getScadenza().getMonth(), attivita[_indice].getScadenza().getYear());
						   }
						fclose(file);
						return true;
			case HTML:	
						strncpy(nome_file_completo, nome_file, sizeof(nome_file_completo) - 1);
						strcat(nome_file_completo, ".html");
						file = fopen(nome_file_completo, "wt");
						if (file == NULL)
						  return false;
						writeHTMLheader(file, this->denominazione);
						for (unsigned int _indice = 0; _indice < numero_attivita; _indice++)
						   {
							attivita[_indice].getDescrizione(descrizione);
							fprintf(file, "<tr>\r\n");
							if (attivita[_indice].getSvolgimento() == 100)
								fprintf(file, "<td>%s</td><td>%u%%</td><td>%u-%u-%u</td>\r\n", descrizione, 100, attivita[_indice].getCompletamento().getDay(), attivita[_indice].getCompletamento().getMonth(), attivita[_indice].getCompletamento().getYear());
							else
								fprintf(file, "<td>%s</td><td>%u%%</td><td>%u-%u-%u</td>\r\n", descrizione, attivita[_indice].getSvolgimento(), attivita[_indice].getScadenza().getDay(), attivita[_indice].getScadenza().getMonth(), attivita[_indice].getScadenza().getYear());
							fprintf(file, "</tr>\r\n");
						}
						writeHTMLfooter(file);
						fclose(file);
						return true;
			default:	return false;
		  }
}


bool Progetto::esportaAttivitaCompletateSuFile(FILETYPE tipo_file, char nome_file[])
{
	FILE* file;
	char descrizione[256];
	char nome_file_completo[1024];

	switch (tipo_file)
		  {
		   case CSV:
						strncpy(nome_file_completo, nome_file, sizeof(nome_file_completo) - 1);
						strcat(nome_file_completo, ".csv");
						file = fopen(nome_file_completo, "wt");
						if (file == NULL)
						  return false;
						for (unsigned int _indice = 0; _indice < numero_attivita; _indice++)
						   {
							attivita[_indice].getDescrizione(descrizione);
							// descrizione, svolgimento, giorno, mese, anno
							if (attivita[_indice].getSvolgimento() == 100)
							  fprintf(file, "%s,%u,%u,%u,%u\r\n", descrizione, 100, attivita[_indice].getCompletamento().getDay(), attivita[_indice].getCompletamento().getMonth(), attivita[_indice].getCompletamento().getYear());
						   }
						fclose(file);
						return true;
		   case HTML:
						strncpy(nome_file_completo, nome_file, sizeof(nome_file_completo) - 1);
						strcat(nome_file_completo, ".html");
						file = fopen(nome_file_completo, "wt");
						if (file == NULL)
						  return false;
						writeHTMLheader(file, this->denominazione);
						for (unsigned int _indice = 0; _indice < numero_attivita; _indice++)
						   {
							attivita[_indice].getDescrizione(descrizione);
							if (attivita[_indice].getSvolgimento() == 100)
							  {
								fprintf(file, "<tr>\r\n");
				   				fprintf(file, "<td>%s</td><td>%u%%</td><td>%u-%u-%u</td>\r\n", descrizione, 100, attivita[_indice].getCompletamento().getDay(), attivita[_indice].getCompletamento().getMonth(), attivita[_indice].getCompletamento().getYear());
				   				fprintf(file, "</tr>\r\n");
							  }
			               }
						writeHTMLfooter(file);
						fclose(file);
						return true;
			default:	return false;
		   }
}

bool Progetto::esportaAttivitaInScadenzaSuFile(Date data, FILETYPE tipo_file, char nome_file[])
{
	FILE* file;
	char descrizione[256];
	char nome_file_completo[1024];

	switch (tipo_file)
	      {
		   case CSV:
						strncpy(nome_file_completo, nome_file, sizeof(nome_file_completo) - 1);
						strcat(nome_file_completo, ".csv");
						file = fopen(nome_file_completo, "wt");
						if (file == NULL)
						  return false;
						for (unsigned int _indice = 0; _indice < numero_attivita; _indice++)
						   {
							attivita[_indice].getDescrizione(descrizione);
							// descrizione, svolgimento, giorno, mese, anno
							if (attivita[_indice].getSvolgimento() < 100 && data.diff(attivita[_indice].getScadenza()) < 0)
							  fprintf(file, "%s,%u,%u,%u,%u\r\n", descrizione, 100, attivita[_indice].getCompletamento().getDay(), attivita[_indice].getCompletamento().getMonth(), attivita[_indice].getCompletamento().getYear());
						   }
						fclose(file);
						return true;
		   case HTML:
						strncpy(nome_file_completo, nome_file, sizeof(nome_file_completo) - 1);
						strcat(nome_file_completo, ".html");
						file = fopen(nome_file_completo, "wt");
						if (file == NULL)
						  return false;
						writeHTMLheader(file, this->denominazione);
						for (unsigned int _indice = 0; _indice < numero_attivita; _indice++)
						   {
							attivita[_indice].getDescrizione(descrizione);
							if (attivita[_indice].getSvolgimento() < 100 && data.diff(attivita[_indice].getScadenza()) < 0)
							  {
								fprintf(file, "<tr>\r\n");
								fprintf(file, "<td>%s</td><td>%u%%</td><td>%u-%u-%u</td>\r\n", descrizione, attivita[_indice].getSvolgimento(), attivita[_indice].getScadenza().getDay(), attivita[_indice].getScadenza().getMonth(), attivita[_indice].getScadenza().getYear());
								fprintf(file, "</tr>\r\n");
							  }
						   }
						writeHTMLfooter(file);
						fclose(file);
						return true;
			default:	return false;
		   }
}
