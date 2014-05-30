package code_graph;

import java.util.List;
import java.util.ArrayList;

public class BBVertex {
	private int id;
	private int start_instr_addr;
	private int end_instr_addr;
	private int cycles;
	private List<InstrVertex> instructions;
	private List<BBEdge> incomingEdges;
	private List<BBEdge> outgoingEdges;
	private CodeGraph c_graph;
	
	public BBVertex(int id, CodeGraph c_graph) {
		this.id = id;
		this.cycles = 0;
		this.c_graph = c_graph;
		instructions = new ArrayList<InstrVertex>();
		incomingEdges = new ArrayList<BBEdge>();
		outgoingEdges = new ArrayList<BBEdge>();
	}
	
	public void setStartInstrAddr(int addr) {
		this.start_instr_addr = addr;
	}
	
	public void setEndInstrAddr(int addr) {
		this.end_instr_addr = addr;
	}
	
	public void addInstr(InstrVertex v) {
		this.instructions.add(v);
	}
	
	public void calCycles() {
		this.cycles = this.c_graph.getCyclesOfExecutionPath(this.instructions);
	}
	
	public int getID() {
		return id;
	}
	
	public int getCycles() {
		return cycles;
	}
	
	public int getStartInstrAddr() {
		return start_instr_addr;
	}
	
	public int getEndInstrAddr() {
		return end_instr_addr;
	}
	
	
	public List<InstrVertex> getAllInstructions() {
		return instructions;
	}
	
	public boolean containsInstr(InstrVertex v) {
		if (instructions.contains(v)) {
			return true;
		}
		else {
			return false;
		}
	}
//	public boolean addEdge(BBEdge edge) { 
//		if (edge.getSource() == this) {
//			outgoingEdges.add(edge);
//		}
//		else if (edge.getDestination() == this) {
//			incomingEdges.add(edge);
//		}
//		else {
//			return false;
//		}
//		return true;
//	}
	public boolean addIncomingEdge(BBEdge edge) {
		if (edge.getDestination() == this) {
			incomingEdges.add(edge);
			return true;
		}
		else {
			return false;
		}
	}
	
	public boolean addOutgoingEdge(BBEdge edge) {
		if (edge.getSource() == this) {
			outgoingEdges.add(edge);
			return true;
		}
		else {
			return false;
		}
	}
	
//	public boolean removeEdge(BBEdge edge) {
//		if (edge.getSource() == this) {
//			outgoingEdges.remove(edge);
//		}
//		else if (edge.getDestination() == this) {
//			incomingEdges.remove(edge);
//		}
//		else {
//			return false;
//		}
//		return true;
//	}
	
	public boolean removeIncomingEdge(BBEdge edge) {
		if (edge.getDestination() == this) {
			incomingEdges.remove(edge);
			return true;
		}
		else {
			return false;
		}
	}
	
	public boolean removeOutgoingEdge(BBEdge edge) {
		if (edge.getSource() == this) {
			outgoingEdges.remove(edge);
			return true;
		}
		else {
			return false;
		}
	}
	
	public List<BBEdge> getIncomingEdges() {
		return incomingEdges;
	}
	
	public int getNumOfIncomingEdges() {
		return incomingEdges.size();
	}
	
	public List<BBEdge> getOutgoingEdges() {
		return outgoingEdges;
	}
	
	public int getNumOfOutgoingEdges() {
		return outgoingEdges.size();
	}
	
	public BBEdge searchOutgoingEdge(BBVertex dest) {
		for (BBEdge edge : outgoingEdges) {
			if (edge.getDestination() == dest) {
				return edge;
			}
		}
		return null;
	}
}
