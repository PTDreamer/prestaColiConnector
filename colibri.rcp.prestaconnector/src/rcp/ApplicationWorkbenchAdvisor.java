/*******************************************************************************
 * 2008-2011 Projecto Colibri
 * Marco Lopes (marcolopes@projectocolibri.com)
 *******************************************************************************/
package rcp;

import org.dma.utils.eclipse.core.jobs.JobManager;
import org.dma.utils.java.Debug;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.application.IWorkbenchConfigurer;
import org.eclipse.ui.application.IWorkbenchWindowConfigurer;
import org.eclipse.ui.application.WorkbenchAdvisor;
import org.eclipse.ui.application.WorkbenchWindowAdvisor;

import rcp.colibri.workbench.ColibriUI;
import rcp.colibri.workbench.perspectives.IColibriPerspective;
import rcp.example.workbench.ExamplePerspective;
/*
 * Metodos por ordem de execucao
 */
public class ApplicationWorkbenchAdvisor extends WorkbenchAdvisor {

	/*
	 * (non-Javadoc)
	 * @see org.eclipse.ui.application.WorkbenchAdvisor#createWorkbenchWindowAdvisor(org.eclipse.ui.application.IWorkbenchWindowConfigurer)
	 */
	public WorkbenchWindowAdvisor createWorkbenchWindowAdvisor(IWorkbenchWindowConfigurer configurer) {
		return new ApplicationWorkbenchWindowAdvisor(configurer);
	}


	/*
	 * (non-Javadoc)
	 * @see org.eclipse.ui.application.WorkbenchAdvisor#getInitialWindowPerspectiveId()
	 *
	 * A perspectiva inicial e' sempre aberta em primeiro lugar
	 * (desde que nao seja gravada a' saida do Workbench)
	 * e controla a saida da aplicacao atraves do menu
	 * (desde que seja usado o metodo ClosePerspectiveAction)
	 */
	public String getInitialWindowPerspectiveId() {
		return ExamplePerspective.ID;
	}


	/*
	 * (non-Javadoc)
	 * @see org.eclipse.ui.application.WorkbenchAdvisor#initialize(org.eclipse.ui.application.IWorkbenchConfigurer)
	 */
	public void initialize(IWorkbenchConfigurer configurer) {
		/*
		 * Configura o workbench
		 * Deve ser passado o ID da perspectiva inicial
		 */
		ColibriUI.configure(configurer, getInitialWindowPerspectiveId());
	}


	/*
	 * (non-Javadoc)
	 * @see org.eclipse.ui.application.WorkbenchAdvisor#postStartup()
	 */
	public void postStartup() {
		Debug.out(Activator.PLUGIN_ID);
		/*
		 * Inicializa o UI
		 * Devem ser passadas as perspectivas implementadas
		 */
		if (!ColibriUI.start(new IColibriPerspective[]{new ExamplePerspective()})){
			PlatformUI.getWorkbench().close();
		}
	}


	/*
	 * (non-Javadoc)
	 * @see org.eclipse.ui.application.WorkbenchAdvisor#preShutdown()
	 */
	public final boolean preShutdown() {
		Debug.out(Activator.PLUGIN_ID);
		JobManager.debug();
		/*
		 * Termina o UI
		 * A aplicacao nao deve ser terminada enquanto existirem processamentos
		 * a correr em background e o User Interface nao conseguir terminar
		 */
		return JobManager.getQueuedJobs()==0 && ColibriUI.stop();
	}


	public final void postShutdown() {
		Debug.out(Activator.PLUGIN_ID);
		ColibriUI.dispose();
	}


}
