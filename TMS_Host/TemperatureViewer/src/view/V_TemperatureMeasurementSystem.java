package view;

import controller.*;

import javax.swing.*;

import model.M_TemperatureMeasurementSystem;
import controller.C_SerialReader;
import controller.C_TemperatureControl;
import controller.C_TimerThread;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;

import java.io.File;
import java.io.FileDescriptor;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;


/**
 * Main window
 * 
 * @author Hendrik Hangmann
 * @since  07.01.2015
 *
 */
public class V_TemperatureMeasurementSystem implements ActionListener {
	// refresh interval in millisecons
//	static final int REFRESH_INTERVAL = 100;
//	static final int SENSOR_GRID_WIDTH = 5, SENSOR_GRID_HEIGHT = 5;
	//	static final int SUBDIVISION = 10, MAX_FPS = 20;
	
//	static private int min_temp = 30;
//	static private int max_temp = 100;
	
	private static JButton mStartCalibration, mStopCalibration, mShowTemperatureOnChip, mToggle3d, stopHeaters, start10mex;
	private M_TemperatureMeasurementSystem m_TMS;
	private final C_TemperatureMeasurementSystem c_TMS;
	
	private static V_TemperatureDistribution mChipView;
	private static JFrame mControlFrame;
	private V_Console consoleWindow;
	private OutputStream out;
	public V_Console outputWindow;
	int i = 0;

	
	
	/**
	 * Build up UI
	 * @param c_TMS Reference to controller class
	 */
	public V_TemperatureMeasurementSystem(final C_TemperatureMeasurementSystem c_TMS) 
	{
		this.m_TMS = c_TMS.getModel();
		this.c_TMS = c_TMS;
		mControlFrame = new JFrame("Temperature Measurement System");
		mControlFrame.setLayout(new FlowLayout());


		mStartCalibration = new JButton("start calibration");
		mStopCalibration = new JButton("stop calibration");
		stopHeaters = new JButton("reset heaters");
		start10mex = new JButton("start 10min experiment");
		mShowTemperatureOnChip = new JButton("Show temperature on Chip");
		mToggle3d = new JButton("Toggle 2D/3D");
		mStopCalibration.setEnabled(false);
		mToggle3d.setEnabled(false);

		mStartCalibration.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				c_TMS.startCalibration();
		    	mStartCalibration.setEnabled(false);
				mStopCalibration.setEnabled(true);
		//		mStartCalibration.setEnabled(false);
		//		mStopCalibration.setEnabled(true);

		//		mTempControl.startCalibration();
		//		c_TMS.getHeatControl().calibrate();
			}
		});
		
		stopHeaters.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				c_TMS.getHeatControl().stopHeaters();
			}
		});
		
		start10mex.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				start10MinExperiment();
			}
		});

		mStopCalibration.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
		    	mStartCalibration.setEnabled(true);
				mStopCalibration.setEnabled(false);
				c_TMS.stopCalibration();
			}
		});

		mShowTemperatureOnChip.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if (mChipView == null) {
					// new ChipView(mTempControl, pSubdivision, pMaxFPS);
					mChipView = new V_TemperatureDistribution(c_TMS.getTempControl(), M_TemperatureMeasurementSystem.MIN_TEMP, M_TemperatureMeasurementSystem.MAX_TEMP, M_TemperatureMeasurementSystem.SUBDIVISION, M_TemperatureMeasurementSystem.MAX_FPS);
				} else {
					mChipView.setVisible(true);
				}
				mToggle3d.setEnabled(true);
			}
		});

		mToggle3d.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				mChipView.toggle3d();
			}
		});
		
		//mTempControl.startCalibration();
		//mTempControl.startReading();
		//Thread.sleep(1000);
		//mTempControl.stopCalibration();

		mControlFrame.add(mStartCalibration);
		mControlFrame.add(mStopCalibration);
		mControlFrame.add(stopHeaters);
		mControlFrame.add(start10mex);
		mControlFrame.add(mShowTemperatureOnChip);
		mControlFrame.add(mToggle3d);
		mControlFrame.pack();
		mControlFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		mControlFrame.setVisible(true);
	}
	
	public static void main(String[] args) throws Exception {	
		File file = new File("output");
		if(file.exists()){
            file.delete();
        }
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		c_TMS.getHeatControl().setAllHeaters(i);				
	}
	
	public void start10MinExperiment()
	{		
		c_TMS.start10MinExperiment();		
	}
	

    	
    /**
     *  set all heaters to 0
     */
    public void resetHeaters()
    {
    	c_TMS.getHeatControl().stopHeaters();
    }
    

    
}

