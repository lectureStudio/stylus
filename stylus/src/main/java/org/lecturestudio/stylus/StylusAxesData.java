/*
 * Copyright 2016 Alex Andres
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.lecturestudio.stylus;

import java.util.Arrays;

public class StylusAxesData {

	private final double[] data;


	public StylusAxesData(double[] data) {
		this.data = data;
	}

	public double getX() {
		return data[StylusAxis.X.ordinal()];
	}

	public double getY() {
		return data[StylusAxis.Y.ordinal()];
	}

	public double getPressure() {
		return data[StylusAxis.PRESSURE.ordinal()];
	}

	public double getTangentPressure() {
		return data[StylusAxis.TANGENT_PRESSURE.ordinal()];
	}

	public double getTiltX() {
		return data[StylusAxis.TILT_X.ordinal()];
	}

	public double getTiltY() {
		return data[StylusAxis.TILT_Y.ordinal()];
	}

	public double getRotation() {
		return data[StylusAxis.ROTATION.ordinal()];
	}

	public void setX(double x) {
		data[StylusAxis.X.ordinal()] = x;
	}

	public void setY(double y) {
		data[StylusAxis.Y.ordinal()] = y;
	}

	@Override
	public boolean equals(Object o) {
		if (this == o) {
			return true;
		}
		if (o == null || getClass() != o.getClass()) {
			return false;
		}

		StylusAxesData other = (StylusAxesData) o;

		return Arrays.equals(data, other.data);
	}

	@Override
	public int hashCode() {
		return Arrays.hashCode(data);
	}

	@Override
	public String toString() {
		return String.format(
				"X: %f, Y: %f, Pressure: %f, Tangent-Pressure: %f, Tilt-X: %f, Tilt-Y: %f, Rotation: %f",
				getX(), getY(), getPressure(), getTangentPressure(), getTiltX(), getTiltY(), getRotation());
	}
}
