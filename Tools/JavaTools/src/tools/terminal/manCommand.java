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

import java.util.ArrayList;

/**
 * The class for the mkdir command
 */

public class manCommand implements cmdcontrol {

	private byte[] reply = new byte[64];

	private void printHelpLS() {
		colorOutput
				.println(colorOutput.COLOR_BRIGHT_GREEN, "LS\tUser Commands");
		colorOutput.println(colorOutput.COLOR_YELLOW, "NAME");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tls - list directory contents");
		colorOutput.println(colorOutput.COLOR_YELLOW, "SYNOPSIS");
		colorOutput
				.println(colorOutput.COLOR_BRIGHT_GREEN, "\tls [OPTION]... ");
		colorOutput.println(colorOutput.COLOR_YELLOW, "DESCRIPTION");
		colorOutput
				.println(colorOutput.COLOR_BRIGHT_GREEN,
						"\tList  information  about  the files (the current directory by default).");
		colorOutput.println(colorOutput.COLOR_YELLOW, "-l");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tList detailed information about files.");
		colorOutput.println(colorOutput.COLOR_YELLOW, "-u");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tDisable the internal cache of the shell.");

	}

	private void printHelpCD() {
		colorOutput
				.println(colorOutput.COLOR_BRIGHT_GREEN, "CD\tUser Commands");
		colorOutput.println(colorOutput.COLOR_YELLOW, "NAME");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tcd - enter a file directory");
		colorOutput.println(colorOutput.COLOR_YELLOW, "SYNOPSIS");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tcd [FILE DIRECTORY] ");
		colorOutput.println(colorOutput.COLOR_YELLOW, "DESCRIPTION");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tEnter a file directory.");
	}

	private void printHelpCP() {
		colorOutput
				.println(colorOutput.COLOR_BRIGHT_GREEN, "CP\tUser Commands");
		colorOutput.println(colorOutput.COLOR_YELLOW, "NAME");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tcp - copy files and directories");
		colorOutput.println(colorOutput.COLOR_YELLOW, "SYNOPSIS");
		colorOutput
				.println(colorOutput.COLOR_BRIGHT_GREEN, "\t cp SOURCE DEST");
		colorOutput.println(colorOutput.COLOR_YELLOW, "DESCRIPTION");
		colorOutput
				.println(
						colorOutput.COLOR_BRIGHT_GREEN,
						"\tCopy file between directories. The directories may be \n"
								+ "\tlocated on PC or mounted sensor nodes. To access dire-\n"
								+ "\tctories on PC, start the address with PC drive name, such\n"
								+ "\tas /c/data.");
		colorOutput.println(colorOutput.COLOR_YELLOW, "EXAMPLE");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tcp /c/Temp/code.lhex /sn01/nodeA/apps/code.lhex");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tcp /sn01/nodeA/usr/data /c/Temp/data");
	}

	private void printHelpDU() {
		colorOutput
				.println(colorOutput.COLOR_BRIGHT_GREEN, "DU\tUser Commands");
		colorOutput.println(colorOutput.COLOR_YELLOW, "NAME");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tdu - estimate file system usage");
		colorOutput.println(colorOutput.COLOR_YELLOW, "SYNOPSIS");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "\t du");
		colorOutput.println(colorOutput.COLOR_YELLOW, "DESCRIPTION");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tReturn the current remaining space in EEPROM and Flash \n");
	}

	private void printHelpFORMAT() {
		colorOutput
				.println(colorOutput.COLOR_BRIGHT_GREEN, "DU\tUser Commands");
		colorOutput.println(colorOutput.COLOR_YELLOW, "NAME");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tdu - estimate file system usage");
		colorOutput.println(colorOutput.COLOR_YELLOW, "SYNOPSIS");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "\t du");
		colorOutput.println(colorOutput.COLOR_YELLOW, "DESCRIPTION");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tReturn the current remaining space in EEPROM and Flash \n");
	}

	private void printHelpMKDIR() {
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"MKDIR\tUser Commands");
		colorOutput.println(colorOutput.COLOR_YELLOW, "NAME");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tmkdir - create a directory");
		colorOutput.println(colorOutput.COLOR_YELLOW, "SYNOPSIS");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\t mkdir DIRECTORY NAME");
		colorOutput.println(colorOutput.COLOR_YELLOW, "DESCRIPTION");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tCreate a new directory in the current position.\n");
	}

	private void printHelpPS() {
		colorOutput
				.println(colorOutput.COLOR_BRIGHT_GREEN, "PS\tUser Commands");
		colorOutput.println(colorOutput.COLOR_YELLOW, "NAME");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tps - view current running processes");
		colorOutput.println(colorOutput.COLOR_YELLOW, "SYNOPSIS");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "\t ps");
		colorOutput.println(colorOutput.COLOR_YELLOW, "DESCRIPTION");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tDisplay the current processes and their status.\n");
	}

	private void printHelpRM() {
		colorOutput
				.println(colorOutput.COLOR_BRIGHT_GREEN, "RM\tUser Commands");
		colorOutput.println(colorOutput.COLOR_YELLOW, "NAME");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\trm - remove a file or a directory");
		colorOutput.println(colorOutput.COLOR_YELLOW, "SYNOPSIS");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\t rm FILE/DIRECTORY NAME");
		colorOutput.println(colorOutput.COLOR_YELLOW, "DESCRIPTION");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tDeletes a file or a directory in the file system.\n");
	}

	private void printHelpSEARCH() {
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"SEARCH\tUser Commands");
		colorOutput.println(colorOutput.COLOR_YELLOW, "NAME");
		colorOutput
				.println(
						colorOutput.COLOR_BRIGHT_GREEN,
						"\tsearch - finds and locates a file or a directory\n"
								+ "\taccording to a particular regular expression name");
		colorOutput.println(colorOutput.COLOR_YELLOW, "SYNOPSIS");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\t search FILE/DIRECTORY NAME");
		colorOutput.println(colorOutput.COLOR_YELLOW, "DESCRIPTION");
		colorOutput
				.println(
						colorOutput.COLOR_BRIGHT_GREEN,
						"\tThis command will search across the network by file\n"
								+ "name and returns a list of files/directories whose names\n"
								+ "contains the particular file names");
	}

	private void printHelpMAN() {
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"MAN\tUser Commands");
		colorOutput.println(colorOutput.COLOR_YELLOW, "NAME");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tman - displays the manual pages for particular commands");
		colorOutput.println(colorOutput.COLOR_YELLOW, "SYNOPSIS");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\t man COMMAND NAME");
		colorOutput.println(colorOutput.COLOR_YELLOW, "DESCRIPTION");
		colorOutput
				.println(
						colorOutput.COLOR_BRIGHT_GREEN,
						"\tThis command displays the manual pages of particular commands.\n"
								+ "If the manual pages are not available, it will be added in future versions.\n");

	}

	private void printHelpExit() {
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"EXIT\tUser Commands");
		colorOutput.println(colorOutput.COLOR_YELLOW, "NAME");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\texit - Exit the shell system");
		colorOutput.println(colorOutput.COLOR_YELLOW, "SYNOPSIS");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "\t exit");
		colorOutput.println(colorOutput.COLOR_YELLOW, "DESCRIPTION");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tThis command exits the shell system.");
	}

	private void printHelpPwd() {
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"PWD\tUser Commands");
		colorOutput.println(colorOutput.COLOR_YELLOW, "NAME");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tpwd - displays the current working directory");
		colorOutput.println(colorOutput.COLOR_YELLOW, "SYNOPSIS");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "\t pwd");
		colorOutput.println(colorOutput.COLOR_YELLOW, "DESCRIPTION");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tThis command displays the current working directory.");
	}

	private void printHelpExec() {
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"EXEC\tUser Commands");
		colorOutput.println(colorOutput.COLOR_YELLOW, "NAME");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\texec -  executes a user application");
		colorOutput.println(colorOutput.COLOR_YELLOW, "SYNOPSIS");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\t exec BINARYNAME.LHEX");
		colorOutput.println(colorOutput.COLOR_YELLOW, "DESCRIPTION");
		colorOutput
				.println(colorOutput.COLOR_BRIGHT_GREEN,
						"\tThis command executes a user application by invoking its binary name.");
	}

	private void printHelpKill() {
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"KILL\tUser Commands");
		colorOutput.println(colorOutput.COLOR_YELLOW, "NAME");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tkill - terminates a running application");
		colorOutput.println(colorOutput.COLOR_YELLOW, "SYNOPSIS");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\t kill BINARYNAME.LHEX");
		colorOutput.println(colorOutput.COLOR_YELLOW, "DESCRIPTION");
		colorOutput
				.println(colorOutput.COLOR_BRIGHT_GREEN,
						"\tThis command terminates a user application by invoking its binary name.");
	}

	private void printHelpTOUCH() {
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"TOUCH\tUser Commands");
		colorOutput.println(colorOutput.COLOR_YELLOW, "NAME");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\ttouch - creates a new file");
		colorOutput.println(colorOutput.COLOR_YELLOW, "SYNOPSIS");
		colorOutput
				.println(colorOutput.COLOR_BRIGHT_GREEN, "\t touch FILENAME");
		colorOutput.println(colorOutput.COLOR_YELLOW, "DESCRIPTION");
		colorOutput
				.println(
						colorOutput.COLOR_BRIGHT_GREEN,
						"\tThis command will create a file at the current position\n"
								+ "and the newly created file is assumed to be empty.\n");
	}

	private void printHelpNOHELP() {
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"UNKNOWN COMMAND\tUser Commands");
		colorOutput.println(colorOutput.COLOR_YELLOW, "NAME");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"\tmanual not available in the demo version");
		colorOutput.println(colorOutput.COLOR_YELLOW, "SYNOPSIS");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "\t not available");
		colorOutput.println(colorOutput.COLOR_YELLOW, "DESCRIPTION");
		colorOutput
				.println(
						colorOutput.COLOR_BRIGHT_GREEN,
						"\tIf this is a legitimate command, we will add its\n"
								+ "\tdescription and manual page in the later versions.");
	}

	// Return the total number of commands will be used
	public int setNewCommand(String[] options, int optioncount,
			String[] parameters, int parametercount, fileDirectory fdir) {

		if (parametercount > 1)
			colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
					"Syntax error. At most one parameter is allowed.");

		else if (parameters[0].compareTo("ls") == 0) {
			printHelpLS();
		} else if (parameters[0].compareTo("cd") == 0) {
			printHelpCD();
		} else if (parameters[0].compareTo("cp") == 0) {
			printHelpCP();
		} else if (parameters[0].compareTo("du") == 0) {
			printHelpDU();
		} else if (parameters[0].compareTo("format") == 0) {
			printHelpFORMAT();
		} else if (parameters[0].compareTo("mkdir") == 0) {
			printHelpMKDIR();
		} else if (parameters[0].compareTo("ps") == 0) {
			printHelpPS();
		} else if (parameters[0].compareTo("rm") == 0) {
			printHelpRM();
		} else if (parameters[0].compareTo("search") == 0) {
			printHelpSEARCH();
		} else if (parameters[0].compareTo("touch") == 0) {
			printHelpTOUCH();
		} else if (parameters[0].compareTo("man") == 0) {
			printHelpMAN();
		}

		else if (parameters[0].compareTo("pwd") == 0) {
			printHelpPwd();
		}

		else if (parameters[0].compareTo("exit") == 0) {
			printHelpExit();
		}

		else if (parameters[0].compareTo("exec") == 0) {
			printHelpExec();
		}

		else if (parameters[0].compareTo("kill") == 0) {
			printHelpKill();
		}

		else
			printHelpNOHELP();
		return 0;
	}

	// Return the delay in milliseconds
	public int getDelay() {
		return 200; // To change body of implemented methods use File | Settings
					// | File Templates.
	}

	// return the command will be used
	public byte[] getNewCommand(int index) {
		return reply;
	}

	public void handleresponse(String[] options, int optioncount,
			String[] parameters, int parametercount, ArrayList reply,
			fileDirectory fdir) {

		return;
	}
}
