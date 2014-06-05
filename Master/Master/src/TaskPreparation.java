import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;

import org.omg.CORBA.portable.InputStream;

import Model.Graph;
import Model.MinerInfo;
import Model.PartialGraph;


public class TaskPreparation {
	
	public static Graph old_g = new Graph();
	public static Graph g = new Graph();
	public static boolean first = true;
	private static int times_same = 0;
	private static int LIMIT = 10;
	private static int DEFAULT_COUNT = 99999999;
	
	public static void assign(){
		if(!ifGraphBetter()){
			shuffleGraph();
		}
		replaceGraph();
		sendGraph();
	}


	private static void shuffleGraph() {
		// TODO Auto-generated method stub
		int times = 10;
		char[] graph = g.getGraph().toCharArray();
		int size = g.getSize();
		for(int i = 0; i < times; i++){
			int random = (int) (Math.random()*(size*size));
			while(random%size <= random/size)
				random = (int) (Math.random()*(size*size));
			if(graph[random] == '0')
				graph[random] = '1';
			else
				graph[random] = '0';
		}
		g.setGraph(String.valueOf(graph));	
	}


	private static boolean ifGraphBetter() {
		// TODO Auto-generated method stub
		if(g.getCount() < old_g.getCount()){
			times_same = 0;
			return true;
		}else if(g.getCount() == old_g.getCount() && g.getGraph().equals(old_g.getGraph()) && times_same < LIMIT){
			times_same++;
			return true;
		}else{
			//times_same = 0;
			return false;
		}
				
	}
	
	private static void replaceGraph(){
		old_g.setCount(g.getCount());
		old_g.setSize(g.getSize());
		old_g.setGraph(g.getGraph());
	}


	public static void updateCurrentGraph(Graph my_g) {
		// TODO Auto-generated method stub
		if(my_g.getSize() == g.getSize() && my_g.getCount() < g.getCount()){
			// if it is the graph with the same size as the current graph
			// and has better count, then replace it
			int length = 0;
			String graph = my_g.getGraph();
			for(int i = 0; i < graph.length(); i++){
				if(graph.charAt(i) != '0' && graph.charAt(i) != '1')
					return;
				length++;
			}
			if(length != my_g.getSize()*my_g.getSize())
				return;
			
			g.setCount(my_g.getCount());
			g.setGraph(my_g.getGraph());
			
			System.out.println("Graph updated!");
			System.out.println("size: "+g.getSize());
			System.out.println("new graph: "+g.getGraph());
			System.out.println("best count: "+g.getCount());
			
		}
	}


	private static void generateNewGraph() {
		// TODO Auto-generated method stub
		int size = g.getSize();
		char[] graph = g.getGraph().toCharArray();
		char[] new_graph = new char[(size+1)*(size+1)];
		for(int i = 0; i < size; i++){
			for(int j = 0; j < size; j++){
				new_graph[i*size+j+i] = graph[i*size+j];
			}
			int random = (int) Math.random()*2;
			if(random == 1)
				new_graph[(i+1)*(size+1)-1] = '1';
			else
				new_graph[(i+1)*(size+1)-1] = '0';
		}
		// the last row
		for(int i = 0; i < size+1; i++)
			new_graph[size*(size+1)+i] = '0';
		
//		char[] hi = new char[]{'0', '1'};
//		System.out.println("hi: "+hi.toString());
		g.setGraph(String.valueOf(new_graph));
		g.setSize(size+1);
		g.setCount(DEFAULT_COUNT);
		//System.out.println("graph: "+g.getGraph());
		
		sendGraph();

	}


	public static void handleComplete(String graph, int size) {
		// TODO Auto-generated method stub
		/**
		 *  need to store the new graph
		 */
		g.setGraph(graph);
		g.setCount(0);
		g.setSize(size);
		//System.out.println("graph: "+g.getGraph());
		generateNewGraph();
	}
	
//	private static PartialGraph[] separateGraph(){
//		int number = MinerInfo.NUMBER;
//		PartialGraph[] partials = new PartialGraph[number];
//		System.out.println("graph: "+g.getGraph());
//		for(int i = 0; i < number; i++){
//			partials[i] = new PartialGraph(g);
//			partials[i].setStartPos(i*g.getSize()/number);
//			partials[i].setEndPos(Math.min((i+1)*g.getSize()/number-1, g.getSize()-1));
//		}
//		return partials;
//	}

	public static void sendGraph(){
		// separate the graph, so that different partial graph will be sent to different client
		
		//PartialGraph[] partials = separateGraph();
		
	    URL url;
	    HttpURLConnection connection = null;
	    
	    for(int i = 0; i < MinerInfo.NUMBER; i++){
	    	//String rawData = "role=0&count="+g.getCount()+"&size="+g.getSize()+"&from="+partials[i].getStartPos()+
	    			//"&to="+partials[i].getEndPos()+"&graph="+partials[i].getGraph().getGraph();
	    	
	    	String targetURL = MinerInfo.urls[i];
	    	try {
	  	      //Create connection
	    		String graph = g.getGraph();
	    		System.out.println("Send size: "+g.getSize()+", graph: "+g.getGraph());
	    		String encodeData = URLEncoder.encode("role", "UTF-8") + "=" + URLEncoder.encode("0", "UTF-8");
		    	encodeData += "&" + URLEncoder.encode("count", "UTF-8") + "=" + URLEncoder.encode(g.getCount()+"", "UTF-8");
		    	encodeData += "&" + URLEncoder.encode("size", "UTF-8") + "=" + URLEncoder.encode(g.getSize()+"", "UTF-8");
		    	encodeData += "&" + URLEncoder.encode("totalnumber", "UTF-8") + "=" + URLEncoder.encode(MinerInfo.NUMBER+"", "UTF-8");
		    	encodeData += "&" + URLEncoder.encode("mynumber", "UTF-8") + "=" + URLEncoder.encode(i+1+"", "UTF-8");
		    	encodeData += "&" + URLEncoder.encode("graph", "UTF-8") + "=" + URLEncoder.encode(graph+"", "UTF-8");
		    	
		    	
	  	      url = new URL(targetURL);
	  	      connection = (HttpURLConnection)url.openConnection();
	  	      connection.setRequestMethod("POST");
	  	      connection.setRequestProperty("Content-Type", 
	  	           "application/x-www-form-urlencoded");
	  				
	  	      connection.setRequestProperty("Content-Length", "" + 
	  	               Integer.toString(encodeData.getBytes().length));
	  	      connection.setRequestProperty("Content-Language", "en-US");  
	  				
	  	      connection.setUseCaches (false);
	  	      connection.setDoInput(true);
	  	      connection.setDoOutput(true);

	  	      //Send request
	  	      OutputStreamWriter wr = new OutputStreamWriter(connection.getOutputStream());
	  	      wr.write(encodeData);
//	  	      DataOutputStream wr = new DataOutputStream (
//	  	                  connection.getOutputStream ());
//	  	      wr.writeBytes (encodeData);
	  	      wr.flush ();
	  	      

	  	      //Get Response	
	  	      InputStream is = (InputStream) connection.getInputStream();
	  	      BufferedReader rd = new BufferedReader(new InputStreamReader(is));
	  	      String line;
	  	      StringBuffer response = new StringBuffer(); 
	  	      while((line = rd.readLine()) != null) {
	  	        response.append(line);
	  	        response.append('\r');
	  	      }
	  	      
	  	      wr.close ();
	  	      rd.close();
	  	      //return response.toString();
	  	      System.out.println("Sent!!"+encodeData);

	  	    } catch (Exception e) {

	  	      e.printStackTrace();
	  	      //return null;

	  	    } finally {

	  	      if(connection != null) {
	  	        connection.disconnect(); 
	  	      }
	  	    }
	    	
	    }
	    
	  }
	
	
	/****************************** TEST *********************************/
	
	public static void testShuffle(){
		int size = 6;
		g.setSize(size);
		g.setCount(10);
		char[] graph = new char[]{
				'0', '1', '1', '1', '1', '1',
				'0', '0', '1', '1', '1', '1',
				'0', '0', '0', '1', '1', '1',
				'0', '0', '0', '0', '1', '1',
				'0', '0', '0', '0', '0', '1',
				'0', '0', '0', '0', '0', '0',
		};
		g.setGraph(String.valueOf(graph));
		
		shuffleGraph();
		
		char[] graphToArray = g.getGraph().toCharArray();
		for(int i = 0; i < size; i++){
			for(int j = 0; j < size; j++){
				System.out.print(graphToArray[i*size+j]);
			}
			System.out.println();
		}
	}
	
//	public static void main(String args[]){
//		//new TaskPreparation();
//		testShuffle();
//		
//	}
}
