package controller;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import view.v_TemperatureDistribution;

/**
 * User: christoph
 * Date: 2/25/12
 * Time: 10:20 AM
 */
public class KeyboardListener3d extends KeyAdapter {
	private v_TemperatureDistribution mChipView;

	public KeyboardListener3d(v_TemperatureDistribution pChipView) {
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
