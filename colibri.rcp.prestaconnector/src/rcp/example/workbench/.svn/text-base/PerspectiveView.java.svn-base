/*******************************************************************************
 * 2008-2012 Projecto Colibri
 * Marco Lopes (marcolopes@projectocolibri.com)
 *******************************************************************************/
package rcp.example.workbench;

import org.dma.utils.eclipse.swt.custom.CustomBrowser;
import org.dma.utils.eclipse.swt.image.ImageManager;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.ui.part.ViewPart;

import rcp.colibri.core.vars.gui.IconVARS;
import rcp.colibri.core.vars.gui.LabelVARS;
import rcp.colibri.core.vars.rcp.FileVARS;
import rcp.colibri.workbench.support.views.IViewStateSupport;

public class PerspectiveView extends ViewPart implements IViewStateSupport {

	/**
	 * O ID da vista tem de ser UNICO
	 * No pode existir no plugin do Colibri
	 */
	public static final String ID = "PerspectiveView";

	public void createPartControl(Composite parent) {

		setPartName(LabelVARS.desktop_browser.name(0));
		setTitleImage(ImageManager.getImage(IconVARS.COOLBAR_BROWSER));

		Composite composite=new Composite(parent, SWT.NONE);
		composite.setLayout(new FillLayout());

		CustomBrowser browser=new CustomBrowser(composite);
		browser.setUrl(FileVARS.BROWSER_BASE_URL);

	}


	public void setFocus() {}


	/*
	 * IViewStateSupport(non-Javadoc)
	 * @see rcp.colibri.workbench.support.views.IViewStateSupport#isViewStateValid()
	 */
	public boolean isViewStateValid() {
		return true;
	}


}