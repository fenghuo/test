package Model;

public class Graph {
	private int count;
	private int size;
	private String graph;
	
	public Graph(){
	}
	
	public Graph(int size){
		this.setSize(size);
	}
	
	public Graph(int count, int size, String graph){
		this.count = count;
		this.size = size;
		this.graph = graph;
	}
	public int getSize() {
		return size;
	}
	public void setSize(int size) {
		this.size = size;
	}
	public String getGraph() {
		return graph;
	}
	public boolean setGraph(String graph) {
//		if(graph.length() == this.size){
//			this.graph = graph;
//			return true;
//		}else
//			return false;
		this.graph = graph;
		return true;
	}
	public int getCount() {
		return count;
	}
	public void setCount(int count) {
		this.count = count;
	}
	
	
}
