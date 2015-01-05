package controller;
import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.UnsupportedCommOperationException;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import model.*;
import view.*;

public class c_TemperatureMeasurementSystem {

	private v_TemperatureMeasurementSystem v_TMS;
	private m_TemperatureMeasurementSystem m_TMS;	
	private c_SerialCommumication c_SCom;
	private c_HeaterControl c_HC;
	
	public c_TemperatureMeasurementSystem() {

		this.m_TMS = new m_TemperatureMeasurementSystem();
		this.v_TMS = new v_TemperatureMeasurementSystem(this);
		
		c_SCom = new c_SerialCommumication(this);
		
		
		// start and configure serial port streams
		try {
			CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(m_TemperatureMeasurementSystem.PORTNAME);

            CommPort commPort;
			commPort = portIdentifier.open("Temperature Measurement System", 2000);
			
            SerialPort serialPort = (SerialPort) commPort;
            serialPort.setSerialPortParams(m_TemperatureMeasurementSystem.BAUD_RATE,m_TemperatureMeasurementSystem.SP_DATABITS ,m_TemperatureMeasurementSystem.SP_STOPBITS,m_TemperatureMeasurementSystem.SP_PARITY);
            m_TMS.setIn_stream(serialPort.getInputStream());
            m_TMS.setOut_stream(serialPort.getOutputStream());
            showHeatControl(); 

		} catch (IOException e3) {
			e3.printStackTrace();
			System.out.println("Error: I/O.");
			System.exit(0);
			
		} catch (UnsupportedCommOperationException e2) {
			e2.printStackTrace();
			System.out.println("Error: Unsopported Comm Operation.");
			System.exit(0);
			
		} catch (NoSuchPortException e1) {
			e1.printStackTrace();
			System.out.println("Error: Port does not exist.");
			System.exit(0);
		
		} catch (PortInUseException e) {
			e.printStackTrace();
			System.out.println("Error: Port is already in use.");
			System.exit(0);
		}
	}
	
	public void showHeatControl()
	{
		this.c_HC = new c_HeaterControl(this);
	}

	public m_TemperatureMeasurementSystem getModel()
	{
		return m_TMS;
	}
	
	public c_SerialCommumication getSerialCommunication()
	{
		return c_SCom;
	}
	
	public c_HeaterControl getHeatControl()
	{
		return this.c_HC;
	}

	public void start10MinExperiment() {
		getHeatControl().startExperiment();
		//heatControl.setOutput(0);
		Thread t = new Thread(new c_TimerThread(600000, 32, this));
		    
	    t.start();
	}
}
