package code_graph;

import java.util.List;
import java.util.ArrayList;

public class InstrVertex {
	private int addr;
	private String variant;
	private String operands;
	private List<InstrEdge> incomingEdges;
	private List<InstrEdge> outgoingEdges;
	private boolean mark;
	
	public InstrVertex(int addr, String variant, String operands) {
		this.addr = addr;
		this.variant = variant;
		this.operands = operands;
		incomingEdges = new ArrayList<InstrEdge>();
		outgoingEdges = new ArrayList<InstrEdge>();
		mark = false;
	}
	
	public int getAddr() {
		return addr;
	}
	
	public String getVariant() {
		return variant;
	}
	
	public String getOperands() {
		return operands;
	}
	
	public String getInstruction() {
		return variant + " " + operands;
	}
	
	public boolean addEdge(InstrEdge e) {
		if (e.getSource() == this) {
			outgoingEdges.add(e);
		}
		else if (e.getDestination() == this) {
			incomingEdges.add(e);
		}
		else {
			return false;
		}
		return true;
	}
	
	public boolean removeEdge(InstrEdge e) {
		if (e.getSource() == this) {
			incomingEdges.remove(e);
		}
		else if (e.getDestination() == this) {
			outgoingEdges.remove(e);
		}
		else {
			return false;
		}
		return true;
	}
	
	public int getIncomingEdgeCount() {
		return incomingEdges.size();
	}
	
	public List<InstrEdge> getIncomingEdges() {
		return this.incomingEdges;
	}
	
	public int getOutgoingEdgeCount() {
		return outgoingEdges.size();
	}
	
	public List<InstrEdge> getOutgoingEdge() {
		return this.outgoingEdges;
	}
	
	public InstrEdge findEdge(InstrVertex dest) {
		for (InstrEdge e : outgoingEdges) {
			if (e.getDestination() == dest) {
				return e;
			}
		}
		return null;
	}
	
	public boolean visited() {
		return mark;
	}
	
	public void mark() {
		mark = true;
    }
	
	public void visit() {
		mark();
    }
	
	public void clearMark() {
		mark = false;
	}
 }