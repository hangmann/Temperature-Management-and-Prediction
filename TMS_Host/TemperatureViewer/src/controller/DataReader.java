package controller;
import java.io.*;
import java.util.ArrayList;

import view.v_TemperatureMeasurementSystem;

/**
 * User: christoph
 * Date: 2/23/12
 * Time: 1:39 PM
 */
public class DataReader {
	static final int WAIT_FOR_NEW_CHARACTERS_TIME = 5;
	
	private enum ReadingStates {startOfNewLine, counter, sensorData, tempData, voltData}

	private TemperatureControl mTempContol;
	private String mFileName;
	private int mRefreshInterval;
	private int mNumberOfSensor;
	private ReaderThread mReaderThread;
	private ArrayList<Double> tempData;
	private v_TemperatureMeasurementSystem control;

	public DataReader(v_TemperatureMeasurementSystem control,TemperatureControl pTempControl, String pFileName, int pRefreshInterval, int pNumberOfSensors) {
		mTempContol = pTempControl;
		control = control;
		mFileName = pFileName;
		mRefreshInterval = pRefreshInterval;
		mNumberOfSensor = pNumberOfSensors;
		mReaderThread = new ReaderThread();
		tempData= new ArrayList<Double>();																	
	}

	public synchronized void startReading() throws FileNotFoundException {
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
	
	public void findChanges(double mTempData) {
		if (tempData.size() >9){
			System.out.println(Math.abs(tempData.get(0) - tempData.get(9)) + " <= 20?\n");
			if (Math.abs(tempData.get(0) - tempData.get(9)) <= 20){
				control.resetHeaters();
			} else {
				for (int i = 0; i<10-1;i++){
					tempData.set(i, tempData.get(i+1));
				}
				tempData.remove(9);
			}
		}
		tempData.add(mTempData);
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

		public ReaderThread() {
			mReading = false;

			mState = ReadingStates.startOfNewLine;
			linesToRead = true;

			mSensorData = new int[mNumberOfSensor];
			mCurrSensorNumber = 0;
			data = "";
		}

		public void run() {
			int ci = -1;
			char c = ' ';

			while (mReading) {
				while (linesToRead) {
					// read new character
					try {
						ci = mFileReader.read();
					} catch (IOException e) {
						System.err.println(e);
					}

					// if no more characters to read
					if (ci == -1) {
						if (mState == ReadingStates.startOfNewLine) {
							// if startOfNewLine, we must wait for new Line
							linesToRead = false;
						} else {
							// otherwise we have to wait for new characters,
							// because the file is altered by minicom while reading
							waitForNewCharacters();
						}
					} else {
						stateMachine(ci, (char) ci);
					}

				}

				// wait for new Line
				linesToRead = true;
				try {
					Thread.sleep(mRefreshInterval);
					// reload file
					reloadFile();
				} catch (InterruptedException e) {
					System.err.println(e);
				}
			}

			try {
				mFileReader.close();
			} catch (IOException e) {
				System.err.println(e);
			}
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
						// TODO Scan for non-increasing non-decreasing temperature rises
						findChanges(mTempData);
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

		public void startReading() throws FileNotFoundException {
			mFileReader = new RandomAccessFile(new File(mFileName), "r");
			mReading = true;
			this.start();
		}

		public void stopReading() {
			mReading = false;
		}
	}

	
}
