package code_graph;

import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;
import java.io.*;

public class CodeGraph {
	private List<InstrVertex> vertices;
	private List<InstrEdge> edges;
	
	public CodeGraph() {
		vertices = new ArrayList<InstrVertex>();
		edges = new ArrayList<InstrEdge>();
	}
	
	public boolean addVertex(InstrVertex v) {
		boolean added = false;
		if (vertices.contains(v) == false) {
			added = vertices.add(v);
		}
		return added;
	}
	
	public int getSize() {
		return vertices.size();
	}
	
	public List<InstrVertex> getVertices() {
		return this.vertices;
	}
	
	public InstrVertex findVertexByAddr(int addr) {
		int low = 0;
		int high = vertices.size()-1;
		int mid;
		while (low <= high) {
			mid = (low + high) / 2;
			if (vertices.get(mid).getAddr() < addr) {
				low = mid + 1;
			}
			else if (vertices.get(mid).getAddr() > addr) {
				high = mid - 1;
			}
			else {
				return vertices.get(mid);
			}
		}
		return null;
	}
	
	/*
	public InstrVertex findNextVertexInList(InstrVertex v) {
		int id_cur = vertices.indexOf(v);
		int id_next = id_cur+1;
		return vertices.get(id_next);
	}
	*/
	
	public boolean addEdge(InstrVertex src, InstrVertex dest, int cycles) {
		if (vertices.contains(src) == false) {
			//throw new IllegalArgumentException("source vertex is not in the graph!");
			return false;
		}
		if (vertices.contains(dest) == false) {
			//throw new IllegalArgumentException("destination vertex is not in the graph!");
			return false;
		}
		InstrEdge e = new InstrEdge(src, dest, cycles);
		if (src.findEdge(dest) != null) {
			return false;
		}
		else {
			src.addEdge(e);
			dest.addEdge(e);
			edges.add(e);
			return true;
		}
		
	}
	
	public List<InstrEdge> getEdges() {
		return this.edges;
	}
	
	public boolean removeEdge(InstrVertex src, InstrVertex dest) {
		InstrEdge e = src.findEdge(dest);
		if (e == null) {
			return false;
		}
		else {
			src.removeEdge(e);
		    dest.removeEdge(e);
		    edges.remove(e);
		    return true;
		}
	}
	
	public void printCodeGraphInPajekFormat() {
		try {
			PrintWriter pw = new PrintWriter(new OutputStreamWriter(new FileOutputStream("CodeGraph.net")), true);
			pw.println("*Vertices "+Integer.toString(vertices.size()));
			for (int i = 0; i < vertices.size(); i++) {
				pw.println(Integer.toString(i+1)+" "+"\""+vertices.get(i).getInstruction()+"\"");
			}
			pw.println("*Arcs");
			for (int j = 0; j < edges.size(); j++) {
				InstrVertex src = edges.get(j).getSource();
				InstrVertex dest = edges.get(j).getDestination();
				int src_id = vertices.indexOf(src)+1;
				int dest_id = vertices.indexOf(dest)+1;
				pw.println(Integer.toString(src_id)+" "+Integer.toString(dest_id)+" "+Integer.toString(edges.get(j).getCycles()));
			}
			pw.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public void printCodeGraphInDotFormat() {
		try {
			PrintWriter pw = new PrintWriter(new OutputStreamWriter(new FileOutputStream("CodeGraph.dot")), true);
			pw.println("digraph G {");
			for (int i = 0; i < edges.size(); i++) {
				InstrVertex src = edges.get(i).getSource();
				InstrVertex dest = edges.get(i).getDestination();
				pw.println("    "+Integer.toString(src.getAddr())+"->"+Integer.toString(dest.getAddr())+";");
			}
			pw.println("}");
			pw.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public int getCyclesOfExecutionPath(List<InstrVertex> ep) {
		int cycles = 0;
		for (int i = 0; i < ep.size()-1; i++) {
			InstrVertex v1 = ep.get(i);
			InstrVertex v2 = ep.get(i+1);
			InstrEdge e = v1.findEdge(v2);
			cycles += e.getCycles();
		}
		return cycles;
		
	}
}