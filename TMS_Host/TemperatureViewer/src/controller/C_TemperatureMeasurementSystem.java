package controller;
import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.UnsupportedCommOperationException;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import model.*;
import view.*;

/**
 * Controller class for main window
 * 
 * @author Hendrik Hangmann
 * @since  07.01.2015
 *
 */
public class C_TemperatureMeasurementSystem {

	private V_TemperatureMeasurementSystem v_TMS;
	private M_TemperatureMeasurementSystem m_TMS;	
	private C_SerialCommumication c_SCom;
	private C_HeaterControl c_HC;
	private C_TemperatureControl mTempControl;
	SerialPort serialPort;
	
	/**
	 * Constructor opens and initializes serial port 
	 */
	public C_TemperatureMeasurementSystem() {


		this.m_TMS = new M_TemperatureMeasurementSystem();
		this.c_HC = new C_HeaterControl(this);
		
		this.v_TMS = new V_TemperatureMeasurementSystem(this);
				
		
		mTempControl = new C_TemperatureControl(this, "output", M_TemperatureMeasurementSystem.REFRESH_INTERVAL, M_TemperatureMeasurementSystem.SENSOR_GRID_WIDTH, M_TemperatureMeasurementSystem.SENSOR_GRID_HEIGHT);
		
		// start and configure serial port streams
		try {
			CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(M_TemperatureMeasurementSystem.PORTNAME);

            CommPort commPort;
			commPort = portIdentifier.open("Temperature Measurement System", 2000);
			
            serialPort = (SerialPort) commPort;
            serialPort.setSerialPortParams(M_TemperatureMeasurementSystem.BAUD_RATE,M_TemperatureMeasurementSystem.SP_DATABITS ,M_TemperatureMeasurementSystem.SP_STOPBITS,M_TemperatureMeasurementSystem.SP_PARITY);
            m_TMS.setIn_stream(serialPort.getInputStream());
            m_TMS.setOut_stream(serialPort.getOutputStream());
            c_SCom = new C_SerialCommumication(this);

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
		
		//loadCalibration();
	}
	

	public M_TemperatureMeasurementSystem getModel()
	{
		return m_TMS;
	}
	
	public C_SerialCommumication getSerialCommunication()
	{
		return c_SCom;
	}
	
	public C_HeaterControl getHeatControl()
	{
		return c_HC;
	}
	
    /** 
     *  Starts Calibration routine
     */
    public void startCalibration() {

		m_TMS.setReadMode(true);
		m_TMS.setCalibrationMode(true);
		getHeatControl().startExperiment();
	}
    
    /**
     *  Stops Calibration
     */
    public void stopCalibration() {

		mTempControl.stopCalibration();
		//getHeatControl().stopCalibration();
		m_TMS.setCalibrationMode(false);
		getSerialCommunication().appendText("\n1: Stopping Calibration...\n");
		

		Double [][] param = getTempControl().getCalibrationParameter();
		for (int i = 0; i < m_TMS.NUMBER_OF_SENSORS; i++) {
			System.out.println("Sensor " + i + ": A = " + param[i][0] + ", B = " + param[i][1]);
		}
		
		//getSerialCommunication().clearText();
		
	}

	public void startHeatExperiment() {
		//getHeatControl().startExperiment();
		//heatControl.setOutput(0);
		Thread t = new Thread(new C_ExperimentThread(16, this));
		    
	    t.start();
	}

	public void startRandomHeaterExperiment() {
		//getHeatControl().startExperiment();
		//heatControl.setOutput(0);
		Thread t = new Thread(new C_RandomExperimentThread(16, this));
		    
	    t.start();
	}

	public C_TemperatureControl getTempControl() {
		return mTempControl;
	}

	public void renewOutputStream()
	{
		try {

            m_TMS.setOut_stream(serialPort.getOutputStream());
		
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println("I/O Error.");
			System.exit(0);
		}
		
	}
	
	public void loadCalibration()
	{
		Double[][] cal = new Double[m_TMS.NUMBER_OF_SENSORS][2];
		
		BufferedReader br = null;
		try {
			br = new BufferedReader(new FileReader(m_TMS.CALIBRATION_FILE_PATH));
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	    try {
	        StringBuilder sb = new StringBuilder();
	        String line = br.readLine();
	        int i = 0;
	        int j = 0;
	        
	        while (line != null) {
	            cal[i][j] = Double.valueOf(line);
	            j = (j==0) ? 1 : 0;
	            line = br.readLine();
	            if (j==0) 
		            i++;
	        }
	        String everything = sb.toString();
	    } catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} finally {
			mTempControl.loadCalibration(cal);
			m_TMS.setTemperatureMode(true);
	        try {
				br.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
	    }
	}

}
