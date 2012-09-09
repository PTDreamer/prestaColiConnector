/*******************************************************************************
 * 2008-2011 Projecto Colibri
 * Marco Lopes (marcolopes@projectocolibri.com)
 *******************************************************************************/
package rcp.example.workbench;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.jface.action.IAction;
import org.eclipse.ui.IPageLayout;
import org.eclipse.ui.IWorkbenchWindow;

import rcp.colibri.core.vars.gui.LabelVARS;
import rcp.colibri.workbench.perspectives.IColibriPerspective;
import rcp.colibri.workbench.support.actions.ClosePerspectiveAction;
import rcp.colibri.workbench.support.actions.OpenPreferencesAction;
import rcp.colibri.workbench.support.actions.ResetPerspectiveAction;
import rcp.colibri.workbench.support.bars.coolbar.ColibriCoolbarItem;
import rcp.colibri.workbench.support.bars.menubar.ColibriMenubarItem;
import rcp.colibri.workbench.support.helpers.WorkbenchHelper;
import rcp.example.workbench.actions.DatabaseExampleAction;
import rcp.example.workbench.actions.OpenExampleViewAction;
import rcp.example.workbench.actions.ReportExampleAction;

public class ExamplePerspective extends WorkbenchHelper implements IColibriPerspective {

	public static final String ID = "ExamplePerspective";

	public void createInitialLayout(IPageLayout layout) {

		layout.setEditorAreaVisible(false);
		//layout.setFixed(true);

		//vista fixa
		layout.addView(PerspectiveView.ID, IPageLayout.RIGHT, 0.75f, layout.getEditorArea());
		layout.getViewLayout(PerspectiveView.ID).setCloseable(false);

		//layout.addPerspectiveShortcut(ID);
		addOtherPerspectiveShortcuts(ID,layout);

	}



	//IColibriPerspective
	public List<ColibriCoolbarItem> createCoolbar(IWorkbenchWindow window){

		List<ColibriCoolbarItem> items=new ArrayList();

		items.add(new ColibriCoolbarItem("Colibri", new IAction[]{
			new OpenPreferencesAction(window)}));

		return items;

	}


	public List<ColibriMenubarItem> createMenubar(IWorkbenchWindow window) {

		List<ColibriMenubarItem> items=new ArrayList();

		items.add(new ColibriMenubarItem(LabelVARS.menubar_ficheiro.name(0), new IAction[]{
				new OpenPreferencesAction(window),
				new ResetPerspectiveAction(),
				new ClosePerspectiveAction(ID)}));

		items.add(new ColibriMenubarItem("#exemplos", new IAction[]{
				new OpenExampleViewAction(),
				new DatabaseExampleAction(),
				new ReportExampleAction()}));

		return items;

	}


	public boolean saveOnClose() {
		return true;
	}


	public String getId() {
		return ID;
	}


}
