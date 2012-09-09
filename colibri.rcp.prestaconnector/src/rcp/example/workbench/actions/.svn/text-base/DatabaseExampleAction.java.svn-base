/*******************************************************************************
 * 2008-2012 Projecto Colibri
 * Marco Lopes (marcolopes@projectocolibri.com)
 *******************************************************************************/
package rcp.example.workbench.actions;

import org.dma.utils.java.string.StringUtils;
import org.eclipse.jface.action.Action;
import org.eclipse.ui.actions.ActionFactory.IWorkbenchAction;

import rcp.colibri.core.vars.database.DatabaseVARS.FIELDS;
import rcp.colibri.core.vars.database.PopulateVARS;
import rcp.colibri.core.vars.database.PopulateVARS.ENTIDADESTIPOS;
import rcp.example.ArtigosExample;
import rcp.example.DocumentosExample;
import rcp.example.EntidadesExample;
import rcp.example.QueryExample;

public class DatabaseExampleAction extends Action implements IWorkbenchAction {

	public DatabaseExampleAction() {
		setText("#database");
	}


	public final void run(){

		/*
		 * Exemplos que PODEM correr COM e SEM workbench
		 */
		QueryExample queryExample=new QueryExample();
		queryExample.executeQuery();

		/*
		 * Artigos
		 */
		ArtigosExample artigosExample=new ArtigosExample();
		artigosExample.createArtigo(StringUtils.random(FIELDS.artigos_codigo.size[0]),
				StringUtils.random(FIELDS.artigos_descricao.size[0]/2));
		artigosExample.showArtigos();

		/*
		 * Entidades
		 */
		EntidadesExample entidadesExample=new EntidadesExample();
		entidadesExample.createCliente(ENTIDADESTIPOS.cliente.codigo,
				StringUtils.random(FIELDS.entidades_nome.size[0]/2));
		entidadesExample.showEntidades(ENTIDADESTIPOS.cliente.codigo);

		/*
		 * Documentos
		 */
		DocumentosExample documentosExample=new DocumentosExample();
		documentosExample.createDocumento(PopulateVARS.DOCUMENTOSTIPOS.clientes_factura.codigo);

	}

	public void dispose() {}


}
