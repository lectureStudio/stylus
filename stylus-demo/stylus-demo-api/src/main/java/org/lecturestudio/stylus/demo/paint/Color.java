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

import java.io.Serializable;

public class Color implements Cloneable, Serializable {
	
	private static final long serialVersionUID = -2703233901325908526L;

	/** The color white in the default sRGB space. */
	public final static Color WHITE = new Color(255, 255, 255);

	/** The color black in the default sRGB space. */
	public final static Color BLACK = new Color(0, 0, 0);

	/** The color blue in the default sRGB space. */
	public final static Color BLUE = new Color(0, 0, 255);

	/** The color value containing RGBA components. */
	private final int value;


	/**
	 * Copy-constructor.
	 *
	 * @param color the color to copy.
	 */
	public Color(Color color) {
		this(color.getRGBA());
	}

	/**
	 * Creates a new instance of {@code Color}. The values must be in the range 0 - 255.
	 * 
	 * @param red the red component of the Color.
	 * @param green the green component of the Color.
	 * @param blue the blue component of the Color.
	 */
	public Color(int red, int green, int blue) {
		this(red, green, blue, 255);
	}
	
	/**
	 * Creates a new instance of {@code Color}. The values must be in the range 0 - 255.
	 * 
	 * @param red the red component of the Color.
	 * @param green the green component of the Color.
	 * @param blue the blue component of the Color.
	 * @param opacity the opacity of the Color.
	 */
	public Color(int red, int green, int blue, int opacity) {
		value = ((opacity & 0xFF) << 24) | ((red & 0xFF) << 16) | ((green & 0xFF) << 8) | ((blue & 0xFF) << 0);
	}
	
	/**
	 * Creates a new instance of {@code Color} with the specified combined RGBA
	 * value consisting of the alpha component in bits 24-31, the red component
	 * in bits 16-23, the green component in bits 8-15, and the blue component
	 * in bits 0-7.
	 * 
	 * @param rgba the combined RGBA components.
	 */
	public Color(int rgba) {
		this.value = rgba;
	}
	
	/**
	 * Creates a new Color with the specified opacity.
	 * 
	 * @param opacity the new opacity of the Color.
	 * 
	 * @return a new Color with the given opacity.
	 */
	public Color derive(int opacity) {
		return new Color(((opacity & 0xFF) << 24) | (value & 0x00FFFFFF));
	}
	
	/**
	 * Returns the RGBA value representing the color.
	 * 
	 * @return the RGBA value of the color.
	 */
	public int getRGBA() {
		return value;
	}

	/**
	 * The red component of the Color, in the range 0 - 255.
	 * 
	 * @return the red component.
	 */
	public int getRed() {
		return (getRGBA() >> 16) & 0xFF;
	}

	/**
	 * The green component of the Color, in the range 0 - 255.
	 * 
	 * @return the green component.
	 */
	public int getGreen() {
		return (getRGBA() >> 8) & 0xFF;
	}

	/**
	 * The blue component of the Color, in the range 0 - 255.
	 * 
	 * @return the blue component.
	 */
	public int getBlue() {
		return (getRGBA() >> 0) & 0xFF;
	}

	/**
	 * The opacity of the Color, in the range 0 - 255.
	 * 
	 * @return the opacity.
	 */
	public int getOpacity() {
		return (getRGBA() >> 24) & 0xFF;
	}
	
	public boolean isOpaque() {
		return getOpacity() == 255;
	}
	
	@Override
	public int hashCode() {
		return value;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		
		Color other = (Color) obj;
		
		return this.value == other.value;
	}

	@Override
	public String toString() {
		return "Color (" + getRed() + " " + getGreen() + " " + getBlue() + " " + getOpacity() + ")";
	}

	@Override
	public Color clone() {
		try {
			return (Color) super.clone();
		}
		catch (CloneNotSupportedException e) {
			throw new InternalError();
		}
	}
	
}
