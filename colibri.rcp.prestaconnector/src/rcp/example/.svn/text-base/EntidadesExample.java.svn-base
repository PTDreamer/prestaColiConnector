/*******************************************************************************
 * 2008-2012 Projecto Colibri
 * Marco Lopes (marcolopes@projectocolibri.com)
 *******************************************************************************/
package rcp.example;

import java.util.Collection;
import java.util.Iterator;

import org.dma.utils.eclipse.swt.DialogHandler;
import org.dma.utils.java.array.ErrorList;

import rcp.colibri.dao.database.ColibriDatabase;
import rcp.colibri.dao.database.model.Entidades;

public class EntidadesExample {

	public EntidadesExample(){
	}



	/**
	 * Cria uma nova entidade
	 */
	public void createCliente(String tipoentidade, String nome) {
		try{
			//cria objecto entidade
			Entidades entidade=new Entidades(
				ColibriDatabase.getNextNumeroEntidades(tipoentidade),
				ColibriDatabase.loadEntidadestipos(tipoentidade));

			entidade.setNome(nome);

			//grava a entidade na base de dados
			ErrorList error=ColibriDatabase.storeEntidades(entidade,false);

			//apresenta possiveis erros
			DialogHandler.error(error.getErrors());

		} catch (Exception e){
			e.printStackTrace();
		}

	}



	/**
	 * Mostra a coleccao de entidades
	 */
	public void showEntidades(String tipoentidade) {
		try{
			/*
			 * Carrega todas as entidades da base de dados
			 * Apenas o campo KEY e' devolvido por defeito na coleccao
			 * E' necessario carregar a entidade para aceder aos outros campos
			 */
			Collection<Entidades> entidades=ColibriDatabase.getCollectionEntidades(tipoentidade);

			//itera a coleccao de entidades
			Iterator<Entidades> iterator=entidades.iterator();
			while(iterator.hasNext()){

				//carrega a entidade da base de dados
				Entidades entidade=ColibriDatabase.loadEntidades(iterator.next().getKey());

				System.out.println(entidade);

			}

		} catch (Exception e){
			e.printStackTrace();
		}

	}


}
