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
package org.lecturestudio.stylus.demo.paint;

import org.lecturestudio.stylus.demo.beans.DoubleProperty;
import org.lecturestudio.stylus.demo.beans.ObjectProperty;

public class StylusBrush {

	private ObjectProperty<Color> color = new ObjectProperty<>();

	private DoubleProperty width = new DoubleProperty();


	public StylusBrush() {
		this(Color.BLUE, 12);
	}

	public StylusBrush(StylusBrush brush) {
		this(brush.getColor(), brush.getWidth());
	}

	public StylusBrush(Color color, double width) {
		setColor(color);
		setWidth(width);
	}

	public ObjectProperty<Color> colorProperty() {
		return color;
	}

	public void setColor(Color color) {
		this.color.set(color);
	}

	public Color getColor() {
		return color.get();
	}

	public DoubleProperty widthProperty() {
		return width;
	}

	public void setWidth(double width) {
		this.width.set(width);
	}

	public double getWidth() {
		return width.get();
	}
}
