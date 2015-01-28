package controller;

import model.M_TemperatureMeasurementSystem;
import view.V_Console;

public class C_SerialCommumication {

	private C_SerialReader c_SR;
	private V_Console console;
	
	public C_SerialCommumication(C_TemperatureMeasurementSystem c_TMS)
	{
		
		c_SR = new C_SerialReader(this, c_TMS.getModel().getIn_stream(), c_TMS);
		console = new V_Console(c_TMS);
		// start reading serial port		
        (new Thread(c_SR)).start();
	}
	
	public void appendText(String str)
	{
		console.appendText(str);
	}
	
	public void appendTextTemp(String str)
	{
		console.appendTextTemp(str);
	}
	
	public void clearText()
	{
		console.clearText();
	}

}
