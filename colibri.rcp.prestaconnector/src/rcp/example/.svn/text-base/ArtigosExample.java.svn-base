/*******************************************************************************
 * 2008-2011 Projecto Colibri
 * Marco Lopes (marcolopes@projectocolibri.com)
 *******************************************************************************/
package rcp.example;

import java.util.Collection;
import java.util.Iterator;

import org.dma.utils.eclipse.swt.DialogHandler;
import org.dma.utils.java.array.ErrorList;

import rcp.colibri.dao.database.ColibriDatabase;
import rcp.colibri.dao.database.model.Artigos;

public class ArtigosExample {

	public ArtigosExample(){
	}



	/**
	 * Cria um novo artigo
	 */
	public void createArtigo(String codigo, String descricao) {
		try{
			//cria objecto artigo
			Artigos artigo=new Artigos(codigo);
			artigo.setDescricao(descricao);

			//adiciona uma unidade de medida
			artigo.addUnidades(artigo.createUnidades("PK"));
			//adiciona um preco
			artigo.addPrecos(artigo.createPrecos("001"));

			//grava o artigo na base de dados
			ErrorList error=ColibriDatabase.storeArtigos(artigo, false);

			//apresenta possiveis erros
			DialogHandler.error(error.getErrors());

		} catch (Exception e){
			e.printStackTrace();
		}

	}



	/**
	 * Mostra a coleccao de artigos
	 */
	public void showArtigos() {
		try{
			/*
			 * Carrega todos os artigos da base de dados
			 * Apenas o campo CODIGO e' devolvido por defeito na coleccao
			 * E' necessario carregar o artigo para aceder aos outros campos
			 */
			Collection<Artigos> artigos=ColibriDatabase.getCollectionArtigos();

			//itera a coleccao de artigos
			Iterator<Artigos> iterator=artigos.iterator();
			while(iterator.hasNext()){

				//carrega o artigo da base de dados
				Artigos artigo=ColibriDatabase.loadArtigos(iterator.next().getCodigo());

				System.out.println(artigo);
				//saca a primeira unidade de medida
				System.out.println(artigo.getUnidades().iterator().next().getUnidademedida());
				//saca o primeiro preco
				System.out.println(artigo.getPrecos().iterator().next().getPrecopvp());

			}

		} catch (Exception e){
			e.printStackTrace();
		}

	}


}
