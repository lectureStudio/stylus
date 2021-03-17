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

package org.lecturestudio.stylus.demo.awt.render;

import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.geom.Ellipse2D;
import java.util.Iterator;
import java.util.List;

import org.lecturestudio.stylus.demo.awt.beans.converter.ColorConverter;
import org.lecturestudio.stylus.demo.geometry.StylusPoint;
import org.lecturestudio.stylus.demo.model.StrokeShape;
import org.lecturestudio.stylus.demo.paint.StylusBrush;
import org.lecturestudio.stylus.demo.paint.StylusStroke;
import org.lecturestudio.stylus.demo.render.RenderSurfaceRenderer;

public class PenRenderer implements RenderSurfaceRenderer<StrokeShape, Graphics2D> {

	private StylusStroke lastStroke;

	private int last;


	@Override
	public void render(StrokeShape shape, Graphics2D gc) {
		StylusStroke stroke = shape.getStroke();
		StylusBrush brush = shape.getBrush();

		gc.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		gc.setPaint(ColorConverter.INSTANCE.to(brush.getColor()));

		if (stroke.isChanging()) {
			if (lastStroke != stroke) {
				resetState();

				lastStroke = stroke;
			}

			renderProgressive(stroke, brush, gc);
		}
		else {
			resetState();

			renderWhole(stroke, brush, gc);
		}
	}

	private void resetState() {
		lastStroke = null;
		last = 0;
	}

	private void renderProgressive(StylusStroke stroke, StylusBrush brush, Graphics2D gc) {
		List<StylusPoint> points = stroke.getPoints();

		synchronized (points) {
			Iterator<StylusPoint> iter = points.listIterator(last);

			if (!iter.hasNext()) {
				return;
			}

			StylusPoint p0 = iter.next();

			if (!iter.hasNext()) {
				drawCircle(brush, gc, p0.getX(), p0.getY(), p0.getPressure());
			}
			else {
				StylusPoint p1;

				while (iter.hasNext()) {
					p1 = iter.next();

					drawLine(brush, gc, p0, p1);

					p0 = p1;

					last++;
				}
			}
		}
	}

	private void renderWhole(StylusStroke stroke, StylusBrush brush, Graphics2D gc) {
		List<StylusPoint> points = stroke.getPoints();

		if (points.isEmpty()) {
			return;
		}

		Iterator<StylusPoint> iter = points.iterator();
		StylusPoint p0 = iter.next();
		StylusPoint p1;

		if (!iter.hasNext()) {
			drawCircle(brush, gc, p0.getX(), p0.getY(), p0.getPressure());
		}

		while (iter.hasNext()) {
			p1 = iter.next();

			drawLine(brush, gc, p0, p1);

			p0 = p1;
		}
	}

	private void drawLine(StylusBrush brush, Graphics2D gc, StylusPoint p0, StylusPoint p1) {
		double dist = p0.distance(p1);
		double pDist = (p1.getPressure() - p0.getPressure()) / dist;
		StylusPoint u = p1.clone().subtract(p0).normalize();

		double x;
		double y;
		double p;

		for (double i = 0; i < dist; i += 1) {
			x = p0.getX() + u.getX() * i;
			y = p0.getY() + u.getY() * i;
			p = p0.getPressure() + pDist * i;

			drawCircle(brush, gc, x, y, p);
		}
	}

	private void drawCircle(StylusBrush brush, Graphics2D gc, double x, double y, double p) {
		double w = p * brush.getWidth();
		double d = w / 2;

		gc.fill(new Ellipse2D.Double(x - d, y - d, w, w));
	}
}
