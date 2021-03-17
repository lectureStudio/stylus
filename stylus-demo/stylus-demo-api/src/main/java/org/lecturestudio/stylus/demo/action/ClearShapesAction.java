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

package org.lecturestudio.stylus.demo.action;

import java.util.ArrayList;
import java.util.List;

import org.lecturestudio.stylus.demo.model.Shape;

public class ClearShapesAction extends ShapeAction {

	private List<Shape> removedShapes;


	public ClearShapesAction() {
	}

	@Override
	public void execute() {
		removedShapes = new ArrayList<>(shapes.getShapes());
		shapes.clear();
	}

	@Override
	public void undo() {
		shapes.addAll(removedShapes);
	}

	@Override
	public void redo() {
		execute();
	}
}
