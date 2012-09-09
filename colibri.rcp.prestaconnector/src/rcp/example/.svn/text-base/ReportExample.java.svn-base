/*******************************************************************************
 * 2008-2012 Projecto Colibri
 * Marco Lopes (marcolopes@projectocolibri.com)
 *******************************************************************************/
package rcp.example;

import org.dma.utils.eclipse.core.jobs.CustomJob;
import org.dma.utils.eclipse.core.jobs.IJobSupport;
import org.dma.utils.eclipse.core.jobs.JobManager;
import org.dma.utils.eclipse.core.jobs.tasks.JobTask;
import org.dma.utils.eclipse.core.jobs.tasks.JobUITask;
import org.dma.utils.eclipse.swt.DialogHandler;
import org.dma.utils.java.apache.mail.EmailAddress;
import org.dma.utils.java.apache.mail.EmailParameters;
import org.eclipse.jface.action.Action;

import rcp.colibri.core.BIRT.report.BIRTReport;
import rcp.colibri.core.mappers.filters.FilterMap;
import rcp.colibri.core.mappers.filters.FilterMap.OPERATORS;
import rcp.colibri.core.mappers.filters.FilterOperandMap;
import rcp.colibri.core.vars.database.DatabaseVARS;
import rcp.colibri.core.vars.database.DatabaseVARS.FIELDS;
import rcp.colibri.core.vars.database.PopulateVARS;
import rcp.colibri.core.vars.gui.LabelVARS;
import rcp.colibri.dao.database.model.Entidadesdocumentos;

public class ReportExample {

	public ReportExample(){
	}


	/**
	 * Processamento normal
	 * <p>
	 * Deve ser usado quando NAO existe workbench
	 * Evita que a aplicacao possa terminar enquanto o processamento decorre
	 */
	public void process(String tipodocumento, String serie, int numerodocumento, int action) {

		BIRTReport report=initReport(tipodocumento, serie, numerodocumento, action);
		String file=report.process();
		System.out.println("Output file: "+file);
		report.output();

	}




	/**
	 * Processamento em background
	 * <p>
	 * Deve ser usado apenas quando existe workbench
	 * Permite que a aplicacao continue a correr enquanto o processamento decorre
	 */
	public void backgroundProcess(String tipodocumento, String serie, int numerodocumento, int action) {

		final CustomJob job=new CustomJob();

		switch(action){
			case BIRTReport.ACTION_PREVIEW: job.setName(LabelVARS.operacao_preverrelatorio.name(0)); break;
			case BIRTReport.ACTION_PRINT: job.setName(LabelVARS.operacao_imprimirrelatorio.name(0)); break;
			case BIRTReport.ACTION_EMAIL: job.setName(LabelVARS.operacao_enviarrelatorio.name(0)); break;
		}

		final BIRTReport report=initReport(tipodocumento, serie, numerodocumento, action);

		job.addTask(new JobTask("Processamento", new Action() {
			public void run() {
				report.process();
			}
		}));

		job.addTask(new JobUITask("Resultados", new Action() {
			public void run() {
				report.output();
			}
		}));

		/*
		 * O registo do JOB em JobManager permite um controle
		 * total sobre todos os jobs agendados ou em execucao.
		 */
		JobManager.register(new IJobSupport(){
			public void jobStarting() {
			}
			public void jobDone() {
				DialogHandler.information(job.getName(), LabelVARS.msgwin_info_operacaoconcluida.name(0));
			}
		}, job);

		job.execute();

	}



	/**
	 * Inicializa o report
	 */
	public BIRTReport initReport(String tipodocumento, String serie, int numerodocumento, int action) {

		BIRTReport report=new BIRTReport(action,
				PopulateVARS.TEMPLATES.entidadesdocumentos_factura.codigo,
			new FilterMap(DatabaseVARS.entidadesdocumentos).
				addRule(FIELDS.entidadesdocumentos_key.name, OPERATORS.MATH.EQUAL,
					new FilterOperandMap(Entidadesdocumentos.generateKey(tipodocumento,serie,numerodocumento))),
			new EmailParameters(new EmailAddress[]{new EmailAddress("marcolopes@projectocolibri.com", "Marco Lopes")},
				"Envio de documento "+numerodocumento));

		return report;

	}


}
