// $Id: MessageSelection.java,v 1.9 2004/10/21 22:26:30 selfreference Exp $

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
 * MessageSelection class that tries to find available message types
 * in TinyOS tools directory structure. It assumes that the root
 * directory provided ("." in null constructor) is in the Java
 * CLASSPATH variable, as it uses directory paths to generate class
 * names to load.
 * @author Philip Levis <pal@cs.berkeley.edu>
 */

package tools.message;

import java.io.*;
import java.util.*;

public class MessageSelection {
	private String[] paths;
	private Message[] messages = new Message[0];
	private Vector vector = new Vector();
	private Class messageClass;

	public static final int MAX_DEPTH = 12;

	public MessageSelection() {
		paths = getPaths();
		loadMessages();
	}

	public Message[] messages() {
		return messages;
	}

	public String[] getPaths() {
		String classPath = System.getProperty("java.class.path");
		if (classPath.indexOf(":\\") == -1) {
			classPath = classPath.replace(':', ' ');
		} else {
			classPath = classPath.replace(';', ' ');
		}
		Vector vector = new Vector();
		while (classPath.length() > 0) {
			String path;
			int end = classPath.indexOf(" ");
			if (end == -1) {
				path = classPath;
				end = classPath.length() - 1;
			} else {
				path = classPath.substring(0, end);
			}
			classPath = classPath.substring(end + 1);
			vector.addElement(path);
		}

		String[] paths = new String[vector.size()];
		for (int i = 0; i < vector.size(); i++) {
			paths[i] = (String) vector.elementAt(i);
		}

		return paths;
	}

	private void loadMessages() {
		for (int i = 0; i < paths.length; i++) {
			try {
				File rootFile = new File(paths[i]);
				Message m = new Message(0);
				this.messageClass = m.getClass();

				findMessages(paths[i], rootFile, 0);
				messages = new Message[vector.size()];
				for (int j = 0; j < vector.size(); j++) {
					messages[j] = (Message) vector.elementAt(j);
				}
			} catch (IOException exception) {
				System.err.println(exception);
				messages = new Message[0];
			} catch (InstantiationException exception) {
				System.err.println(exception);
				messages = new Message[0];
			} catch (ClassNotFoundException exception) {
				System.err.println(exception);
				messages = new Message[0];
			} catch (IllegalAccessException exception) {
				System.err.println(exception);
				messages = new Message[0];
			}
		}
	}

	private boolean isRelated(Class subC, Class superC) {
		if (subC == superC) {
			return false;
		}
		for (Class tmp = subC.getSuperclass(); tmp != null; tmp = tmp
				.getSuperclass()) {
			if (tmp.equals(superC)) {
				return true;
			}
		}
		return false;
	}

	private boolean alreadyHave(Class c) {
		Enumeration elements = vector.elements();
		while (elements.hasMoreElements()) {
			Message m = (Message) elements.nextElement();
			if (m.getClass().getName().equals(c.getName())) {
				return true;
			}
		}
		return false;
	}

	private void findMessages(String root, File file, int depth)
			throws IOException, ClassNotFoundException, InstantiationException,
			IllegalAccessException {

		String indent = "";
		for (int cnt = 0; cnt < depth; cnt++) {
			indent += "  ";
		}

		if (depth > MAX_DEPTH) {
			System.err.println("Maximum search depth (" + MAX_DEPTH
					+ ") reached by: " + file.getName());
		}

		else if (file.isDirectory() && !file.getName().equals("sim")) {
			String[] dirents = file.list();
			for (int i = 0; i < dirents.length; i++) {
				File subFile = new File(file.getPath() + "/" + dirents[i]);
				findMessages(root, subFile, depth + 1);
			}
		}

		else if (file.isFile() && // Could be a Java class
				file.getPath().endsWith("Msg.class")) { // Only messages
			String fileName = file.getPath();

			// Cut off leading path stuff
			if (fileName.startsWith(root)) {
				fileName = fileName.substring(root.length() + 1);
			} else if (file.getCanonicalPath().startsWith(root)) {
				fileName = file.getCanonicalPath().substring(root.length() + 1);
			}

			String classEnd = ".class";
			if (fileName.endsWith(classEnd)) {
				// Cut off the .class,
				// then change '/'s in path to '.'s
				String className = fileName.substring(0, fileName.length()
						- classEnd.length());
				className = className.replace('/', '.');
				className = className.replace('\\', '.');
				try {
					Class newClass = Class.forName(className);
					if (isRelated(newClass, messageClass)
							&& !alreadyHave(newClass)) {
						Message m = (Message) newClass.newInstance();
						vector.addElement(m);
					}
				} catch (NoClassDefFoundError e) {
					// System.out.println("  not defined.");
				} catch (ClassNotFoundException e) {
					// System.out.println("  not found.");
				} catch (SecurityException e) {
					// System.out.println("  mistaken path with java in it.");
				}
			}
		} else {
			// System.out.println(indent + "Unknown file type: " + file + "!");
		}
	}

	public static void main(String[] args) {
		MessageSelection ms = new MessageSelection();
	}
}
