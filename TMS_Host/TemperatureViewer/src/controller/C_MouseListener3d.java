package controller;
import java.awt.Dimension;
import java.awt.event.InputEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

import view.V_TemperatureDistribution;

/**
 * User: christoph
 * Date: 2/25/12
 * Time: 10:18 AM
 */
public class C_MouseListener3d extends MouseAdapter implements MouseWheelListener {
	private int mPrevX, mPrevY;
	private V_TemperatureDistribution mChipView;

	public C_MouseListener3d(V_TemperatureDistribution pChipView) {
		mPrevX = 0;
		mPrevY = 0;
		mChipView = pChipView;
	}

	public void mousePressed(MouseEvent e) {
		mPrevX = e.getX();
		mPrevY = e.getY();
	}

	public void mouseDragged(MouseEvent e) {
		int x = e.getX();
		int y = e.getY();
		float thetaX, thetaY;
		Dimension size = e.getComponent().getSize();

		if ((e.getModifiers() & InputEvent.BUTTON1_MASK) != 0) {
			if (mChipView.is3d()) {
				thetaX = 360 * (float) (x - mPrevX) / (float) size.width;
				thetaY = 360 * (float) (mPrevY - y) / (float) size.height;
				mChipView.addMouseRotZ(thetaX);
				mChipView.addMouseRotX(-thetaY);
				mPrevX = x;
				mPrevY = y;
			}
		}

		if ((e.getModifiers() & InputEvent.BUTTON3_MASK) != 0) {
			if (mChipView.is3d()) {
				thetaX = 70 * (float) (x - mPrevX) / (float) size.width;
				thetaY = 70 * (float) (mPrevY - y) / (float) size.height;
				mChipView.addMouseTransX(thetaX);
				mChipView.addMouseTransY(thetaY);
				mPrevX = x;
				mPrevY = y;
			}
		}
	}

	public void mouseWheelMoved(MouseWheelEvent e) {
		if (mChipView.is3d()) {
			mChipView.addMouseTransZ(4 * e.getWheelRotation());
		}
	}
}
