package controller;

public class C_TimerThread implements Runnable {

	int duration = 1000;
	int maxlevel = 32;
	C_TemperatureMeasurementSystem c_TMS;
	int count = 1;
	
	public C_TimerThread(int duration, int maxlevel, C_TemperatureMeasurementSystem c_TMS)
	{
		this.duration = duration;
		this.c_TMS = c_TMS;
		this.maxlevel = maxlevel;
	}
	
	@Override
	public void run() {
		while (count<=maxlevel){
			try {
				Thread.sleep(duration);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			c_TMS.getHeatControl().setAllHeaters(count);
			count++;
		}
		
	}

}
