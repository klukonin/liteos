// $Id: MessageField.java,v 1.6 2003/10/07 21:45:58 idgay Exp $

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

import tools.util.*;

public class MessageField extends JPanel {
	private String name;
	private boolean isArray;
	private boolean isSigned;
	private int length;
	private int unitLength;
	private Class type;
	private Message msg;
	private JTextPane[] field;
	private JLabel label;
	private Message message;

	private Font lFont = TinyLook.constLabelFont();
	private Font eFont = TinyLook.constFont();

	public MessageField(Message msg, String name, int unitLength,
			boolean signed, Class type) {
		super();
		this.msg = msg;
		this.name = name;
		this.unitLength = unitLength;
		this.length = 1;
		this.type = type;
		this.isArray = false;
		this.isSigned = signed;
		this.message = msg;
		this.setForeground(Color.black);

		String labelName = new String(name);
		for (int i = labelName.length(); i < 24; i++) {
			labelName += " ";
		}
		labelName += " ";
		label = new JLabel(labelName);
		label.setForeground(Color.black);

		field = new JTextPane[1];
		field[0] = new JTextPane(new LimitedStyledDocument(unitLength * 2));

		String text = "";
		for (int i = 0; i < (unitLength * 2); i++) {
			text = text + "0";
		}
		field[0].setText(text);

		label.setFont(lFont);
		label.setAlignmentX(LEFT_ALIGNMENT);
		add(label);

		field[0].setFont(eFont);
		field[0].setAlignmentX(RIGHT_ALIGNMENT);
		field[0].setSize(6 + unitLength * 2 * 5, 16);
		field[0].setMaximumSize(field[0].getSize());
		add(field[0]);

		this.setLayout(new BoxLayout(this, BoxLayout.X_AXIS));
		this.setAlignmentX(LEFT_ALIGNMENT);
	}

	public MessageField(Message msg, String name, int unitLength,
			boolean signed, Class type, boolean isArray, int arrayLength) {
		super();
		this.msg = msg;
		this.name = name;
		this.unitLength = unitLength;
		this.type = type;
		this.isArray = isArray;
		this.isSigned = signed;
		this.length = arrayLength;
		this.message = msg;
		this.setForeground(Color.black);

		String labelName = new String(name);
		for (int i = labelName.length(); i < 24; i++) {
			labelName += " ";
		}
		labelName += " ";
		label = new JLabel(labelName);
		label.setForeground(Color.black);
		label.setAlignmentX(LEFT_ALIGNMENT);
		label.setFont(lFont);
		add(label);

		if (isArray) {
			field = new JTextPane[arrayLength];
			// System.err.println("Setting text to ");
			for (int i = 0; i < arrayLength; i++) {
				field[i] = new JTextPane(new LimitedStyledDocument(
						unitLength * 2));
				String text = "";
				for (int j = 0; j < (unitLength * 2); j++) {
					text += "0";
				}
				field[i].setFont(eFont);
				field[i].setText(text);
				field[i].setAlignmentX(RIGHT_ALIGNMENT);
				field[i].setAlignmentX(RIGHT_ALIGNMENT);
				field[i].setSize(8 * unitLength * 2, 16);
				field[i].setMaximumSize(field[i].getSize());
				// System.err.print(text + " ");
				add(field[i]);
			}
			// System.err.println();
		} else {
			this.setLayout(new BoxLayout(this, BoxLayout.X_AXIS));
			field = new JTextPane[1];
			field[0] = new JTextPane(new LimitedStyledDocument(
					(unitLength + 3) / 4));
			this.length = 1;
		}
		this.setLayout(new BoxLayout(this, BoxLayout.X_AXIS));
		this.setAlignmentX(LEFT_ALIGNMENT);
	}

	public int getLength() {
		return length;
	}

	public String getName() {
		return name;
	}

	public void write() throws Exception {
		if (isArray) {
			writeArray();
		} else {
			String methodName = "set_" + name;
			Class[] argTypes = new Class[1];
			Object arg;
			argTypes[0] = type;

			// System.err.println("Writing out " + field[0].getText());
			if (type.equals(Byte.TYPE)) {
				arg = Byte.valueOf(field[0].getText(), 16);
			} else if (type.equals(Short.TYPE)) {
				arg = Short.valueOf(field[0].getText(), 16);
			} else if (type.equals(Character.TYPE)) {
				arg = new Character((char) Integer.parseInt(field[0].getText(),
						16));
			} else if (type.equals(Integer.TYPE)) {
				arg = Integer.valueOf(field[0].getText(), 16);
			} else if (type.equals(Long.TYPE)) {
				arg = Long.valueOf(field[0].getText(), 16);
			} else if (type.equals(methodName.getClass())) {
				arg = field[0].getText();
			} else {
				System.err.println("Unrecognized type: " + type.getName());
				return;
			}
			// System.err.println("Trying to call void " + methodName + "(" +
			// argTypes[0].getName() + "); on " + message.getClass().getName());
			Method method = message.getClass().getMethod(methodName, argTypes);
			Object[] args = new Object[1];
			args[0] = arg;
			method.invoke(message, args);
		}
	}

	private void writeArray() throws Exception {
		String methodName = "set_" + name;
		Class[] argTypes = new Class[1];
		Integer index;
		Object arg;
		argTypes[0] = type;

		if (type.getComponentType().equals(Byte.TYPE)) {
			byte[] tArg = new byte[field.length];
			arg = tArg;
			for (int i = 0; i < field.length; i++) {
				tArg[i] = Byte.valueOf(field[i].getText(), 16).byteValue();
			}
		} else if (type.getComponentType().equals(Short.TYPE)) {
			short[] tArg = new short[field.length];
			arg = tArg;
			for (int i = 0; i < field.length; i++) {
				tArg[i] = Short.valueOf(field[i].getText(), 16).shortValue();
			}
		} else if (type.getComponentType().equals(Character.TYPE)) {
			String tmparg = new String();
			for (int i = 0; i < field.length; i++) {
				tmparg += field[i].getText();
			}
			argTypes[0] = tmparg.getClass();
			arg = tmparg;
		} else if (type.getComponentType().equals(Integer.TYPE)) {
			int[] tArg = new int[field.length];
			arg = tArg;
			for (int i = 0; i < field.length; i++) {
				tArg[i] = Integer.valueOf(field[i].getText(), 16).intValue();
			}
		} else if (type.getComponentType().equals(Long.TYPE)) {
			long[] tArg = new long[field.length];
			arg = tArg;
			for (int i = 0; i < field.length; i++) {
				tArg[i] = Long.valueOf(field[i].getText(), 16).longValue();
			}
		} else {
			System.err.println("Unrecognized array subtype: " + type.getName());
			return;
		}
		Method method = message.getClass().getMethod(methodName, argTypes);
		// System.err.println("Trying to call void " + methodName + "(" +
		// argTypes[0].getName() + ", "+ argTypes[1].getName() + "); on " +
		// message.getClass().getName());
		Object[] args = new Object[1];
		args[0] = arg;
		// System.err.println("Calling " + methodName + "(" + args[0] + ");");
		method.invoke(message, args);
	}

	public void read() {

	}

}
