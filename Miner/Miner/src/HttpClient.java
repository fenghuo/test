import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.HttpURLConnection;
import java.net.InetSocketAddress;
import java.net.URI;
import java.net.URL;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;


public class HttpClient {
	
	/**
	 * Define the request format:
	 * 	"role=0&count="+g.getCount()+"&size="+g.getSize()+"&totalnumber="+MinerInfo.NUMBER+
	    			"&mynumber="+i+"&graph="+partials[i].getGraph().getGraph();
	 */

    public static void main(String[] args) throws Exception {
        HttpServer server = HttpServer.create(new InetSocketAddress(8080), 0);
        server.createContext("/recv", new MyHandler());
        server.setExecutor(null); // creates a default executor
        server.start();
        new Thread(new Recv()).start();
    }

    static class MyHandler implements HttpHandler {
    	ArrayList<Integer> miner_recorder = new ArrayList<Integer>();
        public void handle(HttpExchange t) throws IOException {
//        	URI uri = t.getRequestURI();
//        	t.getRequestBody()
//        	String query = uri.getQuery(); 
//        	query = query.substring(8);
        	InputStreamReader isr =  new InputStreamReader(t.getRequestBody(),"utf-8");
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
        	System.out.println("role =" + params.get("role"));
        	System.out.println("count =" + params.get("count"));
        	System.out.println("size =" + params.get("size"));
        	System.out.println("total number =" + params.get("totalnumber"));
        	System.out.println("my number =" + params.get("mynumber"));
        	System.out.println("graph =" +params.get("graph"));
        	
        	int role = Integer.parseInt(params.get("role"));
        	if(role == 0){
        		// it is from master
        		String content = "count="+params.get("count")+"\n";
        		content += "size="+params.get("size")+"\n";
        		content += "totalnumber="+params.get("totalnumber")+"\n";
        		content += "mynumber="+params.get("mynumber")+"\n";
        		content += "graph="+params.get("graph");

        		File file = new File("../../graph.txt");
        		 
    			// if file doesnt exists, then create it
    			if (file.exists()) {
    				file.delete();
    			}
    			file.createNewFile();
     
    			FileWriter fw = new FileWriter(file.getAbsoluteFile());
    			BufferedWriter bw = new BufferedWriter(fw);
    			bw.write(content);
    			bw.close();
     
    			System.out.println("Done");
        		
        	}
        	
            String response = "This is the response";
            t.sendResponseHeaders(200, response.length());
            OutputStream os = t.getResponseBody();
            os.write(response.getBytes());
            os.close();
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
