package model;

import java.io.InputStream;
import java.io.OutputStream;

import gnu.io.SerialPort;

public class m_TemperatureMeasurementSystem {
	
	static final public String OUTPUT_FILE_PATH = new String("output");  
	
	// refresh interval in millisecons
	static final public int REFRESH_INTERVAL = 100;
	static final public int SENSOR_GRID_WIDTH = 5, SENSOR_GRID_HEIGHT = 5;
	static final public int SUBDIVISION = 10, MAX_FPS = 20;
	
	static final public int MIN_TEMP = 30;
	static final public int MAX_TEMP = 100;

	// Serial Port parameters
	static final public int BAUD_RATE = 115200;
	static final public int SP_DATABITS = SerialPort.DATABITS_8;
	static final public int SP_STOPBITS = SerialPort.STOPBITS_1;
	static final public int SP_PARITY = SerialPort.PARITY_NONE;
	
	// for windows
	static public final String W_PORTNAME = "COM1";

	// for linux
	static public final String L_PORTNAME = "/dev/tty1";
	
	static public final String PORTNAME = W_PORTNAME;
	
	static final public int NUMBER_OF_HEATERS = 22;
	static final public int HEATER_ADJUSTABILITY = 32;
	static final public int NUMBER_OF_SENSORS = 25;
	
	private InputStream in_stream;
	private OutputStream out_stream;
	
	public m_TemperatureMeasurementSystem()
	{
		
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

}
