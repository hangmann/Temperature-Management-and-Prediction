package controller;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import view.V_TemperatureDistribution;

/**
 * User: christoph
 * Date: 2/25/12
 * Time: 10:20 AM
 */
public class C_KeyboardListener3d extends KeyAdapter {
	private V_TemperatureDistribution mChipView;

	public C_KeyboardListener3d(V_TemperatureDistribution pChipView) {
		mChipView = pChipView;
	}

	public void keyPressed(KeyEvent e) {
		switch (e.getKeyCode()) {
			case KeyEvent.VK_ESCAPE:
				if (mChipView.is3d()) {
					mChipView.resetMouseRot();
					mChipView.resetMouseTrans();
				}
				break;
			case KeyEvent.VK_Q:
				mChipView.setVisible(false);
				System.exit(0);
				break;
			default:
				break;
		}
	}
}
