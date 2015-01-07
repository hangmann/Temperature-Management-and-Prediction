package view;
import javax.media.opengl.GL;

/**
 * User: christoph
 * Date: 2/24/12
 * Time: 12:49 PM
 */
public class V_TemperatureGrid {
	
	private int mSubdivision, mProportionalSize;
	private int mSensorGridWidth, mSensorGridHeight;
	private int mGridWidth, mGridHeight;
	private int mMinTemp, mMaxTemp;

	double[][] mGrid;

	public V_TemperatureGrid(int min_temp, int max_temp, int pSubdivision, int pGridWidth, int pGridHeight) {
		mSubdivision = pSubdivision;
		mProportionalSize = Math.min(pGridHeight, pGridWidth) * mSubdivision;
		mSensorGridWidth = pGridWidth;
		mSensorGridHeight = pGridHeight;

		mMinTemp = min_temp;
		mMaxTemp = max_temp;

		mGridWidth = (mSensorGridWidth + 1) * mSubdivision + 1;
		mGridHeight = (mSensorGridHeight + 1) * mSubdivision + 1;

		mGrid = new double[mGridHeight][mGridWidth];
	}

	public void drawGrid2d(GL gl) {
		gl.glBegin(GL.GL_QUADS);

		for (int i = 0; i < mGridHeight - 1; i++) {
			for (int j = 0; j < mGridWidth - 1; j++) {
				gl.glColor3dv(tempToColor(mGrid[i][j]), 0);
				gl.glVertex2d(j, i);
				gl.glColor3dv(tempToColor(mGrid[i][j + 1]), 0);
				gl.glVertex2d(j + 1, i);
				gl.glColor3dv(tempToColor(mGrid[i + 1][j + 1]), 0);
				gl.glVertex2d(j + 1, i + 1);
				gl.glColor3dv(tempToColor(mGrid[i + 1][j]), 0);
				gl.glVertex2d(j, i + 1);
			}
		}

		gl.glEnd();
	}

	public void drawGrid3d(GL gl) {
		for (int i = 0; i < mGridHeight - 1; i++) {
			for (int j = 0; j < mGridWidth - 1; j++) {
				gl.glBegin(GL.GL_QUADS);
				gl.glPolygonMode(GL.GL_FRONT_AND_BACK, GL.GL_FILL);
				gl.glColor3dv(tempToColor(mGrid[i][j]), 0);
				gl.glVertex3d(j, i, mGrid[i][j]);
				gl.glColor3dv(tempToColor(mGrid[i][j + 1]), 0);
				gl.glVertex3d(j + 1, i, mGrid[i][j + 1]);
				gl.glColor3dv(tempToColor(mGrid[i + 1][j + 1]), 0);
				gl.glVertex3d(j + 1, i + 1, mGrid[i + 1][j + 1]);
				gl.glColor3dv(tempToColor(mGrid[i + 1][j]), 0);
				gl.glVertex3d(j, i + 1, mGrid[i + 1][j]);
				gl.glEnd();

				gl.glBegin(GL.GL_LINE_LOOP);
				gl.glColor3d(0, 0, 0);
				gl.glVertex3d(j, i, mGrid[i][j] + 0.0);
				gl.glVertex3d(j + 1, i, mGrid[i][j + 1] + 0.0);
				gl.glVertex3d(j + 1, i + 1, mGrid[i + 1][j + 1] + 0.0);
				gl.glVertex3d(j, i + 1, mGrid[i + 1][j] + 0.0);
				gl.glVertex3d(j, i, mGrid[i][j] + 0.0);
				gl.glEnd();
			}
		}
	}

	public void setTemperature(double[] pTemp) {
		int currX, currY;
		int currSensorNumber;

		resetGrid();

		for (int i = 0; i < mSensorGridHeight; i++) {
			for (int j = 0; j < mSensorGridWidth; j++) {
				currX = (j + 1) * mSubdivision;
				currY = (i + 1) * mSubdivision;
				currSensorNumber = i * mSensorGridWidth + j;

				//mGrid[currY][currX] = pTemp[currSensorNumber];
				proportionalAdd(pTemp[currSensorNumber], currX, currY);
			}
		}
	}

	private void proportionalAdd(double pTemp, int pX, int pY) {
		double distance;
		int currX, currY;

		for (int i = -mProportionalSize; i <= mProportionalSize; i++) {
			for (int j = -mProportionalSize; j <= mProportionalSize; j++) {
				currX = pX + j;
				currY = pY + i;

				// check if current point is outside grid
				if (currX >= 0 && currY >= 0 && currX < mGridWidth && currY < mGridHeight) {
					// calculate distance to current sensor
					distance = Math.sqrt(i * i + j * j);
					//distance = Math.sqrt(i * i + j * j + mGrid[currY][currX] * mGrid[currY][currX]);
					if (distance <= mProportionalSize) {
						//mGrid[currY][currX] = Math.max(mGrid[currY][currX], gauss(distance, pTemp));
						mGrid[currY][currX] = mGrid[currY][currX] + gauss(distance, pTemp - mGrid[currY][currX]);
					}
				}
			}
		}
	}

	private double gauss(double pDistance, double pMaxDisplacement) {
		// using gaussian distribution to calculate displacement
		return pMaxDisplacement * Math.exp(-0.5 * pDistance * pDistance / (mProportionalSize * mProportionalSize / 12));
	}

	public int getGridWith() {
		return mGridWidth;
	}

	public int getGridHeight() {
		return mGridHeight;
	}


	private double[] tempToColor(double pTemp) {
		double key = (pTemp - mMinTemp) * 891 / (mMaxTemp - mMinTemp);
		double[] color = new double[3];

		if (key >= 0 && key < 255) {
			color[0] = 0;
			color[1] = key / 255;
			color[2] = 1;
		} else if (key >= 255 && key < 510) {
			color[0] = (key - 255) / 255;
			color[1] = 1;
			color[2] = (255 - (key - 255)) / 255;
		} else if (key >= 510 && key < 765) {
			color[0] = 1;
			color[1] = (255 - (key - 510)) / 255;
			color[2] = 0;
		} else if (key >= 765 && key <= 891) {
			color[0] = (255 - (key - 765)) / 255;
			color[1] = 0;
			color[2] = 0;
		} else {
			color[0] = 0;
			color[1] = 0;
			color[2] = 0;
		}

		return color;
	}


	private void resetGrid() {
		for (int i = 0; i < mGridHeight; i++) {
			for (int j = 0; j < mGridWidth; j++) {
				mGrid[i][j] = 0;
			}
		}
	}
}
