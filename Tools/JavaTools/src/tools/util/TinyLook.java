// $Id: TinyLook.java,v 1.3 2003/10/07 21:46:09 idgay Exp $

/*									tab:4
 * "Copyright (c) 2000-2003 The Regents of the University  of California.  
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the author appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Copyright (c) 2002-2003 Intel Corporation
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached INTEL-LICENSE     
 * file. If you do not find these files, copies can be found by writing to
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300, Berkeley, CA, 
 * 94704.  Attention:  Intel License Inquiry.
 */
/* Authors:	Phil Levis
 * Date:        February 16, 2003
 * Desc:        Class to allow different GUIs to have a similar look.
 *
 */

/**
 * @author Phil Levis
 */

package tools.util;

import java.awt.*;
import javax.swing.*;

public class TinyLook {

	private static Font defaultFont;
	private static Font labelFont;
	private static Font constLabelFont;
	private static Font smallFont;
	private static Font constFont;
	private static Font boldConstFont;
	private static Font boldFont;
	private static Color paleBlue;

	static {
		defaultFont = new Font("Helvetica", Font.PLAIN, 10);
		labelFont = new Font("Helvetica", Font.BOLD, 11);
		constLabelFont = new Font("Courier", Font.PLAIN, 11);
		smallFont = new Font("Helvetica", Font.PLAIN, 9);
		constFont = new Font("Courier", Font.PLAIN, 9);
		boldConstFont = new Font("Courier", Font.BOLD, 9);
		boldFont = new Font("Helvetica", Font.BOLD, 10);
		paleBlue = new Color(0x97, 0x97, 0xc8);
	}

	public static Font defaultFont() {
		return defaultFont;
	}

	public static Font labelFont() {
		return labelFont;
	}

	public static Font constLabelFont() {
		return constLabelFont;
	}

	public static Font smallFont() {
		return smallFont;
	}

	public static Font constFont() {
		return constFont;
	}

	public static Font boldConstFont() {
		return boldConstFont;
	}

	public static Font boldFont() {
		return boldFont;
	}

	public static Color getColor() {
		return paleBlue;
	}

	public static void setLookAndFeel(java.awt.Component component) {
		try {
			Class oclass = Class
					.forName("com.oyoaha.swing.plaf.oyoaha.OyoahaLookAndFeel");
			Object olnf = oclass.newInstance();
			// java.net.URL otm_res =
			// olnf.getClass().getResource("slushy8.otm");
			java.io.File otm_res = new java.io.File("slushy8.otm");
			if (otm_res != null) {
				Class params[] = new Class[1];
				params[0] = otm_res.getClass();
				Object args[] = new Object[1];
				args[0] = otm_res;
				java.lang.reflect.Method meth = oclass.getMethod(
						"setOyoahaTheme", params);
				meth.invoke(olnf, args);
				System.err.println("set theme OK");
			}
			UIManager.setLookAndFeel((javax.swing.LookAndFeel) olnf);
			System.err.println("Set L&F");
		} catch (Exception e) {
			System.err.println("Got exception loading Oyoaha: " + e);
			System.err.println("Using default look and feel");
			e.printStackTrace();
		}
	}

}
