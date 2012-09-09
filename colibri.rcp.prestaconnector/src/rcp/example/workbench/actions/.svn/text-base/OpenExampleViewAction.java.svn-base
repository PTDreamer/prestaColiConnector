/*******************************************************************************
 * 2008-2011 Projecto Colibri
 * Marco Lopes (marcolopes@projectocolibri.com)
 *******************************************************************************/
package rcp.example.workbench.actions;

import org.dma.utils.eclipse.swt.image.SWTImageUtils;
import org.eclipse.jface.action.Action;
import org.eclipse.ui.actions.ActionFactory.IWorkbenchAction;

import rcp.colibri.core.vars.gui.IconVARS;
import rcp.colibri.workbench.support.views.OpenViewAction;
import rcp.example.workbench.ExampleView;

public class OpenExampleViewAction extends Action implements IWorkbenchAction {

	public OpenExampleViewAction() {
		setText("#view");
		setImageDescriptor(SWTImageUtils.getImageDescriptor(IconVARS.COOLBAR_AJUDA));
	}


	public final void run(){

		new OpenViewAction(ExampleView.ID).run();

	}

	public void dispose() {}


}
