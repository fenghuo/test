import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URI;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import Model.Graph;
import Model.MinerInfo;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;


public class HTTP {
	/**
	 * Define the request format:
	 * 	role=0&status=minimum(/none)&count=100&size=79&coin=0000001111111
	 */

    public static void main(String[] args) throws Exception {
        HttpServer server = HttpServer.create(new InetSocketAddress(1918), 0);
        server.createContext("/handle", new MyHandler());
        server.setExecutor(null); // creates a default executor
        server.start();
    }

    static class MyHandler implements HttpHandler {
    	ArrayList<Integer> miner_recorder = new ArrayList<Integer>();
        public void handle(HttpExchange t) throws IOException {
//        	URI uri = t.getRequestURI();
//        	String query = uri.getPath(); 
//        	query = query.substring(8);
        	
        	InputStreamReader isr =  new InputStreamReader(t.getRequestBody(),"utf-8");
        	if(isr != null){
        		BufferedReader br = new BufferedReader(isr);
            	int b;
            	StringBuilder buf = new StringBuilder(12000);
            	while ((b = br.read()) != -1) {
            	    buf.append((char) b);
            	}
            	String request = buf.toString();
            	br.close();
            	isr.close();
            	
            	Map<String, String> params = queryToMap(request);
            	System.out.println("miner =" + params.get("miner"));
            	System.out.println("status =" + params.get("status"));
            	System.out.println("count =" + params.get("count"));
            	System.out.println("size =" + params.get("size"));
            	System.out.println("graph =" + params.get("graph"));
            	
            	new TaskPreparation();
            	
            	String status = params.get("status");
            	if(status.equals("complete")){
            		int size = Integer.parseInt(params.get("size")); 
            		String graph = params.get("graph");
            		TaskPreparation.handleComplete(graph, size);
            	}else{
            		int size = Integer.parseInt(params.get("size"));
            		// ignore the past graph
            		if(size == TaskPreparation.g.getSize()){
            			int count = Integer.parseInt(params.get("count"));
                    	
                    	String graph = params.get("graph");
                    	Graph g = new Graph(count, size, graph);
                    	TaskPreparation.updateCurrentGraph(g);
                    	
                    	int number = recordMiner(params.get("miner"));
                    	if(number >= MinerInfo.NUMBER){
                    		// have got enough number of miner response
                    		// is ready for new task assign
                    		miner_recorder.clear();
                    		TaskPreparation.assign();
                    	}
            		}
            	}
            	
                String response = "This is the response";
                t.sendResponseHeaders(200, response.length());
                OutputStream os = t.getResponseBody();
                os.write(response.getBytes());
                os.close();
        	}
        }
        
        public int recordMiner(String miner){
        	int no = Integer.parseInt(miner);
        	for(int i : miner_recorder){
        		if(no == i)
        			return miner_recorder.size();
        	}
        	miner_recorder.add(no);
        	return miner_recorder.size();
        	
        }
        public Map<String, String> queryToMap(String query){
            Map<String, String> result = new HashMap<String, String>();
            for (String param : query.split("&")) {
            	
                String pair[] = param.split("=");
                //System.out.println(pair[0]+": "+pair[1]);
                if (pair.length>1) {
                    result.put(pair[0], pair[1]);
                }else{
                    result.put(pair[0], "");
                }
            }
            return result;
        }

    }
    
   
}