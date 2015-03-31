package controller;
import java.io.*;
import java.util.ArrayList;

import model.M_TemperatureMeasurementSystem;

import view.V_TemperatureMeasurementSystem;

/**
 * User: christoph
 * Date: 2/23/12
 * Time: 1:39 PM
 */
public class C_DataReader {
	static final int WAIT_FOR_NEW_CHARACTERS_TIME = 5;
	
	private enum ReadingStates {startOfNewLine, counter, sensorData, tempData, voltData}

	private C_TemperatureControl mTempContol;
	private String mFileName;
	private int mRefreshInterval;
	private int mNumberOfSensor;
	private ReaderThread mReaderThread;
	private Double [] tempData;
	private C_TemperatureMeasurementSystem control;
	private float delta = 0.1f;
	private boolean [] even;
	private boolean alleven;
	private int requiredEvenValues;

	public C_DataReader(C_TemperatureMeasurementSystem control,C_TemperatureControl pTempControl, String pFileName, int pRefreshInterval, int pNumberOfSensors) {
		mTempContol = pTempControl;
		this.control = control;
		mFileName = pFileName;
		mRefreshInterval = pRefreshInterval;
		mNumberOfSensor = pNumberOfSensors;
		mReaderThread = new ReaderThread();
		tempData= new Double[32];	
		requiredEvenValues = 32;
		alleven = false;
		even = new boolean[requiredEvenValues];
		
		for (int i = 0; i<32;i++)
		{
			tempData[i]=-1d;
		}
	}

	public synchronized void startReading()  {
		mReaderThread.startReading();
	}

	public synchronized void stopReading() {
		mReaderThread.stopReading();
		// wait until Thread is terminated
		try {
			mReaderThread.join();
		} catch (InterruptedException e) {
			System.err.println(e);
		}
	}
	
	public boolean findLevelTemperatures(double mTempData, float delta) {
		
		for (int i = 0; i<requiredEvenValues-1;i++){
			tempData[i] = tempData[i+1];
		}
		
		tempData[requiredEvenValues-1] = mTempData;
		
//		System.out.println(tempData[0] + " " + tempData[1] + " " + tempData[2] + " " + tempData[3]);
		
	
		if (tempData[0] != 0d && tempData[requiredEvenValues-1] != 0d){
			if (Math.abs(tempData[0] - tempData[requiredEvenValues-1]) <= delta){
				even[0]=true;
				even[requiredEvenValues-1]=true;
				for (int i = 1; i<requiredEvenValues-1;i++){
					if (Math.abs(tempData[0] - tempData[i]) <= delta)
					{
						even[i] = true;
					}
				}
				
				for (int i = 0; i< requiredEvenValues;i++)
				{
					if (!even[i]){
						alleven = false;
						return false;
					}else {
						for (int j = 0; j< requiredEvenValues;j++)
						{
							even[j]=false;
						}
					}
					alleven = true;
				}
				
			}
		}
		return false;
	}

	private class ReaderThread extends Thread {
		private volatile boolean mReading;
		private ReadingStates mState;
		private RandomAccessFile mFileReader;
		private boolean linesToRead;

		private double mTempData;
		private double mVoltData;
		private int[] mSensorData;
		private int mCurrSensorNumber;
		private String data;

		C_SensorDataLine line;
		
		public ReaderThread() {
			mReading = false;

			mState = ReadingStates.startOfNewLine;
			linesToRead = true;

			mSensorData = new int[mNumberOfSensor];
			mCurrSensorNumber = 0;
			data = "";
			
		}

		public synchronized void run() {
//			int ci = -1;
//			char c = ' ';
//
//			while (mReading) {
//				while (linesToRead) {
//					// read new character
//					try {
//						ci = mFileReader.read();
//					} catch (IOException e) {
//						System.err.println(e);
//					}
//
//					// if no more characters to read
//					if (ci == -1) {
//						if (mState == ReadingStates.startOfNewLine) {
//							// if startOfNewLine, we must wait for new Line
//							linesToRead = false;
//						} else {
//							// otherwise we have to wait for new characters,
//							// because the file is altered by minicom while reading
//							waitForNewCharacters();
//						}
//					} else {
//						stateMachine(ci, (char) ci);
//					}
//
//				}
//
//				// wait for new Line
//				linesToRead = true;
//				try {
//					Thread.sleep(mRefreshInterval);
//					// reload file
//					reloadFile();
//				} catch (InterruptedException e) {
//					System.err.println(e);
//				}
//			}
//
//			try {
//				mFileReader.close();
//			} catch (IOException e) {
//				System.err.println(e);
//			}
		}

		private void reloadFile() {
			long filePos;

			try {
				filePos = mFileReader.getFilePointer();
				mFileReader.close();
				mFileReader = new RandomAccessFile(mFileName, "r");
				mFileReader.seek(filePos);
			} catch (IOException e) {
				System.err.println(e);
			}
		}

		private void stateMachine(int ci, char c) {

			System.out.println("STATE MACHINE! mSTATE is: " + mState);
			switch (mState) {
				case startOfNewLine:
					// start of a new line
					mState = ReadingStates.counter;
					break;
				case counter:
					// skip the counter
					if (c == ' ') {
						data = "";
						mCurrSensorNumber = 0;
						
						mState = ReadingStates.sensorData;
					}
					break;
				case sensorData:
					// if space, store data, increase sensor number
					// and go to tempData, if all sensors are red
					// otherwise continue reading sensor data
					if (c == ' ') {
						mSensorData[mCurrSensorNumber] = Integer.parseInt(data);
						data = "";

						mCurrSensorNumber++;
						//System.out.println(mCurrSensorNumber + ": " + mSensorData[mCurrSensorNumber]);
						if (mCurrSensorNumber == mNumberOfSensor) {
							mState = ReadingStates.tempData;
						}
					} else {
						data += c;
					}
					break;
				case tempData:
					// if space, go to voltData
					// otherwise read temperature Data
					if (c == ' ') {
						
						mTempData = Double.parseDouble(data);
						if (findLevelTemperatures(mTempData, delta))
						{
							mTempContol.stopCalibration();
						}
						
						data = "";

						mState = ReadingStates.voltData;
					} else {
						data += c;
					}
					break;
				case voltData:
					// if new line, goto waitingLineBreak
					// otherwise read voltage Data
					if (c == '\n') {
						mVoltData = Double.parseDouble(data);
						data = "";

						// safe data to control
						sendData();

						mState = ReadingStates.startOfNewLine;
					} else {
						data += c;
					}
					break;
			}
		}

		private void waitForNewCharacters() {
			try {
				Thread.sleep(WAIT_FOR_NEW_CHARACTERS_TIME);
				// reload file
				reloadFile();
			} catch (InterruptedException e) {
				System.err.println(e);
			}
		}

		private void sendData() {
			mTempContol.updateData(mSensorData, mTempData, mVoltData);
		}

		public void startReading()  {
		//	mFileReader = new RandomAccessFile(new File(mFileName), "r");
		//	mReading = true;
		//	this.start();
		}

		public void stopReading() {
			mReading = false;
			this.interrupt();
		}
	}

	
}
