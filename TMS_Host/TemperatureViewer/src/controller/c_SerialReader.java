package controller;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;

import view.v_Console;

 public class c_SerialReader implements Runnable 
    {
	 	private InputStream in;
	 	private PrintWriter out;
        private c_SerialCommumication c_SR;
        
        public c_SerialReader ( c_SerialCommumication c_SR, InputStream in)
        {
        	this.c_SR = c_SR;
        	try {
				out = new PrintWriter(new BufferedWriter(new FileWriter("output", true)));
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
            
            this.in = in;
        }
        
        public void run ()
        {
            byte[] buffer = new byte[1024];
            int len = -1;
            try
            {
                while ( ( len = this.in.read(buffer)) > -1 )
                {
                    System.out.print(new String(buffer,0,len));
                    c_SR.appendText(new String(buffer,0,len));
                    out.print(new String(buffer,0,len));
                }
            }
            catch ( IOException e )
            {
                e.printStackTrace();
            }
        }
    }

  