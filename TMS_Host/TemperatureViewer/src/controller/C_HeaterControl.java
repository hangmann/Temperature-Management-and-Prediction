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
	

	public void setAllHeaters(int j){
		if (j >= 0 && j <= 32){	
			for (int i = 0;  i < M_TemperatureMeasurementSystem.NUMBER_OF_HEATERS; i++)
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
