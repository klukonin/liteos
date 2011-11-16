// $Id: MessageInjector.java,v 1.6 2003/10/07 21:45:58 idgay Exp $

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
 * MessageInjector is a Java GUI for filling in and sending TinyOS
 * packets.
 * @author Philip Levis <pal@cs.berkeley.edu>
 */

package tools.message;

import java.awt.*;
import java.awt.event.*;
import java.io.IOException;
import java.lang.reflect.*;
import java.util.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.event.*;

import tools.message.*;
import tools.packet.*;
import tools.util.*;

public class MessageInjector extends JFrame {
	private JScrollPane pane;
	private JPanel buttonPanel;
	private JPanel nestedPanel;
	private MessageSelectionPanel selection;
	private JMenuBar menuBar;
	private MoteIF moteIF;

	public MessageInjector(String spec) {
		super("TinyOS Message Injector");
		TinyLook.setLookAndFeel(this);
		try {
			selection = new MessageSelectionPanel();
			moteIF = new MoteIF(BuildSource.makePhoenix(spec,
					PrintStreamMessenger.err));
			moteIF.start();
			initialize();
		} catch (Exception exception) {
			exception.printStackTrace();
		}
	}

	private void initialize() {
		nestedPanel = new JPanel();
		nestedPanel.setForeground(Color.black);
		nestedPanel.setLayout(new BoxLayout(nestedPanel, BoxLayout.Y_AXIS));

		selection.setForeground(Color.black);

		JScrollPane pane = new JScrollPane(selection);
		Dimension size = pane.getPreferredSize();
		if (size.getHeight() > 380) {
			size.setSize(size.getWidth(), 400);
		} else {
			size.setSize(size.getWidth(), size.getHeight() + 20);
		}
		if (size.getWidth() > 980) {
			size.setSize(1000, size.getHeight());
		} else {
			size.setSize(size.getWidth() + 20, size.getHeight());
		}
		pane.setPreferredSize(size);
		nestedPanel.add(pane);

		buttonPanel = new ButtonPanel(selection, moteIF, this);
		nestedPanel.add(buttonPanel);
		getContentPane().add(nestedPanel);

		menuBar = new MainMenuBar(this);
		menuBar.setFont(TinyLook.defaultFont());
		setJMenuBar(menuBar);
		pack();
		setVisible(true);
	}

	private class ButtonPanel extends JPanel {
		MessageSelectionPanel panel;
		MoteIF sender;

		JButton quitButton;
		JLabel label;
		JTextPane text;
		JButton sendButton;

		public ButtonPanel(MessageSelectionPanel panel, MoteIF sender,
				MessageInjector injector) {
			this.panel = panel;
			this.sender = sender;
			this.setLayout(new BoxLayout(this, BoxLayout.X_AXIS));

			label = new JLabel("Mote ID");
			label.setForeground(Color.black);

			text = new JTextPane(new LimitedStyledDocument(4));
			text.setText("ffff");

			sendButton = new JButton("Send");
			sendButton
					.addActionListener(new InjectListener(panel, sender, text));

			add(sendButton);
			add(label);
			add(text);
		}

	}

	protected class InjectListener implements ActionListener {
		private MessageSelectionPanel panel;
		private MoteIF sender;
		private JTextPane text;

		public InjectListener(MessageSelectionPanel panel, MoteIF sender,
				JTextPane text) {
			this.panel = panel;
			this.sender = sender;
			this.text = text;
		}

		public void actionPerformed(ActionEvent e) {
			try {
				int moteID = Integer.parseInt(text.getText(), 16);
				Message msg = panel.getMessage();
				sender.send(moteID, msg);
				System.out.println("Sending packet to address 0x"
						+ text.getText() + ":\n" + msg);
				System.out.print("Raw bytes: ");
				byte[] data = msg.dataGet();
				for (int i = 0; i < data.length; i++) {
					String datum = Integer.toHexString((int) (data[i] & 0xff));
					if (datum.length() < 2) {
						datum = "0" + datum;
					}
					if (datum.length() < 2) {
						datum = "0" + datum;
					}
					datum += " ";
					System.out.print(datum);
				}
				System.out.println();
			} catch (Exception exception) {
				System.err.println(exception);
				// exception.printStackTrace();
			}
		}
	}

	public static void main(String[] args) {
		try {
			int index = 0;
			String spec = null;
			while (index < args.length) {
				String arg = args[index];
				if (arg.equals("-h") || arg.equals("--help")) {
					usage();
					System.exit(0);
				}
				if ((arg.length() == 5) && arg.substring(0, 5).equals("-comm")) {
					index++;
					spec = args[index];
				} else {
					usage();
					System.exit(1);
				}
				index++;
			}
			System.out.println("Starting MessageInjector with comm " + spec);
			MessageInjector window = new MessageInjector(spec);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	protected class MainMenuBar extends JMenuBar implements ActionListener {

		private MessageInjector injector;
		private JMenu fileMenu;
		private JMenuItem quitMenuItem;

		public MainMenuBar(MessageInjector injector) {
			this.injector = injector;
			fileMenu = new JMenu("File");

			quitMenuItem = new JMenuItem("Quit");
			quitMenuItem.addActionListener(this);
			quitMenuItem.setFont(TinyLook.defaultFont());
			fileMenu.add(quitMenuItem);

			addMenu(fileMenu);
		}

		public void actionPerformed(ActionEvent e) {
			if (e.getActionCommand() == "Quit") {
				System.exit(0);
			}
		}

		private void addMenu(JMenu menu) {
			menu.setFont(TinyLook.labelFont());
			add(menu);
		}
	}

	private static void usage() {
		System.err
				.println("usage: MessageInjector [-h|--help|-comm [motecom spec]");
	}

}
