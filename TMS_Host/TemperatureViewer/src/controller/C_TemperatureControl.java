package controller;
import java.io.FileNotFoundException;

import model.M_TemperatureMeasurementSystem;

import view.V_TemperatureMeasurementSystem;

/**
 * User: christoph
 * Date: 2/23/12
 * Time: 1:40 PM
 */
public class C_TemperatureControl {
	final int VCC = 1;

	private String mFileName;
	private int mRefreshInterval;
	private int mNumberOfSensors;
	private int mSensorGridWidth, mSensorGridHeight;

	private boolean mIsCalibrating;
	private C_Calibration mCalibration;
	private C_DataReader mDataReader;
	private boolean foundFirstEvenTemperatures;
	private C_TemperatureMeasurementSystem control;

	private double[] mCurrTemp;

	public C_TemperatureControl(C_TemperatureMeasurementSystem control, String pFileName, int pRefreshInterval, int pGridWidth, int pGridHeight) {
		mFileName = pFileName;
		mRefreshInterval = pRefreshInterval;
		mNumberOfSensors = pGridWidth * pGridHeight;
		mSensorGridWidth = pGridWidth;
		mSensorGridHeight = pGridWidth;
		this.control = control;
		mIsCalibrating = false;
		mCalibration = new C_Calibration(mNumberOfSensors);

		mCurrTemp = new double[mNumberOfSensors];

		foundFirstEvenTemperatures = false;
		
		mDataReader = new C_DataReader(control, this, mFileName, mRefreshInterval, mNumberOfSensors);
	}

	public synchronized void updateData(int[] pSensorData, double pTempData, double pVoltData) {
		if (control.getModel().getCalibrationMode()) {
			mCalibration.addData(pSensorData, pTempData, pVoltData - VCC);
			
//			if (foundFirstEvenTemperatures && mDataReader.findLevelTemperatures(pTempData, 0.55f)){
//				control.stopCalibration();
//				//stopCalibration();
//				foundFirstEvenTemperatures = false;
//				System.out.println("Reached second even (low) temperatures."); //chip fully calibrated
//			}
//			
//			if (!foundFirstEvenTemperatures && mDataReader.findLevelTemperatures(pTempData, 0.05f)){
//				foundFirstEvenTemperatures = true;
//				System.out.println("Reached first even (high) temperatures."); 
//				control.getHeatControl().setAllHeaters(0);
//			}	
		} else {
			mCalibration.getTemperature(mCurrTemp, pSensorData);
		}
	}

	public synchronized void startReading()  {
	}

	public synchronized void stopReading() {
		mDataReader.stopReading();
	}

	public synchronized void resetCalibration() {
		mCalibration.reset();
	}

	public synchronized void startCalibration() {
		mIsCalibrating = true;
		mDataReader.startReading();
	}

	public synchronized void stopCalibration() {
		mIsCalibrating = false;
		mCalibration.calibrate();
	}

	public synchronized double[] getTemperature() {

		return mCurrTemp;
	}
	
	public int getNumberOfSensors() {
		return mNumberOfSensors;
	}
	
	public int getSensorGridWidth() {
		return mSensorGridWidth;
	}
	
	public int getSensorGridHeight() {
		return mSensorGridHeight;
	}
}
