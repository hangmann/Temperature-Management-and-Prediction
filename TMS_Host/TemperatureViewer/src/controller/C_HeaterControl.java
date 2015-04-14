package controller;

import java.io.IOException;

import model.M_TemperatureMeasurementSystem;
import view.V_HeaterControl;

public class C_HeaterControl {

	V_HeaterControl v_HC;
	C_TemperatureMeasurementSystem c_TMS;
	
	public C_HeaterControl(C_TemperatureMeasurementSystem c_TMS)
	{
		this.c_TMS = c_TMS;
		v_HC = new V_HeaterControl(c_TMS, this);
		
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

	public void startCalibration(){
		
		setAllHeaters(M_TemperatureMeasurementSystem.HEATER_CALIBRATION_LEVEL );
		
		c_TMS.getSerialCommunication().appendText("\nStarting Calibration...\n");
		
		try {
			Thread.sleep(500);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		startExperiment();		
	}
	
	public void stopCalibration(){
		
		setAllHeaters(0);
		
		
		try {
			Thread.sleep(500);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}
	
	public void stopHeaters(){
		//controller.stopCalibration();
		setAllHeaters(0);
		
	}
	
	public void sendMsg(String msg, boolean n)
	{
		try {

			if (n) msg = msg + "!";
			for (int i=0; i<msg.getBytes().length;i++)
			{
				c_TMS.getModel().getOut_stream().write(msg.getBytes()[i]);
			}
			c_TMS.getModel().getOut_stream().flush();
			c_TMS.getModel().getOut_stream().close();
			c_TMS.renewOutputStream();
			
		} catch (IOException e) {
			// 
			System.out.println(e);
		}
		
	}
	
	public void setAllHeaters(int j){

		sendMsg("setallheaters " + (j),true);
	}


	public void setHeater(int number, int intensity)
	{
		sendMsg("setheater " + number + " " + (intensity), true);
	}
	
	public void set5Heaters(int h0, int h1, int h2, int h3, int h4, int intensity)
	{
		sendMsg("set5heaters " + (h0) + " " +(h1) + " " + (h2) + " " + (h3) + " " + (h4) +  " " + (intensity), true);
	}
	
	public void setTopHeaters(int intensity)
	{
		sendMsg("settopheaters " + (intensity), true);
	}
	
	public void setBottomHeaters(int intensity)
	{
		sendMsg("setbottomheaters " + (intensity), true);
	}
	
	public void setRightHeaters(int intensity)
	{
		sendMsg("setrightheaters " + (intensity), true);
	}
	
	public void setLeftHeaters(int intensity)
	{
		sendMsg("setleftheaters " + (intensity), true);
	}
	
	public void getHeater(int number)
	{
		sendMsg("getheater " + (number),true);
	}
	
	public void getTemperature()
	{
		sendMsg("gettemp",true);
	}
	
	public void getCounts()
	{
		sendMsg("getcounts",true);
	}


	public void getVCC() {
		sendMsg("getvcc",true);
	}

	public void stopExperiment() {
		sendMsg("stopex",true);
	}
	
	public void startExperiment() {
		sendMsg("startex",true);
	}


	public void toggleOutput() {
		sendMsg("toggleoutput",true);
	}


	public void setOutput(int parseInt) {
		sendMsg("setoutput " + (parseInt),true);
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
