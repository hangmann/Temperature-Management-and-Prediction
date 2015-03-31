package model;

import gnu.io.SerialPort;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.StringReader;

import controller.C_SensorDataLine;
import controller.C_TemperatureMeasurementSystem;
import controller.C_UnreadSensorData;



/**
 * Holds important variables to set up the system
 * 
 * @author Hendrik Hangmann
 * @since  07.01.2015
 *
 */
public class M_TemperatureMeasurementSystem {
	
	static final public String OUTPUT_FILE_PATH = new String("output");  
	
	// refresh interval in millisecons
	static final public int REFRESH_INTERVAL = 100;
	static final public int SENSOR_GRID_WIDTH = 5, SENSOR_GRID_HEIGHT = 5;
	static final public int SUBDIVISION = 10, MAX_FPS = 20;
	
	static final public int MIN_TEMP = 30;
	static final public int MAX_TEMP = 190;

	// Serial Port parameters
	static final public int BAUD_RATE = 19200;
	static final public int SP_DATABITS = SerialPort.DATABITS_7;
	static final public int SP_STOPBITS = SerialPort.STOPBITS_1;
	static final public int SP_PARITY = SerialPort.PARITY_NONE;
	
	// for windows
	static public final String W_PORTNAME = "COM1";

	// for linux
	static public final String L_PORTNAME = "/dev/ttyUSB0";
	
	static public final String PORTNAME = L_PORTNAME;
	
	static final public int NUMBER_OF_HEATERS = 25;
	static final public int HEATER_MAX_ADJUSTABILITY = 32;
	static final public int HEATER_CALIBRATION_LEVEL = 16; 
	static final public int NUMBER_OF_SENSORS = 25;
	
	private InputStream in_stream;
	private OutputStream out_stream;
	private String output;
	private String tempOutput;
	

	private boolean calibrationMode = false;
	private boolean readMode = true;
	private boolean tempMode = false;
	
	private C_UnreadSensorData unreadSensorData;
	private C_TemperatureMeasurementSystem c;
	
	public M_TemperatureMeasurementSystem()
	{
		unreadSensorData=new C_UnreadSensorData();
		tempOutput = "";
	}

	public void setCalibrationMode(boolean bo)
	{
		calibrationMode=bo;
	}	
	
	public synchronized boolean getReadMode() {
		return readMode;
	}
	
	public void setReadMode(boolean b) {
		readMode=b;
	}

	public boolean getTemperatureMode() {
		return tempMode;
	}

	public void setTemperatureMode(boolean b) {
		tempMode=b;
	}
	public synchronized boolean getCalibrationMode()
	{
		return calibrationMode;
	}
	
	public InputStream getIn_stream() {
		return in_stream;
	}

	public void setIn_stream(InputStream in_stream) {
		this.in_stream = in_stream;
	}

	public OutputStream getOut_stream() {
		return out_stream;
	}

	public void setOut_stream(OutputStream out_stream) {
		this.out_stream = out_stream;
	}

	public String getOutput() {
		return output;
	}
	
	public C_SensorDataLine getNextCompleteLine()
	{
		return unreadSensorData.getNextUnreadLine();
	}

	public void addUnreadLine(String string) {
		 unreadSensorData.addLine(string);
	}

	public synchronized void addTempLine(String string) {
		 tempOutput += string;
	}

	public String getUnprintedTempLine() {
		 return tempOutput;
	}
}
