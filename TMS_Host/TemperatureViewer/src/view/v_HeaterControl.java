package view;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.io.OutputStream;

import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.WindowConstants;

import controller.c_HeaterControl;
import controller.c_TemperatureMeasurementSystem;
import model.m_TemperatureMeasurementSystem;

public class v_HeaterControl implements ActionListener{

	private String [] items;
	private JLabel [] labels;
	private ComboBox [] combos;
	private JFrame frame;
    public m_TemperatureMeasurementSystem m_TMS;
    private c_HeaterControl c_HC;
    
    
    public v_HeaterControl (c_TemperatureMeasurementSystem c_TMS)
	{
    	this.m_TMS = c_TMS.getModel();
    	this.c_HC = c_TMS.getHeatControl();

		frame = new JFrame("Heater Control");
		GridLayout experimentLayout = new GridLayout(m_TemperatureMeasurementSystem.NUMBER_OF_HEATERS, m_TemperatureMeasurementSystem.HEATER_ADJUSTABILITY);
		frame.setLayout(experimentLayout);
              
        items = new String[m_TemperatureMeasurementSystem.HEATER_ADJUSTABILITY +1 ];
        
        String si;
        for (int i = 0; i <= m_TemperatureMeasurementSystem.HEATER_ADJUSTABILITY; i++)
        {
        	if (i<10)
        		si = "0"+i;
        	else
        		si=String.valueOf(i);
        	
        	items[i] = si;
        }
        
        labels = new JLabel [m_TemperatureMeasurementSystem.NUMBER_OF_HEATERS];
        combos = new ComboBox[m_TemperatureMeasurementSystem.NUMBER_OF_HEATERS];
        
        addGUIComponents();
        frame.setSize(200,1000);
        frame.setLocation(0,100);
        frame.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);
        

	}
	
	public void setComboIndex(int i, int j)
	{
		combos[i].setSelectedIndex(j);
	}
    
	private void addGUIComponents()
	{
		for (int i = 0; i < m_TemperatureMeasurementSystem.NUMBER_OF_HEATERS; i++)
		{
			labels[i] = new JLabel("Heater " + i);
			
			frame.add(labels[i]);
			
			combos[i] = new ComboBox(idToString(i), items);
			
			combos[i].addActionListener(this);
			frame.add(combos[i]);
		}
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
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// 

	}

	public class ComboBox extends JComboBox{

		String id;
		
		public ComboBox(String id, Object[] o) {
			super(o);
			this.id = id;
		}
		
		public String getID(){
			return id;
		}
		
	}
	


	@Override
	public void actionPerformed(ActionEvent e) {		
		ComboBox cb = (ComboBox)e.getSource();
		String id = cb.getID();
		cb.getName();
		String intensity = cb.getSelectedItem().toString();
		
		c_HC.setHeater(Integer.valueOf(id), Integer.valueOf(intensity));
		
	}
	

	
	
	
}