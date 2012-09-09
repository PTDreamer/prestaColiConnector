/*******************************************************************************
 * 2008-2012 Projecto Colibri
 * Marco Lopes (marcolopes@projectocolibri.com)
 *******************************************************************************/
package rcp.example;

import java.math.BigDecimal;

import org.dma.utils.eclipse.swt.DialogHandler;
import org.dma.utils.java.array.ErrorList;

import rcp.colibri.core.vars.database.PopulateVARS;
import rcp.colibri.dao.business.process.EntidadesdocumentosProcess;
import rcp.colibri.dao.business.process.EntidadesdocumentoslinhasProcess;
import rcp.colibri.dao.business.rules.EntidadesdocumentosRules;
import rcp.colibri.dao.business.rules.EntidadesdocumentoslinhasRules;
import rcp.colibri.dao.database.ColibriDatabase;
import rcp.colibri.dao.database.model.Entidades;
import rcp.colibri.dao.database.model.Entidadesdocumentos;
import rcp.colibri.dao.database.model.Entidadesdocumentoslinhas;

public class DocumentosExample {

	public DocumentosExample(){
	}


	/**
	 * Cria um novo documento
	 */
	public void createDocumento(String codigo) {
		try{
			//cria objecto documento
			Entidadesdocumentos documento=new Entidadesdocumentos(ColibriDatabase.loadDocumentostipos(codigo));

			//carrega uma entidade do tipo CLIENTE
			Entidades entidade=ColibriDatabase.loadEntidades(
				Entidades.generateKey(PopulateVARS.ENTIDADESTIPOS.cliente.codigo, 1));
			//inicializa a entidade do documento
			EntidadesdocumentosRules.init(documento, entidade);
			documento.setEntidade(entidade);

			//cria as linhas do documento
			createLinhasdocumento(documento, entidade);

			//processa regras
			EntidadesdocumentosProcess.rules(documento, documento.getLinhasdocumento());

			//finaliza processamento
			EntidadesdocumentosProcess.finish(documento, documento.getLinhasdocumento(), false);

			//grava o documento na base de dados
			ErrorList error=ColibriDatabase.storeEntidadesdocumentos(documento, false);

			//apresenta possiveis erros
			DialogHandler.error(error.getErrors());

		} catch (Exception e){
			e.printStackTrace();
		}

	}


	/**
	 * Cria as linhas do documento
	 */
	public void createLinhasdocumento(Entidadesdocumentos documento, Entidades entidade) {
		try{
			//cria objecto linha
			Entidadesdocumentoslinhas linha=documento.createLinhasdocumento();

			//inicializa a linha
			EntidadesdocumentoslinhasRules.init(linha, entidade, ColibriDatabase.loadArtigos("1"));

			//actualiza a quantidade
			linha.setQuantidade(BigDecimal.valueOf(5));

			//actualiza o preco
			linha.setPreco(BigDecimal.valueOf(100));

			//processa a linha
			EntidadesdocumentoslinhasProcess.rules(documento, linha);

			//adiciona a linha ao documento
			documento.addLinhasdocumento(linha);

		} catch (Exception e){
			e.printStackTrace();
		}

	}


}
