/*******************************************************************************
 * 2008-2012 Projecto Colibri
 * Marco Lopes (marcolopes@projectocolibri.com)
 *******************************************************************************/
package rcp.example;

import java.util.Collection;
import java.util.Iterator;

import org.dma.utils.java.Debug;

import rcp.colibri.core.mappers.filters.FilterMap;
import rcp.colibri.core.mappers.filters.FilterMap.OPERATORS;
import rcp.colibri.core.mappers.filters.FilterOperandMap;
import rcp.colibri.core.vars.database.DatabaseVARS;
import rcp.colibri.core.vars.database.DatabaseVARS.FIELDS;
import rcp.colibri.dao.database.ColibriDatabase;
import rcp.colibri.dao.database.model.Entidades;
import rcp.colibri.dao.database.query.QueryDefinition;
import rcp.colibri.dao.database.query.QueryDefinition.QUERY_ORDER;

public class QueryExample {

	public QueryExample(){
	}

	/**
	 * Cria uma nova query
	 */
	public QueryDefinition createQuery() {

		FilterMap filterMap=new FilterMap(DatabaseVARS.entidades).
			//nome comecado por "a"
			addRule(FIELDS.entidades_nome.name,
					OPERATORS.MATH.STARTS_WITH, new FilterOperandMap("a")).
			//OU nome comenado por "b"
			addRule(FIELDS.entidades_nome.name,
					OPERATORS.MATH.STARTS_WITH, new FilterOperandMap("b"),
					OPERATORS.LOGICAL.OR).
			//OU nome comenado por "c"
			addRule(FIELDS.entidades_nome.name,
					OPERATORS.MATH.STARTS_WITH, new FilterOperandMap("c"),
					OPERATORS.LOGICAL.OR).
			//E morada contendo "rua"
			addRule(FIELDS.entidades_morada.name,
					OPERATORS.MATH.CONTAINS, new FilterOperandMap("rua"));

		return new QueryDefinition(filterMap,
				new String[]{FIELDS.entidades_key.name},
				QUERY_ORDER.ASCENDING);

	}



	/**
	 * Executa a query
	 */
	public void executeQuery() {
		try{
			QueryDefinition queryDefinition=createQuery();

			Collection<Entidades> collection=ColibriDatabase.getCollectionEntidades(queryDefinition);

			Iterator<Entidades> iterator=collection.iterator();
			while(iterator.hasNext()){
				Entidades entidade=ColibriDatabase.loadEntidades(iterator.next().getKey());
				Debug.out(entidade);
			}

			queryDefinition.closeQuery();

		} catch (Exception e){
			e.printStackTrace();
		}

	}


}
