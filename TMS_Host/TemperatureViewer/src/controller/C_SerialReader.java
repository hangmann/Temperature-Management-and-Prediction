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
        private String unstoredLines;
        private String nextLine;
        
        public C_SerialReader ( C_SerialCommumication c_SR, InputStream in, M_TemperatureMeasurementSystem m_TMS)
        {
        	this.c_SR = c_SR;
        	try {
				out = new PrintWriter(new BufferedWriter(new FileWriter("output", true)));
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
            
        	this.m_TMS = m_TMS;
        	unstoredLines="";
        	nextLine="";
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
                    c_SR.appendText(new String(buffer,0,len));
                    out.print(new String(buffer,0,len)); // do NOT write in file!!!
                    m_TMS.addOutput(new String(buffer,0,len));
                }
            }
            catch ( IOException e )
            {
                e.printStackTrace();
            }
        }
        

    }

  
