import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.nio.charset.Charset;
import java.util.ArrayList;



public class Recv implements Runnable {

	@Override
	public void run() {
		// TODO Auto-generated method stub
		while(true){
			try {
				Thread.sleep(5000);
			} catch (InterruptedException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			String filename = "../../result.txt";
			File file = new File(filename);
			while(!file.exists()){
				// polling
			}
			
			
			try {
				InputStream fis = new FileInputStream(filename);
				BufferedReader br = new BufferedReader(new InputStreamReader(fis, Charset.forName("UTF-8")));
				String  line = null;
				
				ArrayList<String> data = new ArrayList<String>();
				while ((line = br.readLine()) != null) {
				    data.add(line);
				}
				// Done with the file
				br.close();
				
				System.out.println(data.size());
				
				for(int i = 0; i < data.size(); i++){
					System.out.println(data.get(i));
				}
				line = null;
				
				// delete the file
				file.delete();
				
				URL url;
			    HttpURLConnection connection = null;
			    
				String targetURL = MasterInfo.url;
		    	try {
		  	      //Create connection
		    		String encodeData = "";
		    		for(int i = 0; i < data.size(); i++){
		    			String oneline = data.get(i);
		    			int index = oneline.indexOf('=');
		    			String first = oneline.substring(0, index);
		    			String second = oneline.substring(index+1);
		    			if(i != 0)
		    				encodeData += "&";
		    			encodeData += URLEncoder.encode(first, "UTF-8") + "=" + URLEncoder.encode(second, "UTF-8");
		    		}
		    		
			    	
			    	
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
//		  	      DataOutputStream wr = new DataOutputStream (
//		  	                  connection.getOutputStream ());
//		  	      wr.writeBytes (encodeData);
		  	      wr.flush ();
		  	      

		  	      //Get Response	
		  	      InputStream is = (InputStream) connection.getInputStream();
		  	      BufferedReader rd = new BufferedReader(new InputStreamReader(is));
		  	      //String line;
		  	      StringBuffer response = new StringBuffer(); 
		  	      while((line = rd.readLine()) != null) {
		  	        response.append(line);
		  	        response.append('\r');
		  	      }
		  	      
		  	      wr.close ();
		  	      rd.close();
		  	      //return response.toString();
		  	      System.out.println("Sent partial graph!!"+encodeData);

		  	    } catch (Exception e) {

		  	      e.printStackTrace();
		  	      //return null;

		  	    } finally {

		  	      if(connection != null) {
		  	        connection.disconnect(); 
		  	      }
		  	    }
				
				
				
				
				
				
				
				
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}
	}

}
