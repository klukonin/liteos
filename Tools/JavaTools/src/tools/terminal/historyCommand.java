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
 * Created by IntelliJ IDEA. User: Qing Cao Date: Sep 1, 2007 Time: 9:02:38 PM
 * To change this template use File | Settings | File Templates.
 */
public class historyCommand {

	String[] history = new String[100];
	static int currentIndex = 0;
	static int lastselect = 0;

	public historyCommand() {

		int i;
		for (i = 0; i < 100; i++)
			history[i] = null;
	}

	public void addCommand(String command) {
		history[currentIndex++] = command.substring(0);
		if (currentIndex == 100)
			currentIndex = 0;
	}

	public void printCommand() {

		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"The previous commands are as follows.");
		int start = (currentIndex - 20 + 100) % 100;
		if (history[start] == null)
			start = 0;
		for (int i = start; i < currentIndex; i++)
			colorOutput.println(colorOutput.COLOR_YELLOW, "" + (i - start)
					+ ": " + history[i]);
		lastselect = start;

	}

	public String getCommand(String input) {
		return history[lastselect + Integer.parseInt(input)];

	}

}
