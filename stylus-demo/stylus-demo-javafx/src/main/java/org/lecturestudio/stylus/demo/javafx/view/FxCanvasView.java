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

package org.lecturestudio.stylus.demo.javafx.view;

import static java.util.Objects.nonNull;

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

import javafx.scene.Cursor;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;

import org.lecturestudio.stylus.StylusCursor;
import org.lecturestudio.stylus.StylusListener;
import org.lecturestudio.stylus.demo.javafx.render.PenRenderer;
import org.lecturestudio.stylus.demo.javafx.util.FxUtils;
import org.lecturestudio.stylus.demo.model.Shape;
import org.lecturestudio.stylus.demo.model.StrokeShape;
import org.lecturestudio.stylus.demo.render.RenderSurfaceRenderer;
import org.lecturestudio.stylus.demo.view.Action;
import org.lecturestudio.stylus.demo.view.CanvasView;
import org.lecturestudio.stylus.javafx.JavaFxStylusManager;

public class FxCanvasView extends Pane implements CanvasView {

	private Map<Class<? extends Shape>, RenderSurfaceRenderer<? extends Shape, GraphicsContext>> renderers;

	private Action resizeAction;

	private Canvas canvas;

	private Cursor stylusCursor;


	public FxCanvasView() {
		super();

		initialize();
	}

	@Override
	public void setStylusCursor(StylusCursor cursor) {
		FxUtils.invoke(() -> {
			switch (cursor) {
				case PEN:
				case ERASER:
					setCursor(stylusCursor);
					break;

				default:
					setCursor(Cursor.DEFAULT);
					break;
			}
		});
	}

	@Override
	public void setStylusListener(StylusListener listener) {
		JavaFxStylusManager manager = JavaFxStylusManager.getInstance();
		manager.attachStylusListener(canvas, listener);
	}

	@Override
	public void setOnResize(Action action) {
		this.resizeAction = action;
	}

	@Override
	public void clear() {
		GraphicsContext gc = canvas.getGraphicsContext2D();
		gc.setFill(Color.WHITE);
		gc.fillRect(0, 0, canvas.getWidth(), canvas.getHeight());
	}

	@Override
	public <S extends Shape> void rerender(Collection<S> shapes) {
		FxUtils.invoke(() -> {
			clear();

			shapes.forEach(this::renderShape);
		});
	}

	@Override
	public <S extends Shape> void render(final Collection<S> shapes) {
		FxUtils.invoke(() -> {
			shapes.forEach(this::renderShape);
		});
	}

	@Override
	public <S extends Shape> void render(final S shape) {
		FxUtils.invoke(() -> {
			renderShape(shape);
		});
	}

	@SuppressWarnings("unchecked")
	private <S extends Shape> void renderShape(S shape) {
		RenderSurfaceRenderer<S, GraphicsContext> renderer = (RenderSurfaceRenderer<S, GraphicsContext>) getRenderer(shape.getClass());
		renderer.render(shape, canvas.getGraphicsContext2D());
	}

	private RenderSurfaceRenderer<? extends Shape, GraphicsContext> getRenderer(Class<? extends Shape> cls) {
		return renderers.get(cls);
	}

	private <S extends Shape> void setRenderer(Class<S> cls, RenderSurfaceRenderer<S, GraphicsContext> renderer) {
		renderers.put(cls, renderer);
	}

	private void onResize() {
		if (nonNull(resizeAction)) {
			resizeAction.execute();
		}
	}

	private void initialize() {
		renderers = new HashMap<>();

		canvas = new Canvas();
		canvas.widthProperty().bind(widthProperty());
		canvas.heightProperty().bind(heightProperty());

		canvas.widthProperty().addListener(observable -> onResize());
		canvas.heightProperty().addListener(observable -> onResize());

		setRenderer(StrokeShape.class, new PenRenderer());

		getChildren().add(canvas);
	}
}
