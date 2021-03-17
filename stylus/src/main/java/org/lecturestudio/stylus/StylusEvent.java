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

import java.util.Objects;

public class StylusEvent {

	private final StylusButton button;
	private final StylusCursor cursor;
	private final StylusAxesData axesData;


	public StylusEvent(StylusButton button, StylusCursor cursor, double[] axesData) {
		this.button = button;
		this.cursor = cursor;
		this.axesData = new StylusAxesData(axesData);
	}

	public void translate(double x, double y) {
		axesData.setX(x);
		axesData.setY(y);
	}

	public StylusButton getButton() {
		return button;
	}

	public StylusCursor getCursor() {
		return cursor;
	}

	public StylusAxesData getAxesData() {
		return axesData;
	}

	@Override
	public boolean equals(Object o) {
		if (this == o) {
			return true;
		}
		if (o == null || getClass() != o.getClass()) {
			return false;
		}

		StylusEvent other = (StylusEvent) o;

		boolean axesDataEqual = Objects.equals(axesData, other.axesData);

		return button == other.button && cursor == other.cursor && axesDataEqual;
	}

	@Override
	public int hashCode() {
		return Objects.hash(button, cursor, axesData);
	}

	@Override
	public String toString() {
		return String.format("Button: %s, Cursor: %s, [%s]", button, cursor, axesData);
	}
}
