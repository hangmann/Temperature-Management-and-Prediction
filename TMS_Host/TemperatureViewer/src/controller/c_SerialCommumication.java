package controller;

import model.m_TemperatureMeasurementSystem;
import view.v_Console;

public class c_SerialCommumication {

	private c_SerialReader c_SR;
	private v_Console console;
	private String consoleText;
	
	public c_SerialCommumication(c_TemperatureMeasurementSystem c_TMS)
	{
		
		c_SR = new c_SerialReader(this, c_TMS.getModel().getIn_stream());
		console = new v_Console(c_TMS);
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
