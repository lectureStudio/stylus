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

package org.lecturestudio.stylus.demo.presenter;

import java.util.Collection;

import javax.inject.Inject;

import org.lecturestudio.stylus.demo.model.Shape;
import org.lecturestudio.stylus.demo.model.Shapes;
import org.lecturestudio.stylus.demo.model.ShapesChangeListener;
import org.lecturestudio.stylus.demo.tool.ToolController;
import org.lecturestudio.stylus.demo.view.CanvasView;

public class CanvasPresenter extends Presenter<CanvasView> {

	private final ToolController toolController;

	private final Shapes shapes;


	@Inject
	CanvasPresenter(CanvasView view, Shapes shapes, ToolController toolController) {
		super(view);

		this.shapes = shapes;
		this.toolController = toolController;

		initialize();
	}

	private void initialize() {
		toolController.setRenderSurface(view);
		toolController.setStylusCursorListener(view::setStylusCursor);

		view.setStylusListener(toolController);
		view.setOnResize(() -> {
			view.rerender(shapes.getShapes());
		});

		shapes.addShapesChangeListener(new ShapesChangeListener() {

			@Override
			public void onShapeInserted(Shapes shapes, Shape shape) {
				view.render(shape);
			}

			@Override
			public void onShapesInserted(Shapes shapes, Collection<Shape> list) {
				view.render(list);
			}

			@Override
			public void onShapeRemoved(Shapes shapes, Shape shape) {
				view.rerender(shapes.getShapes());
			}

			@Override
			public void onShapesRemoved(Shapes shapes, Collection<Shape> list) {
				view.rerender(shapes.getShapes());
			}
		});
	}
}
