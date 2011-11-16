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

import java.io.*;
import java.util.ArrayList;

public class debugCommand implements cmdcontrol {

	variable variables[] = new variable[1002];
	String localdirectory = null;
	static int sizeOfVariables;
	static boolean isSetUp;

	public debugCommand() {
		sizeOfVariables = 0;
		isSetUp = false;

	}

	public String getLocalDirectory() {
		return localdirectory;
	}

	public boolean isSetUp() {
		return isSetUp;

	}

	public variable getVariable(int num) {
		return variables[num];

	}

	public int getNumOfVariables() {
		return sizeOfVariables;
	}

	public void cleanUp() {
		sizeOfVariables = 0;
		isSetUp = false;

	}

	public void printAll() {
		int i;
		for (i = 0; i < sizeOfVariables; i++) {
			variables[i].print();

		}

	}

	public int setNewCommand(String[] options, int optioncount,
			String[] parameters, int parametercount, fileDirectory fdir) {
		// the parameters[0] should contain the file directory location of the
		// target file.

		int kerneldebug = 0;
		String destDir = parameters[0];
		sizeOfVariables = 0;

		destDir = cpCommand.stdFileName(destDir);

		File a = new File(destDir + "\\default\\extract.pl");
		File b = new File(destDir + "\\default\\LiteOS.elf");
		if ((a.exists() == false) || (b.exists() == false)) {
			a = new File(destDir + "\\bin\\micaz\\extract.pl");
			b = new File(destDir + "\\bin\\micaz\\LiteOS.elf");
			if ((a.exists() == false) || (b.exists() == false)) {
				colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
						"Directory error. File does not exist here. ");
				return 0;
			} else
				kerneldebug = 1;

		}

		isSetUp = true;
		localdirectory = new String(destDir);

		try {
			String line;
			Process p;

			if (kerneldebug == 0)
				p = Runtime.getRuntime().exec(
						"perl " + destDir + "\\default\\extract.pl " + destDir
								+ "\\default\\LiteOS.elf");
			else
				p = Runtime.getRuntime().exec(
						"perl " + destDir + "\\bin\\micaz\\extract.pl " + destDir
								+ "\\bin\\micaz\\LiteOS.elf");

			BufferedReader input = new BufferedReader(new InputStreamReader(p
					.getInputStream()));

			while ((line = input.readLine()) != null) {
				// System.out.println(line);
				variables[sizeOfVariables++] = new variable(line);
			}

			input.close();
		} catch (Exception err) {
			err.printStackTrace();
		}

		// printAll();
		return 0;
	}

	// Return the delay in milliseconds
	public int getDelay() {

		return 0;

	}

	// return the command will be used
	public byte[] getNewCommand(int index) {

		return null;

	}

	// This function must handle multiple cases, for type I, II, and III
	// respectively.

	public void handleresponse(String[] options, int optioncount,
			String[] parameters, int parametercount, ArrayList reply,
			fileDirectory fdir) {

		return;

	}

}
