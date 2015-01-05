package controller;
import java.io.FileNotFoundException;

import view.v_TemperatureMeasurementSystem;

/**
 * User: christoph
 * Date: 2/23/12
 * Time: 1:40 PM
 */
public class TemperatureControl {
	final int VCC = 1;

	private String mFileName;
	private int mRefreshInterval;
	private int mNumberOfSensors;
	private int mSensorGridWidth, mSensorGridHeight;

	private boolean mIsCalibrating;
	private Calibration mCalibration;
	private DataReader mDataReader;

	private double[] mCurrTemp;

	public TemperatureControl(v_TemperatureMeasurementSystem control, String pFileName, int pRefreshInterval, int pGridWidth, int pGridHeight) {
		mFileName = pFileName;
		mRefreshInterval = pRefreshInterval;
		mNumberOfSensors = pGridWidth * pGridHeight;
		mSensorGridWidth = pGridWidth;
		mSensorGridHeight = pGridWidth;

		mIsCalibrating = false;
		mCalibration = new Calibration(mNumberOfSensors);

		mCurrTemp = new double[mNumberOfSensors];

		mDataReader = new DataReader(control, this, mFileName, mRefreshInterval, mNumberOfSensors);
	}

	public synchronized void updateData(int[] pSensorData, double pTempData, double pVoltData) {
		if (mIsCalibrating) {
			mCalibration.addData(pSensorData, pTempData, pVoltData - VCC);
		} else {
			mCalibration.getTemperature(mCurrTemp, pSensorData);
		}
	}

	public synchronized void startReading() throws FileNotFoundException {
		mDataReader.startReading();
	}

	public synchronized void stopReading() {
		mDataReader.stopReading();
	}

	public synchronized void resetCalibration() {
		mCalibration.reset();
	}

	public synchronized void startCalibration() {
		mIsCalibrating = true;
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
