package controller;

public class C_UnreadSensorData {

	C_SensorDataLine dummyLine;
	
	public C_UnreadSensorData()
	{
		dummyLine=new C_SensorDataLine();
	}
	
	
	
	@Override
	public String toString() {
		return dummyLine.getNextLine().toString();
	}



	public void addLine(String line)
	{
		if (!line.equals("") && line!=null){
			C_SensorDataLine nextLine = new C_SensorDataLine(line);
			addLine(nextLine);

		}
	}
	
	public void addLine(C_SensorDataLine line)
	{
		if (dummyLine.getPreviousLine() != null)
		{
			dummyLine.getPreviousLine().setNextLine(line);
			line.setNextLine(dummyLine);
			line.setPreviousLine(dummyLine.getPreviousLine());
			dummyLine.setPreviousLine(line);
		} else{
			dummyLine.setNextLine(line);
			line.setNextLine(dummyLine);
			dummyLine.setPreviousLine(line);
			line.setPreviousLine(dummyLine);
		}
	}
	
	public void deleteNextLine()
	{
		if (dummyLine.getNextLine() != null && dummyLine.getNextLine().getNextLine() != null)
		{
			dummyLine.getNextLine().getNextLine().setPreviousLine(dummyLine);
			dummyLine.setNextLine(dummyLine.getNextLine().getNextLine());
		}
		
		if (dummyLine.getNextLine() != null && dummyLine.getNextLine().getNextLine() == null)
		{
			dummyLine.setNextLine(null);
		}
	}
	
	public C_SensorDataLine getNextUnreadLine() throws NullPointerException
	{
		if (dummyLine.getNextLine() != null)
		{
			C_SensorDataLine nextLine = dummyLine.getNextLine();
			deleteNextLine();
			return nextLine;			
		} else {
			return null;
		}
	}
}
