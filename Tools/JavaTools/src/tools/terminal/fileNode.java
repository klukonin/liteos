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

/**
 * The class to store the information of a file node
 */
public class fileNode {

	// File name is the name of the file. It could be test, whatever

	private String FileName;

	// The type of the file
	private String FileType;

	// the actual size
	private int size;

	private int block;
	private int address;

	// children and parent relationship
	private fileNode[] children = new fileNode[32];
	private fileNode parent;
	private int childNum = 0;

	// initilization
	private boolean listed = false;

	/**
	 * The default constructor for the file node
	 */
	public fileNode() {
		FileName = null;
		FileType = "unknown";
		listed = false;
	}

	/**
	 * Another constructor for the file node with file name and file type
	 * 
	 * @param fileName
	 *            file name
	 * @param fileType
	 *            type of the file
	 */
	public fileNode(String fileName, String fileType) {
		FileName = fileName;
		FileType = fileType;
		listed = false;
	}

	/**
	 * The third constructor for the file node with more info
	 * 
	 * @param fileName
	 * @param fileType
	 * @param address
	 * @param block
	 */
	public fileNode(String fileName, String fileType, int address, int block) {
		FileName = fileName;
		FileType = fileType;
		this.address = address;
		this.block = block;
		listed = false;
	}

	/**
	 * Another constructor of the file node with info
	 * 
	 * @param fileName
	 * @param fileType
	 * @param address
	 * @param block
	 * @param size
	 */
	public fileNode(String fileName, String fileType, int address, int block,
			int size) {
		FileName = fileName;
		FileType = fileType;
		this.address = address;
		this.block = block;
		this.size = size;
		listed = false;
	}

	/**
	 * Set the parent of a file node
	 * 
	 * @param p
	 */
	public void setParent(fileNode p) {
		parent = p;
	}

	/**
	 * Get the parent of a file node
	 * 
	 * @return the parent of the file node
	 */
	public fileNode getParent() {
		if (FileName.compareTo("root") == 0)
			return this;
		else
			return this.parent;
	}

	/**
	 * Add a child of a file node
	 * 
	 * @param p
	 *            The child of the file node
	 */

	public void addChild(fileNode p) {
		this.children[childNum++] = p;
		p.setParent(this);
	}

	/**
	 * Delete a child of a file node
	 * 
	 * @param name
	 *            the child file node name
	 */
	public void deleteChildByName(String name) {
		int ret = this.findChild(name);
		this.removeChild(ret);
	}

	/**
	 * Remove a child node file by index
	 * 
	 * @param index
	 */
	public void removeChild(int index) {
		for (int i = index; i < this.childNum - 1; i++)
			this.children[i] = this.children[i + 1];
		this.childNum--;
	}

	public String getName() {
		return FileName;
	}

	public String getType() {
		return FileType;
	}

	public int getBlock() {
		return block;
	}

	public int getNodeAddress() {
		return address;
	}

	public void cleanAll() {
		this.childNum = 0;

	}

	public boolean duplicateChild(String name) {
		for (int i = 0; i < this.childNum; i++) {
			if (this.children[i].FileName.compareTo(name) == 0) {
				return true;
			}
		}
		return false;
	}

	public int findChild(String name) {
		for (int i = 0; i < this.childNum; i++) {
			if (this.children[i].FileName.compareTo(name) == 0) {
				return i;
			}
		}
		return -1;
	}

	public fileNode enterSubDirectory(String name) {
		for (int i = 0; i < this.childNum; i++) {
			if ((this.children[i].getName().compareTo(name) == 0)
					&& (this.children[i].getType().compareTo("file") != 0)) {
				// System.out.println("cd command successful");
				colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
						"cd command successful");

				return this.children[i];
			}
		}
		// System.out.println("No such subdirectory exists");
		colorOutput
				.println(
						colorOutput.COLOR_BRIGHT_RED,
						"No subdirectory exists when trying to enter it. Probably you have not used the ls command to list such directory. Currently cd only works for directories that are listed using ls.");

		return null;
	}

	public void printChildren(String option) {
		if (option.compareTo("short") == 0) {
			for (int i = 0; i < this.childNum; i++) {
				// System.out.println(this.children[i].FileName);
				colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
						this.children[i].FileName);
			}
			System.out.println();
		}
		if (option.compareTo("long") == 0) {
			colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
					"Name\tType\tSize\tProtection\n");
			for (int i = 0; i < this.childNum; i++) {
				this.children[i].printLong();
			}
			System.out.println();

		}
	}

	public void printChildren() {
		this.printChildren("short");
	}

	public void printLong() {

		if (this.FileType.compareTo("network") == 0)
			colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, this.FileName
					+ "  " + this.FileType + "   " + "--" + "     "
					+ "rwxrwxrwx");
		else if (this.FileType.compareTo("noderoot") == 0) {
			colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, this.FileName
					+ "  " + this.FileType + "   " + "--" + "     "
					+ "rwxrwxrwx");
		} else if (this.FileType.compareTo("directory") == 0) {
			colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, this.FileName
					+ "  " + this.FileType + "   " + "--" + "     "
					+ "rwxrwxrwx");
		} else if (this.FileType.startsWith("file") == true) {
			colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, this.FileName
					+ "  " + this.FileType + "   " + this.size + "     "
					+ "rwxrwxrwx");
		}

	}

	public void setListed() {
		listed = true;
	}

	public boolean getListed() {
		return listed;
	}

	public String getNodeName() {
		fileNode p;
		p = this;
		while (true) {
			if (p.getType().compareTo("noderoot") == 0)
				return p.getName();
			else
				p = p.getParent();
		}
	}
}
