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
 * Created by IntelliJ IDEA. User: Qing Cao Date: 2007-9-21 Time: 18:17:13 To
 * change this template use File | Settings | File Templates.
 */
public class variable {
	private int addr;
	private int size;
	private String name;

	public variable() {
		addr = size = 0;
		name = null;
	}

	public variable(String line) {
		String[] elements = new String[100];
		String[] temp = new String[10];
		elements = line.split("\\s+");
		// System.out.println(elements[0] + ":" + elements[1]+ ":" +
		// elements[2]);
		this.addr = Integer.parseInt(elements[0], 16)
				- Integer.parseInt("800000", 16);
		this.size = Integer.parseInt(elements[1], 16);
		temp = elements[2].split("\\.");
		this.name = temp[0];

	}

	public String getName() {
		return name;
	}

	public int getSize() {
		return size;
	}

	public int getaddr() {
		return addr;
	}

	public void print() {
		System.out.println("This variable has name of " + name
				+ " and size of " + size);

	}

}
