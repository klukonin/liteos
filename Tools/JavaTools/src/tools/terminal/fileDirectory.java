/*
The following is the license of LiteOS.

This file is part of LiteOS.
Copyright Qing Cao, 2007-2008, University of Illinois , qcao2@uiuc.edu

LiteOS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LiteOS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LiteOS.  If not, see <http://www.gnu.org/licenses/>.
 */

package tools.terminal;

import java.util.regex.Pattern;

/**
 * The file directory is composed of fileNode. Each node stores info on the
 * files. The default location is the root
 */
public class fileDirectory {

	private fileNode root;
	private fileNode currentNode = null;

	/**
	 * Default constructor of the file directory class
	 */
	public fileDirectory() {
		root = new fileNode("root", "root");
	}

	/**
	 * Get the current node of the directory, i.e. the current directory
	 * 
	 * @return a file node that is the current node
	 */
	public fileNode getCurrentNode() {
		return currentNode;
	}

	/**
	 * Get the root node of the file directory
	 * 
	 * @return the root node
	 */
	public fileNode getRoot() {
		return root;
	}

	/**
	 * Change the directory of the file structure
	 * 
	 * @param p
	 *            the directory that wants to change to
	 */
	public void setCurrentNode(fileNode p) {
		currentNode = p;
	}

	private fileNode relativeAddress(String fileLocation, fileNode start,
			fileNode defaultnode) {
		String REGEX = "/";
		Pattern p = Pattern.compile(REGEX);
		String[] items = p.split(fileLocation);
		for (int i = 0; i < items.length; i++) {
			start = start.enterSubDirectory(items[i]);
			if (start == null)
				return defaultnode;
		}
		return start;

	}

	/**
	 * Change the directory
	 * 
	 * @param fileLocation
	 *            the directory to be changed to
	 */
	public void changeDir(String fileLocation) {

		if (fileLocation.startsWith("/")) {
			this.currentNode = relativeAddress(fileLocation, this.root,
					this.currentNode);
		}

		else if (fileLocation.startsWith("../")) {
			this.currentNode = relativeAddress(fileLocation.substring(2),
					this.currentNode.getParent(), this.currentNode);

		} else if (fileLocation.startsWith("./")) {
			if (this.currentNode.getType().startsWith("file") == true)
				this.currentNode = relativeAddress(fileLocation.substring(1),
						this.currentNode.getParent(), this.currentNode);
			else
				this.currentNode = relativeAddress(fileLocation.substring(1),
						this.currentNode, this.currentNode);

		} else if (fileLocation.compareTo("..") == 0) {
			this.currentNode = this.currentNode.getParent();

		} else if (fileLocation.compareTo(".") == 0) {
			if (this.currentNode.getType().startsWith("file") == true)
				this.currentNode = this.currentNode.getParent();
		} else {
			this.currentNode = relativeAddress(fileLocation, this.currentNode,
					this.currentNode);
		}

	}

}
