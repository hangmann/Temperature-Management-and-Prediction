package controller;

import java.io.IOException;

import model.m_TemperatureMeasurementSystem;
import view.v_HeaterControl;

public class c_HeaterControl {

	v_HeaterControl v_HC;
	c_TemperatureMeasurementSystem c_TMS;
	
	public c_HeaterControl(c_TemperatureMeasurementSystem c_TMS)
	{
		this.c_TMS = c_TMS;
		v_HC = new v_HeaterControl(c_TMS);
		
	}
	

	public void setAllHeaters(int j){
		if (j >= 0 && j <= 32){	
			for (int i = 0;  i < m_TemperatureMeasurementSystem.NUMBER_OF_HEATERS; i++)
			{	
				v_HC.setComboIndex(i, j);
	
			//	setHeater(i,j);
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
	}
	
	
	public void calibrate(){
		
		for (int i = 0; i<m_TemperatureMeasurementSystem.NUMBER_OF_HEATERS; i++)
		{	
			v_HC.setComboIndex(i,m_TemperatureMeasurementSystem.HEATER_ADJUSTABILITY);

			//setHeater(i,adjustability);
			try {
				Thread.sleep(50);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		c_TMS.getSerialCommunication().appendText("\nStarting...\n");
		
		try {
			Thread.sleep(500);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		startExperiment();		
	}
	
	public void stopHeaters(){
		//controller.stopCalibration();
		
		for (int i = 0; i<m_TemperatureMeasurementSystem.NUMBER_OF_HEATERS; i++)
		{	
			v_HC.setComboIndex(i,0);
			//setHeater(i,0);
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
	}
	
	public void sendMsg(String msg)
	{
		try {
			
			for (int i=0; i<msg.getBytes().length;i++)
				c_TMS.getModel().getOut_stream().write(msg.getBytes()[i]);
			
			c_TMS.getModel().getOut_stream().write("\n".getBytes());
		} catch (IOException e) {
			// 
			System.out.println(e);
		}
		
	}
	
	public void setHeater(int number, int intensity)
	{
		sendMsg("setheater " + idToString(number) + " " + idToString(intensity));
	}
	
	public void getHeater(int number)
	{
		sendMsg("getheater " + idToString(number));
	}
	
	public void getTemperature()
	{
		sendMsg("gettemp");
	}
	
	public void getCounts()
	{
		sendMsg("getcounts");
	}


	public void getVCC() {
		sendMsg("getvcc");
	}

	public void stopExperiment() {
		sendMsg("stopex");
	}
	
	public void startExperiment() {
		sendMsg("startex");
		c_TMS.getSerialCommunication().clearText();
	}


	public void toggleOutput() {
		// TODO Auto-generated method stub
		sendMsg("toggleoutput");
	}


	public void setOutput(int parseInt) {
		sendMsg("setoutput " + idToString(parseInt));
	}
	
	public String idToString(int id)
	{
        String si;
        
    	if (id<10)
    		si = "0"+id;
    	else
    		si=String.valueOf(id);
    	        
        return si;
	}
	
}
