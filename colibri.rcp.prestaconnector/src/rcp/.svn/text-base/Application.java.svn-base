/*******************************************************************************
 * 2008-2012 Projecto Colibri
 * Marco Lopes (marcolopes@projectocolibri.com)
 *******************************************************************************/
package rcp;

import org.dma.utils.eclipse.swt.DialogHandler;
import org.eclipse.equinox.app.IApplication;
import org.eclipse.equinox.app.IApplicationContext;
import org.eclipse.swt.widgets.Display;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.PlatformUI;

import rcp.colibri.core.login.ColibriLogin;
import rcp.colibri.core.security.LicenceManager;
import rcp.colibri.dao.database.connection.ConnectionManager;
import rcp.colibri.dao.xmlbeans.EmpresasXml;
import rcp.colibri.workbench.shells.login.LoginShell;
import rcp.example.workbench.actions.DatabaseExampleAction;
import rcp.example.workbench.actions.ReportExampleAction;

/**
 * This class controls all aspects of the application's execution
 */
public class Application implements IApplication {

	/*
	 * Alterar CREATE_WORKBENCH para testar COM e SEM workbench
	 *
	 * Caso o workbench nao seja necessario, as classes que
	 * configuram a plataforma RCP podem ser removidas:
	 *
	 * ApplicationWorkbenchAdvisor
	 * ApplicationWorkbenchWindowAdvisor
	 * ApplicationActionBarAdvisor
	 *
	 * Informacao sobre a plataforma RCP
	 * http://www.eclipse.org/articles/Article-RCP-1/tutorial1.html
	 *
	 */
	public static final boolean CREATE_WORKBENCH=true;

	public Object start(IApplicationContext context) {

		System.out.println(Activator.PLUGIN_ID+" APPLICATION <start>");

		Integer exitCode = IApplication.EXIT_OK;

		Display display = PlatformUI.createDisplay();

		try{
			/*
			 * LOGIN
			 * Existem duas formas de efectuar login
			 * 1) Atraves do ecra de login
			 * 2) Atraves do metodo de login
			 */
			//if (login(display)==LoginShell.ACTION_LOGIN){ // 1
			if (login()){ // 2

				System.out.println("COLIBRI LOGIN OK!");

				if (CREATE_WORKBENCH){
					/*
					 * Cria workbench atraves de ApplicationWorkbenchAdvisor
					 */
					if (PlatformUI.createAndRunWorkbench(display, new ApplicationWorkbenchAdvisor()) == PlatformUI.RETURN_RESTART)
						exitCode = IApplication.EXIT_RESTART;
				}else{
					/*
					 * Exemplos que podem correr SEM workbench
					 */
					new DatabaseExampleAction().run();
					new ReportExampleAction().run();
				}

			}

		} catch (Exception e){
			e.printStackTrace();
		} finally {
			display.dispose();
		}

		System.out.println(Activator.PLUGIN_ID+" APPLICATION <exit>: "+exitCode);

		return exitCode;

	}


	public void stop() {

		System.out.println(Activator.PLUGIN_ID+" APPLICATION <stop>");
		if (!PlatformUI.isWorkbenchRunning())
			return;
		final IWorkbench workbench = PlatformUI.getWorkbench();
		final Display display = workbench.getDisplay();
		display.syncExec(new Runnable() {
			public void run() {
				if (!display.isDisposed())
					workbench.close();
			}
		});
	}


	/**
	 * Shell de login
	 */
	private int login(Display display) {

		try{
			LoginShell shell = new LoginShell(display);
			shell.open();

			while (!shell.isDisposed())
				if (!display.readAndDispatch())
					display.sleep();

			return shell.getExitAction();

		} catch (Exception e){
			e.printStackTrace();
		}

		return LoginShell.ACTION_EXIT;

	}


	/**
	 * Login automatico
	 */
	public static boolean login() {

		ColibriLogin login=new ColibriLogin();

		try{

			if (LicenceManager.instance.load(
				/*
				 * LICENCA
				 * A licenca deve ser carregada antes do login ser efectuado
				 * Por defeito e' carregada a licenca FREE (certificada)
				 */
				//FileVARS.LICENCE_FOLDER+"PORTUGAL.rcplicence", "0") &&
				"/colibri/licencas/PRO.rcplicence", "0") &&

				login.process(
				/*
				 * EMPRESA
				 * Existem duas formas de obter a empresa de login
				 * 1) Atraves do carregamento de uma empresa existente
				 * 2) Atraves da criacao de um objecto empresa
				 */
				//EmpresasXml.instance.load(0), // 1
				EmpresasXml.instance.create("H2 Integrado",
					ConnectionManager.DRIVER_H2, "", "colibri7", "sa", ""), // 2
				/*
				 * UTILIZADOR
				 * Por defeito e' assumido o utilizador ADMIN
				 */
				"admin", "admin"))
				return true;

		} catch (Exception e){
			e.printStackTrace();
		}

		DialogHandler.error(login.getErrors());

		return false;

	}


}
