// $Id: MessagePanel.java,v 1.8 2004/05/31 10:30:24 szewczyk Exp $

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
/* Authors:  Philip Levis  <pal@cs.berkeley.edu>
 *
 */

/**
 * MessagePanel is a swing panel representing a TinyOS message type:
 * message fields can be viewed and edited.
 * @author Philip Levis <pal@cs.berkeley.edu>
 */

package tools.message;

import java.awt.*;
import java.lang.reflect.*;
import java.util.*;
import javax.swing.*;
import javax.swing.text.*;

public class MessagePanel extends JPanel {
	private Message message;
	private Vector fields = new Vector();
	private String className;

	public MessagePanel(Message msg) throws Exception {
		message = msg;
		className = msg.getClass().getName();
		loadFields();

		this.setLayout(new GridLayout(0, 1));
		this.setAlignmentX(LEFT_ALIGNMENT);
		for (int i = 0; i < fields.size(); i++) {
			JPanel panel = (JPanel) fields.elementAt(i);
			panel.setAlignmentX(LEFT_ALIGNMENT);
			add(panel);
		}

	}

	private void loadFields() {
		Method[] methods = message.getClass().getMethods();
		for (int i = 0; i < methods.length; i++) {
			Method method = methods[i];
			String name = method.getName();
			if (name.startsWith("set_")) {
				name = name.substring(4);
				Class[] params = method.getParameterTypes();
				if (params.length != 1) {
					System.err.println("Found unknown field "
							+ name.substring(3) + " with "
							+ method.getParameterTypes().length
							+ " parameters: not adding to structure.");
					return;
				} else if (params[0].isArray()) {
					loadArrayField(name, method, params[0]);

				} else {
					loadField(name, method, params[0]);
				}
			}
		}
	}

	// Pass the set method so we can look at its parameters more
	// easily -- otherwise we'd have to scan the method list again
	private void loadField(String name, Method setMethod, Class type) {
		Method get;
		Method set;
		Method offset;
		Method size;
		Method signed;
		Class[] params = null;
		// System.err.println("Loading field: " + name);
		try {
			size = message.getClass().getMethod("size_" + name, params);
			get = message.getClass().getMethod("get_" + name, params);
			signed = message.getClass().getMethod("isSigned_" + name, params);
			set = setMethod;
			Integer lengthObj = (Integer) size.invoke(message, null);
			int unitLength = lengthObj.intValue();
			Boolean signedness = (Boolean) signed.invoke(message, null);
			MessageField field = new MessageField(message, name, unitLength,
					signedness.booleanValue(), get.getReturnType());
			fields.addElement(field);
		} catch (NoSuchMethodException exception) {
			//System.err.println("Exception thrown looking for methods of field "
			// + name + " in class " + className);
			// System.err.println(exception);
		} catch (IllegalAccessException exception) {
			System.err.println("Exception thrown looking for methods of field "
					+ name + " in class " + className);
			System.err.println(exception);
		} catch (InvocationTargetException exception) {
			System.err.println("Exception thrown looking for methods of field "
					+ name + " in class " + className);
			System.err.println(exception);
		}
	}

	private void loadArrayField(String name, Method setMethod, Class type) {
		Method get;
		Method set;
		Method offset;
		Method elements;
		Method unit;
		Method signed;
		Class[] params = new Class[1];
		params[0] = type;

		// System.err.println("Loading array field: " + name);
		try {
			elements = message.getClass()
					.getMethod("numElements_" + name, null);
			get = message.getClass().getMethod("get_" + name, null);
			unit = message.getClass().getMethod("elementSize_" + name, null);
			signed = message.getClass().getMethod("isSigned_" + name, null);
			set = setMethod;
			Integer lengthObj = (Integer) unit.invoke(message, null);
			int unitLength = lengthObj.intValue();
			Integer elemObj = (Integer) elements.invoke(message, null);
			int elemCount = elemObj.intValue();
			Boolean isSigned = (Boolean) signed.invoke(message, null);
			MessageField field = new MessageField(message, name, unitLength,
					isSigned.booleanValue(), type, true, elemCount);
			fields.addElement(field);
		} catch (NoSuchMethodException exception) {
			//System.err.println("Exception thrown looking for methods of field "
			// + name + " in class " + className);
			// System.err.println(exception);
		} catch (IllegalAccessException exception) {
			System.err.println("Exception thrown looking for methods of field "
					+ name + " in class " + className);
			System.err.println(exception);
		} catch (InvocationTargetException exception) {
			System.err.println("Exception thrown looking for methods of field "
					+ name + " in class " + className);
			System.err.println(exception);
		}
	}

	public void write() throws Exception {
		for (int i = 0; i < fields.size(); i++) {
			MessageField field = (MessageField) fields.elementAt(i);
			field.write();
		}
	}

	public Message getMessage() throws Exception {
		write();
		return message;
	}

	public byte[] get() throws Exception {
		write();
		return message.dataGet();
	}

	public static void main(String[] args) throws Exception {
		MessageFactory mf = new MessageFactory();
		Message msg = mf.createTOSMsg();
		MessagePanel p = new MessagePanel(msg);
		JFrame frame = new JFrame("MessagePanel Test");
		frame.getContentPane().add(p);
		frame.pack();
		frame.setVisible(true);
	}
}
