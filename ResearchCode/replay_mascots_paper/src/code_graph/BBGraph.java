package code_graph;

import java.util.List;
import java.util.ArrayList;

public class BBGraph {
	private List<BBVertex> vertices;
	private List<BBEdge> edges;
	
	public BBGraph() {
		vertices = new ArrayList<BBVertex>();
		edges = new ArrayList<BBEdge>();
	}
	
	public boolean addVertex(BBVertex vertex) {
		boolean added = false;
		if (vertices.contains(vertex) == false) {
			added = vertices.add(vertex);
		}
		return added;
	}

	public boolean addEdge(BBVertex src, BBVertex dest, double tranprob) {
		if (vertices.contains(src) == false) {
			throw new IllegalArgumentException("source vertex is not in the graph!");
		}
		if (vertices.contains(dest) == false) {
			throw new IllegalArgumentException("destination vertex is not in the graph!");
		}
		BBEdge edge = new BBEdge(src, dest, tranprob);
		if (src.searchOutgoingEdge(dest) != null) {
			return false;
		}
		else {
			src.addOutgoingEdge(edge);
			dest.addIncomingEdge(edge);
			edges.add(edge);
			return true;
		}
	}
	
	public List<BBVertex> getAllVertices() {
		return vertices;
	}
	
	public BBVertex getVertexByID(int id) {
//		for (BBVertex vertex : vertices) {
//			if (vertex.getID() == id) {
//				return vertex;
//			}
//		}
		for (int i = 0; i < vertices.size(); i++) {
			if (vertices.get(i).getID() == id) {
				return vertices.get(i);
			}
		}
		return null;
	}
	
	public BBVertex getVertexByStartInstrAddr(int addr) {
		for (int i = 0; i < vertices.size(); i++) {
			if (vertices.get(i).getStartInstrAddr() == addr) {
				return vertices.get(i);
			}
		}
		return null;
	}
	
	public BBVertex getVertexContainsInstr(InstrVertex instr) {
		for (BBVertex bb_v : vertices) {
			if(bb_v.containsInstr(instr)) {
				return bb_v;
			}
		}
		return null;
	}
	
	public int getNumOfVertices() {
		return vertices.size();
	}
	
	public List<BBEdge> getAllEdges() {
		return edges;
	}
	
	public int getNumOfEdges() {
		return edges.size();
	}
	
	public boolean removeEdge(BBVertex src, BBVertex dest) {
		BBEdge edge = src.searchOutgoingEdge(dest);
		if (edge == null) {
			return false;
		}
		else {
			src.removeOutgoingEdge(edge);
			dest.removeIncomingEdge(edge);
			edges.remove(edge);
			return true;
		}
	}
}

