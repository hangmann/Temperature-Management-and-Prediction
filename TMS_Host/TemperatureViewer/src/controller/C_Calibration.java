package controller;


public class C_Calibration {
	final int ITERATIONS = 10;
	final int STEPCOUNT = 10;
	
	private int mNumberOfSensors;
	private int mCount;
	private SingleCalibration[] mCalibrations;

	public C_Calibration(int pNumberOfSensors) {
		mNumberOfSensors = pNumberOfSensors;
		mCalibrations = new SingleCalibration[mNumberOfSensors];
		
		for (int i = 0; i < mNumberOfSensors; i++) {
			mCalibrations[i] = new SingleCalibration();
		}

		reset();
	}

	public synchronized void addData(int[] pSensorData, double pTempData, double pVoltData) {
		mCount++;
		for (int i = 0; i < mNumberOfSensors; i++) {
			mCalibrations[i].addData(pSensorData[i], pTempData, pVoltData);
		}
	}

	public void getTemperature(double[] pTempData, int[] pSensorData) {
		for (int i = 0; i < mNumberOfSensors; i++) {
			pTempData[i] = mCalibrations[i].getTemperature(pSensorData[i]);
		}
	}
	
	public void loadCalibration(Double [][] cal)
	{
		for (int i = 0; i < mNumberOfSensors; i++) {
			mCalibrations[i].setA(cal[i][0]);
			mCalibrations[i].setB(cal[i][1]);
		}
	}

	public Double[][] getCalibrationParameters()
	{
		Double[][] ret = new Double[mNumberOfSensors][2];
		for (int i = 0; i < mNumberOfSensors; i++) {
			ret[i][0] = mCalibrations[i].getA();
			ret[i][1] = mCalibrations[i].getB();
		}
		return ret;
	}
	
	public void calibrate() {

		System.out.println("Calibrating...");
		double c = 0;
		
		for (int i = 0; i < mNumberOfSensors; i++) {
			mCalibrations[i].calcCalibrationResult();
			c += mCalibrations[i].getC();
		}
		
		c /= mNumberOfSensors;
		for (int i = 0; i < mNumberOfSensors; i++) {
			mCalibrations[i].setC(c);
			mCalibrations[i].calcAB();
		}
	}

	public void reset() {
		mCount = 0;
		for (int i = 0; i < mNumberOfSensors; i++) {
			mCalibrations[i].reset();
		}
	}

	public class SingleCalibration {
		private double mXSum, mYSum, mVSum;
		private double mXYSum, mXVSum, mYVSum;
		private double mXXSum, mYYSum, mVVSum;
		private double mXAvg, mCorrYAvg;
		private double mCorrXY, mXX, mCorrYY;
		private double a, b, c;

		public SingleCalibration() {
			reset();
		}

		public void addData(int pX, double pY, double pV) {
			mXSum += pX;
			mYSum += pY;
			mVSum += pV;
			mXYSum += pX * pY;
			mXVSum += pX * pV;
			mYVSum += pY * pV;
			mXXSum += pX * pX;
			mYYSum += pY * pY;
			mVVSum += pV * pV;
		}

		public void calcCalibrationResult() {
			double rSquare;
			double begin, end;
			double stepsize;
			double maxC, maxValue;

			maxC = 0;
			maxValue = 0;
			begin = 200;
			end = 500;

			calcXAvg();
			calcXX();

			for (int i = 0; i < ITERATIONS; i++) {
				stepsize = (end - begin) / STEPCOUNT;
				for (int j = 0; j <= STEPCOUNT; j++) {
					c = begin + stepsize * j;
					rSquare = calcRSquare();
					
					if (rSquare > maxValue) {
						maxC = c;
						maxValue = rSquare;
					}
				}
				begin = maxC - stepsize;
				end = maxC + stepsize;
			}
			
			c = maxC;
			
			//System.out.println(c);
		}

		public double calcRSquare() {
			calcCorrYAvg();
			calcCorrXY();
			calcCorrYY();

			return (mCorrXY * mCorrXY) / (mXX * mCorrYY);
		}

		public void calcXAvg() {
			mXAvg = mXSum / mCount;
		}

		public void calcCorrYAvg() {
			mCorrYAvg = (mYSum - c * mVSum) / mCount;
		}

		public void calcCorrXY() {
			
			mCorrXY = mCount * mXAvg * mCorrYAvg;
			mCorrXY += mXYSum - c * mXVSum;
			mCorrXY -= mCorrYAvg * mXSum;
			mCorrXY -= mXAvg * (mYSum - c * mVSum);
		}

		public void calcXX() {
			mXX = mXXSum;
			mXX += mCount * mXAvg * mXAvg;
			mXX -= 2 * mXAvg * mXSum;
		}

		public void calcCorrYY() {
			mCorrYY = mYYSum - 2 * c * mYVSum + c * c* mVVSum;
			mCorrYY += mCount * mCorrYAvg * mCorrYAvg;
			mCorrYY -= 2 * mCorrYAvg * (mYSum - c * mVSum);
		}

		public void reset() {
			mXSum = 0;
			mYSum = 0;
			mVSum = 0;
			mXYSum = 0;
			mXVSum = 0;
			mYVSum = 0;
			mXXSum = 0;
			mYYSum = 0;
			mVVSum = 0;
			mCorrYY = 0;
			mCorrXY = 0;
			a = 0;
			b = 0;
			c = 0 ;
			mCorrYAvg = 0 ;
			mXAvg = 0 ;
			mXX = 0;
		}
		
		public void calcAB() {
			a = getA();
			b = getB();
		}
		
		public double getA() {
			calcCorrXY();
			return mCorrXY / mXX;
		}
		
		public void setA(double a)
		{
			this.a = a;
		}
		
		public void setB(double b)
		{
			this.b = b;
		}

		public double getB() {
			calcCorrYAvg();
			return mCorrYAvg - getA() * mXAvg;
		}
		
		public double getC() {
			return c;
		}
		
		public void setC(double pC) {
			c = pC;
		}

		public double getTemperature(int pSensorData) {
			return a * pSensorData + b;
		}
	}
}
