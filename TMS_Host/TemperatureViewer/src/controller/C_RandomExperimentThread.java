package controller;

public class C_RandomExperimentThread implements Runnable {

	private enum States
	{
		sAll,
		sTopFive,
		sBottomFive,
		sRightFive,
		sLeftFive,
		sExit,
	}
	
	private int maxlevel = 32;
	private C_TemperatureMeasurementSystem c_TMS;
	private int count = 1;
	private States heatingStates;
		
	public C_RandomExperimentThread(int maxlevel, C_TemperatureMeasurementSystem c_TMS)
	{
		this.c_TMS = c_TMS;
		this.maxlevel = maxlevel;
		
		heatingStates = States.sAll;
		
	}
	
	@Override
	public void run() {
		
		switch(heatingStates)
		{
		case sAll:
			c_TMS.getHeatControl().setAllHeaters(maxlevel);
			wait(10*60*1000);
			heatingStates = States.sTopFive;
		case sTopFive:
			//4 9 11 16 21 
			c_TMS.getHeatControl().setAllHeaters(0);
			wait(100);
			c_TMS.getHeatControl().setHeater(4, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(9, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(11, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(16, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(21, maxlevel);
			try {
				Thread.sleep(5*60*1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			heatingStates = States.sBottomFive;
		case sBottomFive:
			c_TMS.getHeatControl().setAllHeaters(0);
			wait(100);
			c_TMS.getHeatControl().setHeater(0, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(5, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(10, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(12, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(17, maxlevel);

			wait(5*60*1000);
			heatingStates = States.sRightFive;
		case sRightFive:
			c_TMS.getHeatControl().setAllHeaters(0);
			wait(100);
			c_TMS.getHeatControl().setHeater(17, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(18, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(19, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(20, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(21, maxlevel);

			wait(5*60*1000);
			heatingStates = States.sLeftFive;
		case sLeftFive:
			//4 9 11 16 21 
			c_TMS.getHeatControl().setAllHeaters(0);
			wait(100);
			c_TMS.getHeatControl().setHeater(0, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(1, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(2, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(3, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(4, maxlevel);

			wait(5*60*1000);
			heatingStates = States.sExit;
		case sExit: 

			c_TMS.getHeatControl().setAllHeaters(0);
		}
		
	}
	
	public void wait(int duration)
	{
		try {
			Thread.sleep(duration);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	/*	while (count<=maxlevel){
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
	*/

}
