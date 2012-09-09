/*******************************************************************************
 * 2008-2011 Projecto Colibri
 * Marco Lopes (marcolopes@projectocolibri.com)
 *******************************************************************************/
package rcp.example.workbench;

import org.dma.utils.eclipse.swt.image.ImageManager;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.ui.part.ViewPart;

import rcp.colibri.core.vars.gui.IconVARS;
/*
 * Informacao sobre SWT (Standard Widget Toolkit)
 * http://www.eclipse.org/swt/
 */
public class ExampleView extends ViewPart {

	public static final String ID = "ExampleView";

	public void createPartControl(Composite parent) {

		setPartName("#view");
		setTitleImage(ImageManager.getImage(IconVARS.COOLBAR_AJUDA));

		Composite composite=new Composite(parent, SWT.NONE);
		composite.setLayout(new FillLayout());

		Label label=new Label(parent, SWT.NONE);
		label.setText("Hello World!!!");

	}


	public void setFocus() {}


}