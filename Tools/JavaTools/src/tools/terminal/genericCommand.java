package tools.terminal;

import java.io.File;

/**
 * Created by IntelliJ IDEA. User: Qing Cao Date: 2008-2-28 Time: 0:03:06 To
 * change this template use File | Settings | File Templates.
 */
public class genericCommand {
	// check if the command is valid. This is a crosscutting issue
	static boolean commandValidCheck(String commandName, String[] options,
			int optioncount, String[] parameters, int parametercount,
			fileDirectory fdir) {
		if (commandName.compareTo("chmod") == 0) {
			if ((optioncount == 0) && (parametercount == 0))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("history") == 0) {
			if ((optioncount == 0) && (parametercount == 0))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("who") == 0) {
			if ((optioncount == 0) && (parametercount == 0))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("foreach") == 0) {
			return true;
		}

		if (commandName.compareTo("echo") == 0) {
			if ((optioncount == 0) && (parametercount == 1))
				return true;
			else
				return false;
		}
		if (commandName.compareTo("login") == 0) {
			return true;
		}

		if (commandName.compareTo("logout") == 0) {
			return true;
		}

		if (commandName.compareTo("passwd") == 0) {
			return true;
		}

		if (commandName.compareTo("quit") == 0) {
			return true;
		}

		if (commandName.compareTo("exit") == 0) {
			return true;
		}

		if (commandName.compareTo("debug") == 0) {
			if (!((optioncount == 0) && (parametercount == 1)))
				return false;
			String dirname = parameters[0];
			if (dirname.startsWith("/") != true)
				colorOutput
						.println(
								colorOutput.COLOR_YELLOW,
								"The directory format is error. Must starting with /. For example, /c/Source/App");
			dirname = cpCommand.stdFileName(dirname);

			File a = new File(dirname + "\\default\\extract.pl");
			File b = new File(dirname + "\\default\\LiteOS.elf");
			if ((a.exists() == false) || (b.exists() == false)) {
				a = new File(dirname + "\\bin\\micaz\\extract.pl");
				b = new File(dirname + "\\bin\\micaz\\LiteOS.elf");
				if ((a.exists() == false) || (b.exists() == false)) {
					colorOutput
							.println(colorOutput.COLOR_BRIGHT_RED,
									"Directory error. Specified debugging files missing. ");
					return false;
				} else
					return true;
			} else
				return true;
		}

		if (commandName.compareTo("list") == 0) {
			if ((optioncount == 0) && (parametercount == 0))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("setchannel") == 0) {
			if ((optioncount == 0) && (parametercount == 1)) {
				String REAL_NUMBER = "^\\d+$";
				if (parameters[0].matches(REAL_NUMBER) == true)
					return true;
				else {
					colorOutput.println(colorOutput.COLOR_BRIGHT_RED,
							"The parameter must be an integer.");
					return false;
				}
			} else
				return false;
		}

		if (commandName.compareTo("listbreakpoint") == 0) {
			if ((optioncount == 0) && (parametercount == 0))
				return true;
			else
				return false;
		}
		if (commandName.compareTo("ls") == 0) {
			if ((parametercount == 0))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("cd") == 0) {
			if ((optioncount == 0) && (parametercount == 1))
				return true;
			else
				return false;
		}
		if (commandName.compareTo("cp") == 0) {
			if ((optioncount == 0) && (parametercount == 2))
				return true;
			else
				return false;
		}
		if (commandName.compareTo("mv") == 0) {
			if ((optioncount == 0) && (parametercount == 2))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("mkdir") == 0) {
			if ((optioncount == 0) && (parametercount == 1)) {
				if ((fdir.getCurrentNode().getType().compareTo("network") == 0)
						|| (fdir.getCurrentNode().getType().compareTo("root") == 0)) {
					return false;
				} else {
					return true;
				}
			} else
				return false;
		}

		if (commandName.compareTo("format") == 0) {
			if ((optioncount == 0) && (parametercount == 0))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("touch") == 0) {
			if ((optioncount == 0) && (parametercount == 1)) {
				if ((fdir.getCurrentNode().getType().compareTo("network") == 0)
						|| (fdir.getCurrentNode().getType().compareTo("root") == 0)) {
					return false;
				} else {
					return true;
				}
			} else
				return false;
		}
		if (commandName.compareTo("rm") == 0) {
			if ((optioncount == 0) && (parametercount == 1))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("uninstall") == 0) {
			if ((optioncount == 0) && (parametercount == 1))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("kill") == 0) {
			if ((optioncount == 0) && (parametercount == 1))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("pwd") == 0) {
			if ((optioncount == 0) && (parametercount == 0))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("du") == 0) {
			if ((optioncount == 0) && (parametercount == 0))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("resetnetwork") == 0) {
			if ((optioncount == 0) && (parametercount == 0))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("rbb") == 0) {
			if ((optioncount == 0) && (parametercount == 0))
				return true;
			else
				return false;
		}
		if (commandName.compareTo("rbn") == 0) {
			if ((optioncount == 0) && (parametercount == 0))
				return true;
			else
				return false;
		}
		if (commandName.compareTo("ps") == 0) {
			if ((optioncount == 0) && (parametercount == 0))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("print") == 0) {
			if ((optioncount == 0) && (parametercount == 1))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("set") == 0) {
			if ((optioncount == 0) && (parametercount > 0)) {
				if (parameters[1].startsWith("0x") == true) {
					System.out
							.println("Please do not start parameters with 0x. Simply use integers without prefix. ");
					return false;

				}
				return true;

			}

			else
				return false;
		}

		if (commandName.compareTo("breakpoint") == 0) {
			if ((optioncount == 0) && (parametercount > 0))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("continue") == 0) {
			if ((optioncount == 0) && (parametercount > 0))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("snapshot") == 0) {
			if ((optioncount == 0) && (parametercount > 0))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("restore") == 0) {
			if ((optioncount == 0) && (parametercount > 0))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("exec") == 0) {
			if ((optioncount == 0) && (parametercount == 1))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("install") == 0) {
			if ((optioncount == 0) && (parametercount == 1))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("find") == 0) {
			if ((optioncount == 0) && (parametercount == 1))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("search") == 0) {
			if ((optioncount == 0) && (parametercount == 1))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("man") == 0) {
			if ((optioncount == 0) && (parametercount == 1))
				return true;
			else
				return false;
		}

		if (commandName.compareTo("memory") == 0) {
			if ((optioncount == 0) && (parametercount == 0))
				return true;
			else
				return false;
		}

		if (commandName.startsWith("./") == true) {
			if ((optioncount == 0) && (parametercount > 0)) {
				if (fdir.getCurrentNode().getName().compareTo("dev") == 0)
					return true;
				else {
					colorOutput
							.println(colorOutput.COLOR_BRIGHT_RED,
									"You are at invalid directory to invoke device commands.");
					return false;
				}
			} else
				return false;
		}

		return false;
	}

}
