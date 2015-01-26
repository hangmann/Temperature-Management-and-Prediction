package controller;

import model.M_TemperatureMeasurementSystem;
import view.V_Console;

public class C_SerialCommumication {

	private C_SerialReader c_SR;
	private V_Console console;
	private String consoleText;
	
	public C_SerialCommumication(C_TemperatureMeasurementSystem c_TMS)
	{
		
		c_SR = new C_SerialReader(this, c_TMS.getModel().getIn_stream(), c_TMS.getModel());
		console = new V_Console(c_TMS);
		// start reading serial port		
        (new Thread(c_SR)).start();
	}
	
	public void appendText(String str)
	{
		consoleText += str;
		console.appendText(str);
	}
	
	public void clearText()
	{
		console.clearText();
	}

}
