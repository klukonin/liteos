/*
The following is the license of LiteOS.

This file is part of LiteOS.
Copyright Qing Cao, Hossein Ahmadi 2007-2008, University of Illinois , qcao2@uiuc.edu

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

import java.io.*; // needed for BufferedReader, InputStreamReader, etc.
import java.util.*;
import java.awt.event.*;

import tools.tools.*;

/**
 * @author Qing Cao The terminal class is the entry class of the whole shell
 *         system. Basically it operates the serial port to communicate with the
 *         base station, and allows the user to type in commands to do all sorts
 *         of operations on a sensor network. <p/> This class uses several other
 *         classes, such as the listcommand class, etc, to operate the as
 *         proxies for the user application.
 */

public class terminal {

	private static BufferedReader input_stream = new BufferedReader(
			new InputStreamReader(System.in));

	private static String commandName;
	private static String[] options = new String[32];
	private static String[] parameters = new String[32];

	private static int optioncount = 0;
	private static int parametercount = 0;

	private static fileDirectory fdir;
	private static fileNode cnode;
	private static fileNode root;

	private static byte[][] command = new byte[10240][100];
	private static byte[] response = new byte[200000];
	private static int responsecount;

	private static ServerPort serverpl;
	private static int SHELLPORT = 239;
	private static int MAXBUFFER = 256;

	private static ArrayList responseList;

	// when countCommand = 1, then there is only one command to be sent
	// when countCommand >1, then there are multiple commands to be sent
	// when countCommand = 0, then there is no command needs to be sent over the
	// serial port

	private static int countCommand = 1;

	private static listCommand lshandle = new listCommand();
	private static cdCommand cdhandle = new cdCommand();
	private static mkdirCommand mdhandle = new mkdirCommand();
	private static rmCommand rmhandle = new rmCommand();
	private static duCommand duhandle = new duCommand();
	private static searchCommand searchhandle = new searchCommand();
	private static cpCommand cphandle = new cpCommand();
	private static touchCommand touchhandle = new touchCommand();
	private static installCommand installhandle = new installCommand();
	private static rbbCommand rbbhandle = new rbbCommand();
	private static rbnCommand rbnhandle = new rbnCommand();
	private static psCommand pshandle = new psCommand();
	private static uninstallCommand uninstallhandle = new uninstallCommand();
	private static formatCommand formathandle = new formatCommand();
	private static resetCommand resethandle = new resetCommand();
	private static devCommand devhandle = new devCommand();
	private static manCommand manhandle = new manCommand();
	private static historyCommand historyhandle = new historyCommand();
	private static setCommand sethandle = new setCommand();
	private static memCommand memhandle = new memCommand();
	private static debugCommand debughandle = new debugCommand();
	private static printCommand printhandle = new printCommand();
	private static breakpointCommand breakpointhandle = new breakpointCommand();
	private static continueCommand continuehandle = new continueCommand();
	private static snapshotCommand snapshothandle = new snapshotCommand();
	private static restoreCommand restorehandle = new restoreCommand();
	private static channelSelectCommand channelhandle = new channelSelectCommand();

	private static long prevtime, aftertime;
	private static String input;
	private static int SkipReading = 0;
	private static boolean devCommandRepeat = false;
	private static boolean choosehistorycommand = false;

	/**
	 * Default constructor for the Terminal class.
	 */
	public terminal() {
	}

	/**
	 * This function inits the file directory. Like all commands, the file
	 * directories are also implemented as classes.
	 */

	public static void init() {
		fdir = new fileDirectory();
		cnode = fdir.getRoot();
		fdir.setCurrentNode(cnode);

	}

	public static void printScore() {

		// System.out.print("$ ");
		colorOutput.print(colorOutput.COLOR_GREEN, "$");
	}

	/**
	 * The main function executes from here. This is the entry point of
	 * everything.
	 * 
	 * @param args
	 *            The standard input arguments. Not used now. May be used in the
	 *            future.
	 * @throws IOException
	 *             Throws exception only if the I/O encounters a problem.
	 */
	public static void main(String[] args) throws IOException {

		// nextcommand is used to determine whether another variable should be
		// read from the console
		boolean nextCommand = true;
		String commport;
		String packetsource;
		packetsource = null;

		commport = null;
		if (args.length > 0) {
			int k;
			colorOutput.setColor(false);
			for (k = 0; k < args.length; k++) {
				if (args[k].startsWith("-color") == true)
					colorOutput.setColor(true);
				else if ((args[k].startsWith("-script") == true)
						&& (k < args.length - 1))
					input_stream = new BufferedReader(new FileReader(args[++k]));
				else
					packetsource = args[k];
			}
		}

		serverpl = new ServerPort(packetsource);
		String[] elements = null;

		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
				"Welcome to use LiteOS shell. You might want to set the communication channel first with the setchannel command.");
		
		
		init();

		while (nextCommand == true) {

			optioncount = parametercount = 0;

			// Read a line of text from the user.
			// skipReading is defined as 0 when the program starts

			if (SkipReading == 0) {
				printScore();
				input = input_stream.readLine();
				devCommandRepeat = false;

				/*
				 * if ((choosehistorycommand == true)) {
				 * 
				 * try { Integer.parseInt(input);
				 * 
				 * } catch (NumberFormatException e) { choosehistorycommand =
				 * false; }
				 * 
				 * if (choosehistorycommand == true) {choosehistorycommand =
				 * false; input = historyhandle.getCommand(input); } else {
				 * SkipReading = 0; nextCommand = true; continue; }
				 * 
				 * }
				 */
				historyhandle.addCommand(input);

				prevtime = System.currentTimeMillis();
			} else {
				SkipReading--;

				if (commandName.startsWith("./")) {
					devCommandRepeat = true;

				} else
					devCommandRepeat = false;

			}

			// Now Process this input. An input has been recorded.
			// Jump out of the loop for executing commands and return

			if (input.compareTo("quit") == 0) {
				nextCommand = false;
				continue;
			}

			// Analyze the elements.

			elements = input.split(" ");

			// set the command Name
			commandName = elements[0];

			// set up the elements and parameters using the first charater.
			for (int i = 1; i < elements.length; i++) {
				if (elements[i].startsWith("-") == true) {
					options[optioncount++] = elements[i];
				} else
					parameters[parametercount++] = elements[i];
			}

			if (genericCommand.commandValidCheck(commandName, options,
					optioncount, parameters, parametercount, fdir) == false) {
				colorOutput.println(colorOutput.COLOR_YELLOW,
						"Bad command or syntax\n");
				nextCommand = true;
				SkipReading = 0;
				continue;
			}

			if (commandName.compareTo("chmod") == 0) {
				colorOutput.println(colorOutput.COLOR_YELLOW,
						"Chmod command will be supported soon");
				nextCommand = true;
				SkipReading = 0;
				continue;
			}

			if (commandName.compareTo("history") == 0) {
				historyhandle.printCommand();
				choosehistorycommand = false;
				nextCommand = true;
				SkipReading = 0;
				System.out.println("\n");
				continue;
			}

			if (commandName.compareTo("who") == 0) {
				colorOutput.println(colorOutput.COLOR_GREEN, "Administrator\n");
				nextCommand = true;
				SkipReading = 0;
				continue;
			}

			if (commandName.compareTo("foreach") == 0) {
				colorOutput
						.println(colorOutput.COLOR_YELLOW,
								"Sorry, Foreach command is not supported in the demo version.");
				nextCommand = true;
				SkipReading = 0;
				continue;
			}

			if (commandName.compareTo("echo") == 0) {
				colorOutput.println(colorOutput.COLOR_GREEN, parameters[0]
						+ "\n");
				nextCommand = true;
				SkipReading = 0;
				continue;
			}
			if (commandName.compareTo("login") == 0) {
				colorOutput
						.println(colorOutput.COLOR_YELLOW,
								"Sorry, Security commands are not supported in the demo version.");
				nextCommand = true;
				SkipReading = 0;
				continue;
			}
			if (commandName.compareTo("logout") == 0) {
				colorOutput
						.println(colorOutput.COLOR_YELLOW,
								"Sorry, Security commands are not supported in the demo version.");
				nextCommand = true;
				SkipReading = 0;
				continue;
			}

			if (commandName.compareTo("passwd") == 0) {
				colorOutput
						.println(colorOutput.COLOR_YELLOW,
								"Sorry, Security commands are not supported in the demo version.");
				nextCommand = true;
				SkipReading = 0;
				continue;
			}

			if ((commandName.compareTo("quit") == 0)
					|| (commandName.compareTo("exit") == 0)) {
				colorOutput.println(colorOutput.COLOR_YELLOW,
						"Thank you for using LiteShell. Bye!");
				System.exit(0);
			}

			if (commandName.compareTo("debug") == 0) {
				debughandle.setNewCommand(options, optioncount, parameters,
						parametercount, fdir);
				colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
						"Reading debugging information successful.\n");
				nextCommand = true;
				SkipReading = 0;
				continue;
			}

			if (commandName.compareTo("list") == 0) {
				if (debughandle.isSetUp() == false)
					colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
							"Debug environment has not been set up.");
				else {
					debughandle.printAll();
					colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
							"Debug environment set up complete.\n");
				}
				nextCommand = true;
				SkipReading = 0;
				continue;
			}
			if (commandName.compareTo("setchannel") == 0) {
				countCommand = channelhandle.setNewCommand(options,
						optioncount, parameters, parametercount, fdir);
				if (countCommand > 0) {
					// pl.setWait(rbnhandle.getDelay());

					serverpl.setPacketWaitTimeout(SHELLPORT, 100);

					for (int i = 0; i < countCommand; i++) {
						byte[] temp = channelhandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}

			}

			if (commandName.compareTo("listbreakpoint") == 0) {
				breakpointhandle.printBreakpoints();
				nextCommand = true;
				SkipReading = 0;
				continue;
			}

			// From now on, the generic pattern is that the command is got from
			// the respective classes, and the delay
			// is got, and the new commands are copied and sent out

			// THe following segments use both delay of the command as well as
			// its length of data received as the
			// key component of judging whether it should return after finishing
			// the receiving job.

			// Now if the command is ls

			if (commandName.compareTo("ls") == 0) {

				countCommand = lshandle.setNewCommand(options, optioncount,
						parameters, parametercount, fdir);

				// set up how many commands are used and the timeout of this
				// command

				if (countCommand > 0) {
					int temp2;
					temp2 = lshandle.getDelay();

					// pl.setWait(temp2);

					serverpl.setPacketWaitTimeout(SHELLPORT, temp2);
					serverpl.setPacketWaitBuffer(SHELLPORT, MAXBUFFER);

					for (int i = 0; i < countCommand; i++) {
						byte[] temp = lshandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}
			}

			// now if the command is cd. the returned number of commands must be
			// 0.

			if (commandName.compareTo("cd") == 0)

			{
				countCommand = cdhandle.setNewCommand(options, optioncount,
						parameters, parametercount, fdir);
				// assert(countCommand == 0);
			}

			// now if it is cp command

			if (commandName.compareTo("cp") == 0) {
				// pl.setThreshold(32);
				serverpl.setPacketWaitBuffer(SHELLPORT, 1);
				serverpl.setPacketWaitTimeout(SHELLPORT, 500);

				countCommand = cphandle.setNewCommand(options, optioncount,
						parameters, parametercount, fdir);
				if (countCommand == -1)

				{
					System.out.println("File does not exist.\n");
					nextCommand = true;
					SkipReading = 0;
					continue;

				}

				if (countCommand == -2)

				{
					System.out.println("File target already exists.\n");
					nextCommand = true;
					SkipReading = 0;
					continue;

				}
				if (countCommand > 0) {

					// pl.setWait()

					for (int i = 0; i < countCommand; i++) {
						byte[] temp = cphandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}

				// System.out.println("Now the countcommand is "+countCommand);
			}

			// now if it is mv command

			if (commandName.compareTo("mv") == 0) {
				// System.out.println(
				// "The move command has not been tested. Use cp and rm instead."
				// );
				colorOutput
						.println(colorOutput.COLOR_BRIGHT_RED,
								"The move command has not been tested. Use cp and rm instead.");
			}

			// now if it is mkdir command

			if (commandName.compareTo("mkdir") == 0)

			{

				// pl.setThreshold(32);
				serverpl.setPacketWaitBuffer(SHELLPORT, 1);
				countCommand = mdhandle.setNewCommand(options, optioncount,
						parameters, parametercount, fdir);

				if (countCommand > 0) {
					// pl.setWait(mdhandle.getDelay());
					serverpl.setPacketWaitTimeout(SHELLPORT, mdhandle
							.getDelay());
					for (int i = 0; i < countCommand; i++) {
						byte[] temp = mdhandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}
			}

			if (commandName.compareTo("format") == 0)

			{

				// pl.setThreshold(32);
				serverpl.setPacketWaitBuffer(SHELLPORT, 1);
				countCommand = formathandle.setNewCommand(options, optioncount,
						parameters, parametercount, fdir);
				if (countCommand > 0) {
					// pl.setWait(formathandle.getDelay());
					serverpl.setPacketWaitTimeout(SHELLPORT, formathandle
							.getDelay());
					for (int i = 0; i < countCommand; i++) {
						byte[] temp = formathandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}
			}

			// now if it is touch command

			if (commandName.compareTo("touch") == 0) {

				// pl.setThreshold(32);
				serverpl.setPacketWaitBuffer(SHELLPORT, 1);

				countCommand = touchhandle.setNewCommand(options, optioncount,
						parameters, parametercount, fdir);

				if (countCommand > 0) {
					// pl.setWait(touchhandle.getDelay());
					serverpl.setPacketWaitTimeout(SHELLPORT, touchhandle
							.getDelay());

					for (int i = 0; i < countCommand; i++) {
						byte[] temp = touchhandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}
			}

			// now if it is rm command

			if (commandName.compareTo("rm") == 0) {
				// pl.setThreshold(32);
				serverpl.setPacketWaitBuffer(SHELLPORT, 1);

				countCommand = rmhandle.setNewCommand(options, optioncount,
						parameters, parametercount, fdir);
				if (countCommand > 0) {
					// pl.setWait(rmhandle.getDelay());
					serverpl.setPacketWaitTimeout(SHELLPORT, rmhandle
							.getDelay());

					for (int i = 0; i < countCommand; i++) {
						byte[] temp = rmhandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}
			}

			if ((commandName.compareTo("uninstall") == 0)
					|| (commandName.compareTo("kill") == 0)) {

				// pl.setThreshold(32);
				serverpl.setPacketWaitBuffer(SHELLPORT, 1);

				countCommand = uninstallhandle.setNewCommand(options,
						optioncount, parameters, parametercount, fdir);
				if (countCommand > 0) {
					// pl.setWait(uninstallhandle.getDelay());
					serverpl.setPacketWaitTimeout(SHELLPORT, uninstallhandle
							.getDelay());
					for (int i = 0; i < countCommand; i++) {
						byte[] temp = uninstallhandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}
			}

			if (commandName.compareTo("pwd") == 0) {
				countCommand = 0;

				fileNode[] temp = new fileNode[24];
				int tempcount = 0;
				fileNode current = fdir.getCurrentNode();
				while (current.getName().compareTo("root") != 0) {
					temp[tempcount++] = current;
					current = current.getParent();
				}

				for (int i = tempcount - 1; i >= 0; i--) {
					System.out.print("/");
					System.out.print(temp[i].getName());
				}

				System.out.println();
			}

			// du command
			if (commandName.compareTo("du") == 0) {
				// pl.setThreshold(32);
				serverpl.setPacketWaitBuffer(SHELLPORT, 1);
				countCommand = duhandle.setNewCommand(options, optioncount,
						parameters, parametercount, fdir);

				if (countCommand > 0) {
					// pl.setWait(duhandle.getDelay());
					serverpl.setPacketWaitTimeout(SHELLPORT, duhandle
							.getDelay());
					for (int i = 0; i < countCommand; i++) {
						byte[] temp = duhandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}

			}

			if (commandName.compareTo("resetnetwork") == 0) {
				serverpl.setPacketWaitBuffer(SHELLPORT, 1);
				countCommand = resethandle.setNewCommand(options, optioncount,
						parameters, parametercount, fdir);

				if (countCommand > 0) {
					// pl.setWait(resethandle.getDelay());
					serverpl.setPacketWaitTimeout(SHELLPORT, resethandle
							.getDelay());
					for (int i = 0; i < countCommand; i++) {
						byte[] temp = resethandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}

			}
			// rbb command
			if (commandName.compareTo("rbb") == 0) {
				// pl.setThreshold(32);
				serverpl.setPacketWaitBuffer(SHELLPORT, 1);

				countCommand = rbbhandle.setNewCommand(options, optioncount,
						parameters, parametercount, fdir);

				if (countCommand > 0) {
					// pl.setWait(rbbhandle.getDelay());
					serverpl.setPacketWaitTimeout(SHELLPORT, rbbhandle
							.getDelay());

					for (int i = 0; i < countCommand; i++) {
						byte[] temp = rbbhandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}
			}

			// ps command
			if (commandName.compareTo("ps") == 0) {
				// pl.setThreshold(32);
				serverpl.setPacketWaitBuffer(SHELLPORT, 100);

				countCommand = pshandle.setNewCommand(options, optioncount,
						parameters, parametercount, fdir);

				if (countCommand > 0) {
					// pl.setWait(pshandle.getDelay());
					serverpl.setPacketWaitTimeout(SHELLPORT, pshandle
							.getDelay());

					for (int i = 0; i < countCommand; i++) {
						byte[] temp = pshandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}
			}

			// rbn command
			if (commandName.compareTo("rbn") == 0) {
				// pl.setThreshold(32);
				serverpl.setPacketWaitBuffer(SHELLPORT, 1);

				countCommand = rbnhandle.setNewCommand(options, optioncount,
						parameters, parametercount, fdir);

				if (countCommand > 0) {
					// pl.setWait(rbnhandle.getDelay());

					serverpl.setPacketWaitBuffer(SHELLPORT, rbnhandle
							.getDelay());

					for (int i = 0; i < countCommand; i++) {
						byte[] temp = rbnhandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}
			}

			// print command
			if (commandName.compareTo("print") == 0) {
				// pl.setThreshold(32);
				// serverpl.setPacketWaitBuffer(SHELLPORT, 1);
				serverpl
						.setPacketWaitTimeout(SHELLPORT, printhandle.getDelay());

				countCommand = printhandle.setNewCommand(options, optioncount,
						parameters, parametercount, fdir, debughandle);

				if (countCommand > 0) {
					// pl.setWait(printhandle.getDelay());
					// serverpl.setPacketWaitTimeout(SHELLPORT,
					// printhandle.getDelay());
					for (int i = 0; i < countCommand; i++) {
						byte[] temp = printhandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}
			}

			// print command
			if (commandName.compareTo("set") == 0) {
				// pl.setThreshold(32);
				serverpl.setPacketWaitBuffer(SHELLPORT, 1);

				countCommand = sethandle.setNewCommand(options, optioncount,
						parameters, parametercount, fdir, debughandle);

				if (countCommand > 0) {
					// pl.setWait(printhandle.getDelay());
					serverpl.setPacketWaitTimeout(SHELLPORT, sethandle
							.getDelay());
					for (int i = 0; i < countCommand; i++) {
						byte[] temp = sethandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}
			}

			// print command
			if (commandName.compareTo("breakpoint") == 0) {
				// pl.setThreshold(32);

				serverpl.setPacketWaitBuffer(SHELLPORT, 3);
				serverpl.setPacketWaitTimeout(SHELLPORT, breakpointhandle
						.getDelay());
				countCommand = breakpointhandle.setNewCommand(options,
						optioncount, parameters, parametercount, fdir);

				if (countCommand > 0) {
					// pl.setWait(breakpointhandle.getDelay());
					serverpl.setPacketWaitTimeout(SHELLPORT, breakpointhandle
							.getDelay());
					for (int i = 0; i < countCommand; i++) {
						byte[] temp = breakpointhandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}
			}

			if (commandName.compareTo("continue") == 0) {
				// pl.setThreshold(32);

				serverpl.setPacketWaitBuffer(SHELLPORT, 1);
				countCommand = continuehandle.setNewCommand(options,
						optioncount, parameters, parametercount, fdir,
						breakpointhandle, debughandle);

				if (countCommand > 0) {
					serverpl.setPacketWaitTimeout(SHELLPORT, continuehandle
							.getDelay());
					// pl.setWait(continuehandle.getDelay());
					for (int i = 0; i < countCommand; i++) {
						byte[] temp = continuehandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}
			}

			if (commandName.compareTo("snapshot") == 0) {
				// pl.setThreshold(32);
				serverpl.setPacketWaitBuffer(SHELLPORT, 1);
				countCommand = snapshothandle.setNewCommand(options,
						optioncount, parameters, parametercount, fdir);

				if (countCommand > 0) {
					// pl.setWait(snapshothandle.getDelay());
					serverpl.setPacketWaitTimeout(SHELLPORT, snapshothandle
							.getDelay());
					for (int i = 0; i < countCommand; i++) {
						byte[] temp = snapshothandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}
			}

			if (commandName.compareTo("restore") == 0) {
				// pl.setThreshold(32);
				serverpl.setPacketWaitBuffer(SHELLPORT, 1);
				countCommand = restorehandle.setNewCommand(options,
						optioncount, parameters, parametercount, fdir);

				if (countCommand > 0) {
					// pl.setWait(restorehandle.getDelay());
					serverpl.setPacketWaitTimeout(SHELLPORT, snapshothandle
							.getDelay());
					for (int i = 0; i < countCommand; i++) {
						byte[] temp = restorehandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}
			}

			// find command or search command

			if ((commandName.compareTo("find") == 0)
					|| (commandName.compareTo("search") == 0)) {
				countCommand = searchhandle.setNewCommand(options, optioncount,
						parameters, parametercount, fdir);
				if (countCommand > 0) {
					// pl.setWait(searchhandle.getDelay());
					serverpl.setPacketWaitTimeout(SHELLPORT, searchhandle
							.getDelay());
					for (int i = 0; i < countCommand; i++) {
						byte[] temp = searchhandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}

			}

			// install command

			if ((commandName.compareTo("install") == 0)
					|| (commandName.compareTo("exec") == 0)) {

				countCommand = installhandle.setNewCommand(options,
						optioncount, parameters, parametercount, fdir);

				if (countCommand > 0) {
					// pl.setWait(installhandle.getDelay());
					serverpl.setPacketWaitTimeout(SHELLPORT, installhandle
							.getDelay());

					for (int i = 0; i < countCommand; i++) {
						byte[] temp = installhandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}

			}
			// install command

			if ((commandName.compareTo("man") == 0)) {
				countCommand = 0;
				if (parametercount == 0) {
					colorOutput.println(colorOutput.COLOR_YELLOW,
							"Syntax error for man command");
					nextCommand = true;
					SkipReading = 0;
					continue;

				}
				manhandle.setNewCommand(options, optioncount, parameters,
						parametercount, fdir);

			}

			// print command
			if (commandName.compareTo("memory") ==0 ||commandName.compareTo("coredump") == 0) {

				serverpl.setPacketWaitTimeout(SHELLPORT, memhandle.getDelay());
				serverpl.setPacketWaitBuffer(SHELLPORT, 100);

				countCommand = memhandle.setNewCommand(options, optioncount,
						parameters, parametercount, fdir);

				if (countCommand > 0) {
					// pl.setWait(printhandle.getDelay());
					// serverpl.setPacketWaitTimeout(SHELLPORT,
					// printhandle.getDelay());
					for (int i = 0; i < countCommand; i++) {
						byte[] temp = memhandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}
			}

			if (commandName.startsWith("./")) {
				int i;

				optioncount = 0;
				options[0] = new String(commandName.getBytes(), 2, commandName
						.length() - 2);
				countCommand = devhandle.setNewCommand(options, optioncount,
						parameters, parametercount, fdir);
				// pl.setThreshold(devhandle.getThreshold());
				// pl.setThreshold(32);
				serverpl.setPacketWaitBuffer(SHELLPORT, 1);
				// serverpl.setPacketWaitTimeout(SHELLPORT,
				// devhandle.getThreshold());

				if (countCommand > 0) {
					// pl.setWait(devhandle.getDelay());
					serverpl.setPacketWaitTimeout(SHELLPORT, devhandle
							.getDelay());

					for (i = 0; i < countCommand; i++) {
						byte[] temp = devhandle.getNewCommand(i);
						int templength = temp.length;
						System.arraycopy(temp, 0, command[i], 0, templength);
					}
				}

			}

			// uninstall command remains to do .

			// Key part: send out the command and wait for reply from the user

			// Now get the command, send it, wait for the response

			if (countCommand == 1) {
				// System.out.println("Now first round is countcommand 1");

				// if this is for the device command, should not write anything.
				if (devCommandRepeat == false)
					// pl.write(command[0]);
					// serverpl.setPacketWaitTimeout(SHELLPORT, NORMALTIME);
					// serverpl.setPacketWaitBuffer(SHELLPORT, 40);

					serverpl.sendProtocolPacket(command[0]);

				// pl.read();
				responseList = serverpl.readProtocolPacket(SHELLPORT);
				int k;
				k = 10;
				// responsecount = pl.getCount();
				// response = pl.getData();
			}

			else if (countCommand > 0) {

				// THIS IS FOR TYPE I OR III

				if ((commandName.compareTo("cp") == 0)) {

					int NORMALTIME = 200;
					// pl.setWait(NORMALTIME);
					serverpl.setPacketWaitTimeout(SHELLPORT, NORMALTIME);
					serverpl.setPacketWaitBuffer(SHELLPORT, 60);
					int PACKETBLOCK = 30;
					int i;

					// System.out.println(
					// "Now count command is larger than 0 with the second round must!"
					// );
					// send N packets and SYNC, and wait to see if ack comes
					// if no ack, then retransmit the N packets. Otherwise,
					// if NACK comes, adjust and retransmit another bulk of
					// packets
					// if successful, then continue

					int currentIndex, nextIndex;
					currentIndex = 0;
					int roundofTry;
					roundofTry = 0;
					int MAXROUNDOFTRY = 7;
					int tempreply;

					// colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
					// "the count command is "+countCommand);

					while (true) {

						if (currentIndex >= countCommand - 1) {
							roundofTry = 0;
							// System.out.println("currentIndex is " +
							// currentIndex + " larger than command num, exit");
							// colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
							// "currentIndex is " + currentIndex +
							// " larger than command num, exit");

							// sending done
							// send finalize packet
							serverpl.setPacketWaitTimeout(SHELLPORT, 200);
							serverpl.setPacketWaitBuffer(SHELLPORT, 1);
							serverpl
									.sendProtocolPacket(cphandle.getEndPacket());

							responseList = serverpl
									.readProtocolPacket(SHELLPORT);
							serverpl
									.setPacketWaitTimeout(SHELLPORT, NORMALTIME);
							break;
						}

						if (currentIndex + PACKETBLOCK <= countCommand - 1)
							nextIndex = currentIndex + PACKETBLOCK;
						else
							nextIndex = countCommand - 1;

						if (roundofTry == MAXROUNDOFTRY - 2) {
							// should break here because the retry has expired

							if (roundofTry >= MAXROUNDOFTRY - 2)
								break;
							if (roundofTry == MAXROUNDOFTRY - 1) {
							} else {
								// System.out.println("round of retry is " +
								// roundofTry + " try to system reboot ");
								colorOutput
										.println(
												colorOutput.COLOR_BRIGHT_RED,
												"Logical error of the copy command. Please report this problem to the developer list.");
								System.exit(0);
								// this part should not happen!

								rbnhandle.setNewCommand(options, optioncount,
										parameters, parametercount, fdir);
								// pl.write(rbnhandle.getNewCommand(0));
								// pl.setWait(500);

								roundofTry++;
								continue;
							}

							// break;

						}

						colorOutput.println(colorOutput.COLOR_YELLOW,
								"Now trying to send " + currentIndex + " "
										+ nextIndex);

						for (i = currentIndex; i < nextIndex; i++) {
							// System.out.println("Now trying to send " +
							// currentIndex + " " + nextIndex);
							serverpl.sendProtocolPacket(command[i]);
							serverpl.readProtocolPacket(SHELLPORT);
							// pl.write(command[i]);
							// pl.read();
						}

						// removed so that at the end of the delivery there
						// should be a sync
						// if (nextIndex == countCommand)
						// break;

						// System.out.println("Now trying to send sync");
						colorOutput.println(colorOutput.COLOR_YELLOW,
								"Now trying to send sync");
						// pl.write(cphandle.getSync(nextIndex));
						serverpl.setPacketWaitTimeout(SHELLPORT, 200);
						serverpl.setPacketWaitBuffer(SHELLPORT, 1);
						serverpl
								.sendProtocolPacket(cphandle.getSync(nextIndex));

						responseList = serverpl.readProtocolPacket(SHELLPORT);
						serverpl.setPacketWaitTimeout(SHELLPORT, NORMALTIME);
						// pl.setThreshold(32);
						// pl.read();
						// pl.setWait(NORMALTIME);

						// responsecount = pl.getCount();
						// response = pl.getData();

						if (responseList.size() == 0) {
							// System.out.println("No sync reply");
							colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
									"No sync reply");
							roundofTry++;
							// pl.write(rbbhandle.getNewCommand(0));
							// pl.read();
							continue;
						} else {

							responsecount = responseList.size();
							response = (byte[]) responseList.remove(0);

							if ((tempreply = cphandle.checkreply(responsecount,
									response)) == 0) {
								//System.out.println("Now reply is good on sync"
								// );

								colorOutput.println(colorOutput.COLOR_YELLOW,
										"Now reply is good on sync");
								currentIndex = nextIndex;
								roundofTry = 0;
							} else if (tempreply == -1) {
								// System.out.println("Wrong sync reply");
								colorOutput.println(
										colorOutput.COLOR_BRIGHT_RED,
										"Wrong sync reply");
								roundofTry++;
								// pl.write(rbbhandle.getNewCommand(0));
								// pl.read();
								continue;

							} else {
								//System.out.println("Sync reply shows has got "
								// + tempreply);

								colorOutput
										.println(colorOutput.COLOR_YELLOW,
												"Sync reply shows has got "
														+ tempreply);
								// pl.write(rbbhandle.getNewCommand(0));
								// pl.read();
								if (tempreply > currentIndex)
									currentIndex = tempreply;
								else {
									currentIndex = tempreply;
									roundofTry++;
								}

							}
						}

					}

					if (roundofTry > 0) {
						// System.out.println(
						// "cp failed because of lack of back signal");
						colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
								"cp failed because of lack of back signal");
					} else {
						// System.out.println("cp succeed");
						colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,
								"cp succeed");

						// pl.write(command[countCommand - 1]);
						// pl.read();
						// pl.write(rbbhandle.getNewCommand(0));
						// pl.read();
					}

				}

			} else {

				if ((commandName.compareTo("cp") == 0)) {

					SkipReading = 0;
					// pl.setThreshold(64);
					// pl.setWait(900);

					serverpl.setPacketWaitBuffer(SHELLPORT, 2);
					// speed optimization for cp
					// /Tuning place change to 100 as an tuning
					serverpl.setPacketWaitTimeout(SHELLPORT, 100);
					// System.out.println("Now prepare to get incoming data");
					// System.out.println("Reading port");

					while (true) {
						// pl.read();
						// responsecount = pl.getCount();
						// response = pl.getData();

						responseList = serverpl.readProtocolPacket(SHELLPORT);

						cphandle.getIncomingData(responseList);

						System.out.println("Reading port 2 packets");
						if (cphandle.terminateSignal() == true)
							break;
					}

					// pl.setThreshold(32);
					// pl.setWait(200);

					serverpl.setPacketWaitBuffer(SHELLPORT, 1);
					// speed optimization for cp
					// /Tuning place change to 100 as an tuning
					serverpl.setPacketWaitTimeout(SHELLPORT, 100);

					// System.out.println("Now Patching");
					colorOutput.println(colorOutput.COLOR_YELLOW,
							"Now Patching");

					while (cphandle.morepatchingdata()) {
						colorOutput.println(colorOutput.COLOR_YELLOW,
								"Now another patch request");
						byte[] tosend;
						// pl.write(cphandle.getpatchcommand(fdir));
						// pl.read();
						// responsecount = pl.getCount();
						// response = pl.getData();
						tosend = cphandle.getpatchcommand(fdir);

						if (tosend.length < 10) {
							colorOutput.println(colorOutput.COLOR_YELLOW,
									"Patching error. Now try again. ");
							continue;
						}
						serverpl.sendProtocolPacket(cphandle
								.getpatchcommand(fdir));
						responseList = serverpl.readProtocolPacket(SHELLPORT);

						cphandle.handlePatch(responseList);

					}

					// responsecount = cphandle.patchresponsecount();
					responseList = cphandle.patchedresponse();
				}

			}

			// if countCommand >0, the results must have been received and
			// should be analyzed to see the results

			if ((countCommand > 0) || (devCommandRepeat == true))

			{

				if (commandName.compareTo("ls") == 0) {

					if (responseList.size() == 0)
						colorOutput.println(colorOutput.COLOR_RED,
								"There is no info packet received.");
					else
						lshandle.handleresponse(options, optioncount,
								parameters, parametercount, responseList, fdir);
				}

				if (commandName.compareTo("cd") == 0) {
					// do nothing
				}

				if (commandName.compareTo("cp") == 0) {

					cphandle.handleresponse(options, optioncount, parameters,
							parametercount, responseList, fdir);
					SkipReading = cphandle.skipReading();
					// colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
					// "this time is "+SkipReading);

				}

				if (commandName.startsWith("./") == true) {

					devhandle.handleresponse(options, optioncount, parameters,
							parametercount, responseList, fdir);
					SkipReading = devhandle.getSkipReading();

				}

				if (commandName.compareTo("mv") == 0) {

				}

				if (commandName.compareTo("mkdir") == 0) {
					mdhandle.handleresponse(options, optioncount, parameters,
							parametercount, responseList, fdir);
				}

				if (commandName.compareTo("ps") == 0) {

					// if (responsecount==0)
					// colorOutput.println(colorOutput.COLOR_RED,
					// "There is no info packet received.");
					// else
					pshandle.handleresponse(options, optioncount, parameters,
							parametercount, responseList, fdir);
				}

				if (commandName.compareTo("memory") == 0) {
					memhandle.handleresponse(options, optioncount, parameters,
							parametercount, responseList, fdir);
				}

				if (commandName.compareTo("touch") == 0) {
					// touchhandle.handleresponse(options, optioncount,
					// parameters, parametercount, responsecount, response,
					// fdir);
					touchhandle.handleresponse(options, optioncount,
							parameters, parametercount, responseList, fdir);
				}

				if (commandName.compareTo("rm") == 0) {
					rmhandle.handleresponse(options, optioncount, parameters,
							parametercount, responseList, fdir);
				}

				if (commandName.compareTo("du") == 0) {
					// if (responsecount==0)
					// colorOutput.println(colorOutput.COLOR_RED,
					// "There is no info packet received.");
					duhandle.handleresponse(options, optioncount, parameters,
							parametercount, responseList, fdir);
				}
				if ((commandName.compareTo("search") == 0)
						|| (commandName.compareTo("find") == 0)) {
					searchhandle.handleresponse(options, optioncount,
							parameters, parametercount, responseList, fdir);
				}

				if (commandName.compareTo("pwd") == 0) {

				}
				if (((commandName.compareTo("install") == 0))
						|| (commandName.compareTo("exec") == 0)) {
					installhandle.handleresponse(options, optioncount,
							parameters, parametercount, responseList, fdir);
				}

				if (((commandName.compareTo("uninstall") == 0))
						|| (commandName.compareTo("kill") == 0)) {
					uninstallhandle.handleresponse(options, optioncount,
							parameters, parametercount, responseList, fdir);
				}

				if ((commandName.compareTo("rbb") == 0)) {
					rbbhandle.handleresponse(options, optioncount, parameters,
							parametercount, responseList, fdir);
				}

				if ((commandName.compareTo("rbn") == 0)) {
					rbnhandle.handleresponse(options, optioncount, parameters,
							parametercount, responseList, fdir);
				}

				if ((commandName.compareTo("print") == 0)) {
					printhandle.handleresponse(options, optioncount,
							parameters, parametercount, responseList, fdir,
							debughandle);
				}

				if ((commandName.compareTo("breakpoint") == 0)) {
					breakpointhandle.handleresponse(options, optioncount,
							parameters, parametercount, responseList, fdir);
				}

				if (commandName.compareTo("continue") == 0) {
					continuehandle.handleresponse(options, optioncount,
							parameters, parametercount, responseList, fdir);
				}
			} else if (countCommand == 0) {

				if (commandName.compareTo("cp") == 0) {

					cphandle.handleresponse(options, optioncount, parameters,
							parametercount, responseList, fdir);
					SkipReading = cphandle.skipReading();
				}

				if (commandName.compareTo("snapshot") == 0) {

					snapshothandle.handleresponse(options, optioncount,
							parameters, parametercount, responseList, fdir);
				}

				if (commandName.compareTo("restore") == 0) {

					restorehandle.handleresponse(options, optioncount,
							parameters, parametercount, responseList, fdir);
				}

			}
			if (SkipReading == 0) {
				aftertime = System.currentTimeMillis();
				aftertime = aftertime - prevtime;
				prevtime = 0;
				// System.out.println("Time elapes " + aftertime + " \n");
				colorOutput.println(colorOutput.COLOR_YELLOW, "Time elapes "
						+ aftertime + " \n");
			}

		}

		// System.out.println("shell exit");
		colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "Time elapes "
				+ aftertime + " \n");
	}
}
