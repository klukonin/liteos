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
 * The generic interface for command control implemnted by LiteOS shell.
 */
public interface cmdcontrol {

	/**
	 * Set up a new command for a particular command class
	 * 
	 * @param options
	 *            the list of options using a String[] array
	 * @param optioncount
	 *            the number of options stored in the options array
	 * @param parameters
	 *            the list of parameters using a parameters String[] array
	 * @param parametercount
	 *            the number of parameters
	 * @param fdir
	 *            the current directory structure for update and cache
	 * @return return the number of commands to be sent over the radio. if 0,
	 *         then no message is needed.
	 */

	int setNewCommand(String[] options, int optioncount, String[] parameters,
			int parametercount, fileDirectory fdir);

	/**
	 * Get the delay for this command
	 * 
	 * @return the required delay in milliseconds
	 */
	int getDelay();

	/**
	 * Get a particular command. *
	 * 
	 * @param index
	 *            of this command list
	 * @return the command in byte array
	 */
	byte[] getNewCommand(int index);

	/**
	 * Handle the response using this particular class for a command.
	 * 
	 * @param options
	 *            the original options
	 * @param optioncount
	 *            the original number of options
	 * @param parameters
	 *            the original parameters
	 * @param parametercount
	 *            the original number of parameters
	 * @param reply
	 *            the reply array data structure
	 * @param fdir
	 *            the current directory
	 */
	void handleresponse(String[] options, int optioncount, String[] parameters,
			int parametercount, ArrayList reply, fileDirectory fdir);

}
