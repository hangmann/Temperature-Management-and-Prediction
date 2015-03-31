package controller;

public class C_ExperimentThread implements Runnable {

	private enum States
	{
		sWait,
		sAll,
		sTopFive,
		sBottomFive,
		sRightFive,
		sLeftFive,
		sCoolDown,
		sExit,
	}
	
	private int maxlevel = 32;
	private C_TemperatureMeasurementSystem c_TMS;
	private int count = 1;
	private States heatingStates;
		
	public C_ExperimentThread(int maxlevel, C_TemperatureMeasurementSystem c_TMS)
	{
		this.c_TMS = c_TMS;
		this.maxlevel = maxlevel;
		
		heatingStates = States.sWait;
		
	}
	
	@Override
	public void run() {
		
		switch(heatingStates)
		{
		case sWait:
			c_TMS.getHeatControl().setAllHeaters(0);
			wait(2*60*1000);
			heatingStates = States.sAll;
		case sAll:
			c_TMS.getHeatControl().setAllHeaters(maxlevel);
			wait(10*60*1000);
			heatingStates = States.sTopFive;
		case sTopFive:
			//4 9 11 16 21 
			c_TMS.getHeatControl().set5Heaters(4, 9, 14, 19, 24, maxlevel);
	/*		c_TMS.getHeatControl().setAllHeaters(0);
			wait(100);
			c_TMS.getHeatControl().setHeater(4, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(9, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(14, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(19, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(24, maxlevel);*/
			try {
				Thread.sleep(5*60*1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			heatingStates = States.sBottomFive;
		case sBottomFive:
			c_TMS.getHeatControl().set5Heaters(0, 5, 10, 15, 20, maxlevel);
		/*	c_TMS.getHeatControl().setAllHeaters(0);
			wait(100);
			c_TMS.getHeatControl().setHeater(0, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(5, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(10, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(15, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(20, maxlevel);
*/
			wait(10*60*1000);
			heatingStates = States.sRightFive;
		case sRightFive:
			c_TMS.getHeatControl().set5Heaters(20, 21, 22, 23, 24, maxlevel);
		/*	c_TMS.getHeatControl().setAllHeaters(0);
			wait(100);
			c_TMS.getHeatControl().setHeater(20, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(21, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(22, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(23, maxlevel);
			wait(100);
			c_TMS.getHeatControl().setHeater(24, maxlevel);
*/
			wait(10*60*1000);
			heatingStates = States.sLeftFive;
		case sLeftFive:
			//4 9 11 16 21 
			c_TMS.getHeatControl().set5Heaters(0, 1, 2, 3, 4, maxlevel);
		/*	c_TMS.getHeatControl().setAllHeaters(0);
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
*/
			wait(10*60*1000);
			heatingStates = States.sCoolDown;
		case sCoolDown: 

			c_TMS.getHeatControl().setAllHeaters(0);
			wait(8*60*1000);
			heatingStates = States.sExit;
		case sExit:
			//reset timer, clear, new text file
			heatingStates = States.sWait;
			c_TMS.getSerialCommunication().get_SR().restart();
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
