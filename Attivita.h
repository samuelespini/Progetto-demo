/*!
\file Attivita.h
\brief definizione e implementazione della classe Attivita
\author G. Meini
\version 1.0
\date 20/08/2016
*/

#ifndef _ATTIVITA
#define _ATTIVITA

#include <cstring>
#include "Date.h"

/*!
\brief classe per la rappresentazione di una singola attivita' di un progetto
*/
class Attivita
{
 private:
	 //! \brief descrizione dell'attivita' (la identifica univocamente nel progetto)
	 char descrizione[256];
	 //" \brief data di scadenza dell'attivita'
	 Date scadenza;
	 //! \brief data di completamento dell'attività (valida solo se svolgimento = 100)
	 Date completamento;
	 //! \brief percentuale di svolgimento dell'attivita' (0 - 100)
	 unsigned short svolgimento;

 public:
	 //! \brief costruttore predefinito
	 Attivita()
	 {
		 strcpy(this->descrizione, "");
		 this->scadenza = Date(); // 01-01-2000
		 this->completamento = Date(); // 01-01-2000
		 svolgimento = 0;
	 }
	 /*! \brief costruttore
		 \param[in] descrizione descrizione dell'attivita'
		 \param[in] scadenza data di scadenza dell'attivita'
	 */
	 Attivita(const char descrizione[], const Date scadenza)
	 {
		 strncpy(this->descrizione, descrizione, sizeof(this->descrizione) - 1);
		 this->scadenza = scadenza;
		 this->completamento = Date(); // 01-01-2000
		 svolgimento = 0;
	 }
	 /*! \brief acquisizione descrizione dell'attivita'
		 \param[out] descrizione descrizione dell'attivita'
	 */
	 void getDescrizione(char* descrizione)
	 {
		 strcpy(descrizione, this->descrizione);
	 }
	 /*! \brief acquisizione della data di scadenza dell'attivita'
		 \return data di scadenza dell'attivita'
	 */
	 Date getScadenza(void)
	 {
		 return scadenza;
	 }
	 /*! \brief acquisizione della data di completamento dell'attivita'
		 \return data di completamento dell'attivita' (in caso di attivita' non completata restituisce la data 01-01-2000)
	 */
	 Date getCompletamento(void)
	 {
		 return completamento;
	 }
	 /*! \brief acquisizione della percentuale di svolgimento dell'attivita'
		 \return percentuale di svolgimento dell'attivita'
	 */
	 unsigned short getSvolgimento()
	 {
		 return svolgimento;
	 }
	 /*! \brief impostazione della percentuale di svolgimento dell'attivita'
		 \param[in] svolgimento percentuale di svolgimento dell'attivita'
		 \param[in] completamento data di completamento (significativa solo se svolgimento = 100)
	 */
	 void setSvoglimento(const unsigned short svolgimento, const Date completamento)
	 {
		 if (svolgimento >= 100)
		 {
			 this->svolgimento = 100;
			 this->completamento = completamento;
		 }
		 else
			 this->svolgimento = svolgimento;
	 }
	 /*! \brief reimpostazione della data di scadenza dell'attivita'
	 	 \param[in] scadenza data di scadenza
	 */
	 void setScadenza(const Date scadenza)
	 {
		 this->scadenza = scadenza;
	 }
};

#endif
