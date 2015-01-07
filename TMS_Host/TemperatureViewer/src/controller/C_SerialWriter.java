package controller;
import java.io.IOException;
import java.io.OutputStream;

  /** */
    public class C_SerialWriter implements Runnable 
    {
        OutputStream out;
        
        public C_SerialWriter ( OutputStream out )
        {
            this.out = out;
        }
        
        public void run ()
        {
            try
            {                
                int c = 0;
                while ( ( c = System.in.read()) > -1 )
                {
                    this.out.write(c);
                }                
            }
            catch ( IOException e )
            {
                e.printStackTrace();
            }            
        }
        
        public void write (int c)
        {
        	try {
				this.out.write(c);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
        }
    }