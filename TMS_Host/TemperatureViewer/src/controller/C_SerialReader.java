package controller;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;

import model.M_TemperatureMeasurementSystem;

import view.V_Console;

 public class C_SerialReader implements Runnable 
    {
	 	private InputStream in;
	 	private PrintWriter out;
        private C_SerialCommumication c_SR;
        private M_TemperatureMeasurementSystem m_TMS;
        private C_TemperatureMeasurementSystem c_TMS;
        private String unstoredLines;
        private int numberOfSensorValues = 0;
        private boolean firstCall = true;
        
        public C_SerialReader ( C_SerialCommumication c_SR, InputStream in, C_TemperatureMeasurementSystem c_TMS)
        {
        	this.c_SR = c_SR;
        	try {
				out = new PrintWriter(new BufferedWriter(new FileWriter("output", true)));
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

        	this.m_TMS = c_TMS.getModel();
        	this.c_TMS = c_TMS;
        	unstoredLines="";
            this.in = in;
        }
        
        public synchronized void run ()
        {
            byte[] buffer = new byte[1024];
            int len = -1;
            try
            {
                while ( ( len = this.in.read(buffer)) > -1 )
                {
                    System.out.print(new String(buffer,0,len));
                    addOutput(new String(buffer,0,len));
                }
            }
            catch ( IOException e )
            {
                e.printStackTrace();
            }
        }
        
        public static double round(double value, int places) {
            if (places < 0) throw new IllegalArgumentException();

            long factor = (long) Math.pow(10, places);
            value = value * factor;
            long tmp = Math.round(value);
            return (double) tmp / factor;
        }
        
        public String fillZeros(String s, int length)
        {
        	String output="";
        	for (int i = 0; i<length - s.length(); i++)
        	{
        		output += "0";
        	}
        	
        	return output + s;
        }
        
    	private String substituteSensorValues(String line){
    		String [] parts = line.split(" ");
    		String output = "";

    		if (firstCall)
    		{
    			numberOfSensorValues = Integer.parseInt(parts[1]);
    			firstCall = false;
    		}
    		
    		parts[1] = fillZeros(String.valueOf(Integer.parseInt(parts[1]) - numberOfSensorValues), 5);
    		
    		for (int i = 0; i < parts.length; i++)
    		{
    			if (2 <= i && i < 2 + m_TMS.NUMBER_OF_SENSORS){
    				parts[i] = String.valueOf(round(c_TMS.getTempControl().getTemperature()[i - 2], 2));
    			}
    			output += parts[i] + " "; 
    		}
    		return output;
    	}
    	
        public synchronized void addOutput(String text) {
    		
        	
    		if((m_TMS.getCalibrationMode() || m_TMS.getReadMode()) && text != "" && text != null)
    		{
    			unstoredLines += text;
    			if (unstoredLines.contains("\n"))
    			{
    				String [] parts = unstoredLines.split("\n");
    				if (parts.length>1){
        				for (int i = 0; i<parts.length-1;i++)
        				{
        					if (parts[i].split(" ")[0].substring(0, 1).equals("3")){
        						if (m_TMS.getTemperatureMode())
            					{
        							String partsTemp = substituteSensorValues(parts[i]);
        							m_TMS.addTempLine(partsTemp);
                                    c_SR.appendTextTemp(partsTemp + "\n");
                                    out.print(partsTemp + "\n");
            					}
            					m_TMS.addUnreadLine(parts[i]);
                                c_SR.appendText(parts[i] + "\n");
            					
        					}
        				}
    					
    				if (parts[parts.length - 1].split(" ")[0].substring(0, 1).equals("3")){
    					unstoredLines = parts[parts.length - 1];
    				} else{
    					if (parts.length==1 && parts[0].split(" ")[0].substring(0, 1).equals("3")){
    						unstoredLines=parts[0];
    					}else
    						unstoredLines = "";
    					}
    				}
    			} 
    			
    			C_SensorDataLine line = m_TMS.getNextCompleteLine();
    			if (line != null && line.getLineNumber() != -1){
    				c_TMS.getTempControl().updateData(line.getSensorData(), line.getTemperature(), line.getVcc());
    			}
    		}
//    		else if ( && text != "" && text != null) {
//    			C_SensorDataLine line = getNextCompleteLine();
//    			if (line != null && line.getLineNumber() != -1){
//    				c.getTempControl().updateData(line.getSensorData(), line.getTemperature(), line.getVcc());
//    			}
//    		}
    		
    		
    	}

    }

  
