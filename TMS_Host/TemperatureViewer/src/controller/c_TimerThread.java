package controller;
import view.v_TemperatureMeasurementSystem;


public class c_TimerThread implements Runnable {

	int duration = 1000;
	int maxlevel = 32;
	c_TemperatureMeasurementSystem c_TMS;
	int count = 1;
	
	public c_TimerThread(int duration, int maxlevel, c_TemperatureMeasurementSystem c_TMS)
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
