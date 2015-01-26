package controller;

import model.M_TemperatureMeasurementSystem;

public class C_SensorDataLine {

	private C_SensorDataLine previousLine;
	private C_SensorDataLine nextLine;
	private int lineNumber;
	private int infoNumber;
	
	private int sensorData[];

	private double temperature;
	private double vcc;
	
	private int heaterLevels[];
	
	private String completeLine;

	@Override
	public String toString() {
		if (getNextLine().getLineNumber() != 0)
			return String.valueOf(lineNumber) + " --> " + getNextLine().toString();
		else
			return String.valueOf(lineNumber);
	}
	
	public C_SensorDataLine(){
		sensorData=new int[0];
		heaterLevels=new int[0];
		temperature=-1;
		vcc=-1;
		lineNumber=-1;
	}
	
	public C_SensorDataLine(String line){

		completeLine = line;
		
		sensorData=new int[M_TemperatureMeasurementSystem.NUMBER_OF_SENSORS];
		heaterLevels=new int[M_TemperatureMeasurementSystem.NUMBER_OF_HEATERS];
		String[] parts = line.split(" ");
		lineNumber=Integer.parseInt(parts[1]);
		infoNumber=Integer.parseInt(parts[0].substring(0,1));
		
		for (int i = 0; i<M_TemperatureMeasurementSystem.NUMBER_OF_SENSORS; i++)
		{
			sensorData[i] = Integer.parseInt(parts[i + 2]);
		}

		temperature=Double.parseDouble(parts[M_TemperatureMeasurementSystem.NUMBER_OF_SENSORS + 2]);
		vcc=Double.parseDouble(parts[M_TemperatureMeasurementSystem.NUMBER_OF_SENSORS + 3]);
		
		for (int i = 0; i<M_TemperatureMeasurementSystem.NUMBER_OF_HEATERS; i++)
		{
			parts[M_TemperatureMeasurementSystem.NUMBER_OF_SENSORS + i + 4]=parts[M_TemperatureMeasurementSystem.NUMBER_OF_SENSORS + i + 4].replaceAll("\\r|\\n", "");
			
			heaterLevels[i] = Integer.parseInt((parts[M_TemperatureMeasurementSystem.NUMBER_OF_SENSORS + i + 4]));
		}
		
	}
	
	public String getSensorDataString()
	{
		try{
			String s="";
			for (int i = 0; i<M_TemperatureMeasurementSystem.NUMBER_OF_SENSORS;i++)
			{
				s+=sensorData[i] + " ";
			}
			return s;
		}catch(ArrayIndexOutOfBoundsException e){
			System.out.println("ArrayIndexOutOfBounds: " + sensorData.length + " would be correct. LineNumber: " + lineNumber);
			return "-";
		}
	}
	
	public C_SensorDataLine getPreviousLine() {
		return previousLine;
	}

	public void setPreviousLine(C_SensorDataLine previousLine) {
		this.previousLine = previousLine;
	}

	public C_SensorDataLine getNextLine() {
		return nextLine;
	}

	public void setNextLine(C_SensorDataLine nextLine) {
		this.nextLine = nextLine;
	}

	public int getLineNumber() {
		return lineNumber;
	}

	public void setLineNumber(int lineNumber) {
		this.lineNumber = lineNumber;
	}

	public int[] getSensorData() {
		return sensorData;
	}

	public void setSensorData(int[] sensorData) {
		this.sensorData = sensorData;
	}

	public double getTemperature() {
		return temperature;
	}

	public void setTemperature(double temperature) {
		this.temperature = temperature;
	}

	public double getVcc() {
		return vcc;
	}

	public void setVcc(double vcc) {
		this.vcc = vcc;
	}

	public int[] getHeaterLevels() {
		return heaterLevels;
	}

	public void setHeaterLevels(int[] heaterLevels) {
		this.heaterLevels = heaterLevels;
	}

}
