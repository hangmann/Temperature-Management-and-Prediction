package view;
import com.sun.opengl.util.FPSAnimator;
import com.sun.opengl.util.GLUT;

import controller.C_MouseListener3d;
import controller.C_TemperatureControl;
import controller.C_KeyboardListener3d;

import javax.media.opengl.*;
import javax.media.opengl.glu.GLU;
import javax.swing.*;

/**
 * User: christoph
 * Date: 2/23/12
 * Time: 9:19 PM
 */
public class V_TemperatureDistribution extends JFrame implements GLEventListener {
	private GLU mGlu;
	private GLUT mGlut;
	private GLCanvas mCanvas;
	private FPSAnimator mFpsAnimator;

	private boolean mVisible;

	private C_TemperatureControl mTempCont;

	private int mSensorGridWidth, mSensorGridHeight;
	private int mNumberOfSensors;
	private V_TemperatureGrid mTempGrid;

	private boolean m3d;
	private boolean mListenersInit;
	private float mViewRotX, mViewRotY, mViewRotZ;
	private float mViewTransX, mViewTransY, mViewTransZ;

	private long mStartTime = 0;
	private int mFrameCounter = 0;

	public V_TemperatureDistribution(C_TemperatureControl pTempCont, int min_temp, int max_temp, int pSubdivision, int pMaxFPS) {
		mTempCont = pTempCont;

		m3d = false;
		mViewRotX = -30;
		mViewRotY = 0;
		mViewRotZ = 0;
		mViewTransX = 0;
		mViewTransY = 0;
		mViewTransZ = -80;

		mNumberOfSensors = mTempCont.getNumberOfSensors();
		mSensorGridWidth = mTempCont.getSensorGridWidth();
		mSensorGridHeight = mTempCont.getSensorGridHeight();

		mTempGrid = new V_TemperatureGrid(min_temp, max_temp, pSubdivision, mSensorGridWidth, mSensorGridHeight);

		mGlu = new GLU();
		mGlut = new GLUT();

		GLCapabilities cap = new GLCapabilities();
		mCanvas = new GLCanvas(cap);
		mCanvas.addGLEventListener(this);

		mFpsAnimator = new FPSAnimator(mCanvas, pMaxFPS);

		mVisible = false;

		this.setTitle("Temperature on Chip");
		this.setSize(800, 800);
		this.setLocation(150, 100);
		this.add(mCanvas);
		this.setVisible(true);
	}

	public void setVisible(boolean pVisible) {
		super.setVisible(pVisible);

		if (mVisible != pVisible) {
			if (pVisible) {
				mFpsAnimator.start();
				mCanvas.requestFocus();
			} else {
				mFpsAnimator.stop();
			}
		}
		mVisible = pVisible;
	}

	public void init(GLAutoDrawable pDrawable) {
		GL gl = pDrawable.getGL();
		System.out.println("GL_VENDOR: " + gl.glGetString(GL.GL_VENDOR));
		System.out.println("GL_RENDERER: " + gl.glGetString(GL.GL_RENDERER));
		System.out.println("GL_VERSION: " + gl.glGetString(GL.GL_VERSION));

		if (!mListenersInit) {
			mListenersInit = true;
			C_MouseListener3d ml = new C_MouseListener3d(this);
			pDrawable.addMouseListener(ml);
			pDrawable.addMouseMotionListener(ml);
			pDrawable.addMouseWheelListener(ml);
			pDrawable.addKeyListener(new C_KeyboardListener3d(this));
		}

		gl.glEnable(GL.GL_DEPTH_TEST);
	}

	public void display(GLAutoDrawable pDrawable) {
		GL gl = pDrawable.getGL();

		setPerspective(gl);

		gl.glClearColor(0, 0, 0, 0);
		gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);

		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glLoadIdentity();

		mTempGrid.setTemperature(mTempCont.getTemperature());
		if (m3d) {
			applyMouseTranslation(gl);
			applyMouseRotation(gl);
			gl.glTranslated(-mTempGrid.getGridWith() / 2, -mTempGrid.getGridHeight() / 2, 0);
			mTempGrid.drawGrid3d(gl);
		} else {
			mTempGrid.drawGrid2d(gl);
		}

		// calculate FPS
		mFrameCounter++;
		long currTime = System.currentTimeMillis();
		if (currTime - mStartTime >= 1000) {
			this.setTitle("Temperature on Chip - FPS: " + mFrameCounter);
			mStartTime = currTime;
			mFrameCounter = 0;
		}
	}

	public void reshape(GLAutoDrawable pDrawable, int pX, int pY, int pWidth, int pHeight) {
		GL gl = pDrawable.getGL();

		gl.glViewport(0, 0, pWidth, pHeight);

		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glLoadIdentity();
	}

	public void displayChanged(GLAutoDrawable pDrawable, boolean pModeChanged,
							   boolean pDeviceChanged) {
	}

	private void setPerspective(GL gl) {
		gl.glMatrixMode(GL.GL_PROJECTION);
		gl.glLoadIdentity();
		if (m3d) {
			mGlu.gluPerspective(50, (mTempGrid.getGridWith() - 1) / (mTempGrid.getGridHeight() - 1), 4, 500);
		} else {
			mGlu.gluOrtho2D(0, mTempGrid.getGridWith() - 1, 0, mTempGrid.getGridHeight() - 1);
		}
	}

	private void applyMouseTranslation(GL gl) {
		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glTranslatef(mViewTransX, mViewTransY, mViewTransZ);
	}

	private void applyMouseRotation(GL gl) {
		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glRotatef(mViewRotX, 1, 0, 0);
		gl.glRotatef(mViewRotY, 0, 1, 0);
		gl.glRotatef(mViewRotZ, 0, 0, 1);
	}

	public void toggle3d() {
		m3d = !m3d;
	}

	public boolean is3d() {
		return m3d;
	}

	public void addMouseTransX(float pValue) {
		mViewTransX += pValue;
	}

	public void addMouseTransY(float pValue) {
		mViewTransY += pValue;
	}

	public void addMouseTransZ(float pValue) {
		mViewTransZ += pValue;
	}

	public void addMouseRotX(float pValue) {
		mViewRotX += pValue;
	}

	public void addMouseRotY(float pValue) {
		mViewRotY += pValue;
	}

	public void addMouseRotZ(float pValue) {
		mViewRotZ += pValue;
	}

	public void setMouseTransX(float pValue) {
		mViewTransX = pValue;
	}

	public void setMouseTransY(float pValue) {
		mViewTransY = pValue;
	}

	public void setMouseTransZ(float pValue) {
		mViewTransZ = pValue;
	}

	public void setMouseRotX(float pValue) {
		mViewRotX = pValue;
	}

	public void setMouseRotY(float pValue) {
		mViewRotY = pValue;
	}

	public void setMouseRotZ(float pValue) {
		mViewRotZ = pValue;
	}

	public void resetMouseRot() {
		mViewRotX = -30;
		mViewRotY = 0;
		mViewRotZ = 0;
	}

	public void resetMouseTrans() {
		mViewTransX = 0;
		mViewTransY = 0;
		mViewTransZ = -80;
	}
}