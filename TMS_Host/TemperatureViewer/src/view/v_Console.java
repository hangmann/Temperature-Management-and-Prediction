package view;
import javax.swing.*;
import controller.*;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultCaret;

import controller.c_CommandParser;
import controller.*;

public class v_Console implements ComponentListener{

	private JFrame f;

	private JTextArea textArea;
	private JScrollPane sbrText;
	private JPanel panel;
	private JTextField inputText;
	private JButton send;
	private c_CommandParser parser;
	
	public v_Console(c_TemperatureMeasurementSystem c_TMS){
		
	//	this.control = control;
		parser = new c_CommandParser(c_TMS);
		
        // Create Frame
        f = new JFrame("Output");
        panel = new JPanel();
        inputText = new JTextField(120);

        DefaultCaret caret = (DefaultCaret)inputText.getCaret();
        caret.setUpdatePolicy(DefaultCaret.ALWAYS_UPDATE);
        
		f.getContentPane().setLayout(new FlowLayout());
		f.setLocationRelativeTo(null);
        panel.setLayout(new GridBagLayout());
        GridBagConstraints c = new GridBagConstraints();
        // Create Scrolling Text Area in Swing
        textArea = new JTextArea("", 20, 120);
        textArea.setEditable(false);
		sbrText = new JScrollPane(textArea);
		sbrText.addComponentListener(this);
		sbrText.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		sbrText.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
		
		send = new JButton("Send");
		send.addActionListener(
				new ActionListener(){
					public void actionPerformed(ActionEvent e){
						sendText();         
					}

			}
		);
		
		f.getRootPane().setDefaultButton(send);
		
		
		c.ipady = 40;
		c.gridx = 0;
		c.gridy = 0;
		c.anchor = GridBagConstraints.PAGE_START;
        panel.add(sbrText,c);
		c.ipady = 0;
		c.gridx = 0;
		c.gridy = 1;
		c.anchor = GridBagConstraints.PAGE_END;
		panel.add(inputText,c);
		c.ipady = 0;
		c.gridx = 1;
		c.gridy = 1;
		c.anchor = GridBagConstraints.LAST_LINE_END;
		panel.add(send,c);
        f.getContentPane().add(panel);
        
		 // Close when the close button is clicked
		f.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);

		//Display Frame
       f.pack(); // Adjusts frame to size of components
       f.setVisible(true);
       
      
    }

	private void sendText() {
		if (parser.parseAndSend(inputText.getText())) {
			System.out.println("Success");
		}
		inputText.setText("");
	}

	
    public void appendText(String str)
    {
    	textArea.setText(textArea.getText() +str);

    	textArea.setCaretPosition(textArea.getDocument().getLength());
    }
    
    public void clearText()
    {
    	textArea.setText("");
    }

	@Override
	public void componentHidden(ComponentEvent arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void componentMoved(ComponentEvent arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void componentResized(ComponentEvent arg0) {
		// TODO Auto-generated method stub

		textArea.setSize(f.getWidth(), f.getHeight());
	}

	@Override
	public void componentShown(ComponentEvent arg0) {
		// TODO Auto-generated method stub
	}


}
