package Model;

public class PartialGraph {
	private Graph g;
	private int startPos;
	private int endPos;
	
	public PartialGraph(Graph g){
		this.g = g;
	}
	public Graph getGraph() {
		return g;
	}
	public void setGraph(Graph g) {
		this.g = g;
	}
	public int getStartPos() {
		return startPos;
	}
	public void setStartPos(int startPos) {
		this.startPos = startPos;
	}
	public int getEndPos() {
		return endPos;
	}
	public void setEndPos(int endPos) {
		this.endPos = endPos;
	}
	
}
