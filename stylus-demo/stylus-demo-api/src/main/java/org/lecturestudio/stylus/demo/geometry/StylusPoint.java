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

package org.lecturestudio.stylus.demo.geometry;

import java.io.Serializable;
import java.util.Objects;

public class StylusPoint implements Cloneable, Serializable {

	private static final long serialVersionUID = 3737399850497337585L;

	/** The x coordinate. */
	private double x;
	
	/** The y coordinate. */
	private double y;

	/** The pressure at current coordinate. */
	private double pressure;
	
	
	/**
	 * Creates a new instance of {@code Point2D} with origin coordinates.
	 */
	public StylusPoint() {
		this(0, 0, 0);
	}
	
	/**
	 * Creates a new instance of {@code Point2D} with coordinates from
	 * provided point.
	 */
	public StylusPoint(StylusPoint point) {
		this(point.getX(), point.getY(), point.getPressure());
	}
	
	/**
	 * Creates a new instance of {@code Point2D} with specified coordinates.
	 * 
	 * @param x The x coordinate of the point.
	 * @param y The y coordinate of the point.
	 */
	public StylusPoint(double x, double y, double pressure) {
		this.x = x;
		this.y = y;
		this.pressure = pressure;
	}
	
	/**
	 * The x coordinate of the point.
	 * 
	 * @return The x coordinate.
	 */
	public double getX() {
		return x;
	}
	
	/**
	 * The y coordinate of the point.
	 * 
	 * @return The y coordinate.
	 */
	public double getY() {
		return y;
	}

	public void setPressure(double pressure) {
		this.pressure = pressure;
	}

	public double getPressure() {
		return pressure;
	}

	/**
	 * Sets new coordinates of this point.
	 * 
	 * @param x The x coordinate.
	 * @param y The y coordinate.
	 */
	public StylusPoint set(double x, double y) {
		this.x = x;
		this.y = y;
		
		return this;
	}
	
	public StylusPoint set(StylusPoint p) {
		set(p.x, p.y);
		
		return this;
	}
	
	/**
	 * Returns the distance from this <code>Point2D</code> to a specified
	 * <code>Point2D</code>.
	 * 
	 * @param p The point to which the distance should be measured.
	 * 
	 * @return The distance to the given point.
	 */
	public double distance(StylusPoint p) {
		double dx = p.x - x;
		double dy = p.y - y;
		
		return Math.sqrt(dx * dx + dy * dy);
	}
	
	public StylusPoint add(StylusPoint p) {
		x += p.x;
		y += p.y;
		
		return this;
	}
	
	public StylusPoint subtract(StylusPoint p) {
		x -= p.x;
		y -= p.y;
		
		return this;
	}
	
	public StylusPoint interpolate(StylusPoint v, double f) {
		return new StylusPoint(this.x + (v.x - this.x) * f, this.y + (v.y - this.y) * f, pressure);
	}
	
	public StylusPoint multiply(double scalar) {
		x *= scalar;
		y *= scalar;
		
		return this;
	}
	
	public double magnitude() {
		return Math.sqrt(x * x + y * y);
	}
	
	public double magnitudeSqr() {
		return x * x + y * y;
	}
	
	public double angleBetween(StylusPoint p) {
		return Math.atan2(x - p.x, y - p.y);
	}
	
	public StylusPoint normalize() {
		double length = Math.sqrt(x * x + y * y);
		
		x /= length;
		y /= length;
		
		return this;
	}
	
	public StylusPoint normalize(double length) {
		double mag = Math.sqrt(x * x + y * y);

		if (mag > 0) {
			mag = length / mag;
			x *= mag;
			y *= mag;
		}

		return this;
	}
	
	public StylusPoint perpendicular() {
		double t = x;
		this.x = -y;
		this.y = t;
		
		return this;
	}
	
	public StylusPoint getNormal(StylusPoint p) {
		double dx = p.x - x;
		double dy = p.y - y;

		return new StylusPoint(-dy, dx, pressure);
	}

	@Override
	public boolean equals(Object o) {
		if (this == o) {
			return true;
		}
		if (o == null || getClass() != o.getClass()) {
			return false;
		}

		StylusPoint that = (StylusPoint) o;

		boolean xc = Double.compare(that.x, x) == 0;
		boolean yc = Double.compare(that.y, y) == 0;
		boolean pc = Double.compare(that.pressure, pressure) == 0;

		return xc && yc && pc;
	}

	@Override
	public int hashCode() {
		return Objects.hash(x, y, pressure);
	}

	@Override
	public StylusPoint clone() {
		return new StylusPoint(x, y, pressure);
	}

	@Override
	public String toString() {
		return getClass().getName() + " (" + x + ", " + y + ", " + pressure + ")";
	}

}
