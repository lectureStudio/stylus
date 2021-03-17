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

package org.lecturestudio.stylus.demo.tool;

import org.lecturestudio.stylus.StylusAxesData;
import org.lecturestudio.stylus.demo.action.ActionController;
import org.lecturestudio.stylus.demo.action.NewShapeAction;
import org.lecturestudio.stylus.demo.geometry.StylusPoint;
import org.lecturestudio.stylus.demo.model.StrokeShape;
import org.lecturestudio.stylus.demo.paint.Color;
import org.lecturestudio.stylus.demo.paint.StylusBrush;
import org.lecturestudio.stylus.demo.paint.StylusStroke;
import org.lecturestudio.stylus.demo.render.RenderSurface;

public class EraserTool implements StylusTool {

	private RenderSurface renderSurface;

	private StrokeShape shape;

	private StylusBrush brush;

	private StylusStroke stroke;


	public EraserTool(StylusBrush brush, RenderSurface renderSurface) {
		this.brush = new StylusBrush(Color.WHITE, brush.getWidth());
//		this.brush.widthProperty().bind(brush.widthProperty());

		brush.widthProperty().addListener((observable, oldValue, newValue) -> {
			this.brush.setWidth(newValue);
		});

		this.renderSurface = renderSurface;
	}

	@Override
	public void execute(ActionController actionController) {
		stroke = new StylusStroke();
		stroke.setChanging(true);

		shape = new StrokeShape(stroke, new StylusBrush(brush));

		actionController.execute(new NewShapeAction(shape));
	}

	@Override
	public void process(StylusAxesData axesData) {
		StylusPoint p = new StylusPoint(axesData.getX(), axesData.getY(), axesData.getPressure());

		stroke.addPoint(p);

		renderSurface.render(shape);
	}

	@Override
	public void finish() {
		stroke.setChanging(false);
	}
}
