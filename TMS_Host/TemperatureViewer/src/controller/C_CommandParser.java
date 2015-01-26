package controller;
import java.util.ArrayList;
import java.util.Scanner;

import view.*;

public class C_CommandParser {
	private Scanner scanner;
	private boolean match = false;	
	private C_SerialCommumication c_SR;
	private C_HeaterControl c_HC;
	private C_TemperatureMeasurementSystem c_TMS;
	private ArrayList<String> inputs;
	private String commands []= {	"help", 
							"setheater \\w+ \\w+", 
							"getheater \\w+",
							"gettemp",
							"getvcc",
							"getcounts",
							"clear",
							"exit|quit",
							"showchip",
							"start",
							"stop",
							"history",
							"toggleoutput",
							"setoutput \\w+",
							"setallheaters \\w+"};
	
	
	public C_CommandParser(C_TemperatureMeasurementSystem c_TMS)
	{
		this.c_SR = c_TMS.getSerialCommunication();
		this.c_HC = c_TMS.getHeatControl();
		this.c_TMS = c_TMS;
		inputs = new ArrayList<String>();
	//	String 
		
	}
	
	public boolean parseAndSend(String str)
	{
		String cmd = "";
		int cmdID = -1;
		String query[] = new String[3];
		scanner = new Scanner(str);
		String arg0 = "-1";
		String arg1 = "-1";
		
		inputs.add(str);
		
		for (int i = 0; i < commands.length; i++) {
		    
			cmd = scanner.findInLine(commands[i]);
			if (cmd != "" && cmd != null) {
				query = str.split(" ");
								
				cmdID = i;
				
				try{
					if (query[1] != "" && query[1] != null)
					{
						arg0 = query[1];
					}
					if (query[2] != "" && query[2] != null)
					{
						arg0 = query[2];
					} 
				}catch(Exception e){}
				match = true;
			}			
		}
		
		if (!match) {
			System.out.println("No Match");
			return false;
		}
		
		switch(cmdID){
		case 0: displayHelp();break;
		case 1: setHeater(Integer.parseInt(query[1]), Integer.parseInt(query[2]));break;
		case 2: getHeater(Integer.parseInt(query[1])); break;
		case 3: getTemp(); break;
		case 4: getVcc(); break;
		case 5: getCount(); break;
		case 6: c_TMS.getSerialCommunication().clearText();; break;
		case 7: System.exit(0); break;
		case 8: displayChip();break;
		case 9: startExperiment(); break;
		case 10: stopExperiment(); break;
		case 11: displayHistory(); break;
		case 12: toggleOutput(); break;
		case 13: setOuput(Integer.parseInt(query[1])); break;
		case 14: setAllheaters(Integer.parseInt(query[1]));break;
		}
		
		
		return true;
		
	}
	
	private void setAllheaters(int intensity) {
		for (int i=0; i<=21; i++)
		{
			c_HC.setHeater(i, intensity);	
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				
				e.printStackTrace();
			}
		}
		
	}

	private void setOuput(int parseInt) {
		c_HC.setOutput(parseInt);
	}

	private void toggleOutput() {
		c_HC.toggleOutput();
	}

	private void displayHistory() {
		int display = Math.min(inputs.size(), 50);
		c_TMS.getSerialCommunication().appendText("\n");
		for (int i = 0;i<display;i++)
		{
			c_TMS.getSerialCommunication().appendText(inputs.get(i)+"\n");
		}
	}

	private void startExperiment() {
		c_HC.startExperiment();
	}
	
	private void stopExperiment() {
		c_HC.stopExperiment();
	}

	private void displayChip() {
		String chip = "\n\n#######################\n"+
					  "# H04 # H09 # H11 # H16 # H21 #\n" +
					  "#######################\n"+
					  "# H03 # H08 # S02 # H15 # H20 #\n" +
					  "#######################\n"+
					  "# H02 # H07 # S01 # H14 # H19 #\n" +
					  "#######################\n"+
					  "# H01 # H06 # S00 # H13 # H18 #\n" +
					  "#######################\n"+
					  "# H00 # H05 # H10 # H12 # H17 #\n"+
					  "#######################\nHXX: Heater\nSXX: System";
		c_TMS.getSerialCommunication().appendText(chip);	
	}

	public static void main (String a[])
	{
		
	}
	
	public void displayHelp()
	{
		String help = "Commands:\n-help\n-setheater int0 int1\tSet heater int0 (0-21) to level int1 (0-32)\n-getheater int0\tGet level of heater int0 (0-21)" +
				"\n-gettemp\t\tDisplay diode's temperature measurement\n-getvcc\t\tDisplay diode's supply voltage\n-clear\t\tClear output window\n-showchip\t\tDisplay" +
				" FPGA heater design\n-start\t\tStart continouus output of temperature and oscillator count\n" +
				"-stop\t\tStop continouus output\n-exit/quit\n";
		
		c_TMS.getSerialCommunication().appendText(help);	
	}
	
	public void setHeater(int id, int intensity)
	{
		c_HC.setHeater(id, intensity);
	}
	
	public void getTemp()
	{
		c_HC.getTemperature();
	}
	
	public void getVcc()
	{
		c_HC.getVCC();
	}
	
	public void getCount()
	{
		c_HC.getCounts();
	}
	
	public void getHeater(int id)
	{
		c_HC.getHeater(id);
	}
	
}
